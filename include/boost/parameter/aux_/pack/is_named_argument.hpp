// Copyright David Abrahams, Daniel Wallin 2003.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_AUX_PACK_IS_NAMED_ARGUMENT_HPP
#define BOOST_PARAMETER_AUX_PACK_IS_NAMED_ARGUMENT_HPP

#include <boost/parameter/aux_/template_keyword.hpp>
#include <boost/parameter/aux_/is_tagged_argument.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>

namespace boost { namespace parameter { namespace aux {

    template <typename T>
    struct is_named_argument
      : ::boost::mpl::if_<
            ::boost::parameter::aux::is_template_keyword<T>
          , ::boost::mpl::true_
          , ::boost::parameter::aux::is_tagged_argument<T>
        >::type
    {
    };
}}} // namespace boost::parameter::aux

#endif  // include guard

