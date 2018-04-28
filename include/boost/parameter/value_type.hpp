// Copyright Daniel Wallin 2006.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_VALUE_TYPE_060921_HPP
#define BOOST_PARAMETER_VALUE_TYPE_060921_HPP

#include <boost/parameter/aux_/void.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/apply_wrap.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/config.hpp>

#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
#include <boost/type_traits/is_same.hpp>
#else
#include <type_traits>
#endif

#include <boost/config/workaround.hpp>

#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
namespace boost { namespace parameter {

    template <typename Parameters, typename Keyword, typename Default>
    struct value_type0
    {
        typedef typename ::boost::mpl::apply_wrap3<
            typename Parameters::binding
          , Keyword
          , Default
          , ::boost::mpl::false_
        >::type type;
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
        BOOST_MPL_ASSERT_NOT((
            typename ::boost::mpl::if_<
                ::boost::is_same<Default,::boost::parameter::void_>
              , ::boost::is_same<type,::boost::parameter::void_>
              , ::boost::mpl::false_
            >::type
        ));
#else
        BOOST_MPL_ASSERT_NOT((
            typename ::boost::mpl::eval_if<
                ::std::is_same<Default,::boost::parameter::void_>
              , ::boost::mpl::if_<
                    ::std::is_same<type,::boost::parameter::void_>
                  , ::boost::mpl::true_
                  , ::boost::mpl::false_
                >
              , ::boost::mpl::false_
            >::type
        ));
#endif // BOOST_NO_CXX11_HDR_TYPE_TRAITS
    };
}} // namespace boost::parameter

#include <boost/mpl/is_placeholder.hpp>
#include <boost/mpl/identity.hpp>
#endif // Borland workarounds needed.

#include <boost/mpl/aux_/lambda_support.hpp>

namespace boost { namespace parameter {

    // A metafunction that, given an argument pack, returns the type of the
    // parameter identified by the given keyword.  If no such parameter has
    // been specified, returns Default.
    template <
        typename Parameters
      , typename Keyword
      , typename Default = ::boost::parameter::void_
    >
    struct value_type
#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
      : ::boost::mpl::eval_if<
            ::boost::mpl::is_placeholder<Parameters>
          , ::boost::mpl::identity<int>
          , ::boost::parameter::value_type0<Parameters,Keyword,Default>
        >
#endif
    {
#if !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
        typedef typename ::boost::mpl::apply_wrap3<
            typename Parameters::binding
          , Keyword
          , Default
          , ::boost::mpl::false_
        >::type type;
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
        BOOST_MPL_ASSERT_NOT((
            typename ::boost::mpl::if_<
                ::boost::is_same<Default,::boost::parameter::void_>
              , ::boost::is_same<type,::boost::parameter::void_>
              , ::boost::mpl::false_
            >::type
        ));
#else
        BOOST_MPL_ASSERT_NOT((
            typename ::boost::mpl::eval_if<
                ::std::is_same<Default,::boost::parameter::void_>
              , ::boost::mpl::if_<
                    ::std::is_same<type,::boost::parameter::void_>
                  , ::boost::mpl::true_
                  , ::boost::mpl::false_
                >
              , ::boost::mpl::false_
            >::type
        ));
#endif // BOOST_NO_CXX11_HDR_TYPE_TRAITS
#endif // Borland workarounds not needed.
        BOOST_MPL_AUX_LAMBDA_SUPPORT(
            3, value_type, (Parameters, Keyword, Default)
        )
    };
}} // namespace boost::parameter

#include <boost/parameter/aux_/result_of0.hpp>

namespace boost { namespace parameter {

    // A metafunction that, given an argument pack, returns the type of the
    // parameter identified by the given keyword.  If no such parameter has
    // been specified, returns the type returned by invoking DefaultFn.
    template <typename Parameters, typename Keyword, typename DefaultFn>
    struct lazy_value_type
      : ::boost::mpl::apply_wrap3<
            typename Parameters::binding
          , Keyword
          , typename ::boost::parameter::aux::result_of0<DefaultFn>::type
          , ::boost::mpl::false_
        >
    {
    };
}} // namespace boost::parameter

#endif // BOOST_PARAMETER_VALUE_TYPE_060921_HPP

