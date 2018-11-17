// Copyright David Abrahams, Daniel Wallin 2003.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_AUX_PACK_TAG_KEYWORD_ARG_HPP
#define BOOST_PARAMETER_AUX_PACK_TAG_KEYWORD_ARG_HPP

#include <boost/parameter/aux_/tag.hpp>

namespace boost { namespace parameter { namespace aux {

    struct tag_keyword_arg
    {
        template <typename K, typename T>
        struct apply
        {
            typedef typename ::boost::parameter::aux::tag<K,T>::type type;
        };
    };
}}} // namespace boost::parameter::aux

#endif  // include guard

