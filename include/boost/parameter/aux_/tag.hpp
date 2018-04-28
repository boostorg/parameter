// Copyright David Abrahams 2005.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_AUX_TAG_DWA2005610_HPP
#define BOOST_PARAMETER_AUX_TAG_DWA2005610_HPP

#include <boost/parameter/aux_/unwrap_cv_reference.hpp>
#include <boost/parameter/aux_/tagged_argument.hpp>
#include <boost/parameter/config.hpp>
#include <boost/mpl/bool.hpp>

#if defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)
#include <boost/mpl/if.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>

#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/is_scalar.hpp>
#include <boost/type_traits/is_lvalue_reference.hpp>
#include <boost/type_traits/remove_const.hpp>
#else
#include <type_traits>
#endif

#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
#include <boost/type_traits/remove_reference.hpp>
#endif
#endif
#endif

namespace boost { namespace parameter { namespace aux { 

    template <
        typename Keyword
      , typename ActualArg
#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
      , typename = typename ::boost::parameter::aux::is_cv_reference_wrapper<
            ActualArg
        >::type
#endif
    >
    struct tag
    {
#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564)) || \
    !defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)
        typedef ::boost::parameter::aux::tagged_argument<
            Keyword
          , typename ::boost::parameter::aux::unwrap_cv_reference<
                ActualArg
            >::type
        > type;
#else
        typedef typename ::boost::parameter::aux::unwrap_cv_reference<
            ActualArg
        >::type Arg;
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
        typedef typename ::boost::add_const<Arg>::type ConstArg;
        typedef typename ::boost::remove_const<Arg>::type MutArg;
#else
        typedef typename ::std::add_const<Arg>::type ConstArg;
        typedef typename ::std::remove_const<Arg>::type MutArg;
#endif
        typedef typename ::boost::mpl::eval_if<
            typename ::boost::mpl::if_<
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
                ::boost::is_lvalue_reference<ActualArg>
#else
                ::std::is_lvalue_reference<ActualArg>
#endif
              , ::boost::mpl::true_
              , ::boost::parameter::aux::is_cv_reference_wrapper<ActualArg>
            >::type
          , ::boost::mpl::identity<
                ::boost::parameter::aux::tagged_argument<Keyword,Arg>
            >
          , ::boost::mpl::if_<
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
                ::boost::is_scalar<MutArg>
#else
                ::std::is_scalar<MutArg>
#endif
              , ::boost::parameter::aux::tagged_argument<Keyword,ConstArg>
              , ::boost::parameter::aux::tagged_argument_rref<Keyword,Arg>
            >
        >::type type;
#endif // Borland/forwarding workarounds needed.
    };

#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
    template <typename Keyword, typename ActualArg>
    struct tag<Keyword,ActualArg,::boost::mpl::false_>
    {
#if defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
        typedef typename ::boost::remove_reference<ActualArg>::type Arg;
        typedef typename ::boost::add_const<Arg>::type ConstArg;
        typedef typename ::boost::remove_const<Arg>::type MutArg;
#else
        typedef typename ::std::remove_reference<ActualArg>::type Arg;
        typedef typename ::std::add_const<Arg>::type ConstArg;
        typedef typename ::std::remove_const<Arg>::type MutArg;
#endif
        typedef typename ::boost::mpl::eval_if<
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
            ::boost::is_lvalue_reference<ActualArg>
#else
            ::std::is_lvalue_reference<ActualArg>
#endif
          , ::boost::mpl::identity<
                ::boost::parameter::aux::tagged_argument<Keyword,Arg>
            >
          , ::boost::mpl::if_<
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
                ::boost::is_scalar<MutArg>
#else
                ::std::is_scalar<MutArg>
#endif
              , ::boost::parameter::aux::tagged_argument<Keyword,ConstArg>
              , ::boost::parameter::aux::tagged_argument_rref<Keyword,Arg>
            >
        >::type type;
#else // !defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)
        typedef ::boost::parameter::aux::tagged_argument<
            Keyword
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
          , typename ::boost::remove_reference<ActualArg>::type
#else
          , typename ::std::remove_reference<ActualArg>::type
#endif
        > type;
#endif // BOOST_PARAMETER_HAS_PERFECT_FORWARDING
    };
#endif // Borland workarounds needed.
}}} // namespace boost::parameter::aux_

#endif // BOOST_PARAMETER_AUX_TAG_DWA2005610_HPP

