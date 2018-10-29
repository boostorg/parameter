// Copyright Daniel Wallin, David Abrahams 2005.
// Copyright Cromwell D. Enage 2017.
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
#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>

#if ( \
    !defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING) && \
    (0 == BOOST_PARAMETER_EXPONENTIAL_OVERLOAD_THRESHOLD_ARITY) \
)
#include <boost/type_traits/is_convertible.hpp>
#include <boost/type_traits/is_lvalue_reference.hpp>
#else
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/remove_reference.hpp>
#endif  // no perfect forwarding and no exponential overloads

namespace boost { namespace parameter { namespace aux {

#if ( \
    !defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING) && \
    (0 == BOOST_PARAMETER_EXPONENTIAL_OVERLOAD_THRESHOLD_ARITY) \
)
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
      : ::boost::mpl::if_<
#if ( \
    !defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING) && \
    (0 == BOOST_PARAMETER_EXPONENTIAL_OVERLOAD_THRESHOLD_ARITY) \
)
            ::boost::is_lvalue_reference<T>
          , ::boost::mpl::false_
          , ::boost::parameter::aux::is_tagged_argument_aux<T>
#else
            // Cannot use is_convertible<> to check if T is derived from
            // tagged_argument_base. -- Cromwell D. Enage
            ::boost::is_base_of<
                ::boost::parameter::aux::tagged_argument_base
              , typename ::boost::remove_const<
                    typename ::boost::remove_reference<T>::type
                >::type
            >
          , ::boost::mpl::true_
          , ::boost::mpl::false_
#endif  // no perfect forwarding and no exponential overloads
        >::type
    {
    };
}}} // namespace boost::parameter::aux

#endif  // include guard

