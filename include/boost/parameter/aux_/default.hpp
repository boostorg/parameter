// Copyright Daniel Wallin, David Abrahams 2005.
// Copyright Cromwell D. Enage 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef DEFAULT_050329_HPP
#define DEFAULT_050329_HPP

#include <boost/parameter/config.hpp>

namespace boost { namespace parameter { namespace aux {

    // A wrapper for the default value passed by the user when resolving
    // the value of the parameter with the given Keyword
    template <typename Keyword, typename Value>
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
    template <typename KW, typename DefaultComputer>
    struct lazy_default_base
    {
        inline lazy_default_base(DefaultComputer& x)
          : compute_default(x)
        {
        }

        DefaultComputer& compute_default;
    };

    template <typename KW, typename DefaultComputer>
    struct lazy_default
      : ::boost::parameter::aux::lazy_default_base<KW,DefaultComputer>
    {
        inline lazy_default(DefaultComputer& x)
          : ::boost::parameter::aux::lazy_default_base<KW,DefaultComputer>(x)
        {
        }
    };
#else   // !BOOST_WORKAROUND(__EDG_VERSION__, <= 300)
    template <typename KW, typename DefaultComputer>
    struct lazy_default
    {
        inline lazy_default(DefaultComputer& x) : compute_default(x)
        {
        }

        DefaultComputer& compute_default;
    };
#endif  // EDG workarounds needed.
}}} // namespace boost::parameter::aux

#if BOOST_WORKAROUND(__EDG_VERSION__, <= 300)
#define BOOST_PARAMETER_lazy_default_fallback \
    ::boost::parameter::aux::lazy_default_base
/**/
#else
#define BOOST_PARAMETER_lazy_default_fallback \
    ::boost::parameter::aux::lazy_default
/**/
#endif

#include <utility>

namespace boost { namespace parameter { namespace aux {

    template <typename Keyword, typename Value>
    struct default_r_
    {
        inline default_r_(Value&& x) : value(::std::forward<Value>(x))
        {
        }

        Value&& value;
    };
}}} // namespace boost::parameter::aux

#endif  // include guard

