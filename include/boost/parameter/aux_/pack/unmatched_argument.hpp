// Copyright David Abrahams, Daniel Wallin 2003.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_AUX_PACK_UNMATCHED_ARGUMENT_HPP
#define BOOST_PARAMETER_AUX_PACK_UNMATCHED_ARGUMENT_HPP

#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace parameter { namespace aux {

    template <typename T>
    struct unmatched_argument
    {
        BOOST_MPL_ASSERT((
            typename ::boost::mpl::if_<
                ::boost::is_same<T,void>
              , ::boost::mpl::true_
              , ::boost::mpl::false_
            >::type
        ));
        typedef int type;
    }; 
}}} // namespace boost::parameter::aux

#endif  // include guard

