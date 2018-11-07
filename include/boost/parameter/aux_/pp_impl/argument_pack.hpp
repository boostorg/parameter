// Copyright Daniel Wallin 2006.
// Copyright Cromwell D. Enage 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_AUX_PP_IMPL_ARGUMENT_PACK_HPP
#define BOOST_PARAMETER_AUX_PP_IMPL_ARGUMENT_PACK_HPP

#include <boost/parameter/aux_/pack/tag_keyword_arg.hpp>
#include <boost/parameter/aux_/pack/make_arg_list.hpp>
#include <boost/parameter/aux_/pack/make_parameter_spec_items.hpp>
#include <boost/mpl/pair.hpp>

namespace boost { namespace parameter { namespace aux {

    template <typename Parameters, typename ...Args>
    struct argument_pack
    {
        typedef typename ::boost::parameter::aux::make_arg_list<
            typename ::boost::parameter::aux::make_parameter_spec_items<
                typename Parameters::parameter_spec
              , Args...
            >::type
          , typename Parameters::deduced_list
          , ::boost::parameter::aux::tag_keyword_arg
          , ::boost::mpl::false_
        >::type result;
        typedef typename ::boost::mpl::first<result>::type type;
    };
}}} // namespace boost::parameter::aux

#endif  // include guard

