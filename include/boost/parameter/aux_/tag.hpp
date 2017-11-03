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

#if defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_lvalue_reference.hpp>

#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
#include <boost/type_traits/remove_reference.hpp>
#endif
#endif

namespace boost { namespace parameter { namespace aux { 

template <
    class Keyword
  , class ActualArg
#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
  , class = typename is_cv_reference_wrapper<ActualArg>::type
#endif
>
struct tag
{
#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564)) || \
    !defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
    typedef tagged_argument<
        Keyword
      , typename unwrap_cv_reference<ActualArg>::type
    > type;
#else
    typedef typename unwrap_cv_reference<ActualArg>::type Arg;
    typedef typename mpl::if_<
        typename mpl::if_<
            is_cv_reference_wrapper<ActualArg>
          , mpl::true_
          , is_lvalue_reference<ActualArg>
        >::type
      , tagged_argument<Keyword,Arg>
      , tagged_argument_rref<Keyword,Arg>
    >::type type;
#endif
};

#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
template <class Keyword, class ActualArg>
struct tag<Keyword,ActualArg,mpl::false_>
{
#if defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
    typedef typename remove_reference<ActualArg>::type Arg;
    typedef typename mpl::if_<
        is_lvalue_reference<ActualArg>
      , tagged_argument<Keyword,Arg>
      , tagged_argument_rref<Keyword,Arg>
    >::type type;
#else
    typedef tagged_argument<
        Keyword
      , typename remove_reference<ActualArg>::type
    > type;
#endif
};
#endif 
}}} // namespace boost::parameter::aux_

#endif // BOOST_PARAMETER_AUX_TAG_DWA2005610_HPP

