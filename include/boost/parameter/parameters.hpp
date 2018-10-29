// Copyright David Abrahams, Daniel Wallin 2003.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETERS_031014_HPP
#define BOOST_PARAMETERS_031014_HPP

#include <boost/parameter/config.hpp>
#include <boost/parameter/aux_/void.hpp>
#include <boost/parameter/aux_/arg_list.hpp>
#include <boost/parameter/aux_/pack/make_arg_list.hpp>
#include <boost/parameter/aux_/pack/make_items.hpp>
#include <boost/parameter/aux_/pack/make_deduced_items.hpp>
#include <boost/parameter/aux_/pack/tag_template_keyword_arg.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_shifted.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/config.hpp>
#include <boost/config/workaround.hpp>

#if !defined(BOOST_NO_SFINAE) && \
    !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x592))
#include <boost/parameter/aux_/pack/tag_keyword_arg.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/is_same.hpp>
#endif

#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
#include <boost/preprocessor/repetition/enum_params.hpp>
#else
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#endif

#include <boost/parameter/aux_/preprocessor/no_perfect_forwarding_begin.hpp>

namespace boost { namespace parameter {

    template <
        typename PS0
      , BOOST_PP_ENUM_SHIFTED(
            BOOST_PARAMETER_MAX_ARITY, BOOST_PARAMETER_template_args, PS
        )
    >
    struct parameters
    {
        typedef typename BOOST_PARAMETER_build_deduced_list(
            BOOST_PARAMETER_MAX_ARITY
          , ::boost::parameter::aux::make_deduced_items
          , PS
        )::type deduced_list;

        // If the elements of NamedList match the criteria of overload
        // resolution, returns a type which can be constructed from
        // parameters.  Otherwise, this is not a valid metafunction
        // (no nested ::type).
#if !defined(BOOST_NO_SFINAE) && \
    !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x592))
        // If NamedList satisfies the PS0, PS1, ..., this is a metafunction
        // returning parameters.  Otherwise it has no nested ::type.
        template <typename ArgumentPackAndError>
        struct match_base
          : ::boost::mpl::if_<
                // ::boost::mpl::and_<
                //    aux::satisfies_requirements_of<NamedList,PS0>
                //  , ::boost::mpl::and_<
                //       aux::satisfies_requirements_of<NamedList,PS1>...
                //           ..., ::boost::mpl::true_
                // ...> >
                typename BOOST_PP_REPEAT(
                    BOOST_PARAMETER_MAX_ARITY
                  , BOOST_PARAMETER_satisfies_begin
                  , PS
                )
                ::boost::is_same<
                    typename ::boost::mpl
                    ::second<ArgumentPackAndError>::type
                  , ::boost::parameter::void_
                >
                BOOST_PP_REPEAT(
                    BOOST_PARAMETER_MAX_ARITY
                  , BOOST_PARAMETER_satisfies_end
                  , ::boost::mpl::false_
                )::type
              , ::boost::mpl::identity<
                    ::boost::parameter::parameters<
                        BOOST_PP_ENUM_PARAMS(BOOST_PARAMETER_MAX_ARITY, PS)
                    >
                >
              , ::boost::parameter::void_
            >
        {
        };
#endif  // SFINAE enabled, not Borland.

        // Specializations are to be used as an optional argument
        // to eliminate overloads via SFINAE.
        template <
#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
            // Borland simply can't handle default arguments in member
            // class templates.  People wishing to write portable code can
            // explicitly specify BOOST_PARAMETER_MAX_ARITY arguments.
            BOOST_PP_ENUM_PARAMS(BOOST_PARAMETER_MAX_ARITY, typename A)
#else
            BOOST_PP_ENUM_BINARY_PARAMS(
                BOOST_PARAMETER_MAX_ARITY
              , typename A
              , = ::boost::parameter::void_ BOOST_PP_INTERCEPT
            )
#endif
        >
        struct match
#if !defined(BOOST_NO_SFINAE) && \
    !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x592))
          : ::boost::parameter::parameters<
                BOOST_PP_ENUM_PARAMS(BOOST_PARAMETER_MAX_ARITY, PS)
            >::BOOST_NESTED_TEMPLATE match_base<
                typename ::boost::parameter::aux::make_arg_list<
                    typename BOOST_PARAMETER_build_arg_list(
                        BOOST_PARAMETER_MAX_ARITY
                      , ::boost::parameter::aux::make_items
                      , PS
                      , A
                    )::type
                  , deduced_list
                  , ::boost::parameter::aux::tag_keyword_arg
                    // Don't emit errors when doing SFINAE.
                  , ::boost::mpl::false_
                >::type
            >::type
        {
        };
#else
        {
            typedef ::boost::parameter::parameters<
                BOOST_PP_ENUM_PARAMS(BOOST_PARAMETER_MAX_ARITY, PS)
            > type;
        };
#endif  // SFINAE enabled, not Borland.

        // Metafunction that returns an ArgumentPack.

        // TODO, bind has to instantiate the error type in the result
        // of make_arg_list.

        template <
#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
            // Borland simply can't handle default arguments in member
            // class templates.  People wishing to write portable code can
            // explicitly specify BOOST_PARAMETER_MAX_ARITY arguments
            BOOST_PP_ENUM_PARAMS(BOOST_PARAMETER_MAX_ARITY, typename A)
#else
            BOOST_PP_ENUM_BINARY_PARAMS(
                BOOST_PARAMETER_MAX_ARITY
              , typename A
              , = ::boost::parameter::void_ BOOST_PP_INTERCEPT
            )
#endif
        >
        struct bind
        {
            typedef typename ::boost::parameter::aux::make_arg_list<
                typename BOOST_PARAMETER_build_arg_list(
                    BOOST_PARAMETER_MAX_ARITY
                  , ::boost::parameter::aux::make_items
                  , PS
                  , A
                )::type
              , deduced_list
              , ::boost::parameter::aux::tag_template_keyword_arg
            >::type result;

            typedef typename ::boost::mpl::first<result>::type type;
        };

        BOOST_PARAMETER_forward_typedefs(
            BOOST_PARAMETER_MAX_ARITY, PS, parameter_spec
        )

        // The function call operator is used to build an arg_list that
        // labels the positional parameters and maintains whatever other
        // tags may have been specified by the caller.
        //
        // !!!NOTE!!!
        //
        // The make_arg_list<> metafunction produces a reversed arg_list,
        // so pass the arguments to the arg_list constructor reversed in turn.
        inline ::boost::parameter::aux::empty_arg_list operator()() const
        {
            return ::boost::parameter::aux::empty_arg_list();
        }

        template <typename A0>
        typename ::boost::mpl::first<
            typename ::boost::parameter::aux::make_arg_list<
                ::boost::parameter::aux::item<
                    PS0,A0
                >
              , deduced_list
              , ::boost::parameter::aux::tag_keyword_arg
            >::type
        >::type
        operator()(A0& a0) const
        {
            typedef typename ::boost::parameter::aux::make_arg_list<
                ::boost::parameter::aux::item<
                    PS0,A0
                >
              , deduced_list
              , ::boost::parameter::aux::tag_keyword_arg
            >::type result;

            typedef typename ::boost::mpl::first<result>::type result_type;
            typedef typename ::boost::mpl::second<result>::type error;
            error();

            return result_type(
                a0
                // , void_(), void_(), void_() ...
                BOOST_PP_ENUM_TRAILING_PARAMS(
                    BOOST_PP_SUB(BOOST_PARAMETER_MAX_ARITY, 1)
                  , ::boost::parameter::aux::void_reference() BOOST_PP_INTERCEPT
                )
            );
        }

        template <typename A0, typename A1>
        typename ::boost::mpl::first<
            typename ::boost::parameter::aux::make_arg_list<
                ::boost::parameter::aux::item<
                    PS0,A0
                  , ::boost::parameter::aux::item<
                        PS1,A1
                    >
                >
              , deduced_list
              , ::boost::parameter::aux::tag_keyword_arg
            >::type
        >::type
        operator()(A0& a0, A1& a1) const
        {
            typedef typename ::boost::parameter::aux::make_arg_list<
                ::boost::parameter::aux::item<
                    PS0,A0
                  , ::boost::parameter::aux::item<
                        PS1,A1
                    >
                >
              , deduced_list
              , ::boost::parameter::aux::tag_keyword_arg
            >::type result;

            typedef typename ::boost::mpl::first<result>::type result_type;
            typedef typename ::boost::mpl::second<result>::type error;
            error();

            return result_type(
                a1,a0
                // , void_(), void_() ...
                BOOST_PP_ENUM_TRAILING_PARAMS(
                    BOOST_PP_SUB(BOOST_PARAMETER_MAX_ARITY, 2)
                  , ::boost::parameter::aux::void_reference() BOOST_PP_INTERCEPT
                )
            );
        }

#if (2 < BOOST_PARAMETER_MAX_ARITY)
        // Higher arities are handled by the preprocessor
#define BOOST_PP_ITERATION_PARAMS_1 \
    (3,( \
        3 \
      , BOOST_PARAMETER_MAX_ARITY \
      , <boost/parameter/aux_/preprocessor/overloads.hpp> \
    ))
#include BOOST_PP_ITERATE()
#endif
    };
}} // namespace boost::parameter

#include <boost/parameter/aux_/preprocessor/no_perfect_forwarding_end.hpp>

#endif  // include guard

