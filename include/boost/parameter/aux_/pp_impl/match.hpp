// Copyright Daniel Wallin 2006.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_AUX_PP_IMPL_MATCH_HPP
#define BOOST_PARAMETER_AUX_PP_IMPL_MATCH_HPP

#include <boost/parameter/config.hpp>
#include <boost/parameter/aux_/void.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>

namespace boost { namespace parameter { namespace aux {

    // Recast the ParameterSpec's nested match metafunction
    // as a free metafunction.
    template <
        typename Parameters
      , BOOST_PP_ENUM_BINARY_PARAMS(
            BOOST_PARAMETER_MAX_ARITY
          , typename A
          , = ::boost::parameter::void_ BOOST_PP_INTERCEPT
        )
    >
    struct match
      : Parameters::BOOST_NESTED_TEMPLATE match<
            BOOST_PP_ENUM_PARAMS(BOOST_PARAMETER_MAX_ARITY, A)
        >
    {
    };
}}} // namespace boost::parameter::aux

#endif  // include guard

