// Copyright Daniel Wallin 2006.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_AUX_NAME_HPP
#define BOOST_PARAMETER_AUX_NAME_HPP

namespace boost { namespace parameter { namespace aux {

    // Tag type passed to MPL lambda.
    struct lambda_tag;

    struct name_tag_base
    {
    };

    template <typename Tag>
    struct name_tag
    {
    };
}}} // namespace boost::parameter::aux

#include <boost/mpl/bool.hpp>

namespace boost { namespace parameter { namespace aux {

    template <typename T>
    struct is_name_tag : ::boost::mpl::false_
    {
    };
}}} // namespace boost::parameter::aux

#include <boost/parameter/value_type.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/config.hpp>
#include <boost/config/workaround.hpp>

#if !defined(BOOST_NO_SFINAE) && \
    !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x592))

#include <boost/mpl/lambda.hpp>
#include <boost/mpl/bind.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/core/enable_if.hpp>

namespace boost { namespace mpl {

    template <typename T>
    struct lambda<
        T
      , typename ::boost::enable_if<
            ::boost::parameter::aux::is_name_tag<T>
          , ::boost::parameter::aux::lambda_tag
        >::type
    >
    {
        typedef ::boost::mpl::true_ is_le;
        typedef ::boost::mpl::bind3<
            ::boost::mpl::quote3< ::boost::parameter::value_type>
          , ::boost::mpl::arg<2>
          , T
          , void
        > result_;
        typedef result_ type;
    };
}} // namespace boost::mpl

#endif // SFINAE enabled, not Borland.

#define BOOST_PARAMETER_TAG_PLACEHOLDER_TYPE(tag)                            \
    ::boost::parameter::value_type<                                          \
        ::boost::mpl::_2,tag,::boost::parameter::void_                       \
    >
/**/

#define BOOST_PARAMETER_NAME_EAT_in(x)
#define BOOST_PARAMETER_NAME_EAT_out(x)
#define BOOST_PARAMETER_NAME_EAT_in_out(x)
#define BOOST_PARAMETER_NAME_EAT_consume(x)
#define BOOST_PARAMETER_NAME_EAT_move_from(x)
#define BOOST_PARAMETER_NAME_EAT_forward(x)

#define BOOST_PARAMETER_GET_NAME_QUALIFIER_in(x) in_reference
#define BOOST_PARAMETER_GET_NAME_QUALIFIER_out(x) out_reference
#define BOOST_PARAMETER_GET_NAME_QUALIFIER_in_out(x) in_out_reference
#define BOOST_PARAMETER_GET_NAME_QUALIFIER_consume(x) consume_reference
#define BOOST_PARAMETER_GET_NAME_QUALIFIER_move_from(x) move_from_reference
#define BOOST_PARAMETER_GET_NAME_QUALIFIER_forward(x) forward_reference

#define BOOST_PARAMETER_STRIP_NAME_QUALIFIER_in(x) x
#define BOOST_PARAMETER_STRIP_NAME_QUALIFIER_out(x) x
#define BOOST_PARAMETER_STRIP_NAME_QUALIFIER_in_out(x) x
#define BOOST_PARAMETER_STRIP_NAME_QUALIFIER_consume(x) x
#define BOOST_PARAMETER_STRIP_NAME_QUALIFIER_move_from(x) x
#define BOOST_PARAMETER_STRIP_NAME_QUALIFIER_forward(x) x

#include <boost/preprocessor/cat.hpp>

#define BOOST_PARAMETER_GET_NAME_QUALIFIER_GET(x) \
    BOOST_PP_CAT(BOOST_PARAMETER_GET_NAME_QUALIFIER_, x)
/**/

#define BOOST_PARAMETER_UNQUALIFIED_NAME_GET(x) \
    BOOST_PP_CAT(BOOST_PARAMETER_STRIP_NAME_QUALIFIER_, x)
/**/

#include <boost/preprocessor/facilities/is_empty.hpp>

// Expands to 1 if x is either "in(k)", "out(k)", "in_out(k)", "consume(k)",
// "move_from(k)", or "forward(k)"; expands to 0 otherwise.
#define BOOST_PARAMETER_IS_NAME_QUALIFIER(x) \
    BOOST_PP_IS_EMPTY(BOOST_PP_CAT(BOOST_PARAMETER_NAME_EAT_, x))
/**/

#include <boost/preprocessor/control/iif.hpp>

// Expands to the qualifier of x, where x is either a keyword qualifier
// or a keyword.
//
//   k => forward_reference
//   in(k) => in_reference
//   out(k) => out_reference
//   in_out(k) => in_out_reference
//   forward(k) => forward_reference
//   consume(k) => consume_reference
//   move_from(k) => move_from_reference
#define BOOST_PARAMETER_GET_NAME_QUALIFIER(x) \
    BOOST_PP_IIF( \
        BOOST_PARAMETER_IS_NAME_QUALIFIER(x) \
      , BOOST_PARAMETER_GET_NAME_QUALIFIER_GET(x) \
      , forward_reference \
    )
/**/

// Expands to the unqualified name of x, where x is either a keyword qualifier
// or a keyword.
//
//   k => k
//   in(k) => k
//   out(k) => k
//   in_out(k) => k
//   forward(k) => k
//   consume(k) => k
//   move_from(k) => k
#define BOOST_PARAMETER_UNQUALIFIED_NAME(x) \
    BOOST_PP_IIF( \
        BOOST_PARAMETER_IS_NAME_QUALIFIER(x) \
      , BOOST_PARAMETER_UNQUALIFIED_NAME_GET(x) \
      , x \
    )
/**/

#endif // BOOST_PARAMETER_AUX_NAME_HPP

