// Copyright Daniel Wallin 2006.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_AUX_PREPROCESSOR_QUALIFIER_HPP
#define BOOST_PARAMETER_AUX_PREPROCESSOR_QUALIFIER_HPP

#define BOOST_PARAMETER_QUALIFIER_EAT_out(x)
#define BOOST_PARAMETER_QUALIFIER_EAT_in_out(x)

#define BOOST_PARAMETER_STRIP_QUALIFIER_out(x) x
#define BOOST_PARAMETER_STRIP_QUALIFIER_in_out(x) x

#include <boost/preprocessor/cat.hpp>

#define BOOST_PARAMETER_GET_UNQUALIFIED(x) \
    BOOST_PP_CAT(BOOST_PARAMETER_STRIP_QUALIFIER_, x)
/**/

#include <boost/preprocessor/facilities/is_empty.hpp>

// Expands to 1 if x is either "out(k)" or "in_out(k)";
// expands to 0 otherwise.
#define BOOST_PARAMETER_IS_QUALIFIER(x) \
    BOOST_PP_IS_EMPTY(BOOST_PP_CAT(BOOST_PARAMETER_QUALIFIER_EAT_, x))
/**/

#include <boost/preprocessor/control/iif.hpp>

// Expands to the unqualified version of x,
// where x is either a keyword qualifier or a keyword.
//
//   k => k
//   out(k) => k
//   in_out(k) => k
#define BOOST_PARAMETER_UNQUALIFIED(x) \
    BOOST_PP_IIF( \
        BOOST_PARAMETER_IS_QUALIFIER(x) \
      , BOOST_PARAMETER_GET_UNQUALIFIED(x) \
      , x \
    )
/**/

#endif  // include guard

