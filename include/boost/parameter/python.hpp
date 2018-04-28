// Copyright Daniel Wallin 2006.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_PYTHON_060209_HPP
#define BOOST_PARAMETER_PYTHON_060209_HPP

namespace boost { namespace parameter { namespace python { namespace aux {

    struct empty_tag
    {
    };

    template <typename K, typename Required, typename Optimized, typename T>
    struct arg_spec
    {
        typedef K keyword;
        typedef Required required;
        typedef T type;
        typedef Optimized optimized_default;
    };

    // Used as start value in the recursive arg() composition below.
    struct no_keywords
    {
        template <typename T>
        T const& operator,(T const& x) const
        {
            return x;
        }
    };

    template <typename K>
    struct keyword
    {
        typedef K type;
    };

    template <typename K>
    struct keyword<K*>
    {
        typedef K type;
    };

    template <typename K>
    struct keyword<K**>
    {
        typedef K type;
    };

    template <typename T>
    struct make_kw_spec;

    template <typename Def, typename F, typename End, typename Keywords>
    void
        def_combination_aux(
            Def def
          , F f
          , End
          , End
          , Keywords const& keywords
        )
    {
        def(f, keywords);
    } 

    template <typename Def, typename F, typename End>
    void
        def_combination_aux(
            Def def
          , F f
          , End
          , End
          , ::boost::parameter::python::aux::no_keywords const&
        )
    {
        def(f);
    }

    template <typename Def, typename Specs, typename End, typename Invoker>
    void def_combinations(Def, Specs*, End, End, Invoker*)
    {
    }

    struct not_specified
    {
    };

    template <
        typename Class
      , typename Options = ::boost::parameter::python::aux::not_specified
    >
    struct def_class
    {
        def_class(Class& c, char const* n, Options o = Options())
          : cl(c), name(n), options(o)
        {
        }

        template <typename F>
        void
            def(
                F f
              , ::boost::parameter::python::aux::not_specified const*
            ) const
        {
            this->cl.def(this->name, f);
        }

        template <typename F>
        void def(F f, void const*) const
        {
            this->cl.def(
                this->name
              , f
              , this->options.doc()
              , this->options.policies()
            );
        }

        template <typename F>
        void operator()(F f) const
        {
            this->def(f, &this->options);
        }

        template <typename F, typename Keywords>
        void
            def(
                F f
              , Keywords const& keywords
              , ::boost::parameter::python::aux::not_specified const*
            ) const
        {
            this->cl.def(this->name, f, keywords);
        }

        template <typename F, typename Keywords>
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

        template <typename F, typename Keywords>
        void operator()(F f, Keywords const& keywords) const
        {
            this->def(f, keywords, &this->options);
        }

        Class& cl;
        char const* name;
        Options options;
    };
}}}} // namespace boost::parameter::python::aux

#include <boost/mpl/bool.hpp>

namespace boost { namespace parameter { namespace python { namespace aux {

    template <
        typename K
      , typename T
      , typename Optimized = ::boost::mpl::false_
    >
    struct make_arg_spec_impl
    {
        typedef ::boost::parameter::python::aux::arg_spec<
            typename K::first
          , typename K::second
          , Optimized
          , T
        > type;
    };

    template <typename K, typename T>
    struct make_arg_spec_impl<K,T,typename K::third>
    {
        typedef ::boost::parameter::python::aux::arg_spec<
            typename K::first
          , typename K::second
          , typename K::third
          , T
        > type;
    };

    template <typename K, typename T>
    struct make_arg_spec
      : ::boost::parameter::python::aux::make_arg_spec_impl<K,T>
    {
    };

    template <typename K>
    struct required
    {
        typedef ::boost::mpl::true_ type;
    };

    template <typename K>
    struct required<K*>
    {
        typedef ::boost::mpl::false_ type;
    };

    template <typename K>
    struct optimized
    {
        typedef ::boost::mpl::true_ type;
    };

    template <typename K>
    struct optimized<K**>
    {
        typedef ::boost::mpl::false_ type;
    };

    template <typename K, typename T>
    struct make_kw_spec<K(T)>
    {
        typedef ::boost::parameter::python::aux::arg_spec<
            typename ::boost::parameter::python::aux::keyword<K>::type
          , typename ::boost::parameter::python::aux::required<K>::type
          , typename ::boost::parameter::python::aux::optimized<K>::type
          , T
        > type;
    };
}}}} // namespace boost::parameter::python::aux

#include <boost/tti/detail/dnullptr.hpp>

namespace boost { namespace parameter { namespace python { namespace aux {

    template <typename CallPolicies>
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
            return static_cast<char const*>(BOOST_TTI_DETAIL_NULLPTR);
        }

        CallPolicies call_policies;
    };
}}}} // namespace boost::parameter::python::aux

#include <boost/python/def.hpp>

namespace boost { namespace parameter { namespace python { namespace aux {

    struct def_function
    {
        def_function(char const* n)
          : name(n)
        {
        }

        template <typename F>
        void operator()(F f) const
        {
            ::boost::python::def(this->name, f);
        }

        template <typename F, typename Keywords>
        void operator()(F f, Keywords const& keywords) const
        {
            ::boost::python::def(this->name, f, keywords);
        }

        char const* name;
    };
}}}} // namespace boost::parameter::python::aux

#include <boost/python/make_constructor.hpp>

namespace boost { namespace parameter { namespace python { namespace aux {

    template <
        typename Class
      , typename CallPolicies = ::boost::python::default_call_policies
    >
    struct def_init
    {
        def_init(Class& c, CallPolicies policies = CallPolicies())
          : cl(c), call_policies(policies)
        {
        }

        template <typename F>
        void operator()(F f) const
        {
            this->cl.def(
                "__init__"
              , ::boost::python::make_constructor(f, this->call_policies)
            );
        }

        template <typename F, typename Keywords>
        void operator()(F f, Keywords const& keywords) const
        {
            this->cl.def(
                "__init__"
              , ::boost::python::make_constructor(
                    f
                  , this->call_policies
                  , keywords
                )
            );
        }

        Class& cl;
        CallPolicies call_policies;
    };
}}}} // namespace boost::parameter::python::aux

#include <boost/python/init.hpp>

namespace boost { namespace parameter { namespace python { namespace aux {

    inline PyObject* unspecified_type()
    {
        static PyTypeObject unspecified =
        {
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

    struct empty_tag_to_python
    {
        static PyObject* convert(::boost::parameter::python::aux::empty_tag)
        {
            return ::boost::python::xincref(
                ::boost::parameter::python::aux::unspecified_type()
            );
        }
    };
}}}} // namespace boost::parameter::python::aux

#include <boost/python/to_python_converter.hpp>

namespace boost { namespace parameter { namespace python { namespace aux {

    inline void initialize_converter()
    {
        static ::boost::python::to_python_converter<
            ::boost::parameter::python::aux::empty_tag
          , ::boost::parameter::python::aux::empty_tag_to_python
        > x;
    }
}}}} // namespace boost::parameter::python::aux

#include <boost/parameter/aux_/maybe.hpp>

namespace boost { namespace python {

    // Converts a Python value to a maybe<T>
    template <typename T>
    struct arg_from_python< ::boost::parameter::aux::maybe<T> >
      : arg_from_python<T>
    {
        arg_from_python(PyObject* p)
          : arg_from_python<T>(p)
          , empty(::boost::parameter::python::aux::unspecified_type() == p)
        {
        }

        bool convertible() const
        {
            return this->empty || (
                ::boost::python::arg_from_python<T>::convertible()
            );
        }

        ::boost::parameter::aux::maybe<T> operator()()
        {
            if (this->empty)
            {
                return ::boost::parameter::aux::maybe<T>();
            }
            else
            {
                return ::boost::parameter::aux::maybe<T>(
                    ::boost::python::arg_from_python<T>::operator()()
                );
            }
        }

        bool empty;
    };
}} // namespace boost::python

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/if.hpp>

namespace boost { namespace parameter { namespace python { namespace aux {

    template <typename K>
    struct is_optional
      : ::boost::mpl::eval_if<
            typename K::required
          , ::boost::mpl::false_
          , ::boost::mpl::if_<
                typename K::optimized_default
              , ::boost::mpl::false_
              , ::boost::mpl::true_
            >
        >::type
    {
    };
}}}} // namespace boost::parameter::python::aux

#include <boost/mpl/deref.hpp>
#include <boost/mpl/next.hpp>

namespace boost { namespace parameter { namespace python { namespace aux {

    template <
        typename Def
      , typename F
      , typename Iter
      , typename End
      , typename Keywords
    >
    void
        def_combination_aux0(
            Def def
          , F f
          , Iter
          , End
          , Keywords const& keywords
          , ::boost::mpl::false_
        )
    {
        typedef typename ::boost::mpl::deref<Iter>::type spec;
        typedef typename spec::keyword kw;

        ::boost::parameter::python::aux::def_combination_aux(
            def
          , f
          , typename ::boost::mpl::next<Iter>::type()
          , End()
          , (keywords, ::boost::python::arg(kw::keyword_name()))
        );
    }

    template <
        typename Def
      , typename F
      , typename Iter
      , typename End
      , typename Keywords
    >
    void
        def_combination_aux0(
            Def def
          , F f
          , Iter
          , End
          , Keywords const& keywords
          , ::boost::mpl::true_
        )
    {
        typedef typename ::boost::mpl::deref<Iter>::type spec;
        typedef typename spec::keyword kw;

        ::boost::parameter::python::aux::def_combination_aux(
            def
          , f
          , typename ::boost::mpl::next<Iter>::type()
          , End()
          , (
                keywords
              , ::boost::python::arg(
                    kw::keyword_name()
                ) = ::boost::parameter::python::aux::empty_tag()
            )
        );
    }

    template <
        typename Def
      , typename F
      , typename Iter
      , typename End
      , typename Keywords
    >
    void
        def_combination_aux(
            Def def
          , F f
          , Iter
          , End
          , Keywords const& keywords
        )
    {
        typedef typename ::boost::mpl::deref<Iter>::type spec;
        typedef typename ::boost::mpl::eval_if<
            typename spec::optimized_default
          , ::boost::mpl::if_<
                typename spec::required
              , ::boost::mpl::false_
              , ::boost::mpl::true_
            >
          , ::boost::mpl::false_
        >::type optimized_default;

        ::boost::parameter::python::aux::def_combination_aux0(
            def
          , f
          , Iter()
          , End()
          , keywords
          , optimized_default()
        );
    }
}}}} // namespace boost::parameter::python::aux

#include <boost/mpl/long.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/bitand.hpp>
#include <boost/mpl/shift_right.hpp>
#include <boost/mpl/push_back.hpp>

namespace boost { namespace parameter { namespace python { namespace aux {

    template <typename Spec, typename State>
    struct combinations_op
    {
        typedef typename State::second bits;
        typedef typename State::first result0;

        typedef typename ::boost::mpl::eval_if<
            typename ::boost::mpl::eval_if<
                typename Spec::required
              , ::boost::mpl::true_
              , ::boost::mpl::if_<
                    typename Spec::optimized_default
                  , ::boost::mpl::true_
                  , ::boost::mpl::bitand_<bits,::boost::mpl::long_<1> >
                >
            >::type
          , ::boost::mpl::push_back<result0,Spec>
          , ::boost::mpl::identity<result0>
        >::type result;

        typedef typename ::boost::mpl::eval_if<
            typename ::boost::mpl::if_<
                typename Spec::required
              , ::boost::mpl::true_
              , typename Spec::optimized_default
            >::type
          , ::boost::mpl::identity<bits>
          , ::boost::mpl::shift_right<bits,::boost::mpl::long_<1> >
        >::type next_bits;

        typedef ::boost::mpl::pair<
            result
          , next_bits
        > type;
    };
}}}} // namespace boost::parameter::python::aux

#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/apply_wrap.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/begin_end.hpp>

namespace boost { namespace parameter { namespace python { namespace aux {

    template <typename Def, typename Specs, typename Bits, typename Invoker>
    void def_combination(Def def, Specs*, Bits, Invoker*)
    {
        typedef typename ::boost::mpl::fold<
            Specs
          , ::boost::mpl::pair< ::boost::mpl::vector0<>,Bits>
          , ::boost::parameter::python::aux::combinations_op<
                ::boost::mpl::_2
              , ::boost::mpl::_1
            >
        >::type combination0;
        typedef typename combination0::first combination;
        typedef typename ::boost::mpl::apply_wrap1<
            Invoker
          , combination
        >::type invoker;

        ::boost::parameter::python::aux::def_combination_aux(
            def
          , &invoker::execute
          , typename ::boost::mpl::begin<combination>::type()
          , typename ::boost::mpl::end<combination>::type()
          , ::boost::parameter::python::aux::no_keywords()
        );
    }

    template <
        typename Def
      , typename Specs
      , typename Bits
      , typename End
      , typename Invoker
    >
    void def_combinations(Def def, Specs*, Bits, End, Invoker*)
    {
        ::boost::parameter::python::aux::initialize_converter();
        ::boost::parameter::python::aux::def_combination(
            def
          , static_cast<Specs*>(BOOST_TTI_DETAIL_NULLPTR)
          , Bits()
          , static_cast<Invoker*>(BOOST_TTI_DETAIL_NULLPTR)
        );
        ::boost::parameter::python::aux::def_combinations(
            def
          , static_cast<Specs*>(BOOST_TTI_DETAIL_NULLPTR)
          , ::boost::mpl::long_<Bits::value + 1>()
          , End()
          , static_cast<Invoker*>(BOOST_TTI_DETAIL_NULLPTR)
        );
    }
}}}} // namespace boost::parameter::python::aux

#include <boost/mpl/prior.hpp>
#include <boost/mpl/shift_left.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/count_if.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/iterator_range.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/parameter/aux_/python/invoker.hpp>

namespace boost { namespace parameter { namespace python {

    template <typename M, typename Signature>
    void def(char const* name, Signature)
    {
        typedef ::boost::mpl::iterator_range<
            typename ::boost::mpl::next<
                typename ::boost::mpl::begin<Signature>::type
            >::type
          , typename ::boost::mpl::end<Signature>::type
        > arg_types;

        typedef typename ::boost::mpl::transform<
            typename M::keywords
          , arg_types
          , ::boost::parameter::python::aux::make_arg_spec<
                ::boost::mpl::_1
              , ::boost::mpl::_2
            >
          , ::boost::mpl::back_inserter< ::boost::mpl::vector0<> >
        >::type arg_specs;

        typedef typename ::boost::mpl::count_if<
            arg_specs
          , ::boost::parameter::python::aux::is_optional< ::boost::mpl::_1>
        >::type optional_arity;

        typedef typename ::boost::mpl::front<Signature>::type result_type;
        typedef typename ::boost::mpl::shift_left<
            ::boost::mpl::long_<1>
          , optional_arity
        >::type upper;

        ::boost::parameter::python::aux::def_combinations(
            ::boost::parameter::python::aux::def_function(name)
          , static_cast<arg_specs*>(BOOST_TTI_DETAIL_NULLPTR)
          , ::boost::mpl::long_<0>()
          , ::boost::mpl::long_<upper::value>()
          , static_cast<
                ::boost::parameter::python::aux::make_invoker<M,result_type>*
            >(BOOST_TTI_DETAIL_NULLPTR)
        );
    }

    template <typename M, typename Class, typename Signature>
    void def(Class& cl, char const* name, Signature)
    {
        typedef ::boost::mpl::iterator_range<
            typename ::boost::mpl::next<
                typename ::boost::mpl::begin<Signature>::type
            >::type
          , typename ::boost::mpl::end<Signature>::type
        > arg_types;

        typedef typename ::boost::mpl::transform<
            typename M::keywords
          , arg_types
          , ::boost::parameter::python::aux::make_arg_spec<
                ::boost::mpl::_1
              , ::boost::mpl::_2
            >
          , ::boost::mpl::back_inserter< ::boost::mpl::vector0<> >
        >::type arg_specs;

        typedef typename ::boost::mpl::count_if<
            arg_specs
          , ::boost::parameter::python::aux::is_optional< ::boost::mpl::_1>
        >::type optional_arity;

        typedef typename ::boost::mpl::front<Signature>::type result_type;
        typedef typename ::boost::mpl::shift_left<
            ::boost::mpl::long_<1>
          , optional_arity
        >::type upper;

        ::boost::parameter::python::aux::def_combinations(
            ::boost::parameter::python::aux::def_class<Class>(cl, name)
          , static_cast<arg_specs*>(BOOST_TTI_DETAIL_NULLPTR)
          , ::boost::mpl::long_<0>()
          , ::boost::mpl::long_<upper::value>()
          , static_cast<
                ::boost::parameter::python::aux::make_invoker<M,result_type>*
            >(BOOST_TTI_DETAIL_NULLPTR)
        );
    }

    template <
        typename ParameterSpecs
      , typename CallPolicies = ::boost::python::default_call_policies
    >
    struct init 
      : ::boost::python::def_visitor<init<ParameterSpecs,CallPolicies> >
    {
        init(CallPolicies policies = CallPolicies())
          : call_policies(policies)
        {
        }

        template <typename CallPolicies1>
        init<ParameterSpecs,CallPolicies1> 
        operator[](CallPolicies1 const& policies) const
        {
            return init<ParameterSpecs,CallPolicies1>(policies);
        }

        template <typename Class>
        void visit_aux(Class& cl, ::boost::mpl::true_) const
        {
            cl.def(::boost::python::init<>()[this->call_policies]);
        }

        template <typename Class>
        void visit_aux(Class& cl, ::boost::mpl::false_) const
        {
            typedef typename ::boost::mpl::transform<
                ParameterSpecs
              , ::boost::parameter::python::aux::make_kw_spec<
                    ::boost::mpl::_
                >
              , ::boost::mpl::back_inserter< ::boost::mpl::vector0<> >
            >::type arg_specs;

            typedef typename ::boost::mpl::count_if<
                arg_specs
              , boost::parameter::python::aux::is_optional< ::boost::mpl::_>
            >::type optional_arity;

            typedef typename ::boost::mpl::shift_left<
                ::boost::mpl::long_<1>
              , optional_arity
            >::type upper;

            ::boost::parameter::python::aux::def_combinations(
                ::boost::parameter::python::aux::def_init<Class,CallPolicies>(
                    cl
                  , this->call_policies
                )
              , static_cast<arg_specs*>(BOOST_TTI_DETAIL_NULLPTR)
              , ::boost::mpl::long_<0>()
              , ::boost::mpl::long_<upper::value>()
              , static_cast<
                    ::boost::parameter::python::aux::make_init_invoker<
                        typename Class::wrapped_type
                    >*
                >(BOOST_TTI_DETAIL_NULLPTR)
            );
        }

        template <typename Class>
        void visit(Class& cl) const
        {
            visit_aux(cl, ::boost::mpl::empty<ParameterSpecs>());
        }

        CallPolicies call_policies;
    };

    template <
        typename ParameterSpecs
      , typename CallPolicies = ::boost::python::default_call_policies
    >
    struct call 
      : ::boost::python::def_visitor<call<ParameterSpecs,CallPolicies> >
    {
        call(CallPolicies const& policies = CallPolicies())
          : call_policies(policies)
        {
        }

        template <typename CallPolicies1>
        call<ParameterSpecs,CallPolicies1>
        operator[](CallPolicies1 const& policies) const
        {
            return call<ParameterSpecs,CallPolicies1>(policies);
        }

        template <typename Class>
        void visit(Class& cl) const
        {
            typedef ::boost::mpl::iterator_range<
                typename ::boost::mpl::next<
                    typename ::boost::mpl::begin<ParameterSpecs>::type
                >::type
              , typename ::boost::mpl::end<ParameterSpecs>::type
            > arg_types;

            typedef typename ::boost::mpl::front<
                ParameterSpecs
            >::type result_type;

            typedef typename ::boost::mpl::transform<
                arg_types
              , ::boost::parameter::python::aux::make_kw_spec<
                    ::boost::mpl::_
                >
              , ::boost::mpl::back_inserter<boost::mpl::vector0<> >
            >::type arg_specs;

            typedef typename ::boost::mpl::count_if<
                arg_specs
              , ::boost::parameter::python::aux::is_optional< ::boost::mpl::_>
            >::type optional_arity;

            typedef typename ::boost::mpl::shift_left<
                ::boost::mpl::long_<1>
              , optional_arity
            >::type upper;

            typedef ::boost::parameter::python::aux::call_policies_as_options<
                CallPolicies
            > options;

            ::boost::parameter::python::aux::def_combinations(
                ::boost::parameter::python::aux::def_class<Class,options>(
                    cl
                  , "__call__"
                  , options(this->call_policies)
                )
              , static_cast<arg_specs*>(BOOST_TTI_DETAIL_NULLPTR)
              , ::boost::mpl::long_<0>()
              , ::boost::mpl::long_<upper::value>()
              , static_cast<
                    ::boost::parameter::python::aux::make_call_invoker<
                        typename Class::wrapped_type
                      , result_type
                    >*
                >(BOOST_TTI_DETAIL_NULLPTR)
            );
        }

        CallPolicies call_policies;
    };

    template <typename Fwd, typename ParameterSpecs>
    struct function 
      : ::boost::python::def_visitor<function<Fwd,ParameterSpecs> >
    {
        template <typename Class, typename Options>
        void visit(Class& cl, char const* name, Options const& options) const
        {
            typedef ::boost::mpl::iterator_range<
                typename ::boost::mpl::next<
                    typename ::boost::mpl::begin<ParameterSpecs>::type
                >::type
              , typename ::boost::mpl::end<ParameterSpecs>::type
            > arg_types;

            typedef typename ::boost::mpl::front<
                ParameterSpecs
            >::type result_type;

            typedef typename ::boost::mpl::transform<
                arg_types
              , ::boost::parameter::python::aux::make_kw_spec<
                    ::boost::mpl::_
                >
              , ::boost::mpl::back_inserter< ::boost::mpl::vector0<> >
            >::type arg_specs;

            typedef typename ::boost::mpl::count_if<
                arg_specs
              , ::boost::parameter::python::aux::is_optional< ::boost::mpl::_>
            >::type optional_arity;

            typedef typename ::boost::mpl::shift_left<
                ::boost::mpl::long_<1>
              , optional_arity
            >::type upper;

            ::boost::parameter::python::aux::def_combinations(
                ::boost::parameter::python::aux::def_class<
                    Class
                  , Options
                >(cl, name, options)
              , static_cast<arg_specs*>(BOOST_TTI_DETAIL_NULLPTR)
              , ::boost::mpl::long_<0>()
              , ::boost::mpl::long_<upper::value>()
              , static_cast<
                    ::boost::parameter::python::aux::make_member_invoker<
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

