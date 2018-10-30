// Copyright David Abrahams, Daniel Wallin 2003.
// Copyright Cromwell D. Enage 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETERS_031014_HPP
#define BOOST_PARAMETERS_031014_HPP

#include <boost/parameter/config.hpp>

namespace boost { namespace parameter { namespace aux {

    // The make_arg_list<> metafunction produces a reversed arg_list,
    // so pass the arguments to the arg_list constructor reversed in turn.
    template <typename ArgList, typename ...Args>
    struct arg_list_factory;
}}} // namespace boost::parameter::aux

#include <boost/parameter/aux_/arg_list.hpp>
#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <utility>

namespace boost { namespace parameter { namespace aux {

    // TODO: Reduce template code bloat. -- Cromwell D. Enage
    template <typename ArgList>
    struct arg_list_factory<ArgList>
    {
        template <typename ...ReversedArgs>
        static inline ArgList reverse(ReversedArgs&&... reversed_args)
        {
            return ArgList(
                typename ::boost::mpl::if_<
                    ::boost::is_same<
                        typename ArgList::tagged_arg::value_type
                      , ::boost::parameter::void_
                    >
                  , ::boost::parameter::aux::value_type_is_void
                  , ::boost::parameter::aux::value_type_is_not_void
                >::type()
              , ::std::forward<ReversedArgs>(reversed_args)...
            );
        }
    };

    template <typename ArgList, typename A0, typename ...Args>
    struct arg_list_factory<ArgList,A0,Args...>
    {
        template <typename ...ReversedArgs>
        static inline ArgList
            reverse(A0&& a0, Args&&... args, ReversedArgs&&... reversed_args)
        {
            return ::boost::parameter::aux
            ::arg_list_factory<ArgList,Args...>::reverse(
                ::std::forward<Args>(args)...
              , ::std::forward<A0>(a0)
              , ::std::forward<ReversedArgs>(reversed_args)...
            );
        }
    };
}}} // namespace boost::parameter::aux

#include <boost/parameter/aux_/void.hpp>
#include <boost/parameter/aux_/pack/make_arg_list.hpp>
#include <boost/parameter/aux_/pack/make_parameter_spec_items.hpp>
#include <boost/parameter/aux_/pack/tag_keyword_arg.hpp>
#include <boost/parameter/aux_/pack/tag_template_keyword_arg.hpp>
#include <boost/fusion/container/vector/vector_fwd.hpp>
#include <boost/fusion/container/deque/deque_fwd.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/identity.hpp>

#if !defined(BOOST_PARAMETER_VARIADIC_MPL_SEQUENCE)
// Newer versions of MSVC fail on the evaluate_category and
// preprocessor_eval_category test programs when parameters uses
// boost::fusion::list.
// -- Cromwell D. Enage
#if defined(BOOST_FUSION_HAS_VARIADIC_LIST) && ( \
        !defined(BOOST_MSVC) || (BOOST_MSVC < 1800) \
    )
#include <boost/fusion/container/list.hpp>
#include <boost/fusion/mpl.hpp>
#define BOOST_PARAMETER_VARIADIC_MPL_SEQUENCE ::boost::fusion::list
#elif defined(BOOST_FUSION_HAS_VARIADIC_DEQUE)
#include <boost/fusion/container/deque.hpp>
#include <boost/fusion/mpl.hpp>
#define BOOST_PARAMETER_VARIADIC_MPL_SEQUENCE ::boost::fusion::deque
#else
#include <boost/mpl/vector.hpp>
#define BOOST_PARAMETER_VARIADIC_MPL_SEQUENCE ::boost::mpl::vector
#endif
#endif  // BOOST_PARAMETER_VARIADIC_MPL_SEQUENCE

namespace boost { namespace parameter {

    template <typename ...Spec>
    struct parameters
    {
        typedef BOOST_PARAMETER_VARIADIC_MPL_SEQUENCE<Spec...> parameter_spec;

        typedef typename ::boost::parameter::aux
        ::make_deduced_list<parameter_spec>::type deduced_list;

        // If the elements of NamedList match the criteria of overload
        // resolution, returns a type which can be constructed from
        // parameters.  Otherwise, this is not a valid metafunction
        // (no nested ::type).
        template <typename ArgumentPackAndError>
        struct match_base
          : ::boost::mpl::if_<
                typename ::boost::parameter::aux::match_parameters_base_cond<
                    ArgumentPackAndError
                  , parameter_spec
                >::type
              , ::boost::mpl::identity<
                    ::boost::parameter::parameters<Spec...>
                >
              , ::boost::parameter::void_
            >
        {
        };

        // Specializations are to be used as an optional argument
        // to eliminate overloads via SFINAE.
        template <typename ...Args>
        struct match
          : ::boost::parameter::parameters<Spec...>
            ::BOOST_NESTED_TEMPLATE match_base<
                typename ::boost::parameter::aux::make_arg_list<
                    typename ::boost::parameter::aux
                    ::make_parameter_spec_items<parameter_spec,Args...>::type
                  , deduced_list
                  , ::boost::parameter::aux::tag_keyword_arg
                    // Don't emit errors when doing SFINAE.
                  , ::boost::mpl::false_
                >::type
            >::type
        {
        };

        // Metafunction that returns an ArgumentPack.
        template <typename ...Args>
        struct bind
          : ::boost::mpl::first<
                typename ::boost::parameter::aux::make_arg_list<
                    typename ::boost::parameter::aux
                    ::make_parameter_spec_items<parameter_spec,Args...>::type
                  , deduced_list
                  , ::boost::parameter::aux::tag_template_keyword_arg
                >::type
            >
        {
        };

        // The function call operator is used to build an arg_list that
        // labels the positional parameters and maintains whatever other
        // tags may have been specified by the caller.
        inline ::boost::parameter::aux::empty_arg_list operator()() const
        {
            return ::boost::parameter::aux::empty_arg_list();
        }

        template <typename A0, typename ...Args>
        inline typename ::boost::mpl::first<
            typename ::boost::parameter::aux::make_arg_list<
                typename ::boost::parameter::aux::make_parameter_spec_items<
                    parameter_spec
                  , A0
                  , Args...
                >::type
              , deduced_list
              , ::boost::parameter::aux::tag_keyword_arg
            >::type
        >::type
        operator()(A0&& a0, Args&& ...args) const
        {
            typedef typename ::boost::parameter::aux::make_arg_list<
                typename ::boost::parameter::aux::make_parameter_spec_items<
                    parameter_spec
                  , A0
                  , Args...
                >::type
              , deduced_list
              , ::boost::parameter::aux::tag_keyword_arg
            >::type list_error_pair;

            typedef typename ::boost::mpl
            ::first<list_error_pair>::type result_type;

            typedef typename ::boost::mpl
            ::second<list_error_pair>::type error;

            error();

            return ::boost::parameter::aux
            ::arg_list_factory<result_type,A0,Args...>::reverse(
                ::std::forward<A0>(a0)
              , ::std::forward<Args>(args)...
            );
        }
    };
}} // namespace boost::parameter

#endif  // include guard

