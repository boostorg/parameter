// Copyright Daniel Wallin, David Abrahams 2005.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_TAGGED_ARGUMENT_050328_HPP
#define BOOST_PARAMETER_TAGGED_ARGUMENT_050328_HPP

#include <boost/parameter/aux_/tagged_argument_fwd.hpp>
#include <boost/parameter/aux_/is_tagged_argument.hpp>
#include <boost/parameter/aux_/unwrap_cv_reference.hpp>
#include <boost/parameter/aux_/void.hpp>
#include <boost/parameter/aux_/arg_list.hpp>
#include <boost/parameter/aux_/result_of0.hpp>
#include <boost/parameter/config.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/apply_wrap.hpp>

#if defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
#include <boost/mpl/identity.hpp>
#include <boost/move/utility_core.hpp>
#include <boost/core/enable_if.hpp>
#endif

#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_function.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/config.hpp>

#if defined BOOST_NO_CXX11_HDR_FUNCTIONAL
#include <boost/function.hpp>
#else
#include <functional>
#endif

namespace boost { namespace parameter { namespace aux {

// Holds an lvalue reference to an argument of type Arg associated with
// keyword Keyword
template <class Keyword, class Arg>
class tagged_argument : tagged_argument_base
{
    typedef typename boost::remove_const<Arg>::type arg_type;

 public:
    typedef Keyword key_type;

    // Wrap plain (non-UDT) function objects in either
    // a boost::function or a std::function. -- Cromwell D. Enage
    typedef typename mpl::if_<
        boost::is_function<arg_type>
#if defined BOOST_NO_CXX11_HDR_FUNCTIONAL
      , boost::function<arg_type>
#else
      , std::function<arg_type>
#endif
      , Arg
    >::type value_type;

    // If Arg is void_, then this type will evaluate to void_&.
    // If the supplied argument is a plain function, then this type
    // will evaluate to a reference-to-const function wrapper type.
    // If the supplied argument is an lvalue, then Arg will be deduced
    // to the lvalue reference. -- Cromwell D. Enage
    typedef typename mpl::if_<
        boost::is_function<arg_type>
      , value_type const&
      , Arg&
    >::type reference;

 private:
    // Store plain functions by value, everything else by reference.
    // -- Cromwell D. Enage
    typename mpl::if_<
        boost::is_function<arg_type>
      , value_type
      , reference
    >::type value;

 public:
    template <typename A>
    inline explicit tagged_argument(A& x) : value(x)
    {
    }

    inline tagged_argument(tagged_argument const& copy) : value(copy.value)
    {
    }

    // A metafunction class that, given a keyword and a default type, returns
    // the appropriate result type for a keyword lookup given that default.
    struct binding
    {
        template <class KW, class Default, class Reference>
        struct apply
          : mpl::eval_if<
                boost::is_same<KW,key_type>
              , mpl::if_<Reference,reference,value_type>
              , mpl::identity<Default>
            >
        {
        };
    };

    // Comma operator to compose argument list without using parameters<>.
    // Useful for argument lists with undetermined length.
    template <class Keyword2, class Arg2>
    inline arg_list<
        tagged_argument<Keyword,Arg>
      , arg_list<tagged_argument<Keyword2,Arg2> > 
    >
    operator,(tagged_argument<Keyword2,Arg2> const& x) const
    {
        return arg_list<
            tagged_argument<Keyword,Arg>
          , arg_list<tagged_argument<Keyword2,Arg2> > 
        >(
            *this
          , arg_list<tagged_argument<Keyword2,Arg2> >(x, empty_arg_list())
        );
    }

#if defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
    template <class Keyword2, class Arg2>
    inline arg_list<
        tagged_argument<Keyword,Arg>
      , arg_list<tagged_argument_rref<Keyword2,Arg2> > 
    >
    operator,(tagged_argument_rref<Keyword2,Arg2> const& x) const
    {
        return arg_list<
            tagged_argument<Keyword,Arg>
          , arg_list<tagged_argument_rref<Keyword2,Arg2> > 
        >(
            *this
          , arg_list<tagged_argument_rref<Keyword2,Arg2> >(x, empty_arg_list())
        );
    }
#endif

    // Accessor interface.
    inline reference get_value() const
    {
        return this->value;
    }

    inline reference operator[](keyword<Keyword> const&) const
    {
        return this->get_value();
    }

#if defined(BOOST_NO_FUNCTION_TEMPLATE_ORDERING) || \
    BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
    template <class KW, class Default>
    inline Default& get_with_default(default_<KW,Default> const& x, int) const
    {
        return x.value;
    }

    template <class Default>
    inline reference
    get_with_default(default_<key_type,Default> const&, long) const
    {
        return this->get_value();
    }

    template <class KW, class Default>
    inline typename mpl::apply_wrap3<binding,KW,Default&,mpl::true_>::type
    operator[](default_<KW,Default> const& x) const
    {
        return this->get_with_default(x, 0L);
    }

    template <class KW, class F>
    inline typename result_of0<F>::type 
    get_with_lazy_default(lazy_default<KW,F> const& x, int) const
    {
        return x.compute_default();
    }

    template <class F>
    inline reference
    get_with_lazy_default(lazy_default<key_type,F> const&, long) const
    {
        return this->get_value();
    }

    template <class KW, class F>
    inline typename mpl::apply_wrap3<
        binding
      , KW
      , typename result_of0<F>::type
      , mpl::true_
    >::type
    operator[](lazy_default<KW,F> const& x) const
    {
        return this->get_with_lazy_default(x, 0L);
    }
#else
//#if !defined(BOOST_NO_FUNCTION_TEMPLATE_ORDERING) && \
//    !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
    template <class Default>
    inline reference operator[](default_<key_type,Default> const&) const
    {
        return this->get_value();
    }

    template <class F>
    inline reference operator[](lazy_default<key_type,F> const&) const
    {
        return this->get_value();
    }

    template <class KW, class Default>
    inline Default& operator[](default_<KW,Default> const& x) const
    {
        return x.value;
    }

    template <class KW, class F>
    inline typename result_of0<F>::type
    operator[](lazy_default<KW,F> const& x) const
    {
        return x.compute_default();
    }

    template <class ParameterRequirements>
    static typename ParameterRequirements::has_default
    satisfies(ParameterRequirements*);

    template <class HasDefault, class Predicate>
    static typename mpl::apply_wrap1<Predicate,value_type>::type
    satisfies(parameter_requirements<key_type,Predicate,HasDefault>*);
#endif // Function template ordering, Borland workarounds needed.

#if BOOST_WORKAROUND(BOOST_MSVC, BOOST_TESTED_AT(1310))
    // warning suppression
 private:
    void operator=(tagged_argument const&);

 public:    
#endif
    // MPL sequence support
    typedef tagged_argument type;     // Convenience for users
    typedef empty_arg_list tail_type; // For the benefit of iterators
    typedef arg_list_tag tag; // For dispatching to sequence intrinsics
};

#if defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
// Holds an rvalue reference to an argument of type Arg associated with
// keyword Keyword
template <class Keyword, class Arg>
struct tagged_argument_rref : tagged_argument_base
{
    typedef Keyword key_type;
    typedef Arg value_type;
    typedef Arg&& reference;

 private:
    reference value;

 public:
    inline explicit tagged_argument_rref(reference x)
      : value(static_cast<reference>(x))
    {
    }

    inline tagged_argument_rref(tagged_argument_rref const& copy)
      : value(static_cast<reference>(copy.value))
    {
    }

    // A metafunction class that, given a keyword and a default type, returns
    // the appropriate result type for a keyword lookup given that default.
    struct binding
    {
        template <class KW, class Default, class Reference>
        struct apply
          : mpl::eval_if<
                boost::is_same<KW,key_type>
              , mpl::if_<Reference,reference,value_type>
              , mpl::identity<Default>
            >
        {
        };
    };

    // Comma operator to compose argument list without using parameters<>.
    // Useful for argument lists with undetermined length.
    template <class Keyword2, class Arg2>
    inline arg_list<
        tagged_argument_rref<Keyword,Arg>
      , arg_list<tagged_argument<Keyword2,Arg2> > 
    >
    operator,(tagged_argument<Keyword2,Arg2> const& x) const
    {
        return arg_list<
            tagged_argument_rref<Keyword,Arg>
          , arg_list<tagged_argument<Keyword2,Arg2> > 
        >(
            *this
          , arg_list<tagged_argument<Keyword2,Arg2> >(x, empty_arg_list())
        );
    }

    template <class Keyword2, class Arg2>
    inline arg_list<
        tagged_argument_rref<Keyword,Arg>
      , arg_list<tagged_argument_rref<Keyword2,Arg2> > 
    >
    operator,(tagged_argument_rref<Keyword2,Arg2> const& x) const
    {
        return arg_list<
            tagged_argument_rref<Keyword,Arg>
          , arg_list<tagged_argument_rref<Keyword2,Arg2> > 
        >(
            *this
          , arg_list<tagged_argument_rref<Keyword2,Arg2> >(x, empty_arg_list())
        );
    }

    // Accessor interface.
    inline reference get_value() const
    {
        return boost::move(this->value);
    }

    inline reference operator[](keyword<Keyword> const&) const
    {
        return this->get_value();
    }

#if defined(BOOST_NO_FUNCTION_TEMPLATE_ORDERING) || \
    BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
    template <class KW, class Default>
    inline Default& get_with_default(default_<KW,Default> const& x, int) const
    {
        return x.value;
    }

    template <class Default>
    inline reference
    get_with_default(default_<key_type,Default> const&, long) const
    {
        return this->get_value();
    }

    template <class KW, class Default>
    inline typename mpl::apply_wrap3<binding,KW,Default&,mpl::true_>::type
    operator[](default_<KW,Default> const& x) const
    {
        return this->get_with_default(x, 0L);
    }

    template <class KW, class F>
    inline typename result_of0<F>::type 
    get_with_lazy_default(lazy_default<KW,F> const& x, int) const
    {
        return x.compute_default();
    }

    template <class F>
    inline reference
    get_with_lazy_default(lazy_default<key_type,F> const&, long) const
    {
        return this->get_value();
    }

    template <class KW, class F>
    inline typename mpl::apply_wrap3<
        binding
      , KW
      , typename result_of0<F>::type
      , mpl::true_
    >::type
    operator[](lazy_default<KW,F> const& x) const
    {
        return this->get_with_lazy_default(x, 0L);
    }
#else
//#if !defined(BOOST_NO_FUNCTION_TEMPLATE_ORDERING) && \
//    !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
    template <class Default>
    inline reference operator[](default_<key_type,Default> const&) const
    {
        return this->get_value();
    }

    template <class F>
    inline reference operator[](lazy_default<key_type,F> const&) const
    {
        return this->get_value();
    }

    template <class KW, class Default>
    inline Default& operator[](default_<KW,Default> const& x) const
    {
        return x.value;
    }

    template <class KW, class F>
    inline typename result_of0<F>::type
    operator[](lazy_default<KW,F> const& x) const
    {
        return x.compute_default();
    }

    template <class ParameterRequirements>
    static typename ParameterRequirements::has_default
    satisfies(ParameterRequirements*);

    template <class HasDefault, class Predicate>
    static typename mpl::apply_wrap1<Predicate,value_type>::type
    satisfies(parameter_requirements<key_type,Predicate,HasDefault>*);
#endif // Function template ordering, Borland workarounds needed.

#if BOOST_WORKAROUND(BOOST_MSVC, BOOST_TESTED_AT(1310))
    // warning suppression
 private:
    void operator=(tagged_argument_rref const&);

 public:    
#endif
    // MPL sequence support
    typedef tagged_argument_rref type; // Convenience for users
    typedef empty_arg_list tail_type;  // For the benefit of iterators
    typedef arg_list_tag tag; // For dispatching to sequence intrinsics
};
#endif // BOOST_PARAMETER_HAS_PERFECT_FORWARDING
}}} // namespace boost::parameter::aux

#endif // BOOST_PARAMETER_TAGGED_ARGUMENT_050328_HPP

