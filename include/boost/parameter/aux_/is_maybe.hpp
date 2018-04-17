// Copyright Daniel Wallin, David Abrahams 2010.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_IS_MAYBE_050329_HPP
#define BOOST_PARAMETER_IS_MAYBE_050329_HPP

#include <boost/config.hpp>

#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/remove_const.hpp>
#else
#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <type_traits>
#endif

namespace boost { namespace parameter { namespace aux {

    struct maybe_base
    {
    };

    template <class T>
    struct is_maybe
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
      : boost::is_base_of<maybe_base,typename boost::remove_const<T>::type>
#else
      : boost::mpl::if_<
            std::is_base_of<maybe_base,typename std::remove_const<T>::type>
          , boost::mpl::true_
          , boost::mpl::false_
        >::type
#endif
    {
    };
}}} // namespace boost::parameter::aux

#endif // BOOST_PARAMETER_IS_MAYBE_050329_HPP

