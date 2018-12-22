// Copyright Daniel Wallin, David Abrahams 2005.
// Copyright Cromwell D. Enage 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_CONFIG_050403_HPP
#define BOOST_PARAMETER_CONFIG_050403_HPP

#include <boost/config.hpp>
#include <boost/config/workaround.hpp>

// Allow projects to #define BOOST_PARAMETER_DISABLE_PERFECT_FORWARDING to
// turn off perfect forwarding as necessary.  Otherwise, also require correct
// SFINAE support, needed explicitly by tagged_argument & keyword & cast;
// correct function template ordering, needed by the code generation macros;
// rvalue references, needed throughout; variadic templates, needed by
// parameters; and the ability to handle multiple parameter packs, needed by
// parameters.  Older versions of GCC either don't have the latter ability or
// cannot disambiguate between keyword's overloaded operators.  Older versions
// of Clang either fail to compile due to differences in length between
// parameter packs 'Args' and 'args' or fail at runtime due to segmentation
// faults.
// -- Cromwell D. Enage
#if !defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING) && \
    !defined(BOOST_PARAMETER_DISABLE_PERFECT_FORWARDING) && \
    !defined(BOOST_NO_SFINAE) && \
    !defined(BOOST_NO_FUNCTION_TEMPLATE_ORDERING) && \
    !defined(BOOST_NO_CXX11_RVALUE_REFERENCES) && \
    !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES) && \
    !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564)) && !( \
        defined(BOOST_CLANG) && (1 == BOOST_CLANG) && ( \
            (__clang_major__ < 3) || ( \
                (3 == __clang_major__) && (__clang_minor__ < 2) \
            ) \
        ) \
    ) && !BOOST_WORKAROUND(BOOST_GCC, < 40900)
#define BOOST_PARAMETER_HAS_PERFECT_FORWARDING
#endif

#if defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)
#if !defined(BOOST_PARAMETER_MAX_ARITY)
// Unlike the variadic MPL sequences provided by Boost.Fusion,
// boost::mpl::vector has a size limit. -- Cromwell D. Enage
#include <boost/mpl/limits/vector.hpp>
#define BOOST_PARAMETER_MAX_ARITY BOOST_MPL_LIMIT_VECTOR_SIZE
#endif
#else   // !defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)
#if !defined(BOOST_PARAMETER_EXPONENTIAL_OVERLOAD_THRESHOLD_ARITY)
#define BOOST_PARAMETER_EXPONENTIAL_OVERLOAD_THRESHOLD_ARITY 0
#endif
#if !defined(BOOST_PARAMETER_MAX_ARITY)
// Libraries such as Boost.Log need a higher maximum arity
// than the old value of 8. -- Cromwell D. Enage
#define BOOST_PARAMETER_MAX_ARITY 20
#endif
#endif  // BOOST_PARAMETER_HAS_PERFECT_FORWARDING
#endif  // include guard

