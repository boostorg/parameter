// Copyright David Abrahams, Daniel Wallin 2003.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_AUX_PACK_TAG_TEMPLATE_KEYWORD_ARG_HPP
#define BOOST_PARAMETER_AUX_PACK_TAG_TEMPLATE_KEYWORD_ARG_HPP

#include <boost/parameter/aux_/template_keyword.hpp>

namespace boost { namespace parameter { namespace aux {

    struct tag_template_keyword_arg
    {
        template <typename K, typename T>
        struct apply
        {
            typedef ::boost::parameter::template_keyword<K,T> type;
        };
    };
}}} // namespace boost::parameter::aux

#endif  // include guard

