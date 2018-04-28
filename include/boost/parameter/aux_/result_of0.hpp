// Copyright David Abrahams 2005.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_AUX_RESULT_OF0_DWA2005511_HPP
#define BOOST_PARAMETER_AUX_RESULT_OF0_DWA2005511_HPP

#include <boost/parameter/aux_/use_default_tag.hpp>
#include <boost/parameter/config.hpp>
#include <boost/mpl/if.hpp>
#include <boost/utility/result_of.hpp>

#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
#include <boost/type_traits/is_void.hpp>
#else
#include <type_traits>
#endif

// A metafunction returning the result of invoking a nullary function object
// of the given type.

namespace boost { namespace parameter { namespace aux {

    template <typename F>
    struct result_of0
    {
#if defined(BOOST_NO_RESULT_OF)
        typedef typename F::result_type result_of_F;
#else
        typedef typename ::boost::result_of<F()>::type result_of_F;
#endif
        typedef typename ::boost::mpl::if_<
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
            ::boost::is_void<result_of_F>
#else
            ::std::is_void<result_of_F>
#endif
          , ::boost::parameter::aux::use_default_tag
          , result_of_F
        >::type type;
    };
}}} // namespace boost::parameter::aux

#endif // BOOST_PARAMETER_AUX_RESULT_OF0_DWA2005511_HPP

