// Copyright Daniel Wallin, David Abrahams 2005.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_IS_TAGGED_ARGUMENT_HPP
#define BOOST_PARAMETER_IS_TAGGED_ARGUMENT_HPP

namespace boost { namespace parameter { namespace aux {

    struct tagged_argument_base
    {
    };
}}} // namespace boost::parameter::aux

#include <boost/parameter/config.hpp>

#if 1//defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/remove_reference.hpp>
#else
#include <type_traits>
#endif
#else
#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/type_traits/is_lvalue_reference.hpp>
#endif

namespace boost { namespace parameter { namespace aux {

#if 0//!defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)
    template <typename T>
    struct is_tagged_argument_aux
      : ::boost::is_convertible<
            T*
          , ::boost::parameter::aux::tagged_argument_base const*
        >
    {
    };
#endif

    // This metafunction identifies tagged_argument specializations
    // and their derived classes.
    template <typename T>
    struct is_tagged_argument
#if 1//defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)
        // Cannot use is_convertible<> to check if T is derived from
        // tagged_argument_base. -- Cromwell D. Enage
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
      : ::boost::is_base_of<
            ::boost::parameter::aux::tagged_argument_base
          , typename ::boost::remove_const<
                typename ::boost::remove_reference<T>::type
            >::type
        >
#else
      : ::std::is_base_of<
            ::boost::parameter::aux::tagged_argument_base
          , typename ::std::remove_const<
                typename ::std::remove_reference<T>::type
            >::type
        >
#endif
#else
      : ::boost::mpl::if_<
            ::boost::is_lvalue_reference<T>
          , ::boost::mpl::false_
          , ::boost::parameter::aux::is_tagged_argument_aux<T>
        >::type
#endif
    {
    };
}}} // namespace boost::parameter::aux

#endif // BOOST_PARAMETER_IS_TAGGED_ARGUMENT_HPP

