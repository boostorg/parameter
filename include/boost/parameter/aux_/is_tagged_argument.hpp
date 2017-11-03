// Copyright Daniel Wallin, David Abrahams 2005.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_IS_TAGGED_ARGUMENT_HPP
#define BOOST_PARAMETER_IS_TAGGED_ARGUMENT_HPP

#include <boost/parameter/config.hpp>

#if 1//defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/remove_reference.hpp>
#else
#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/type_traits/is_reference.hpp>
#endif

namespace boost { namespace parameter { namespace aux {

struct tagged_argument_base
{
};

#if 0//!defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
template <class T>
struct is_tagged_argument_aux
  : is_convertible<T*,tagged_argument_base const*>
{
};
#endif

// This metafunction identifies tagged_argument specializations
// and their derived classes.
template <class T>
struct is_tagged_argument
#if 1//defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
    // Cannot use is_convertible<> to check if T is derived from
    // tagged_argument_base. -- Cromwell D. Enage
  : is_base_of<
        tagged_argument_base
      , typename remove_const<typename remove_reference<T>::type>::type
    >
#else
  : mpl::if_<
        is_reference<T>
      , mpl::false_
      , is_tagged_argument_aux<T>
    >::type
#endif
{
};
}}} // namespace boost::parameter::aux

#endif // BOOST_PARAMETER_IS_TAGGED_ARGUMENT_HPP

