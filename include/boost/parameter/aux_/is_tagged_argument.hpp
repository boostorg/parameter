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

#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/type_traits/is_lvalue_reference.hpp>

namespace boost { namespace parameter { namespace aux {

    template <typename T>
    struct is_tagged_argument_aux
      : ::boost::is_convertible<
            T*
          , ::boost::parameter::aux::tagged_argument_base const*
        >
    {
    };

    // This metafunction identifies tagged_argument specializations
    // and their derived classes.
    template <typename T>
    struct is_tagged_argument
      : ::boost::mpl::if_<
            ::boost::is_lvalue_reference<T>
          , ::boost::mpl::false_
          , ::boost::parameter::aux::is_tagged_argument_aux<T>
        >::type
    {
    };
}}} // namespace boost::parameter::aux

#endif  // include guard

