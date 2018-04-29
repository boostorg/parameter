// Copyright Daniel Wallin 2006.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_TEMPLATE_KEYWORD_060203_HPP
#define BOOST_PARAMETER_TEMPLATE_KEYWORD_060203_HPP

namespace boost { namespace parameter { namespace aux {

    struct template_keyword_base
    {
    };
}}} // namespace boost::parameter::aux

#include <boost/parameter/config.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>

#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
#if defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/remove_reference.hpp>
#else
#include <boost/type_traits/is_convertible.hpp>
#include <boost/type_traits/is_lvalue_reference.hpp>
#endif // BOOST_PARAMETER_HAS_PERFECT_FORWARDING
#include <boost/type_traits/remove_const.hpp>
#else
#include <type_traits>
#endif // BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS

namespace boost { namespace parameter { namespace aux {

#if !defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)
    template <typename T>
    struct is_template_keyword_aux
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
      : ::boost::is_convertible<
#else
      : ::boost::mpl::if_<
            ::std::is_convertible<
#endif
                T*
              , ::boost::parameter::aux::template_keyword_base const*
            >
#if !defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
          , ::boost::mpl::true_
          , ::boost::mpl::false_
        >::type
#endif
    {
    };
#endif // BOOST_PARAMETER_HAS_PERFECT_FORWARDING

    template <typename T>
    struct is_template_keyword
#if defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)
        // Cannot use is_convertible<> to check if T is derived from
        // template_keyword_base. -- Cromwell D. Enage
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
      : ::boost::is_base_of<
            ::boost::parameter::aux::template_keyword_base
          , typename ::boost::remove_const<
                typename ::boost::remove_reference<T>::type
            >::type
        >
#else
      : ::boost::mpl::if_<
            ::std::is_base_of<
                ::boost::parameter::aux::template_keyword_base
              , typename ::std::remove_const<
                    typename ::std::remove_reference<T>::type
                >::type
            >
          , ::boost::mpl::true_
          , ::boost::mpl::false_
        >::type
#endif
#else // !defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)
      : ::boost::mpl::if_<
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
            ::boost::is_lvalue_reference<T>
#else
            ::std::is_lvalue_reference<T>
#endif
          , ::boost::mpl::false_
          , ::boost::parameter::aux::is_template_keyword_aux<T>
        >::type
#endif // BOOST_PARAMETER_HAS_PERFECT_FORWARDING
    {
    };
}}} // namespace boost::parameter::aux

#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
#include <boost/type_traits/is_function.hpp>
#endif

#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_FUNCTIONAL)
#include <boost/function.hpp>
#else
#include <functional>
#endif

namespace boost { namespace parameter { 

    template <typename Tag, typename T>
    struct template_keyword : ::boost::parameter::aux::template_keyword_base
    {
        typedef Tag key_type;

        // Wrap plain (non-UDT) function objects in either
        // a boost::function or a std::function. -- Cromwell D. Enage
        typedef typename ::boost::mpl::if_<
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
            ::boost::is_function<T>
#else
            ::std::is_function<T>
#endif
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_FUNCTIONAL)
          , ::boost::function<T>
#else
          , ::std::function<T>
#endif
          , T
        >::type value_type;

        typedef value_type reference;
    };
}} // namespace boost::parameter

#endif // BOOST_PARAMETER_TEMPLATE_KEYWORD_060203_HPP

