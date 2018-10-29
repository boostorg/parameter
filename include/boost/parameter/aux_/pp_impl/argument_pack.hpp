// Copyright Daniel Wallin 2006.
// Copyright Cromwell D. Enage 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_AUX_PP_IMPL_ARGUMENT_PACK_HPP
#define BOOST_PARAMETER_AUX_PP_IMPL_ARGUMENT_PACK_HPP

#include <boost/parameter/aux_/pack/tag_keyword_arg.hpp>
#include <boost/parameter/aux_/pack/make_arg_list.hpp>
#include <boost/parameter/config.hpp>
#include <boost/mpl/pair.hpp>

#if defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)
#include <boost/parameter/aux_/pack/make_parameter_spec_items.hpp>
#else
#include <boost/parameter/aux_/void.hpp>
#include <boost/parameter/aux_/pack/make_items.hpp>
#include <boost/parameter/aux_/preprocessor/no_perfect_forwarding_begin.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#endif

namespace boost { namespace parameter { namespace aux {

    template <
        typename Parameters
#if defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)
      , typename ...Args
#else
      , BOOST_PP_ENUM_BINARY_PARAMS(
            BOOST_PARAMETER_MAX_ARITY
          , typename A
          , = ::boost::parameter::void_ BOOST_PP_INTERCEPT
        )
#endif
    >
    struct argument_pack
    {
        typedef typename ::boost::parameter::aux::make_arg_list<
#if defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)
            typename ::boost::parameter::aux::make_parameter_spec_items<
                typename Parameters::parameter_spec
              , Args...
            >::type
#else
            typename BOOST_PARAMETER_build_arg_list(
                BOOST_PARAMETER_MAX_ARITY
              , ::boost::parameter::aux::make_items
              , typename Parameters::parameter_spec
              , A
            )::type
#endif
          , typename Parameters::deduced_list
          , ::boost::parameter::aux::tag_keyword_arg
          , ::boost::mpl::false_
        >::type result;
        typedef typename ::boost::mpl::first<result>::type type;
    };
}}} // namespace boost::parameter::aux

#if !defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)
#include <boost/parameter/aux_/preprocessor/no_perfect_forwarding_end.hpp>
#endif
#endif  // include guard

