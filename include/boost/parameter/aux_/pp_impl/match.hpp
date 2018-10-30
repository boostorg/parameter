// Copyright Daniel Wallin 2006.
// Copyright Cromwell D. Enage 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_AUX_PP_IMPL_MATCH_HPP
#define BOOST_PARAMETER_AUX_PP_IMPL_MATCH_HPP

#include <boost/parameter/config.hpp>

namespace boost { namespace parameter { namespace aux {

    // Recast the ParameterSpec's nested match metafunction
    // as a free metafunction.
    // No more limits set by BOOST_PARAMETER_MAX_ARITY. -- Cromwell D. Enage
    template <typename Parameters, typename ...Args>
    struct match : Parameters::BOOST_NESTED_TEMPLATE match<Args...>
    {
    };
}}} // namespace boost::parameter::aux

#endif  // include guard

