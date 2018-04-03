// Copyright Daniel Wallin 2006.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_VALUE_TYPE_060921_HPP
#define BOOST_PARAMETER_VALUE_TYPE_060921_HPP

#include <boost/mpl/apply_wrap.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/and.hpp>
#include <boost/parameter/aux_/result_of0.hpp>
#include <boost/parameter/aux_/void.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace parameter { 

#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
    template <class Parameters, class Keyword, class Default>
    struct value_type0
      : boost::mpl::apply_wrap3<
            typename Parameters::binding,Keyword,Default,boost::mpl::false_
        >
    {
        BOOST_MPL_ASSERT_NOT((
            boost::mpl::and_<
                boost::is_same<Default,boost::parameter::void_>
              , boost::is_same<
                    typename boost::parameter::value_type0<
                        Parameters,Keyword,Default
                    >::type
                  , boost::parameter::void_
                >
            >
        ));
    };
#endif

    // A metafunction that, given an argument pack, returns the type of the
    // parameter identified by the given keyword.  If no such parameter has
    // been specified, returns Default.
    template <
        class Parameters
      , class Keyword
      , class Default = boost::parameter::void_
    >
    struct value_type
#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
      : boost::mpl::eval_if<
            boost::mpl::is_placeholder<Parameters>
          , boost::mpl::identity<int>
          , boost::parameter::value_type0<Parameters,Keyword,Default>
        >
#else
      : boost::mpl::apply_wrap3<
            typename Parameters::binding,Keyword,Default,boost::mpl::false_
        >
#endif
    {
#if !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
        BOOST_MPL_ASSERT_NOT((
            boost::mpl::and_<
                boost::is_same<Default,boost::parameter::void_>
              , boost::is_same<
                    typename boost::parameter::value_type<
                        Parameters,Keyword,Default
                    >::type
                  , boost::parameter::void_
                >
            >
        ));
#endif
        BOOST_MPL_AUX_LAMBDA_SUPPORT(
            3, value_type, (Parameters, Keyword, Default)
        )
    };

    // A metafunction that, given an argument pack, returns the type of the
    // parameter identified by the given keyword.  If no such parameter has
    // been specified, returns the type returned by invoking DefaultFn.
    template <class Parameters, class Keyword, class DefaultFn>
    struct lazy_value_type
      : boost::mpl::apply_wrap3<
            typename Parameters::binding
          , Keyword
          , typename boost::parameter::aux::result_of0<DefaultFn>::type
          , boost::mpl::false_
        >
    {
    };
}} // namespace boost::parameter

#endif // BOOST_PARAMETER_VALUE_TYPE_060921_HPP

