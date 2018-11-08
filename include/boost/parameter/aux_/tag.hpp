// Copyright David Abrahams 2005.
// Copyright Cromwell D. Enage 2017.
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
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/is_scalar.hpp>
#include <boost/type_traits/is_lvalue_reference.hpp>
#include <boost/type_traits/remove_const.hpp>

namespace boost { namespace parameter { namespace aux { 

    template <typename Keyword, typename ActualArg>
    struct tag
    {
        typedef typename ::boost::parameter::aux
        ::unwrap_cv_reference<ActualArg>::type Arg;
        typedef typename ::boost::add_const<Arg>::type ConstArg;
        typedef typename ::boost::remove_const<Arg>::type MutArg;
        typedef typename ::boost::mpl::eval_if<
            typename ::boost::mpl::if_<
                ::boost::is_lvalue_reference<ActualArg>
              , ::boost::mpl::true_
              , ::boost::parameter::aux::is_cv_reference_wrapper<ActualArg>
            >::type
          , ::boost::mpl::identity<
                ::boost::parameter::aux::tagged_argument<Keyword,Arg>
            >
          , ::boost::mpl::if_<
                ::boost::is_scalar<MutArg>
              , ::boost::parameter::aux::tagged_argument<Keyword,ConstArg>
              , ::boost::parameter::aux::tagged_argument_rref<Keyword,Arg>
            >
        >::type type;
    };
}}} // namespace boost::parameter::aux_

#else   // !defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)

namespace boost { namespace parameter { namespace aux { 

    template <
        typename Keyword
      , typename ActualArg
#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
      , typename = typename ::boost::parameter::aux
        ::is_cv_reference_wrapper<ActualArg>::type
#endif
    >
    struct tag
    {
        typedef ::boost::parameter::aux::tagged_argument<
            Keyword
          , typename ::boost::parameter::aux
            ::unwrap_cv_reference<ActualArg>::type
        > type;
    };
}}} // namespace boost::parameter::aux_

#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
#include <boost/type_traits/remove_reference.hpp>

namespace boost { namespace parameter { namespace aux { 

    template <typename Keyword, typename ActualArg>
    struct tag<Keyword,ActualArg,::boost::mpl::false_>
    {
        typedef ::boost::parameter::aux::tagged_argument<
            Keyword
          , typename ::boost::remove_reference<ActualArg>::type
        > type;
    };
}}} // namespace boost::parameter::aux_

#endif  // Borland workarounds needed.
#endif  // BOOST_PARAMETER_HAS_PERFECT_FORWARDING
#endif  // include guard

