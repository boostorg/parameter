// Copyright Daniel Wallin 2006.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_PYTHON_060209_HPP
#define BOOST_PARAMETER_PYTHON_060209_HPP

#include <boost/tti/detail/dnullptr.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/prior.hpp>
#include <boost/mpl/shift_right.hpp>
#include <boost/mpl/shift_left.hpp>
#include <boost/mpl/bitand.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/count_if.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/iterator_range.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/python/def.hpp>
#include <boost/python/make_constructor.hpp>
#include <boost/python/init.hpp>
#include <boost/python/to_python_converter.hpp>
#include <boost/parameter/aux_/maybe.hpp>
#include <boost/parameter/aux_/python/invoker.hpp>

namespace boost { namespace parameter { namespace python {

    namespace python_ = boost::python;
}}}

namespace boost { namespace parameter { namespace python { namespace aux {

    inline PyObject* unspecified_type()
    {
        static PyTypeObject unspecified = {
            PyObject_HEAD_INIT(BOOST_TTI_DETAIL_NULLPTR)
            0,                                /* ob_size        */
            "Boost.Parameter.Unspecified",    /* tp_name        */
            PyType_Type.tp_basicsize,         /* tp_basicsize   */
            0,                                /* tp_itemsize    */
            0,                                /* tp_dealloc     */
            0,                                /* tp_print       */
            0,                                /* tp_getattr     */
            0,                                /* tp_setattr     */
            0,                                /* tp_compare     */
            0,                                /* tp_repr        */
            0,                                /* tp_as_number   */
            0,                                /* tp_as_sequence */
            0,                                /* tp_as_mapping  */
            0,                                /* tp_hash        */
            0,                                /* tp_call        */
            0,                                /* tp_str         */
            0,                                /* tp_getattro    */
            0,                                /* tp_setattro    */
            0,                                /* tp_as_buffer   */
            Py_TPFLAGS_DEFAULT,               /* tp_flags       */
            0,                                /* tp_doc         */
        };

        if (unspecified.ob_type == BOOST_TTI_DETAIL_NULLPTR)
        {
            unspecified.ob_type = &PyType_Type;
            PyType_Ready(&unspecified);
        }

        return reinterpret_cast<PyObject*>(&unspecified);
    }

    struct empty_tag
    {
    };

    struct empty_tag_to_python
    {
        static PyObject* convert(empty_tag)
        {
            return python_::xincref(unspecified_type());
        }
    };
}}}} // namespace boost::parameter::python::aux

namespace boost { namespace python {

    // Converts a Python value to a maybe<T>
    template <class T>
    struct arg_from_python<parameter::aux::maybe<T> >
      : arg_from_python<T>
    {
        arg_from_python(PyObject* p)
          : arg_from_python<T>(p)
          , empty(parameter::python::aux::unspecified_type() == p)
        {
        }

        bool convertible() const
        {
            return this->empty || arg_from_python<T>::convertible();
        }

        parameter::aux::maybe<T> operator()()
        {
            if (this->empty)
            {
                return parameter::aux::maybe<T>();
            }
            else
            {
                return parameter::aux::maybe<T>(
                    arg_from_python<T>::operator()()
                );
            }
        }

        bool empty;
    };
}} // namespace boost::python

namespace boost { namespace parameter { namespace python { namespace aux {

    template <class K>
    struct is_optional
      : boost::mpl::not_<
            boost::mpl::or_<
                typename K::required
              , typename K::optimized_default
            >
        >
    {
    };

    template <class K, class Required, class Optimized, class T>
    struct arg_spec
    {
        typedef K keyword;
        typedef Required required;
        typedef T type;
        typedef Optimized optimized_default;
    };

    template <class K, class T, class Optimized = boost::mpl::false_>
    struct make_arg_spec_impl
    {
        typedef boost::parameter::python::aux::arg_spec<
            typename K::first,typename K::second,Optimized,T
        > type;
    };

    template <class K, class T>
    struct make_arg_spec_impl<K,T,typename K::third>
    {
        typedef boost::parameter::python::aux::arg_spec<
            typename K::first,typename K::second,typename K::third,T
        > type;
    };

    template <class K, class T>
    struct make_arg_spec
      : boost::parameter::python::aux::make_arg_spec_impl<K,T>
    {
    };

    template <class Spec, class State>
    struct combinations_op
    {
        typedef typename State::second bits;
        typedef typename State::first result0;

        typedef typename boost::mpl::if_<
            boost::mpl::or_<
                typename Spec::required
              , typename Spec::optimized_default
              , boost::mpl::bitand_<bits,boost::mpl::long_<1> >
            >
          , typename boost::mpl::push_back<result0,Spec>::type
          , result0
        >::type result;

        typedef typename boost::mpl::if_<
            boost::mpl::or_<
                typename Spec::required
              , typename Spec::optimized_default
            >
          , bits
          , typename boost::mpl::shift_right<bits,boost::mpl::long_<1> >::type
        >::type next_bits;

        typedef boost::mpl::pair<
            result
          , next_bits
        > type;
    };

    // Used as start value in the recursive arg() composition below.
    struct no_keywords
    {
        template <class T>
        T const& operator,(T const& x) const
        {
            return x;
        }
    };

    template <class Def, class F, class Iter, class End, class Keywords>
    void def_combination_aux0(
        Def def, F f, Iter, End, Keywords const& keywords, boost::mpl::false_
    )
    {
        typedef typename boost::mpl::deref<Iter>::type spec;
        typedef typename spec::keyword kw;

        def_combination_aux(
            def, f, typename boost::mpl::next<Iter>::type(), End()
          , (
                keywords, boost::python::arg(kw::keyword_name())
            )
        );
    }

    template <class Def, class F, class Iter, class End, class Keywords>
    void def_combination_aux0(
        Def def, F f, Iter, End, Keywords const& keywords, boost::mpl::true_
    )
    {
        typedef typename boost::mpl::deref<Iter>::type spec;
        typedef typename spec::keyword kw;

        def_combination_aux(
            def, f, typename boost::mpl::next<Iter>::type(), End()
          , (
                keywords, boost::python::arg(kw::keyword_name()) = empty_tag()
            )
        );
    }

    inline void initialize_converter()
    {
        static python_::to_python_converter<empty_tag,empty_tag_to_python> x;
    }

    template <class Def, class F, class Iter, class End, class Keywords>
    void def_combination_aux(
        Def def, F f, Iter, End, Keywords const& keywords
    )
    {
        typedef typename boost::mpl::deref<Iter>::type spec;

        typedef typename boost::mpl::and_<
            typename spec::optimized_default
          , boost::mpl::not_<typename spec::required>
        >::type optimized_default;

        def_combination_aux0(
            def, f, Iter(), End(), keywords, optimized_default()
        );
    }

    template <class Def, class F, class End, class Keywords>
    void def_combination_aux(
        Def def, F f, End, End, Keywords const& keywords
    )
    {
        def(f, keywords);
    } 

    template <class Def, class F, class End>
    void def_combination_aux(
        Def def, F f, End, End, no_keywords const&
    )
    {
        def(f);
    }

    template <
        class Def, class Specs, class Bits, class Invoker
    >
    void def_combination(
        Def def, Specs*, Bits, Invoker*
    )
    {
        typedef typename boost::mpl::fold<
            Specs
          , boost::mpl::pair<boost::mpl::vector0<>,Bits>
          , combinations_op<boost::mpl::_2,boost::mpl::_1>
        >::type combination0;

        typedef typename combination0::first combination;

        typedef typename boost::mpl::apply_wrap1<
            Invoker, combination
        >::type invoker;

        def_combination_aux(
            def
          , &invoker::execute
          , typename boost::mpl::begin<combination>::type()
          , typename boost::mpl::end<combination>::type()
          , no_keywords()
        );
    }

    template <
        class Def, class Specs, class Bits, class End, class Invoker
    >
    void def_combinations(
        Def def, Specs*, Bits, End, Invoker*
    )
    {
        initialize_converter();

        def_combination(
            def
          , static_cast<Specs*>(BOOST_TTI_DETAIL_NULLPTR)
          , Bits()
          , static_cast<Invoker*>(BOOST_TTI_DETAIL_NULLPTR)
        );

        def_combinations(
            def
          , static_cast<Specs*>(BOOST_TTI_DETAIL_NULLPTR)
          , boost::mpl::long_<Bits::value + 1>()
          , End()
          , static_cast<Invoker*>(BOOST_TTI_DETAIL_NULLPTR)
        );
    }

    template <
        class Def, class Specs, class End, class Invoker
    >
    void def_combinations(
        Def, Specs*, End, End, Invoker*
    )
    {
    }

    struct not_specified
    {
    };

    template <class CallPolicies>
    struct call_policies_as_options
    {
        call_policies_as_options(CallPolicies const& policies)
          : call_policies(policies)
        {
        }

        CallPolicies const& policies() const
        {
            return this->call_policies;
        }

        char const* doc() const
        {
            return BOOST_TTI_DETAIL_NULLPTR;
        }

        CallPolicies call_policies;
    };

    template <class Class, class Options = not_specified>
    struct def_class
    {
        def_class(Class& c, char const* n, Options o = Options())
          : cl(c), name(n), options(o)
        {
        }

        template <class F>
        void def(F f, not_specified const*) const
        {
            this->cl.def(this->name, f);
        }

        template <class F>
        void def(F f, void const*) const
        {
            this->cl.def(
                this->name
              , f
              , this->options.doc()
              , this->options.policies()
            );
        }

        template <class F>
        void operator()(F f) const
        {
            this->def(f, &this->options);
        }

        template <class F, class Keywords>
        void def(F f, Keywords const& keywords, not_specified const*) const
        {
            this->cl.def(this->name, f, keywords);
        }

        template <class F, class Keywords>
        void def(F f, Keywords const& keywords, void const*) const
        {
            this->cl.def(
                this->name
              , f
              , keywords
              , this->options.doc()
              , this->options.policies()
            );
        }

        template <class F, class Keywords>
        void operator()(F f, Keywords const& keywords) const
        {
            this->def(f, keywords, &this->options);
        }

        Class& cl;
        char const* name;
        Options options;
    };

    template <
        class Class
      , class CallPolicies = boost::python::default_call_policies
    >
    struct def_init
    {
        def_init(Class& c, CallPolicies policies = CallPolicies())
          : cl(c), call_policies(policies)
        {
        }

        template <class F>
        void operator()(F f) const
        {
            this->cl.def(
                "__init__"
              , boost::python::make_constructor(f, this->call_policies)
            );
        }

        template <class F, class Keywords>
        void operator()(F f, Keywords const& keywords) const
        {
            this->cl.def(
                "__init__"
              , boost::python::make_constructor(
                    f
                  , this->call_policies
                  , keywords
                )
            );
        }

        Class& cl;
        CallPolicies call_policies;
    };

    struct def_function
    {
        def_function(char const* n)
          : name(n)
        {
        }

        template <class F>
        void operator()(F f) const
        {
            boost::python::def(this->name, f);
        }

        template <class F, class Keywords>
        void operator()(F f, Keywords const& keywords) const
        {
            boost::python::def(this->name, f, keywords);
        }

        char const* name;
    };
}}}} // namespace boost::parameter::python::aux

namespace boost { namespace parameter { namespace python {

    template <class M, class Signature>
    void def(char const* name, Signature)
    {
        typedef boost::mpl::iterator_range<
            typename boost::mpl::next<
                typename boost::mpl::begin<Signature>::type
            >::type
          , typename boost::mpl::end<Signature>::type
        > arg_types;

        typedef typename boost::mpl::transform<
            typename M::keywords
          , arg_types
          , boost::parameter::python::aux::make_arg_spec<
                boost::mpl::_1
              , boost::mpl::_2
            >
          , boost::mpl::back_inserter<boost::mpl::vector0<> >
        >::type arg_specs;

        typedef typename boost::mpl::count_if<
            arg_specs
          , boost::parameter::python::aux::is_optional<boost::mpl::_1>
        >::type optional_arity;

        typedef typename boost::mpl::front<Signature>::type result_type;
        typedef typename boost::mpl::shift_left<
            boost::mpl::long_<1>
          , optional_arity
        >::type upper;

        boost::parameter::python::aux::def_combinations(
            boost::parameter::python::aux::def_function(name)
          , static_cast<arg_specs*>(BOOST_TTI_DETAIL_NULLPTR)
          , boost::mpl::long_<0>()
          , boost::mpl::long_<upper::value>()
          , static_cast<
                boost::parameter::python::aux::make_invoker<M,result_type>*
            >(BOOST_TTI_DETAIL_NULLPTR)
        );
    }

    template <class M, class Class, class Signature>
    void def(Class& cl, char const* name, Signature)
    {
        typedef boost::mpl::iterator_range<
            typename boost::mpl::next<
                typename boost::mpl::begin<Signature>::type
            >::type
          , typename boost::mpl::end<Signature>::type
        > arg_types;

        typedef typename boost::mpl::transform<
            typename M::keywords
          , arg_types
          , boost::parameter::python::aux::make_arg_spec<
                boost::mpl::_1
              , boost::mpl::_2
            >
          , boost::mpl::back_inserter<boost::mpl::vector0<> >
        >::type arg_specs;

        typedef typename boost::mpl::count_if<
            arg_specs
          , boost::parameter::python::aux::is_optional<boost::mpl::_1>
        >::type optional_arity;

        typedef typename boost::mpl::front<Signature>::type result_type;
        typedef typename boost::mpl::shift_left<
            boost::mpl::long_<1>
          , optional_arity
        >::type upper;

        boost::parameter::python::aux::def_combinations(
            boost::parameter::python::aux::def_class<Class>(cl, name)
          , static_cast<arg_specs*>(BOOST_TTI_DETAIL_NULLPTR)
          , boost::mpl::long_<0>()
          , boost::mpl::long_<upper::value>()
          , static_cast<
                boost::parameter::python::aux::make_invoker<M,result_type>*
            >(BOOST_TTI_DETAIL_NULLPTR)
        );
    }
}}} // namespace boost::parameter::python

namespace boost { namespace parameter { namespace python { namespace aux {

    template <class K>
    struct keyword
    {
        typedef K type;
    };

    template <class K>
    struct keyword<K*>
    {
        typedef K type;
    };

    template <class K>
    struct keyword<K**>
    {
        typedef K type;
    };

    template <class K>
    struct required
    {
        typedef boost::mpl::true_ type;
    };

    template <class K>
    struct required<K*>
    {
        typedef boost::mpl::false_ type;
    };

    template <class K>
    struct optimized
    {
        typedef boost::mpl::true_ type;
    };

    template <class K>
    struct optimized<K**>
    {
        typedef boost::mpl::false_ type;
    };

    template <class T>
    struct make_kw_spec;

    template <class K, class T>
    struct make_kw_spec<K(T)>
    {
        typedef boost::parameter::python::aux::arg_spec<
            typename boost::parameter::python::aux::keyword<K>::type
          , typename boost::parameter::python::aux::required<K>::type
          , typename boost::parameter::python::aux::optimized<K>::type
          , T
        > type;
    };
}}}} // namespace boost::parameter::python::aux

namespace boost { namespace parameter { namespace python {

    template <
        class ParameterSpecs
      , class CallPolicies = boost::python::default_call_policies
    >
    struct init 
      : boost::python::def_visitor<init<ParameterSpecs,CallPolicies> >
    {
        init(CallPolicies policies = CallPolicies())
          : call_policies(policies)
        {
        }

        template <class CallPolicies1>
        init<ParameterSpecs,CallPolicies1> 
        operator[](CallPolicies1 const& policies) const
        {
            return init<ParameterSpecs,CallPolicies1>(policies);
        }

        template <class Class>
        void visit_aux(Class& cl, boost::mpl::true_) const
        {
            cl.def(boost::python::init<>()[this->call_policies]);
        }

        template <class Class>
        void visit_aux(Class& cl, boost::mpl::false_) const
        {
            typedef typename boost::mpl::transform<
                ParameterSpecs
              , boost::parameter::python::aux::make_kw_spec<boost::mpl::_>
              , boost::mpl::back_inserter<boost::mpl::vector0<> >
            >::type arg_specs;

            typedef typename boost::mpl::count_if<
                arg_specs
              , boost::parameter::python::aux::is_optional<boost::mpl::_>
            >::type optional_arity;

            typedef typename boost::mpl::shift_left<
                boost::mpl::long_<1>
              , optional_arity
            >::type upper;

            boost::parameter::python::aux::def_combinations(
                boost::parameter::python::aux::def_init<Class,CallPolicies>(
                    cl
                  , this->call_policies
                )
              , static_cast<arg_specs*>(BOOST_TTI_DETAIL_NULLPTR)
              , boost::mpl::long_<0>()
              , boost::mpl::long_<upper::value>()
              , static_cast<
                    boost::parameter::python::aux::make_init_invoker<
                        typename Class::wrapped_type
                    >*
                >(BOOST_TTI_DETAIL_NULLPTR)
            );
        }

        template <class Class>
        void visit(Class& cl) const
        {
            visit_aux(cl, boost::mpl::empty<ParameterSpecs>());
        }

        CallPolicies call_policies;
    };

    template <
        class ParameterSpecs
      , class CallPolicies = boost::python::default_call_policies
    >
    struct call 
      : boost::python::def_visitor<call<ParameterSpecs,CallPolicies> >
    {
        call(CallPolicies const& policies = CallPolicies())
          : call_policies(policies)
        {
        }

        template <class CallPolicies1>
        call<ParameterSpecs,CallPolicies1>
        operator[](CallPolicies1 const& policies) const
        {
            return call<ParameterSpecs,CallPolicies1>(policies);
        }

        template <class Class>
        void visit(Class& cl) const
        {
            typedef boost::mpl::iterator_range<
                typename boost::mpl::next<
                    typename boost::mpl::begin<ParameterSpecs>::type
                >::type
              , typename boost::mpl::end<ParameterSpecs>::type
            > arg_types;

            typedef typename boost::mpl::front<
                ParameterSpecs
            >::type result_type;

            typedef typename boost::mpl::transform<
                arg_types
              , boost::parameter::python::aux::make_kw_spec<boost::mpl::_>
              , boost::mpl::back_inserter<boost::mpl::vector0<> >
            >::type arg_specs;

            typedef typename boost::mpl::count_if<
                arg_specs
              , boost::parameter::python::aux::is_optional<boost::mpl::_>
            >::type optional_arity;

            typedef typename boost::mpl::shift_left<
                boost::mpl::long_<1>
              , optional_arity
            >::type upper;

            typedef boost::parameter::python::aux::call_policies_as_options<
                CallPolicies
            > options;

            boost::parameter::python::aux::def_combinations(
                boost::parameter::python::aux::def_class<Class,options>(
                    cl
                  , "__call__"
                  , options(this->call_policies)
                )
              , static_cast<arg_specs*>(BOOST_TTI_DETAIL_NULLPTR)
              , boost::mpl::long_<0>()
              , boost::mpl::long_<upper::value>()
              , static_cast<
                    boost::parameter::python::aux::make_call_invoker<
                        typename Class::wrapped_type
                      , result_type
                    >*
                >(BOOST_TTI_DETAIL_NULLPTR)
            );
        }

        CallPolicies call_policies;
    };

    template <class Fwd, class ParameterSpecs>
    struct function 
      : boost::python::def_visitor<function<Fwd,ParameterSpecs> >
    {
        template <class Class, class Options>
        void visit(Class& cl, char const* name, Options const& options) const
        {
            typedef boost::mpl::iterator_range<
                typename boost::mpl::next<
                    typename boost::mpl::begin<ParameterSpecs>::type
                >::type
              , typename boost::mpl::end<ParameterSpecs>::type
            > arg_types;

            typedef typename boost::mpl::front<
                ParameterSpecs
            >::type result_type;

            typedef typename boost::mpl::transform<
                arg_types
              , boost::parameter::python::aux::make_kw_spec<boost::mpl::_>
              , boost::mpl::back_inserter<boost::mpl::vector0<> >
            >::type arg_specs;

            typedef typename boost::mpl::count_if<
                arg_specs
              , boost::parameter::python::aux::is_optional<boost::mpl::_>
            >::type optional_arity;

            typedef typename boost::mpl::shift_left<
                boost::mpl::long_<1>
              , optional_arity
            >::type upper;

            boost::parameter::python::aux::def_combinations(
                boost::parameter::python::aux::def_class<
                    Class
                  , Options
                >(cl, name, options)
              , static_cast<arg_specs*>(BOOST_TTI_DETAIL_NULLPTR)
              , boost::mpl::long_<0>()
              , boost::mpl::long_<upper::value>()
              , static_cast<
                    boost::parameter::python::aux::make_member_invoker<
                        Fwd
                      , result_type
                      , typename Class::wrapped_type
                    >*
                >(BOOST_TTI_DETAIL_NULLPTR)
            );
        }
    };
}}} // namespace boost::parameter::python

#endif // BOOST_PARAMETER_PYTHON_060209_HPP

