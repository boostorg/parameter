// Copyright Daniel Wallin, David Abrahams 2005.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_CONFIG_050403_HPP
#define BOOST_PARAMETER_CONFIG_050403_HPP

#include <boost/move/detail/workaround.hpp>
#include <boost/fusion/container/deque/deque_fwd.hpp>

// Allow projects to #define BOOST_PARAMETER_DISABLE_PERFECT_FORWARDING
// to turn off perfect forwarding as necessary.  Otherwise, also require
// SFINAE support, needed explicitly by tagged_argument; correct function
// template ordering, needed by the code generation macros; and a variadic
// implementation of boost::fusion::deque, needed by parameters.
// -- Cromwell D. Enage
#if !defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING) && \
    !defined(BOOST_NO_SFINAE) && \
    !defined(BOOST_NO_FUNCTION_TEMPLATE_ORDERING) && \
    !defined(BOOST_PARAMETER_DISABLE_PERFECT_FORWARDING) && \
    defined(BOOST_FUSION_HAS_VARIADIC_DEQUE) && \
    defined(BOOST_MOVE_PERFECT_FORWARDING)
#define BOOST_PARAMETER_HAS_PERFECT_FORWARDING
#endif

#if !defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
#if !defined BOOST_PARAMETER_ALL_CONST_THRESHOLD_ARITY
#define BOOST_PARAMETER_ALL_CONST_THRESHOLD_ARITY 11
#endif
#endif

// Only in the absence of perfect forwarding should client code be limited
// by BOOST_PARAMETER_MAX_ARITY.  However, BOOST_PARAMETER_MAX_ARITY will
// remain defined for code that still needs it. -- Cromwell D. Enage
//#if !defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
#if !defined BOOST_PARAMETER_MAX_ARITY
#define BOOST_PARAMETER_MAX_ARITY 8
#endif
//#endif

#endif // BOOST_PARAMETER_CONFIG_050403_HPP

