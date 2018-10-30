// Copyright Daniel Wallin 2006.
// Copyright Cromwell D. Enage 2017.
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
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/remove_reference.hpp>

namespace boost { namespace parameter { namespace aux {

    template <typename T>
    struct is_template_keyword
      : ::boost::mpl::if_<
            ::boost::is_base_of<
                ::boost::parameter::aux::template_keyword_base
              , typename ::boost::remove_const<
                    typename ::boost::remove_reference<T>::type
                >::type
            >
          , ::boost::mpl::true_
          , ::boost::mpl::false_
        >::type
    {
    };
}}} // namespace boost::parameter::aux

#include <boost/type_traits/is_function.hpp>

#if defined(BOOST_NO_CXX11_HDR_FUNCTIONAL)
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
            ::boost::is_function<T>
#if defined(BOOST_NO_CXX11_HDR_FUNCTIONAL)
          , ::boost::function<T>
#else
          , ::std::function<T>
#endif
          , T
        >::type value_type;

        typedef value_type reference;
    };
}} // namespace boost::parameter

#endif  // include guard

