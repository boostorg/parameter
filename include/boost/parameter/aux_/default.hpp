// Copyright Daniel Wallin, David Abrahams 2005.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef DEFAULT_050329_HPP
#define DEFAULT_050329_HPP

#include <boost/parameter/config.hpp>

namespace boost { namespace parameter { namespace aux {

    // A wrapper for the default value passed by the user when resolving
    // the value of the parameter with the given Keyword
    template <class Keyword, class Value>
    struct default_
    {
        inline default_(Value& x) : value(x)
        {
        }

        Value& value;
    };

    // lazy_default -- A wrapper for the default value computation function
    // passed by the user when resolving the value of the parameter with the
    // given keyword.
#if BOOST_WORKAROUND(__EDG_VERSION__, <= 300)
    // These compilers need a little extra help with overload resolution;
    // we have empty_arg_list's operator[] accept a base class
    // to make that overload less preferable.
    template <class KW, class DefaultComputer>
    struct lazy_default_base
    {
        inline lazy_default_base(DefaultComputer const& x)
          : compute_default(x)
        {
        }

        DefaultComputer const& compute_default;
    };

    template <class KW, class DefaultComputer>
    struct lazy_default
      : boost::parameter::aux::lazy_default_base<KW,DefaultComputer>
    {
        inline lazy_default(DefaultComputer const& x)
          : boost::parameter::aux::lazy_default_base<KW,DefaultComputer>(x)
        {
        }
    };
#else // !BOOST_WORKAROUND(__EDG_VERSION__, <= 300)
    template <class KW, class DefaultComputer>
    struct lazy_default
    {
        inline lazy_default(DefaultComputer const& x) : compute_default(x)
        {
        }

        DefaultComputer const& compute_default;
    };
#endif // EDG workarounds needed.
}}} // namespace boost::parameter::aux

#if BOOST_WORKAROUND(__EDG_VERSION__, <= 300)
#define BOOST_PARAMETER_lazy_default_fallback \
    boost::parameter::aux::lazy_default_base
/**/
#else
#define BOOST_PARAMETER_lazy_default_fallback \
    boost::parameter::aux::lazy_default
/**/
#endif

#if defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)

#include <boost/move/utility_core.hpp>

namespace boost { namespace parameter { namespace aux {

    template <class Keyword, class Value>
    struct default_r_
    {
        inline default_r_(Value&& x) : value(boost::forward<Value>(x))
        {
        }

        Value&& value;
    };
}}} // namespace boost::parameter::aux

#endif // BOOST_PARAMETER_HAS_PERFECT_FORWARDING

#endif // DEFAULT_050329_HPP

