// Copyright Daniel Wallin, David Abrahams 2005.
// Copyright Cromwell D. Enage 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_CONFIG_050403_HPP
#define BOOST_PARAMETER_CONFIG_050403_HPP

#include <boost/move/detail/workaround.hpp>
#include <boost/config/workaround.hpp>

// Require correct SFINAE support, needed explicitly by tagged_argument &
// keyword & cast; correct function template ordering, needed by the code
// generation macros; and the ability to handle multiple parameter packs,
// needed by parameters.  Older versions of GCC either don't have the latter
// ability or cannot disambiguate between keyword's overloaded
// operators.
// -- Cromwell D. Enage
#if defined(BOOST_NO_SFINAE) || \
    defined(BOOST_NO_FUNCTION_TEMPLATE_ORDERING) || \
    BOOST_WORKAROUND(BOOST_GCC, < 40900) || \
    !defined(BOOST_MOVE_PERFECT_FORWARDING)
#error Your compiler does not support perfect forwarding.
#endif

#include <boost/mpl/limits/vector.hpp>

// Unlike the variadic MPL sequences provided by Boost.Fusion,
// boost::mpl::vector has a size limit. -- Cromwell D. Enage
#if !defined(BOOST_PARAMETER_MAX_ARITY)
#define BOOST_PARAMETER_MAX_ARITY BOOST_MPL_LIMIT_VECTOR_SIZE
#endif
#endif  // include guard

