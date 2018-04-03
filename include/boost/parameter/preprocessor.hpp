// Copyright Daniel Wallin 2006.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_PREPROCESSOR_060206_HPP
#define BOOST_PARAMETER_PREPROCESSOR_060206_HPP

#include <boost/parameter/aux_/void.hpp>
#include <boost/parameter/config.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/always.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>

#if !defined(BOOST_NO_SFINAE) && \
    !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x592))

namespace boost { namespace parameter { namespace aux {

    // Given Match, which is "void x" where x is an argument matching
    // criterion, extract a corresponding MPL predicate.
    template <class Match>
    struct unwrap_predicate;

    // Match anything
    template <>
    struct unwrap_predicate<void*>
    {
        typedef boost::mpl::always<boost::mpl::true_> type;
    };

#if BOOST_WORKAROUND(__SUNPRO_CC, BOOST_TESTED_AT(0x580))
    typedef void* voidstar;

    // A matching predicate is explicitly specified.
    template <class Predicate>
    struct unwrap_predicate<test::voidstar (Predicate)>
    {
        typedef Predicate type;
    };
#else
    // A matching predicate is explicitly specified.
    template <class Predicate>
    struct unwrap_predicate<void *(Predicate)>
    {
        typedef Predicate type;
    };
#endif // SunProCC workarounds needed.

    // A type to which the argument is supposed to be convertible is
    // specified.
    template <class Target>
    struct unwrap_predicate<void (Target)>
    {
        typedef boost::is_convertible<boost::mpl::_,Target> type;
    };

    // Recast the ParameterSpec's nested match metafunction
    // as a free metafunction.
#if defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
    // No more limits set by BOOST_PARAMETER_MAX_ARITY. -- Cromwell D. Enage
    template <class Parameters, class ...Args>
    struct match : Parameters::BOOST_NESTED_TEMPLATE match<Args...>
#else
    template <
        class Parameters
      , BOOST_PP_ENUM_BINARY_PARAMS(
            BOOST_PARAMETER_MAX_ARITY
          , class A
          , = boost::parameter::void_ BOOST_PP_INTERCEPT
        )
    >
    struct match
      : Parameters::BOOST_NESTED_TEMPLATE match<
            BOOST_PP_ENUM_PARAMS(BOOST_PARAMETER_MAX_ARITY, A)
        >
#endif // BOOST_PARAMETER_HAS_PERFECT_FORWARDING
    {
    };
}}} // namespace boost::parameter::aux

#endif // SFINAE/Borland workarounds not needed.

#include <boost/parameter/parameters.hpp>
#include <boost/mpl/pair.hpp>

namespace boost { namespace parameter { namespace aux {

    template <
        class Parameters
#if defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
      , class ...Args
#else
      , BOOST_PP_ENUM_BINARY_PARAMS(
            BOOST_PARAMETER_MAX_ARITY
          , class A
          , = boost::parameter::void_ BOOST_PP_INTERCEPT
        )
#endif
    >
    struct argument_pack
    {
        typedef typename boost::parameter::aux::make_arg_list<
#if defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
            typename boost::parameter::aux::make_parameter_spec_items<
                typename Parameters::parameter_spec
              , Args...
            >::type
#else
            typename BOOST_PARAMETER_build_arg_list(
                BOOST_PARAMETER_MAX_ARITY
              , boost::parameter::aux::make_items
              , typename Parameters::parameter_spec
              , A
            )::type
#endif
          , typename Parameters::deduced_list
          , boost::parameter::aux::tag_keyword_arg
          , boost::mpl::false_
        >::type result;
        typedef typename boost::mpl::first<result>::type type;
    };
}}} // namespace boost::parameter::aux

#include <boost/mpl/if.hpp>
#include <boost/mpl/apply_wrap.hpp>

#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))

#include <boost/mpl/assert.hpp>
#include <boost/mpl/apply.hpp>

namespace boost { namespace parameter { namespace aux {

    template <class Predicate, class T, class Args>
    struct apply_predicate
    {
        BOOST_MPL_ASSERT((
            typename boost::mpl::if_<
                boost::mpl::false_
              , T
              , boost::mpl::false_
            >::type
        ));
        typedef typename boost::mpl::if_<
            typename boost::mpl::apply2<Predicate,T,Args>::type
          , char
          , long double
        >::type type;
    };
}}} // namespace boost::parameter::aux

#include <boost/type.hpp>
#include <boost/tti/detail/dnullptr.hpp>

namespace boost { namespace parameter { namespace aux {

    template <class P>
    struct funptr_predicate
    {
        static P p;

        template <class T, class Args, class P0>
        static typename boost::parameter::aux::apply_predicate<
            P0
          , T
          , Args
        >::type
        check_predicate(boost::type<T>, Args*, void**(*)(P0));

        template <class T, class Args, class P0>
        static typename boost::mpl::if_<
            boost::is_convertible<T,P0>
          , char
          , long double
        >::type
        check_predicate(boost::type<T>, Args*, void*(*)(P0));

        template <class T, class Args>
        struct apply
        {
            BOOST_STATIC_CONSTANT(
                bool, result = (
                    sizeof(char) == sizeof(
                        boost::parameter::aux::funptr_predicate<P>
                        ::check_predicate(
                            boost::type<T>()
                          , static_cast<Args*>(BOOST_TTI_DETAIL_NULLPTR)
                          , &p
                        )
                    )
                )
            );

            typedef mpl::bool_<apply<T,Args>::result> type;
        };
    };

    template <>
    struct funptr_predicate<void**> : boost::mpl::always<boost::mpl::true_>
    {
    };
}}} // namespace boost::parameter::aux

#endif // Borland workarounds needed.

#define BOOST_PARAMETER_MEMBER_FUNCTION_CHECK_STATIC_static ()
/**/

#include <boost/parameter/aux_/preprocessor/is_nullary.hpp>
#include <boost/preprocessor/cat.hpp>

#define BOOST_PARAMETER_MEMBER_FUNCTION_IS_STATIC(name)                      \
    BOOST_PARAMETER_IS_NULLARY(                                              \
        BOOST_PP_CAT(BOOST_PARAMETER_MEMBER_FUNCTION_CHECK_STATIC_, name)    \
    )
/**/

#include <boost/preprocessor/seq/seq.hpp>

#if defined BOOST_MSVC
// Workaround for MSVC preprocessor.
//
// When stripping static from "static f", msvc will produce " f".  The leading
// whitespace doesn't go away when pasting the token with something else, so
// this thing is a hack to strip the whitespace.
#define BOOST_PARAMETER_MEMBER_FUNCTION_STRIP_STATIC_static (
/**/
#define BOOST_PARAMETER_MEMBER_FUNCTION_STRIP_STATIC_AUX(name)               \
    BOOST_PP_CAT(BOOST_PARAMETER_MEMBER_FUNCTION_STRIP_STATIC_, name))
/**/
#define BOOST_PARAMETER_MEMBER_FUNCTION_STRIP_STATIC(name)                   \
    BOOST_PP_SEQ_HEAD(                                                       \
        BOOST_PARAMETER_MEMBER_FUNCTION_STRIP_STATIC_AUX(name)               \
    )
/**/
#else
#define BOOST_PARAMETER_MEMBER_FUNCTION_STRIP_STATIC_static
/**/
#define BOOST_PARAMETER_MEMBER_FUNCTION_STRIP_STATIC(name)                   \
    BOOST_PP_CAT(BOOST_PARAMETER_MEMBER_FUNCTION_STRIP_STATIC_, name)
/**/
#endif // MSVC workarounds needed.

#include <boost/preprocessor/control/expr_if.hpp>

#define BOOST_PARAMETER_MEMBER_FUNCTION_STATIC(name)                         \
    BOOST_PP_EXPR_IF(                                                        \
        BOOST_PARAMETER_MEMBER_FUNCTION_IS_STATIC(name), static              \
    )
/**/

#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/tuple/eat.hpp>

#define BOOST_PARAMETER_MEMBER_FUNCTION_NAME(name)                           \
    BOOST_PP_IF(                                                             \
        BOOST_PARAMETER_MEMBER_FUNCTION_IS_STATIC(name)                      \
      , BOOST_PARAMETER_MEMBER_FUNCTION_STRIP_STATIC                         \
      , name BOOST_PP_TUPLE_EAT(1)                                           \
    )(name)
/**/

// Produces a name for a parameter specification for the function named base.
#define BOOST_PARAMETER_FUNCTION_SPECIFICATION_NAME(base)                    \
    BOOST_PP_CAT(                                                            \
        BOOST_PP_CAT(boost_param_parameters_, __LINE__)                      \
      , BOOST_PARAMETER_MEMBER_FUNCTION_NAME(name)                           \
    )
/**/

// Produces a name for a result type metafunction for the function named base.
#define BOOST_PARAMETER_FUNCTION_RESULT_NAME(base)                           \
    BOOST_PP_CAT(                                                            \
        BOOST_PP_CAT(boost_param_result_, __LINE__)                          \
      , BOOST_PARAMETER_MEMBER_FUNCTION_NAME(name)                           \
    )
/**/

// Can't do boost_param_impl_ ## basee
// because base might start with an underscore.
// daniel: what? how is that relevant? the reason for using CAT()
// is to make sure base is expanded. i'm not sure we need to here,
// but it's more stable to do it.
#define BOOST_PARAMETER_FUNCTION_IMPL_NAME(base)                             \
    BOOST_PP_CAT(boost_param_impl, BOOST_PARAMETER_MEMBER_FUNCTION_NAME(base))
/**/

// Defines the implementation function header.
#define BOOST_PARAMETER_FUNCTION_IMPL_HEAD(name)                             \
    template <class Args>                                                    \
    typename BOOST_PARAMETER_FUNCTION_RESULT_NAME(name)<                     \
       Args                                                                  \
    >::type BOOST_PARAMETER_FUNCTION_IMPL_NAME(name)(Args const& args)
/**/

#include <boost/preprocessor/tuple/elem.hpp>

// Accessor macros for the split_args tuple.
#define BOOST_PARAMETER_SPLIT_ARG_REQ_COUNT(x) BOOST_PP_TUPLE_ELEM(4, 0, x)
#define BOOST_PARAMETER_SPLIT_ARG_REQ_SEQ(x) BOOST_PP_TUPLE_ELEM(4, 1, x)
#define BOOST_PARAMETER_SPLIT_ARG_OPT_COUNT(x) BOOST_PP_TUPLE_ELEM(4, 2, x)
#define BOOST_PARAMETER_SPLIT_ARG_OPT_SEQ(x) BOOST_PP_TUPLE_ELEM(4, 3, x)

// Accessor macros for the input tuple to the dispatch macros.
#define BOOST_PARAMETER_FUNCTION_DISPATCH_BASE_NAME(x)                       \
    BOOST_PP_TUPLE_ELEM(4, 0, x)
/**/

#define BOOST_PARAMETER_FUNCTION_DISPATCH_SPLIT_ARGS(x)                      \
    BOOST_PP_TUPLE_ELEM(4, 1, x)
/**/

#define BOOST_PARAMETER_FUNCTION_DISPATCH_IS_CONST(x)                        \
    BOOST_PP_TUPLE_ELEM(4, 2, x)
/**/

#define BOOST_PARAMETER_FUNCTION_DISPATCH_TAG_NAMESPACE(x)                   \
    BOOST_PP_TUPLE_ELEM(4, 3, x)
/**/

// Produces a name for the dispatch functions.
#define BOOST_PARAMETER_FUNCTION_DISPATCH_NAME(x)                            \
    BOOST_PP_CAT(                                                            \
        BOOST_PP_CAT(boost_param_dispatch_, __LINE__)                        \
      , BOOST_PARAMETER_MEMBER_FUNCTION_NAME(BOOST_PP_TUPLE_ELEM(5, 0, x))   \
    )
/**/

// Accessor macros for the argument specs tuple.
#define BOOST_PARAMETER_FN_ARG_QUALIFIER(x) BOOST_PP_TUPLE_ELEM(4, 0, x)
#define BOOST_PARAMETER_FN_ARG_KEYWORD(x) BOOST_PP_TUPLE_ELEM(4, 1, x)
#define BOOST_PARAMETER_FN_ARG_PRED(x) BOOST_PP_TUPLE_ELEM(4, 2, x)
#define BOOST_PARAMETER_FN_ARG_DEFAULT(x) BOOST_PP_TUPLE_ELEM(4, 3, x)

#include <boost/parameter/name.hpp>

#define BOOST_PARAMETER_FN_ARG_NAME(x)                                       \
    BOOST_PARAMETER_UNQUALIFIED_NAME(BOOST_PARAMETER_FN_ARG_KEYWORD(x))
/**/

#include <boost/parameter/keyword.hpp>
#include <boost/parameter/aux_/cast.hpp>

// Takes in the arg tuple (name, pred) and the tag namespace.
// Extracts the corresponding required argument from the pack.
#define BOOST_PARAMETER_FUNCTION_DISPATCH_ARG_CAST_0(arg, tag_ns)            \
    BOOST_PARAMETER_FUNCTION_CAST(                                           \
        args[                                                                \
            boost::parameter::keyword<                                       \
                tag_ns::BOOST_PARAMETER_FN_ARG_NAME(arg)                     \
            >::instance                                                      \
        ]                                                                    \
      , BOOST_PARAMETER_FN_ARG_PRED(arg)                                     \
      , Args                                                                 \
    )
/**/

// Enables BOOST_PARAMETER_FUNCTION_DISPATCH_ARG_REPEAT to use
// BOOST_PARAMETER_FUNCTION_DISPATCH_ARG_CAST_0 within BOOST_PP_SEQ_FOR_EACH.
#define BOOST_PARAMETER_FUNCTION_DISPATCH_ARG_CAST_0_R(r, tag_ns, arg)       \
  , BOOST_PARAMETER_FUNCTION_DISPATCH_ARG_CAST_0(arg, tag_ns)
/**/

// Takes in the arg tuple (name, pred, default) and the tag namespace.
// Extracts the corresponding optional argument from the pack if specified,
// otherwise temporarily passes use_default_tag() to the dispatch functions.
#define BOOST_PARAMETER_FUNCTION_DISPATCH_ARG_CAST_1(arg, tag_ns)            \
    BOOST_PARAMETER_FUNCTION_CAST(                                           \
        args[                                                                \
            boost::parameter::keyword<                                       \
                tag_ns::BOOST_PARAMETER_FN_ARG_NAME(arg)                     \
            >::instance || boost::parameter::aux::use_default_tag()          \
        ]                                                                    \
      , BOOST_PARAMETER_FN_ARG_PRED(arg)                                     \
      , Args                                                                 \
    )
/**/

// Enables BOOST_PARAMETER_FUNCTION_DISPATCH_ARG_REPEAT to use
// BOOST_PARAMETER_FUNCTION_DISPATCH_ARG_CAST_1 within BOOST_PP_SEQ_FOR_EACH.
#define BOOST_PARAMETER_FUNCTION_DISPATCH_ARG_CAST_1_R(r, tag_ns, arg)       \
  , BOOST_PARAMETER_FUNCTION_DISPATCH_ARG_CAST_1(arg, tag_ns)
/**/

#include <boost/preprocessor/arithmetic/inc.hpp>

// Helper macros for BOOST_PARAMETER_ARITY_RANGE.
#define BOOST_PARAMETER_ARITY_RANGE_M_optional(state) state
#define BOOST_PARAMETER_ARITY_RANGE_M_deduced_optional(state) state
#define BOOST_PARAMETER_ARITY_RANGE_M_required(state) BOOST_PP_INC(state)
#define BOOST_PARAMETER_ARITY_RANGE_M_deduced_required(state)                \
    BOOST_PP_INC(state)
/**/

#define BOOST_PARAMETER_ARITY_RANGE_M(s, state, x)                           \
    BOOST_PP_CAT(                                                            \
        BOOST_PARAMETER_ARITY_RANGE_M_                                       \
      , BOOST_PARAMETER_FN_ARG_QUALIFIER(x)                                  \
    )(state)
/**/

#include <boost/preprocessor/seq/push_back.hpp>

// Helper macros for BOOST_PARAMETER_FUNCTION_SPLIT_ARGS.
#define BOOST_PARAMETER_FUNCTION_SPLIT_ARG_required(s_a, arg)                \
    (                                                                        \
        BOOST_PP_INC(BOOST_PARAMETER_SPLIT_ARG_REQ_COUNT(s_a))               \
      , BOOST_PP_SEQ_PUSH_BACK(BOOST_PARAMETER_SPLIT_ARG_REQ_SEQ(s_a), arg)  \
      , BOOST_PARAMETER_SPLIT_ARG_OPT_COUNT(s_a)                             \
      , BOOST_PARAMETER_SPLIT_ARG_OPT_SEQ(s_a)                               \
    )
/**/

#define BOOST_PARAMETER_FUNCTION_SPLIT_ARG_deduced_required(split_args, arg) \
    BOOST_PARAMETER_FUNCTION_SPLIT_ARG_required(split_args, arg)
/**/

#define BOOST_PARAMETER_FUNCTION_SPLIT_ARG_optional(s_a, arg)                \
    (                                                                        \
        BOOST_PARAMETER_SPLIT_ARG_REQ_COUNT(s_a)                             \
      , BOOST_PARAMETER_SPLIT_ARG_REQ_SEQ(s_a)                               \
      , BOOST_PP_INC(BOOST_PARAMETER_SPLIT_ARG_OPT_COUNT(s_a))               \
      , BOOST_PP_SEQ_PUSH_BACK(BOOST_PARAMETER_SPLIT_ARG_OPT_SEQ(s_a), arg)  \
    )
/**/

#define BOOST_PARAMETER_FUNCTION_SPLIT_ARG_deduced_optional(split_args, arg) \
    BOOST_PARAMETER_FUNCTION_SPLIT_ARG_optional(split_args, arg)
/**/

#define BOOST_PARAMETER_FUNCTION_SPLIT_ARG(s, split_args, arg)               \
    BOOST_PP_CAT(                                                            \
        BOOST_PARAMETER_FUNCTION_SPLIT_ARG_                                  \
      , BOOST_PARAMETER_FN_ARG_QUALIFIER(arg)                                \
    )(split_args, arg)
/**/

#include <boost/preprocessor/seq/fold_left.hpp>

// Expands from the flattened BOOST_PARAMETER_FUNCTION et. al. arg sequence to
// the tuple (required_count, required_args, optional_count, optional_args).
#define BOOST_PARAMETER_FUNCTION_SPLIT_ARGS(args)                            \
    BOOST_PP_SEQ_FOLD_LEFT(                                                  \
        BOOST_PARAMETER_FUNCTION_SPLIT_ARG                                   \
      , (0, BOOST_PP_SEQ_NIL, 0, BOOST_PP_SEQ_NIL)                           \
      , args \
    )
/**/

#include <boost/preprocessor/seq/size.hpp>

// Calculates [begin, end) arity range.
#define BOOST_PARAMETER_ARITY_RANGE(args)                                    \
    (                                                                        \
        BOOST_PP_SEQ_FOLD_LEFT(BOOST_PARAMETER_ARITY_RANGE_M, 0, args)       \
      , BOOST_PP_INC(BOOST_PP_SEQ_SIZE(args))                                \
    )
/**/

#if !defined(BOOST_NO_SFINAE) && \
    !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x592))
// Expands to an extra argument that is well-formed
// iff all Args... satisfy the requirements set by params.
#define BOOST_PARAMETER_FUNCTION_FORWARD_MATCH(name, params, n, prefix)      \
    typename boost::parameter::aux::match<                                   \
        params, BOOST_PP_ENUM_PARAMS(n, prefix)                              \
    >::type = params()
/**/
#define BOOST_PARAMETER_FUNCTION_FORWARD_MATCH_Z(z, name, params, n, prefix) \
    typename boost::parameter::aux::match<                                   \
        params, BOOST_PP_ENUM_PARAMS_Z(z, n, prefix)                         \
    >::type = params()
/**/
#else
#define BOOST_PARAMETER_FUNCTION_FORWARD_MATCH(name, params, n, prefix)
#define BOOST_PARAMETER_FUNCTION_FORWARD_MATCH_Z(z, name, params, n, prefix)
#endif

#include <boost/parameter/aux_/parenthesized_type.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition/enum.hpp>

#if defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING

// Expands to a forwarding parameter for a constructor or forwarding function.
#define BOOST_PARAMETER_FUNCTION_ARG_TYPE_Z(z, n, type_prefix)               \
    BOOST_PP_CAT(type_prefix, n)&&
/**/

#include <boost/move/utility_core.hpp>

// Expands to an argument passed from a forwarding function to the front-end
// implementation function, or from a constructor to its delegate.
#define BOOST_PARAMETER_FUNCTION_FORWARD_PARAM_Z(z, n, type_prefix)          \
    boost::forward<BOOST_PP_CAT(type_prefix, n)>(BOOST_PP_CAT(a, n))
/**/

// Expands to a forwarding function, whose job is to consolidate its arguments
// into a pack for the front-end implementation function to take in.
#define BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_Z(z, n, data)              \
    BOOST_PP_EXPR_IF(n, template <)                                          \
        BOOST_PP_ENUM_PARAMS_Z(z, n, class ParameterArgumentType)            \
    BOOST_PP_EXPR_IF(n, >)                                                   \
    BOOST_PARAMETER_MEMBER_FUNCTION_STATIC(BOOST_PP_TUPLE_ELEM(3, 1, data))  \
    inline BOOST_PP_EXPR_IF(n, typename)                                     \
    BOOST_PARAMETER_FUNCTION_RESULT_NAME(BOOST_PP_TUPLE_ELEM(3, 1, data))<   \
        BOOST_PP_EXPR_IF(n, typename) boost::parameter::aux::argument_pack<  \
            BOOST_PARAMETER_FUNCTION_SPECIFICATION_NAME(                     \
                BOOST_PP_TUPLE_ELEM(3, 1, data)                              \
            )                                                                \
            BOOST_PP_COMMA_IF(n)                                             \
            BOOST_PP_CAT(BOOST_PP_ENUM_, z)(                                 \
                n                                                            \
              , BOOST_PARAMETER_FUNCTION_ARG_TYPE_Z                          \
              , ParameterArgumentType                                        \
            )                                                                \
        >::type                                                              \
    >::type                                                                  \
    BOOST_PARAMETER_MEMBER_FUNCTION_NAME(BOOST_PP_TUPLE_ELEM(3, 0, data))(   \
        BOOST_PP_ENUM_BINARY_PARAMS_Z(z, n, ParameterArgumentType, && a)     \
        BOOST_PP_COMMA_IF(n)                                                 \
        BOOST_PP_IF(                                                         \
            n                                                                \
          , BOOST_PARAMETER_FUNCTION_FORWARD_MATCH_Z                         \
          , BOOST_PP_TUPLE_EAT(5)                                            \
        )(                                                                   \
            z                                                                \
          , BOOST_PP_TUPLE_ELEM(3, 1, data)                                  \
          , BOOST_PARAMETER_FUNCTION_SPECIFICATION_NAME(                     \
                BOOST_PP_TUPLE_ELEM(3, 1, data)                              \
            )                                                                \
          , n                                                                \
          , ParameterArgumentType                                            \
        )                                                                    \
    ) BOOST_PP_EXPR_IF(BOOST_PP_TUPLE_ELEM(3, 2, data), const)               \
    {                                                                        \
        return BOOST_PARAMETER_FUNCTION_IMPL_NAME(                           \
            BOOST_PP_TUPLE_ELEM(3, 1, data)                                  \
        )(                                                                   \
            BOOST_PARAMETER_FUNCTION_SPECIFICATION_NAME(                     \
                BOOST_PP_TUPLE_ELEM(3, 1, data)                              \
            )()(                                                             \
                BOOST_PP_CAT(BOOST_PP_ENUM_, z)(                             \
                    n                                                        \
                  , BOOST_PARAMETER_FUNCTION_FORWARD_PARAM_Z                 \
                  , ParameterArgumentType                                    \
                )                                                            \
            )                                                                \
        );                                                                   \
    }
/**/

// Expands to a constructor whose job is to consolidate its arguments into a
// pack for the delegate to take in.
#define BOOST_PARAMETER_CONSTRUCTOR_OVERLOAD_Z(z, n, data)                   \
    BOOST_PP_EXPR_IF(n, template <)                                          \
        BOOST_PP_ENUM_PARAMS_Z(z, n, class ParameterArgumentType)            \
    BOOST_PP_EXPR_IF(n, >) BOOST_PP_EXPR_IF(BOOST_PP_EQUAL(n, 1), explicit)  \
    inline BOOST_PP_TUPLE_ELEM(2, 0, data)(                                  \
        BOOST_PP_ENUM_BINARY_PARAMS_Z(z, n, ParameterArgumentType, && a)     \
        BOOST_PP_COMMA_IF(n)                                                 \
        BOOST_PP_IF(                                                         \
            n                                                                \
          , BOOST_PARAMETER_FUNCTION_FORWARD_MATCH_Z                         \
          , BOOST_PP_TUPLE_EAT(5)                                            \
        )(                                                                   \
            z                                                                \
          , BOOST_PP_TUPLE_ELEM(2, 0, data)                                  \
          , BOOST_PP_CAT(constructor_parameters, __LINE__)                   \
          , n                                                                \
          , ParameterArgumentType                                            \
        )                                                                    \
    ) : BOOST_PARAMETER_PARENTHESIZED_TYPE(BOOST_PP_TUPLE_ELEM(2, 1, data))( \
            BOOST_PP_CAT(constructor_parameters, __LINE__)()(                \
                BOOST_PP_CAT(BOOST_PP_ENUM_, z)(                             \
                    n                                                        \
                  , BOOST_PARAMETER_FUNCTION_FORWARD_PARAM_Z                 \
                  , ParameterArgumentType                                    \
                )                                                            \
            )                                                                \
        )                                                                    \
    {                                                                        \
    }
/**/

#else // !defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING

// Expands to a definition of a constructor or forwarding function argument
// passed by const reference.  Used by BOOST_PARAMETER_CONSTRUCTOR_OVERLOAD_0
// and BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_0.
#define BOOST_PARAMETER_FUNCTION_ARG_TYPE_Z(z, n, type_prefix)               \
    BOOST_PP_CAT(type_prefix, n) const&
/**/

// Expands to a forwarding function, whose job is to consolidate its arguments
// into a pack for the front-end implementation function to take in.  Used by
// BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_Z when the number of arguments
// the forwarding function will take in is greater than or equal to
// BOOST_PARAMETER_ALL_CONST_THRESHOLD_ARITY, in which case all the arguments
// will be passed by const reference.
#define BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_0(z, n, data)              \
    BOOST_PP_EXPR_IF(n, template <)                                          \
        BOOST_PP_ENUM_PARAMS_Z(z, n, class ParameterArgumentType)            \
    BOOST_PP_EXPR_IF(n, >)                                                   \
    BOOST_PARAMETER_MEMBER_FUNCTION_STATIC(BOOST_PP_TUPLE_ELEM(3, 1, data))  \
    inline BOOST_PP_EXPR_IF(n, typename)                                     \
    BOOST_PARAMETER_FUNCTION_RESULT_NAME(BOOST_PP_TUPLE_ELEM(3, 1, data))<   \
        BOOST_PP_EXPR_IF(n, typename) boost::parameter::aux::argument_pack<  \
            BOOST_PARAMETER_FUNCTION_SPECIFICATION_NAME(                     \
                BOOST_PP_TUPLE_ELEM(3, 1, data)                              \
            )                                                                \
            BOOST_PP_COMMA_IF(n)                                             \
            BOOST_PP_CAT(BOOST_PP_ENUM_, z)(                                 \
                n                                                            \
              , BOOST_PARAMETER_FUNCTION_ARG_TYPE_Z                          \
              , ParameterArgumentType                                        \
            )                                                                \
        >::type                                                              \
    >::type                                                                  \
    BOOST_PARAMETER_MEMBER_FUNCTION_NAME(BOOST_PP_TUPLE_ELEM(3, 0, data))(   \
        BOOST_PP_ENUM_BINARY_PARAMS_Z(                                       \
            z, n, ParameterArgumentType, const& a                            \
        )                                                                    \
        BOOST_PP_COMMA_IF(n)                                                 \
        BOOST_PP_IF(                                                         \
            n                                                                \
          , BOOST_PARAMETER_FUNCTION_FORWARD_MATCH_Z                         \
          , BOOST_PP_TUPLE_EAT(5)                                            \
        )(                                                                   \
            z                                                                \
          , BOOST_PP_TUPLE_ELEM(3, 1, data)                                  \
          , BOOST_PARAMETER_FUNCTION_SPECIFICATION_NAME(                     \
                BOOST_PP_TUPLE_ELEM(3, 1, data)                              \
            )                                                                \
          , n                                                                \
          , ParameterArgumentType                                            \
        )                                                                    \
    ) BOOST_PP_EXPR_IF(BOOST_PP_TUPLE_ELEM(3, 2, data), const)               \
    {                                                                        \
        return BOOST_PARAMETER_FUNCTION_IMPL_NAME(                           \
            BOOST_PP_TUPLE_ELEM(3, 1, data)                                  \
        )(                                                                   \
            BOOST_PARAMETER_FUNCTION_SPECIFICATION_NAME(                     \
                BOOST_PP_TUPLE_ELEM(3, 1, data)                              \
            )()(BOOST_PP_ENUM_PARAMS_Z(z, n, a))                             \
        );                                                                   \
    }
/**/

// Expands to a constructor whose job is to consolidate its arguments into a
// pack for the delegate to take in.
#define BOOST_PARAMETER_CONSTRUCTOR_OVERLOAD_0(z, n, data)                   \
    BOOST_PP_EXPR_IF(n, template <)                                          \
        BOOST_PP_ENUM_PARAMS_Z(z, n, class ParameterArgumentType)            \
    BOOST_PP_EXPR_IF(n, >) BOOST_PP_EXPR_IF(BOOST_PP_EQUAL(n, 1), explicit)  \
    inline BOOST_PP_TUPLE_ELEM(2, 0, data)(                                  \
        BOOST_PP_ENUM_BINARY_PARAMS_Z(                                       \
            z, n, ParameterArgumentType, const& a                            \
        )                                                                    \
        BOOST_PP_COMMA_IF(n)                                                 \
        BOOST_PP_IF(                                                         \
            n                                                                \
          , BOOST_PARAMETER_FUNCTION_FORWARD_MATCH_Z                         \
          , BOOST_PP_TUPLE_EAT(5)                                            \
        )(                                                                   \
            z                                                                \
          , BOOST_PP_TUPLE_ELEM(2, 0, data)                                  \
          , BOOST_PP_CAT(constructor_parameters, __LINE__)                   \
          , n                                                                \
          , ParameterArgumentType                                            \
        )                                                                    \
    ) : BOOST_PARAMETER_PARENTHESIZED_TYPE(BOOST_PP_TUPLE_ELEM(2, 1, data))( \
            BOOST_PP_CAT(constructor_parameters, __LINE__)()(                \
                BOOST_PP_ENUM_PARAMS_Z(z, n, a)                              \
            )                                                                \
        )                                                                    \
    {                                                                        \
    }
/**/

#include <boost/parameter/aux_/preprocessor/binary_seq_to_args.hpp>

// Expands to a forwarding function, whose job is to consolidate its arguments
// into a pack for the front-end implementation function to take in.  Each
// element in BOOST_PP_SEQ_TAIL(seq) determines the const-ness of the
// corresponding argument.
#define BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_R(r, seq)                  \
    BOOST_PP_EXPR_IF(BOOST_PP_SEQ_SIZE(BOOST_PP_SEQ_TAIL(seq)), template <)  \
        BOOST_PP_ENUM_PARAMS(                                                \
            BOOST_PP_SEQ_SIZE(BOOST_PP_SEQ_TAIL(seq))                        \
          , class ParameterArgumentType                                      \
        )                                                                    \
    BOOST_PP_EXPR_IF(BOOST_PP_SEQ_SIZE(BOOST_PP_SEQ_TAIL(seq)), >)           \
    inline BOOST_PARAMETER_MEMBER_FUNCTION_STATIC(                           \
        BOOST_PP_TUPLE_ELEM(3, 1, BOOST_PP_SEQ_HEAD(seq))                    \
    )                                                                        \
    BOOST_PP_EXPR_IF(BOOST_PP_SEQ_SIZE(BOOST_PP_SEQ_TAIL(seq)), typename)    \
    BOOST_PARAMETER_FUNCTION_RESULT_NAME(                                    \
        BOOST_PP_TUPLE_ELEM(3, 1, BOOST_PP_SEQ_HEAD(seq))                    \
    )<BOOST_PP_EXPR_IF(BOOST_PP_SEQ_SIZE(BOOST_PP_SEQ_TAIL(seq)), typename)  \
        boost::parameter::aux::argument_pack<                                \
            BOOST_PARAMETER_FUNCTION_SPECIFICATION_NAME(                     \
                BOOST_PP_TUPLE_ELEM(3, 1, BOOST_PP_SEQ_HEAD(seq))            \
            )                                                                \
            BOOST_PP_COMMA_IF(BOOST_PP_SEQ_SIZE(BOOST_PP_SEQ_TAIL(seq)))     \
            BOOST_PARAMETER_AUX_PP_BINARY_SEQ_TO_ARGS(                       \
                BOOST_PP_SEQ_TAIL(seq), (ParameterArgumentType)              \
            )                                                                \
        >::type                                                              \
    >::type                                                                  \
    BOOST_PARAMETER_MEMBER_FUNCTION_NAME(                                    \
        BOOST_PP_TUPLE_ELEM(3, 0, BOOST_PP_SEQ_HEAD(seq))                    \
    )(                                                                       \
        BOOST_PARAMETER_AUX_PP_BINARY_SEQ_TO_ARGS(                           \
            BOOST_PP_SEQ_TAIL(seq), (ParameterArgumentType)(a)               \
        )                                                                    \
        BOOST_PP_COMMA_IF(BOOST_PP_SEQ_SIZE(BOOST_PP_SEQ_TAIL(seq)))         \
        BOOST_PP_IF(                                                         \
            BOOST_PP_SEQ_SIZE(BOOST_PP_SEQ_TAIL(seq))                        \
          , BOOST_PARAMETER_FUNCTION_FORWARD_MATCH                           \
          , BOOST_PP_TUPLE_EAT(4)                                            \
        )(                                                                   \
            BOOST_PP_TUPLE_ELEM(3, 1, BOOST_PP_SEQ_HEAD(seq))                \
          , BOOST_PARAMETER_FUNCTION_SPECIFICATION_NAME(                     \
                BOOST_PP_TUPLE_ELEM(3, 1, BOOST_PP_SEQ_HEAD(seq))            \
            )                                                                \
          , BOOST_PP_SEQ_SIZE(BOOST_PP_SEQ_TAIL(seq))                        \
          , ParameterArgumentType                                            \
        )                                                                    \
    ) BOOST_PP_EXPR_IF(                                                      \
        BOOST_PP_TUPLE_ELEM(3, 2, BOOST_PP_SEQ_HEAD(seq)), const             \
    )                                                                        \
    {                                                                        \
        return BOOST_PARAMETER_FUNCTION_IMPL_NAME(                           \
            BOOST_PP_TUPLE_ELEM(3, 1, BOOST_PP_SEQ_HEAD(seq))                \
        )(                                                                   \
            BOOST_PARAMETER_FUNCTION_SPECIFICATION_NAME(                     \
                BOOST_PP_TUPLE_ELEM(3, 1, BOOST_PP_SEQ_HEAD(seq))            \
            )()(                                                             \
                BOOST_PP_ENUM_PARAMS(                                        \
                    BOOST_PP_SEQ_SIZE(BOOST_PP_SEQ_TAIL(seq)), a             \
                )                                                            \
            )                                                                \
        );                                                                   \
    }
/**/

// Expands to a constructor whose job is to consolidate its arguments into a
// pack for the delegate to take in.  Each element in BOOST_PP_SEQ_TAIL(seq)
// determines the const-ness of the corresponding argument.
#define BOOST_PARAMETER_CONSTRUCTOR_OVERLOAD_R(r, seq)                       \
    BOOST_PP_EXPR_IF(BOOST_PP_SEQ_SIZE(BOOST_PP_SEQ_TAIL(seq)), template <)  \
        BOOST_PP_ENUM_PARAMS(                                                \
            BOOST_PP_SEQ_SIZE(BOOST_PP_SEQ_TAIL(seq))                        \
          , class ParameterArgumentType                                      \
        )                                                                    \
    BOOST_PP_EXPR_IF(BOOST_PP_SEQ_SIZE(BOOST_PP_SEQ_TAIL(seq)), >)           \
    inline BOOST_PP_EXPR_IF(                                                 \
        BOOST_PP_EQUAL(BOOST_PP_SEQ_SIZE(BOOST_PP_SEQ_TAIL(seq)), 1)         \
      , explicit                                                             \
    )                                                                        \
    BOOST_PP_TUPLE_ELEM(2, 0, BOOST_PP_SEQ_HEAD(seq))(                       \
        BOOST_PARAMETER_AUX_PP_BINARY_SEQ_TO_ARGS(                           \
            BOOST_PP_SEQ_TAIL(seq), (ParameterArgumentType)(a)               \
        )                                                                    \
        BOOST_PP_COMMA_IF(BOOST_PP_SEQ_SIZE(BOOST_PP_SEQ_TAIL(seq)))         \
        BOOST_PP_IF(                                                         \
            BOOST_PP_SEQ_SIZE(BOOST_PP_SEQ_TAIL(seq))                        \
          , BOOST_PARAMETER_FUNCTION_FORWARD_MATCH                           \
          , BOOST_PP_TUPLE_EAT(4)                                            \
        )(                                                                   \
            BOOST_PP_TUPLE_ELEM(2, 0, BOOST_PP_SEQ_HEAD(seq))                \
          , BOOST_PP_CAT(constructor_parameters, __LINE__)                   \
          , BOOST_PP_SEQ_SIZE(BOOST_PP_SEQ_TAIL(seq))                        \
          , ParameterArgumentType                                            \
        )                                                                    \
    ) : BOOST_PARAMETER_PARENTHESIZED_TYPE(                                  \
            BOOST_PP_TUPLE_ELEM(2, 1, BOOST_PP_SEQ_HEAD(seq))                \
        )(                                                                   \
            BOOST_PP_CAT(constructor_parameters, __LINE__)()(                \
                BOOST_PP_ENUM_PARAMS(                                        \
                    BOOST_PP_SEQ_SIZE(BOOST_PP_SEQ_TAIL(seq)), a             \
                )                                                            \
            )                                                                \
        )                                                                    \
    {                                                                        \
    }
/**/

#include <boost/parameter/aux_/preprocessor/binary_seq_for_each.hpp>

// Expands to all forwarding functions that take in n arguments.  Used by
// BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_Z when n is less than
// BOOST_PARAMETER_ALL_CONST_THRESHOLD_ARITY.
#define BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_1(z, n, data)              \
    BOOST_PARAMETER_AUX_PP_BINARY_SEQ_FOR_EACH_Z(                            \
        z, n, (BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_R)(data)            \
    )
/**/

// Expands to all constructors that take in n arguments.  Used by
// BOOST_PARAMETER_CONSTRUCTOR_OVERLOAD_Z when n is less than
// BOOST_PARAMETER_ALL_CONST_THRESHOLD_ARITY.
#define BOOST_PARAMETER_CONSTRUCTOR_OVERLOAD_1(z, n, data)                   \
    BOOST_PARAMETER_AUX_PP_BINARY_SEQ_FOR_EACH_Z(                            \
        z, n, (BOOST_PARAMETER_CONSTRUCTOR_OVERLOAD_R)(data)                 \
    )
/**/

#include <boost/preprocessor/comparison/less.hpp>
#include <boost/preprocessor/logical/and.hpp>

// Enables BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOADS_AUX to use either
// BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_0 or
// BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_1 within BOOST_PP_REPEAT_FROM_TO.
#define BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_Z(z, n, data)              \
    BOOST_PP_CAT(                                                            \
        BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_                           \
      , BOOST_PP_AND(                                                        \
            BOOST_PP_LESS(n, BOOST_PARAMETER_ALL_CONST_THRESHOLD_ARITY), n   \
        )                                                                    \
    )(z, n, data)
/**/

// Enables BOOST_PARAMETER_CONSTRUCTOR_OVERLOADS_AUX to use either
// BOOST_PARAMETER_CONSTRUCTOR_OVERLOAD_0 or
// BOOST_PARAMETER_CONSTRUCTOR_OVERLOAD_1 within BOOST_PP_REPEAT_FROM_TO.
#define BOOST_PARAMETER_CONSTRUCTOR_OVERLOAD_Z(z, n, data)                   \
    BOOST_PP_CAT(                                                            \
        BOOST_PARAMETER_CONSTRUCTOR_OVERLOAD_                                \
      , BOOST_PP_LESS(n, BOOST_PARAMETER_ALL_CONST_THRESHOLD_ARITY)          \
    )(z, n, data)
/**/

#endif // BOOST_PARAMETER_HAS_PERFECT_FORWARDING

#include <boost/preprocessor/repetition/repeat_from_to.hpp>

// Helper macro for BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOADS.
#define BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOADS_AUX(name, impl, range, c) \
    BOOST_PP_REPEAT_FROM_TO(                                                 \
        BOOST_PP_TUPLE_ELEM(2, 0, range)                                     \
      , BOOST_PP_TUPLE_ELEM(2, 1, range)                                     \
      , BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_Z                          \
      , (name, impl, c)                                                      \
    )
/**/

// Expands to the layer of forwarding functions for the function with the
// specified name, whose arguments determine the range of arities.
#define BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOADS(name, impl, args, const_) \
    BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOADS_AUX(                          \
        name, impl, BOOST_PARAMETER_ARITY_RANGE(args), const_                \
    )
/**/

// Helper macro for BOOST_PARAMETER_CONSTRUCTOR_OVERLOADS.
#define BOOST_PARAMETER_CONSTRUCTOR_OVERLOADS_AUX(class_, base, range)       \
    BOOST_PP_REPEAT_FROM_TO(                                                 \
        BOOST_PP_TUPLE_ELEM(2, 0, range)                                     \
      , BOOST_PP_TUPLE_ELEM(2, 1, range)                                     \
      , BOOST_PARAMETER_CONSTRUCTOR_OVERLOAD_Z                               \
      , (class_, base)                                                       \
    )
/**/

// Expands to the layer of forwarding functions for the constructor in the
// specified class, whose arguments determine the range of arities.
#define BOOST_PARAMETER_CONSTRUCTOR_OVERLOADS(class_, base, args)            \
    BOOST_PARAMETER_CONSTRUCTOR_OVERLOADS_AUX(                               \
        class_, base, BOOST_PARAMETER_ARITY_RANGE(args)                      \
    )
/**/

// Helper macros for BOOST_PARAMETER_SPECIFICATION_ELEM_R.
#define BOOST_PARAMETER_QUALIFIED_TAG_optional(tag)                          \
    optional<tag
/**/

#define BOOST_PARAMETER_QUALIFIED_TAG_required(tag)                          \
    required<tag
/**/

#define BOOST_PARAMETER_QUALIFIED_TAG_deduced_optional(tag)                  \
    optional<boost::parameter::deduced<tag>
/**/

#define BOOST_PARAMETER_QUALIFIED_TAG_deduced_required(tag)                  \
    required<boost::parameter::deduced<tag>
/**/

#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))

#define BOOST_PARAMETER_SPECIFICATION_ELEM_R(r, tag_namespace, i, elem)      \
    BOOST_PP_COMMA_IF(i) boost::parameter::BOOST_PP_CAT(                     \
        BOOST_PARAMETER_QUALIFIED_TAG_                                       \
      , BOOST_PARAMETER_FN_ARG_QUALIFIER(elem)                               \
    )(tag_namespace::BOOST_PARAMETER_FN_ARG_NAME(elem))                      \
      , boost::parameter::aux::use_default                                   \
    >
/**/

#else // !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))

// Expands to each boost::parameter::parameters<> element type.
#define BOOST_PARAMETER_SPECIFICATION_ELEM_R(r, tag_namespace, i, elem)      \
    BOOST_PP_COMMA_IF(i) boost::parameter::BOOST_PP_CAT(                     \
        BOOST_PARAMETER_QUALIFIED_TAG_                                       \
      , BOOST_PARAMETER_FN_ARG_QUALIFIER(elem)                               \
    )(tag_namespace::BOOST_PARAMETER_FN_ARG_NAME(elem))                      \
      , typename boost::parameter::aux::unwrap_predicate<                    \
            void BOOST_PARAMETER_FN_ARG_PRED(elem)                           \
        >::type                                                              \
    >
/**/

#endif // Borland workarounds needed.

#include <boost/preprocessor/seq/for_each_i.hpp>

// Expands to a boost::parameter::parameters<> specialization for the
// function named base.  Used by BOOST_PARAMETER_CONSTRUCTOR_AUX and
// BOOST_PARAMETER_FUNCTION_HEAD for their respective ParameterSpec models.
#define BOOST_PARAMETER_SPECIFICATION(tag_ns, base, split_args)              \
    template <class BoostParameterDummy>                                     \
    struct BOOST_PP_CAT(                                                     \
        BOOST_PP_CAT(boost_param_params_, __LINE__)                          \
      , BOOST_PARAMETER_MEMBER_FUNCTION_NAME(base)                           \
    ) : boost::parameter::parameters<                                        \
            BOOST_PP_SEQ_FOR_EACH_I(                                         \
                BOOST_PARAMETER_SPECIFICATION_ELEM_R, tag_ns, split_args     \
            )                                                                \
        >                                                                    \
    {                                                                        \
    };                                                                       \
    typedef BOOST_PP_CAT(                                                    \
        BOOST_PP_CAT(boost_param_params_, __LINE__)                          \
      , BOOST_PARAMETER_MEMBER_FUNCTION_NAME(base)                           \
    )<int>
/**/

// Helper macro for BOOST_PARAMETER_CONSTRUCTOR.
#define BOOST_PARAMETER_CONSTRUCTOR_AUX(class_, base, tag_namespace, args)   \
    BOOST_PARAMETER_SPECIFICATION(tag_namespace, ctor, args)                 \
        BOOST_PP_CAT(constructor_parameters, __LINE__);                      \
    BOOST_PARAMETER_CONSTRUCTOR_OVERLOADS(class_, base, args)
/**/

// Expands to the result metafunction and the parameters specialization.
#define BOOST_PARAMETER_FUNCTION_HEAD(result, name, tag_namespace, args)     \
    template <class Args>                                                    \
    struct BOOST_PARAMETER_FUNCTION_RESULT_NAME(name)                        \
    {                                                                        \
        typedef typename BOOST_PARAMETER_PARENTHESIZED_TYPE(result) type;    \
    };                                                                       \
    BOOST_PARAMETER_SPECIFICATION(tag_namespace, name, args)                 \
        BOOST_PARAMETER_FUNCTION_SPECIFICATION_NAME(name);
/**/

// Helper macro for BOOST_PARAMETER_BASIC_FUNCTION.
#define BOOST_PARAMETER_BASIC_FUNCTION_AUX(result, name, tag_ns, args)       \
    BOOST_PARAMETER_FUNCTION_HEAD(result, name, tag_ns, args)                \
    BOOST_PARAMETER_FUNCTION_IMPL_HEAD(name);                                \
    BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOADS(name, name, args, 0)          \
    BOOST_PARAMETER_FUNCTION_IMPL_HEAD(name)
/**/

// Helper macro for BOOST_PARAMETER_BASIC_MEMBER_FUNCTION
// and BOOST_PARAMETER_BASIC_CONST_MEMBER_FUNCTION.
#define BOOST_PARAMETER_BASIC_MEMBER_FUNCTION_AUX(r, name, tag_ns, args, c)  \
    BOOST_PARAMETER_FUNCTION_HEAD(r, name, tag_ns, args)                     \
    BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOADS(name, name, args, c)          \
    BOOST_PARAMETER_FUNCTION_IMPL_HEAD(name) BOOST_PP_EXPR_IF(c, const)
/**/

#include <boost/parameter/aux_/preprocessor/flatten.hpp>

// Expands to a Boost.Parameter-enabled constructor header.  All arguments are
// accessible via args and keywords only.
#define BOOST_PARAMETER_CONSTRUCTOR(class_, base, tag_namespace, args)       \
    BOOST_PARAMETER_CONSTRUCTOR_AUX(                                         \
        class_, base, tag_namespace, BOOST_PARAMETER_FLATTEN(2, 2, 3, args)  \
    )
/**/

// Expands to a Boost.Parameter-enabled function header.  All arguments are
// accessible via args and keywords only.
#define BOOST_PARAMETER_BASIC_FUNCTION(result, name, tag_namespace, args)    \
    BOOST_PARAMETER_BASIC_FUNCTION_AUX(                                      \
        result, name, tag_namespace, BOOST_PARAMETER_FLATTEN(2, 2, 3, args)  \
    )
/**/

// Expands to a Boost.Parameter-enabled member function header.  All arguments
// are accessible via args and keywords only.
#define BOOST_PARAMETER_BASIC_MEMBER_FUNCTION(result, name, tag_ns, args)    \
    BOOST_PARAMETER_BASIC_MEMBER_FUNCTION_AUX(                               \
        result, name, tag_ns, BOOST_PARAMETER_FLATTEN(2, 2, 3, args), 0      \
    )
/**/

// Expands to a Boost.Parameter-enabled const-qualified member function
// header.  All arguments are accessible via args and keywords only.
#define BOOST_PARAMETER_BASIC_CONST_MEMBER_FUNCTION(r, name, tag_ns, args)   \
    BOOST_PARAMETER_BASIC_MEMBER_FUNCTION_AUX(                               \
        r, name, tag_ns, BOOST_PARAMETER_FLATTEN(2, 2, 3, args), 1           \
    )
/**/

// Expands to keyword_tag_type for some keyword_tag.
#define BOOST_PARAMETER_FUNCTION_DISPATCH_ARG_TYPE(keyword_tag)              \
    BOOST_PP_CAT(BOOST_PP_CAT(keyword_tag, _), type)
/**/

// Expands to a template parameter for each dispatch function.
#define BOOST_PARAMETER_FUNCTION_DISPATCH_TEMPLATE_ARG(r, macro, arg)        \
  , class BOOST_PARAMETER_FUNCTION_DISPATCH_ARG_TYPE(macro(arg))
/**/

#if defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING

// Expands to a forwarding parameter for a dispatch function.
#define BOOST_PARAMETER_FUNCTION_DISPATCH_ARG_DEFN(r, macro, arg)            \
  , BOOST_PARAMETER_FUNCTION_DISPATCH_ARG_TYPE(macro(arg))&& macro(arg)
/**/

// Expands to an argument passed from a dispatch function to the dispatch
// function with the next higher number of specified parameters.
#define BOOST_PARAMETER_FUNCTION_DISPATCH_ARG_FWD(r, macro, arg)             \
  , boost::forward<                                                          \
        BOOST_PARAMETER_FUNCTION_DISPATCH_ARG_TYPE(macro(arg))               \
    >(macro(arg))
/**/

#else // !defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING

// Expands to a forwarding parameter for a dispatch function.  The parameter
// type stores its const-ness.
#define BOOST_PARAMETER_FUNCTION_DISPATCH_ARG_DEFN(r, macro, arg)            \
  , BOOST_PARAMETER_FUNCTION_DISPATCH_ARG_TYPE(macro(arg))& macro(arg)
/**/

// Expands to an argument passed from a dispatch function to the dispatch
// function with the next higher number of specified parameters.  Explicit
// forwarding not needed.
#define BOOST_PARAMETER_FUNCTION_DISPATCH_ARG_FWD(r, macro, arg)             \
  , macro(arg)
/**/

#endif // BOOST_PARAMETER_HAS_PERFECT_FORWARDING

#include <boost/preprocessor/seq/first_n.hpp>

// Iterates through all required arguments and the first n optional arguments,
// passing each argument to the specified macro.
#define BOOST_PARAMETER_FUNCTION_DISPATCH_ARG_REPEAT(macro, n, split_args)   \
    BOOST_PP_SEQ_FOR_EACH(                                                   \
        macro                                                                \
      , BOOST_PARAMETER_FN_ARG_NAME                                          \
      , BOOST_PARAMETER_SPLIT_ARG_REQ_SEQ(split_args)                        \
    )                                                                        \
    BOOST_PP_SEQ_FOR_EACH(                                                   \
        macro                                                                \
      , BOOST_PARAMETER_FN_ARG_NAME                                          \
      , BOOST_PP_SEQ_FIRST_N(                                                \
            n, BOOST_PARAMETER_SPLIT_ARG_OPT_SEQ(split_args)                 \
        )                                                                    \
    )
/**/

// Expands to the template parameter list of the dispatch function with the
// specified number of non-defaulted parameters; also extracts the static
// keyword if present.
#define BOOST_PARAMETER_FUNCTION_DISPATCH_HEAD_0(n, x)                       \
    template <                                                               \
        class ResultType, class Args                                         \
        BOOST_PARAMETER_FUNCTION_DISPATCH_ARG_REPEAT(                        \
            BOOST_PARAMETER_FUNCTION_DISPATCH_TEMPLATE_ARG                   \
          , n                                                                \
          , BOOST_PARAMETER_FUNCTION_DISPATCH_SPLIT_ARGS(x)                  \
        )                                                                    \
    > BOOST_PARAMETER_MEMBER_FUNCTION_STATIC(                                \
        BOOST_PARAMETER_FUNCTION_DISPATCH_BASE_NAME(x)                       \
    )
/**/

// Expands to the argument-pack parameter and all non-defaulted parameters.
#define BOOST_PARAMETER_FUNCTION_DISPATCH_HEAD_1_PARAMS_FIRST(n, x)          \
        ResultType(*)(), Args const& args, long                              \
        BOOST_PARAMETER_FUNCTION_DISPATCH_ARG_REPEAT(                        \
            BOOST_PARAMETER_FUNCTION_DISPATCH_ARG_DEFN                       \
          , n                                                                \
          , BOOST_PARAMETER_FUNCTION_DISPATCH_SPLIT_ARGS(x)                  \
        )
/**/

#define BOOST_PARAMETER_FUNCTION_DISPATCH_HEAD_1_PARAMS_LAST_Z(z, n, p) , p
/**/

#include <boost/preprocessor/arithmetic/sub.hpp>

// Expands to the result type, name, parenthesized parameter list, and
// const-ness of the dispatch function with the specified number of
// non-defaulted parameters.
#define BOOST_PARAMETER_FUNCTION_DISPATCH_HEAD_1(n, x)                       \
    ResultType BOOST_PARAMETER_FUNCTION_DISPATCH_NAME(x)(                    \
        BOOST_PARAMETER_FUNCTION_DISPATCH_HEAD_1_PARAMS_FIRST(n, x)          \
        BOOST_PP_REPEAT_FROM_TO(                                             \
            0                                                                \
          , BOOST_PP_SUB(                                                    \
                BOOST_PARAMETER_SPLIT_ARG_OPT_COUNT(                         \
                    BOOST_PARAMETER_FUNCTION_DISPATCH_SPLIT_ARGS(x)          \
                )                                                            \
              , n                                                            \
            )                                                                \
          , BOOST_PARAMETER_FUNCTION_DISPATCH_HEAD_1_PARAMS_LAST_Z           \
          , boost::parameter::aux::use_default_tag                           \
        )                                                                    \
    ) BOOST_PP_EXPR_IF(BOOST_PARAMETER_FUNCTION_DISPATCH_IS_CONST(x), const)
/**/

// BOOST_PP_REPEAT_FROM_TO-reentrant version of
// BOOST_PARAMETER_FUNCTION_DISPATCH_HEAD_1.
#define BOOST_PARAMETER_FUNCTION_DISPATCH_HEAD_1_Z(z, n, x)                  \
    ResultType BOOST_PARAMETER_FUNCTION_DISPATCH_NAME(x)(                    \
        BOOST_PARAMETER_FUNCTION_DISPATCH_HEAD_1_PARAMS_FIRST(n, x)          \
        BOOST_PP_CAT(BOOST_PP_REPEAT_FROM_TO_, z)(                           \
            0                                                                \
          , BOOST_PP_SUB(                                                    \
                BOOST_PARAMETER_SPLIT_ARG_OPT_COUNT(                         \
                    BOOST_PARAMETER_FUNCTION_DISPATCH_SPLIT_ARGS(x)          \
                )                                                            \
              , n                                                            \
            )                                                                \
          , BOOST_PARAMETER_FUNCTION_DISPATCH_HEAD_1_PARAMS_LAST_Z           \
          , boost::parameter::aux::use_default_tag                           \
        )                                                                    \
    ) BOOST_PP_EXPR_IF(BOOST_PARAMETER_FUNCTION_DISPATCH_IS_CONST(x), const)
/**/

// Expands to a forward declaration of the dispatch function with the
// specified number of non-defaulted parameters.
#define BOOST_PARAMETER_FUNCTION_DISPATCH_FWD_DECL_Z(z, n, x)                \
    BOOST_PARAMETER_FUNCTION_DISPATCH_HEAD_0(n, x)                           \
    BOOST_PARAMETER_FUNCTION_DISPATCH_HEAD_1_Z(z, n, x);
/**/

#include <boost/preprocessor/seq/elem.hpp>

// Expands to the default value of the (n + 1)th optional parameter.
#define BOOST_PARAMETER_FUNCTION_DISPATCH_DEFAULT_AUX(n, s_args)             \
    BOOST_PARAMETER_FN_ARG_DEFAULT(                                          \
        BOOST_PP_SEQ_ELEM(n, BOOST_PARAMETER_SPLIT_ARG_OPT_SEQ(s_args))      \
    )
/**/

// Expands to the assignment portion which binds the default value to the
// (n + 1)th optional parameter before composing it with the argument-pack
// parameter of a dispatch function.
#define BOOST_PARAMETER_FUNCTION_DISPATCH_ARG_DFLT(n, s_args, tag_ns)        \
    boost::parameter::keyword<                                               \
        tag_ns::BOOST_PARAMETER_FN_ARG_NAME(                                 \
            BOOST_PP_SEQ_ELEM(n, BOOST_PARAMETER_SPLIT_ARG_OPT_SEQ(s_args))  \
        )                                                                    \
    >::instance = BOOST_PARAMETER_FUNCTION_DISPATCH_DEFAULT_AUX(n, s_args)
/**/

#if defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING

// Each dispatch function passes the default value of the (n + 1)th optional
// parameter to the next dispatch function.
#define BOOST_PARAMETER_FUNCTION_DISPATCH_DEFAULT(n, s_args)                 \
    BOOST_PARAMETER_FUNCTION_DISPATCH_DEFAULT_AUX(n, s_args)
/**/

#else

namespace boost { namespace parameter { namespace aux {

    template <class T>
    T& as_lvalue(T& value, long)
    {
        return value;
    }

    template <class T>
    T const& as_lvalue(T const& value, int)
    {
        return value;
    }
}}} // namespace boost::parameter::aux

// Default values are often rvalues, which cannot be implicitly bound to
// non-const lvalue references.
#define BOOST_PARAMETER_FUNCTION_DISPATCH_DEFAULT(n, s_args)                 \
    boost::parameter::aux::as_lvalue(                                        \
        BOOST_PARAMETER_FUNCTION_DISPATCH_DEFAULT_AUX(n, s_args), 0L         \
    )
/**/

#endif // BOOST_PARAMETER_HAS_PERFECT_FORWARDING

// Expands to the dispatch function with the specified number of non-defaulted
// parameters.
//
// x is a tuple:
//
//   (name, split_args, is_const, tag_namespace)
//
// Where name is the base name of the function, and split_args is a tuple:
//
//   (required_count, required_args, optional_count, required_args)
//
#define BOOST_PARAMETER_FUNCTION_DISPATCH_OVERLOAD_Z(z, n, x)                \
    BOOST_PARAMETER_FUNCTION_DISPATCH_HEAD_0(n, x)                           \
    inline BOOST_PARAMETER_FUNCTION_DISPATCH_HEAD_1_Z(z, n, x)               \
    {                                                                        \
        return BOOST_PARAMETER_FUNCTION_DISPATCH_NAME(x)(                    \
            static_cast<ResultType(*)()>(BOOST_TTI_DETAIL_NULLPTR)           \
          , (args                                                            \
              , BOOST_PARAMETER_FUNCTION_DISPATCH_ARG_DFLT(                  \
                    n                                                        \
                  , BOOST_PARAMETER_FUNCTION_DISPATCH_SPLIT_ARGS(x)          \
                  , BOOST_PARAMETER_FUNCTION_DISPATCH_TAG_NAMESPACE(x)       \
                )                                                            \
            )                                                                \
          , 0L                                                               \
            BOOST_PARAMETER_FUNCTION_DISPATCH_ARG_REPEAT(                    \
                BOOST_PARAMETER_FUNCTION_DISPATCH_ARG_FWD                    \
              , n                                                            \
              , BOOST_PARAMETER_FUNCTION_DISPATCH_SPLIT_ARGS(x)              \
            )                                                                \
          , BOOST_PARAMETER_FUNCTION_DISPATCH_DEFAULT(                       \
                n, BOOST_PARAMETER_FUNCTION_DISPATCH_SPLIT_ARGS(x)           \
            )                                                                \
            BOOST_PP_CAT(BOOST_PP_REPEAT_FROM_TO_, z)(                       \
                0                                                            \
              , BOOST_PP_SUB(                                                \
                    BOOST_PARAMETER_SPLIT_ARG_OPT_COUNT(                     \
                        BOOST_PARAMETER_FUNCTION_DISPATCH_SPLIT_ARGS(x)      \
                    )                                                        \
                  , BOOST_PP_INC(n)                                          \
                )                                                            \
              , BOOST_PARAMETER_FUNCTION_DISPATCH_HEAD_1_PARAMS_LAST_Z       \
              , boost::parameter::aux::use_default_tag()                     \
            )                                                                \
        );                                                                   \
    }
/**/

// x is a tuple:
//
//   (base_name, split_args, is_const, tag_namespace)
//
// Generates all dispatch functions for the function named base_name.  Each
// dispatch function that takes in n optional parameters passes the default
// value of the (n + 1)th optional parameter to the next dispatch function.
// The last dispatch function is the back-end implementation, so only the
// header is generated: the user is expected to supply the body.
//
// Also generates the front-end implementation function, which uses
// BOOST_PARAMETER_FUNCTION_CAST to extract each argument from the argument
// pack.
#define BOOST_PARAMETER_FUNCTION_DISPATCH_LAYER(x, fwd_decl)                 \
    BOOST_PP_IF(fwd_decl, BOOST_PP_REPEAT_FROM_TO, BOOST_PP_TUPLE_EAT(4))(   \
        0                                                                    \
      , BOOST_PP_INC(                                                        \
            BOOST_PARAMETER_SPLIT_ARG_OPT_COUNT(                             \
                BOOST_PARAMETER_FUNCTION_DISPATCH_SPLIT_ARGS(x)              \
            )                                                                \
        )                                                                    \
      , BOOST_PARAMETER_FUNCTION_DISPATCH_FWD_DECL_Z                         \
      , x                                                                    \
    )                                                                        \
    template <class Args> BOOST_PARAMETER_MEMBER_FUNCTION_STATIC(            \
        BOOST_PARAMETER_FUNCTION_DISPATCH_BASE_NAME(x)                       \
    ) inline typename BOOST_PARAMETER_FUNCTION_RESULT_NAME(                  \
        BOOST_PARAMETER_FUNCTION_DISPATCH_BASE_NAME(x)                       \
    )<Args>::type BOOST_PARAMETER_FUNCTION_IMPL_NAME(                        \
        BOOST_PARAMETER_FUNCTION_DISPATCH_BASE_NAME(x)                       \
    )(Args const& args)                                                      \
    BOOST_PP_EXPR_IF(BOOST_PARAMETER_FUNCTION_DISPATCH_IS_CONST(x), const)   \
    {                                                                        \
        return BOOST_PARAMETER_FUNCTION_DISPATCH_NAME(x)(                    \
            static_cast<                                                     \
                typename BOOST_PARAMETER_FUNCTION_RESULT_NAME(               \
                    BOOST_PARAMETER_FUNCTION_DISPATCH_BASE_NAME(x)           \
                )<Args>::type(*)()                                           \
            >(BOOST_TTI_DETAIL_NULLPTR)                                      \
          , args                                                             \
          , 0L                                                               \
            BOOST_PP_SEQ_FOR_EACH(                                           \
                BOOST_PARAMETER_FUNCTION_DISPATCH_ARG_CAST_0_R               \
              , BOOST_PARAMETER_FUNCTION_DISPATCH_TAG_NAMESPACE(x)           \
              , BOOST_PARAMETER_SPLIT_ARG_REQ_SEQ(                           \
                    BOOST_PARAMETER_FUNCTION_DISPATCH_SPLIT_ARGS(x)          \
                )                                                            \
            )                                                                \
            BOOST_PP_SEQ_FOR_EACH(                                           \
                BOOST_PARAMETER_FUNCTION_DISPATCH_ARG_CAST_1_R               \
              , BOOST_PARAMETER_FUNCTION_DISPATCH_TAG_NAMESPACE(x)           \
              , BOOST_PARAMETER_SPLIT_ARG_OPT_SEQ(                           \
                    BOOST_PARAMETER_FUNCTION_DISPATCH_SPLIT_ARGS(x)          \
                )                                                            \
            )                                                                \
        );                                                                   \
    }                                                                        \
    BOOST_PP_REPEAT_FROM_TO(                                                 \
        0                                                                    \
      , BOOST_PARAMETER_SPLIT_ARG_OPT_COUNT(                                 \
            BOOST_PARAMETER_FUNCTION_DISPATCH_SPLIT_ARGS(x)                  \
        )                                                                    \
      , BOOST_PARAMETER_FUNCTION_DISPATCH_OVERLOAD_Z                         \
      , x                                                                    \
    )                                                                        \
    BOOST_PARAMETER_FUNCTION_DISPATCH_HEAD_0(                                \
        BOOST_PARAMETER_SPLIT_ARG_OPT_COUNT(                                 \
            BOOST_PARAMETER_FUNCTION_DISPATCH_SPLIT_ARGS(x)                  \
        )                                                                    \
      , x                                                                    \
    )                                                                        \
    inline BOOST_PARAMETER_FUNCTION_DISPATCH_HEAD_1(                         \
        BOOST_PARAMETER_SPLIT_ARG_OPT_COUNT(                                 \
            BOOST_PARAMETER_FUNCTION_DISPATCH_SPLIT_ARGS(x)                  \
        )                                                                    \
      , x                                                                    \
    )
/**/

// Helper macro for BOOST_PARAMETER_FUNCTION.
#define BOOST_PARAMETER_FUNCTION_AUX(result, name, tag_ns, args)             \
    BOOST_PARAMETER_FUNCTION_HEAD(result, name, tag_ns, args)                \
    BOOST_PARAMETER_FUNCTION_IMPL_HEAD(name);                                \
    BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOADS(name, name, args, 0)          \
    BOOST_PARAMETER_FUNCTION_DISPATCH_LAYER(                                 \
        (name, BOOST_PARAMETER_FUNCTION_SPLIT_ARGS(args), 0, tag_ns), 1      \
    )
/**/

// Expands to a Boost.Parameter-enabled function header.  All arguments are
// accessible via args and keywords, as well as by name.
#define BOOST_PARAMETER_FUNCTION(result, name, tag_namespace, args)          \
    BOOST_PARAMETER_FUNCTION_AUX(                                            \
        result, name, tag_namespace, BOOST_PARAMETER_FLATTEN(3, 2, 3, args)  \
    )
/**/

// Helper macro for BOOST_PARAMETER_MEMBER_FUNCTION
// BOOST_PARAMETER_CONST_MEMBER_FUNCTION,
// BOOST_PARAMETER_FUNCTION_CALL_OPERATOR, and
// BOOST_PARAMETER_CONST_FUNCTION_CALL_OPERATOR.
#define BOOST_PARAMETER_MEMBER_FUNCTION_AUX(r, name, impl, tag_ns, c, args)  \
    BOOST_PARAMETER_FUNCTION_HEAD(r, impl, tag_ns, args)                     \
    BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOADS(name, impl, args, c)          \
    BOOST_PARAMETER_FUNCTION_DISPATCH_LAYER(                                 \
        (impl, BOOST_PARAMETER_FUNCTION_SPLIT_ARGS(args), c, tag_ns), 0      \
    )
/**/

// Expands to a Boost.Parameter-enabled member function header.  All
// arguments are accessible via args and keywords, as well as by name.
#define BOOST_PARAMETER_MEMBER_FUNCTION(result, name, tag_ns, args)          \
    BOOST_PARAMETER_MEMBER_FUNCTION_AUX(                                     \
        result, name, name, tag_ns, 0                                        \
      , BOOST_PARAMETER_FLATTEN(3, 2, 3, args)                               \
    )
/**/

// Expands to a Boost.Parameter-enabled const-qualified member function
// header.  All arguments are accessible via args and keywords, as well as
// by name.
#define BOOST_PARAMETER_CONST_MEMBER_FUNCTION(result, name, tag_ns, args)    \
    BOOST_PARAMETER_MEMBER_FUNCTION_AUX(                                     \
        result, name, name, tag_ns, 1                                        \
      , BOOST_PARAMETER_FLATTEN(3, 2, 3, args)                               \
    )
/**/

// Expands to a Boost.Parameter-enabled function call operator header.  All
// arguments are accessible via args and keywords, as well as by name.
#define BOOST_PARAMETER_FUNCTION_CALL_OPERATOR(result, tag_ns, args)         \
    BOOST_PARAMETER_MEMBER_FUNCTION_AUX(                                     \
        result, operator(), operator, tag_ns, 0                              \
      , BOOST_PARAMETER_FLATTEN(3, 2, 3, args)                               \
    )
/**/

// Expands to a Boost.Parameter-enabled const-qualified function call operator
// header.  All arguments are accessible via args and keywords, as well as
// by name.
#define BOOST_PARAMETER_CONST_FUNCTION_CALL_OPERATOR(result, tag_ns, args)   \
    BOOST_PARAMETER_MEMBER_FUNCTION_AUX(                                     \
        result, operator(), operator, tag_ns, 1                              \
      , BOOST_PARAMETER_FLATTEN(3, 2, 3, args)                               \
    )
/**/

#endif // BOOST_PARAMETER_PREPROCESSOR_060206_HPP

