// Copyright Daniel Wallin 2006.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_AUX_PREPROCESSOR_IMPL_FUNCTION_DISPATCH_LAYER_HPP
#define BOOST_PARAMETER_AUX_PREPROCESSOR_IMPL_FUNCTION_DISPATCH_LAYER_HPP

#include <boost/preprocessor/cat.hpp>

// Expands to keyword_tag_type for some keyword_tag.
#define BOOST_PARAMETER_FUNCTION_DISPATCH_ARG_TYPE(keyword)                  \
    BOOST_PP_CAT(keyword, _type)
/**/

#include <boost/parameter/aux_/preprocessor/impl/argument_specs.hpp>

// Helpers used as parameters to BOOST_PARAMETER_FUNCTION_DISPATCH_ARGUMENTS.
#define BOOST_PARAMETER_FUNCTION_DISPATCH_TEMPLATE_ARG(r, _, arg)            \
  , typename BOOST_PARAMETER_FUNCTION_DISPATCH_ARG_TYPE(                     \
        BOOST_PARAMETER_FN_ARG_NAME(arg)                                     \
    )
/**/

#define BOOST_PARAMETER_FUNCTION_DISPATCH_ARG_DEFN(r, _, arg)                \
  , BOOST_PARAMETER_FUNCTION_DISPATCH_ARG_TYPE(                              \
        BOOST_PARAMETER_FN_ARG_NAME(arg)                                     \
    )& BOOST_PARAMETER_FN_ARG_NAME(arg)
/**/

#define BOOST_PARAMETER_FUNCTION_DISPATCH_PARAMETER(r, _, arg)               \
  , BOOST_PARAMETER_FN_ARG_NAME(arg)
/**/

#include <boost/parameter/aux_/preprocessor/impl/split_args.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/seq/first_n.hpp>
#include <boost/preprocessor/seq/for_each.hpp>

// Helper macro used below to produce lists based on the keyword argument
// names.  macro is applied to every element.  n is the number of
// optional arguments that should be included.
#define BOOST_PARAMETER_FUNCTION_DISPATCH_ARGUMENTS(macro, n, split_args)    \
    BOOST_PP_SEQ_FOR_EACH(                                                   \
        macro                                                                \
      , ~                                                                    \
      , BOOST_PARAMETER_SPLIT_ARG_REQ_SEQ(split_args)                        \
    )                                                                        \
    BOOST_PP_SEQ_FOR_EACH(                                                   \
        macro                                                                \
      , ~                                                                    \
      , BOOST_PP_SEQ_FIRST_N(                                                \
            BOOST_PP_SUB(                                                    \
                BOOST_PARAMETER_SPLIT_ARG_OPT_COUNT(split_args)              \
              , n                                                            \
            )                                                                \
          , BOOST_PARAMETER_SPLIT_ARG_OPT_SEQ(split_args)                    \
        )                                                                    \
    )
/**/

namespace boost { namespace parameter { namespace aux {

    template <typename T>
    T& as_lvalue(T& value, long)
    {
        return value;
    }

    template <typename T>
    T const& as_lvalue(T const& value, int)
    {
        return value;
    }
}}} // namespace boost::parameter::aux

#define BOOST_PARAMETER_FUNCTION_DISPATCH_EVAL_ACTUAL_DEFAULT(arg)           \
    BOOST_PARAMETER_FUNCTION_CAST(                                           \
        ::boost::parameter::aux::as_lvalue(                                  \
            BOOST_PARAMETER_FN_ARG_DEFAULT(arg)                              \
          , 0L                                                               \
        )                                                                    \
      , BOOST_PARAMETER_FN_ARG_PRED(arg)                                     \
      , Args                                                                 \
    )
/**/

#include <boost/parameter/keyword.hpp>
#include <boost/parameter/aux_/use_default_tag.hpp>

// Generates a keyword | default expression.
#define BOOST_PARAMETER_FUNCTION_DISPATCH_KW_OR_DEFAULT(arg, tag_namespace)  \
    ::boost::parameter::keyword<                                             \
        tag_namespace::BOOST_PARAMETER_FN_ARG_NAME(arg)                      \
    >::instance | ::boost::parameter::aux::use_default_tag()
/**/

#include <boost/parameter/aux_/preprocessor/impl/function_cast.hpp>

#define BOOST_PARAMETER_FUNCTION_DISPATCH_ARG_OR_DEFAULT(arg, tag_ns)        \
    BOOST_PARAMETER_FUNCTION_CAST(                                           \
        args[BOOST_PARAMETER_FUNCTION_DISPATCH_KW_OR_DEFAULT(arg, tag_ns)]   \
      , BOOST_PARAMETER_FN_ARG_PRED(arg)                                     \
      , Args                                                                 \
    )
/**/

#define BOOST_PARAMETER_FUNCTION_DISPATCH_ARG_CAST_R(r, tag_namespace, arg)  \
  , BOOST_PARAMETER_FUNCTION_CAST(                                           \
        args[                                                                \
            ::boost::parameter::keyword<                                     \
                tag_namespace::BOOST_PARAMETER_FN_ARG_NAME(arg)              \
            >::instance                                                      \
        ]                                                                    \
      , BOOST_PARAMETER_FN_ARG_PRED(arg)                                     \
      , Args                                                                 \
    )
/**/

#include <boost/parameter/aux_/preprocessor/impl/function_dispatch_tuple.hpp>
#include <boost/tti/detail/dnullptr.hpp>
#include <boost/preprocessor/seq/elem.hpp>

#define BOOST_PARAMETER_FUNCTION_DISPATCH_OVERLOAD_BODY(x, n)                \
    {                                                                        \
        return BOOST_PARAMETER_FUNCTION_DISPATCH_NAME(x, 0)(                 \
            static_cast<ResultType(*)()>(BOOST_TTI_DETAIL_NULLPTR)           \
          , args                                                             \
          , 0L                                                               \
            BOOST_PARAMETER_FUNCTION_DISPATCH_ARGUMENTS(                     \
                BOOST_PARAMETER_FUNCTION_DISPATCH_PARAMETER                  \
              , n                                                            \
              , BOOST_PARAMETER_FUNCTION_DISPATCH_SPLIT_ARGS(x)              \
            )                                                                \
          , BOOST_PARAMETER_FUNCTION_DISPATCH_ARG_OR_DEFAULT(                \
                BOOST_PP_SEQ_ELEM(                                           \
                    BOOST_PP_SUB(                                            \
                        BOOST_PARAMETER_SPLIT_ARG_OPT_COUNT(                 \
                            BOOST_PARAMETER_FUNCTION_DISPATCH_SPLIT_ARGS(x)  \
                        )                                                    \
                      , n                                                    \
                    )                                                        \
                  , BOOST_PARAMETER_SPLIT_ARG_OPT_SEQ(                       \
                        BOOST_PARAMETER_FUNCTION_DISPATCH_SPLIT_ARGS(x)      \
                    )                                                        \
                )                                                            \
              , BOOST_PARAMETER_FUNCTION_DISPATCH_TAG_NAMESPACE(x)           \
            )                                                                \
        );                                                                   \
    }
/**/

#include <boost/parameter/aux_/preprocessor/impl/function_name.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/control/expr_if.hpp>

#define BOOST_PARAMETER_FUNCTION_DISPATCH_OVERLOAD_DEFAULT_BODY(x, n)        \
    template <                                                               \
        typename ResultType                                                  \
      , typename Args                                                        \
        BOOST_PARAMETER_FUNCTION_DISPATCH_ARGUMENTS(                         \
            BOOST_PARAMETER_FUNCTION_DISPATCH_TEMPLATE_ARG                   \
          , BOOST_PP_INC(n)                                                  \
          , BOOST_PARAMETER_FUNCTION_DISPATCH_SPLIT_ARGS(x)                  \
        )                                                                    \
    >                                                                        \
    BOOST_PARAMETER_MEMBER_FUNCTION_STATIC(                                  \
        BOOST_PARAMETER_FUNCTION_DISPATCH_BASE_NAME(x)                       \
    )                                                                        \
    ResultType BOOST_PARAMETER_FUNCTION_DISPATCH_NAME(x, 0)(                 \
        ResultType(*)()                                                      \
      , Args const& args                                                     \
      , long                                                                 \
        BOOST_PARAMETER_FUNCTION_DISPATCH_ARGUMENTS(                         \
            BOOST_PARAMETER_FUNCTION_DISPATCH_ARG_DEFN                       \
          , BOOST_PP_INC(n)                                                  \
          , BOOST_PARAMETER_FUNCTION_DISPATCH_SPLIT_ARGS(x)                  \
        )                                                                    \
      , ::boost::parameter::aux::use_default_tag                             \
    ) BOOST_PP_EXPR_IF(BOOST_PARAMETER_FUNCTION_DISPATCH_IS_CONST(x), const) \
    {                                                                        \
        return BOOST_PARAMETER_FUNCTION_DISPATCH_NAME(x, 0)(                 \
            static_cast<ResultType(*)()>(BOOST_TTI_DETAIL_NULLPTR)           \
          , args                                                             \
          , 0L                                                               \
            BOOST_PARAMETER_FUNCTION_DISPATCH_ARGUMENTS(                     \
                BOOST_PARAMETER_FUNCTION_DISPATCH_PARAMETER                  \
              , BOOST_PP_INC(n)                                              \
              , BOOST_PARAMETER_FUNCTION_DISPATCH_SPLIT_ARGS(x)              \
            )                                                                \
          , BOOST_PARAMETER_FUNCTION_DISPATCH_EVAL_ACTUAL_DEFAULT(           \
                BOOST_PP_SEQ_ELEM(                                           \
                    BOOST_PP_SUB(                                            \
                        BOOST_PARAMETER_SPLIT_ARG_OPT_COUNT(                 \
                            BOOST_PARAMETER_FUNCTION_DISPATCH_SPLIT_ARGS(x)  \
                        )                                                    \
                      , BOOST_PP_INC(n)                                      \
                    )                                                        \
                  , BOOST_PARAMETER_SPLIT_ARG_OPT_SEQ(                       \
                        BOOST_PARAMETER_FUNCTION_DISPATCH_SPLIT_ARGS(x)      \
                    )                                                        \
                )                                                            \
            )                                                                \
        );                                                                   \
    }
/**/

#include <boost/preprocessor/tuple/eat.hpp>

// Defines the actual function body for
// BOOST_PARAMETER_FUNCTION_DISPATCH_OVERLOAD_Z.
#define BOOST_PARAMETER_FUNCTION_DISPATCH_OVERLOAD_BODY_Z(z, n, x)           \
    template <                                                               \
        typename ResultType                                                  \
      , typename Args                                                        \
        BOOST_PARAMETER_FUNCTION_DISPATCH_ARGUMENTS(                         \
            BOOST_PARAMETER_FUNCTION_DISPATCH_TEMPLATE_ARG                   \
          , n                                                                \
          , BOOST_PARAMETER_FUNCTION_DISPATCH_SPLIT_ARGS(x)                  \
        )                                                                    \
    >                                                                        \
    BOOST_PARAMETER_MEMBER_FUNCTION_STATIC(                                  \
        BOOST_PARAMETER_FUNCTION_DISPATCH_BASE_NAME(x)                       \
    )                                                                        \
    ResultType BOOST_PARAMETER_FUNCTION_DISPATCH_NAME(x, 0)(                 \
        ResultType(*)()                                                      \
      , Args const& args                                                     \
      , int                                                                  \
        BOOST_PARAMETER_FUNCTION_DISPATCH_ARGUMENTS(                         \
            BOOST_PARAMETER_FUNCTION_DISPATCH_ARG_DEFN                       \
          , n                                                                \
          , BOOST_PARAMETER_FUNCTION_DISPATCH_SPLIT_ARGS(x)                  \
        )                                                                    \
    ) BOOST_PP_EXPR_IF(BOOST_PARAMETER_FUNCTION_DISPATCH_IS_CONST(x), const) \
    BOOST_PP_IF(                                                             \
        n                                                                    \
      , BOOST_PARAMETER_FUNCTION_DISPATCH_OVERLOAD_BODY                      \
      , ; BOOST_PP_TUPLE_EAT(2)                                              \
    )(x, n)
/**/

#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/logical/or.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

#define BOOST_PARAMETER_FUNCTION_DISPATCH_OVERLOAD_Z(z, n, data)             \
    BOOST_PP_IF(                                                             \
        BOOST_PP_OR(n, BOOST_PP_TUPLE_ELEM(2, 0, data))                      \
      , BOOST_PARAMETER_FUNCTION_DISPATCH_OVERLOAD_BODY_Z                    \
      , BOOST_PP_TUPLE_EAT(3)                                                \
    )(z, n, BOOST_PP_TUPLE_ELEM(2, 1, data))                                 \
    BOOST_PP_IF(                                                             \
        BOOST_PP_EQUAL(                                                      \
            n                                                                \
          , BOOST_PARAMETER_SPLIT_ARG_OPT_COUNT(                             \
                BOOST_PARAMETER_FUNCTION_DISPATCH_SPLIT_ARGS(                \
                    BOOST_PP_TUPLE_ELEM(2, 1, data)                          \
                )                                                            \
            )                                                                \
        )                                                                    \
      , BOOST_PP_TUPLE_EAT(2)                                                \
      , BOOST_PARAMETER_FUNCTION_DISPATCH_OVERLOAD_DEFAULT_BODY              \
    )(BOOST_PP_TUPLE_ELEM(2, 1, data), n)
/**/

// Generates the function template that receives an ArgumentPack, and then
// goes on to call the layers of overloads generated by 
// BOOST_PARAMETER_FUNCTION_DISPATCH_LAYER.
#define BOOST_PARAMETER_FUNCTION_INITIAL_DISPATCH_FUNCTION(x)                \
    template <typename Args>                                                 \
    typename BOOST_PARAMETER_FUNCTION_RESULT_NAME(                           \
        BOOST_PARAMETER_FUNCTION_DISPATCH_BASE_NAME(x)                       \
    )<Args>::type BOOST_PARAMETER_MEMBER_FUNCTION_STATIC(                    \
        BOOST_PARAMETER_FUNCTION_DISPATCH_BASE_NAME(x)                       \
    ) BOOST_PARAMETER_FUNCTION_IMPL_NAME(                                    \
        BOOST_PARAMETER_FUNCTION_DISPATCH_BASE_NAME(x)                       \
    )(Args const& args)                                                      \
    BOOST_PP_EXPR_IF(BOOST_PARAMETER_FUNCTION_DISPATCH_IS_CONST(x), const)   \
    {                                                                        \
        return BOOST_PARAMETER_FUNCTION_DISPATCH_NAME(x, 0)(                 \
            static_cast<                                                     \
                typename BOOST_PARAMETER_FUNCTION_RESULT_NAME(               \
                    BOOST_PARAMETER_FUNCTION_DISPATCH_BASE_NAME(x)           \
                )<Args>::type(*)()                                           \
            >(BOOST_TTI_DETAIL_NULLPTR)                                      \
          , args                                                             \
          , 0L                                                               \
            BOOST_PP_SEQ_FOR_EACH(                                           \
                BOOST_PARAMETER_FUNCTION_DISPATCH_ARG_CAST_R                 \
              , BOOST_PARAMETER_FUNCTION_DISPATCH_TAG_NAMESPACE(x)           \
              , BOOST_PARAMETER_SPLIT_ARG_REQ_SEQ(                           \
                    BOOST_PARAMETER_FUNCTION_DISPATCH_SPLIT_ARGS(x)          \
                )                                                            \
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
#define BOOST_PARAMETER_FUNCTION_DISPATCH_LAYER(fwd_decl, x)                 \
    BOOST_PP_REPEAT_FROM_TO(                                                 \
        0                                                                    \
      , BOOST_PP_INC(                                                        \
            BOOST_PARAMETER_SPLIT_ARG_OPT_COUNT(                             \
                BOOST_PARAMETER_FUNCTION_DISPATCH_SPLIT_ARGS(x)              \
            )                                                                \
        )                                                                    \
      , BOOST_PARAMETER_FUNCTION_DISPATCH_OVERLOAD_Z                         \
      , (fwd_decl, x)                                                        \
    )                                                                        \
    BOOST_PARAMETER_FUNCTION_INITIAL_DISPATCH_FUNCTION(x)                    \
    template <                                                               \
        typename ResultType                                                  \
      , typename Args                                                        \
        BOOST_PARAMETER_FUNCTION_DISPATCH_ARGUMENTS(                         \
            BOOST_PARAMETER_FUNCTION_DISPATCH_TEMPLATE_ARG                   \
          , 0                                                                \
          , BOOST_PARAMETER_FUNCTION_DISPATCH_SPLIT_ARGS(x)                  \
        )                                                                    \
    >                                                                        \
    BOOST_PARAMETER_MEMBER_FUNCTION_STATIC(                                  \
        BOOST_PARAMETER_FUNCTION_DISPATCH_BASE_NAME(x)                       \
    )                                                                        \
    ResultType BOOST_PARAMETER_FUNCTION_DISPATCH_NAME(x, 0)(                 \
        ResultType(*)()                                                      \
      , Args const&                                                          \
      , int                                                                  \
        BOOST_PARAMETER_FUNCTION_DISPATCH_ARGUMENTS(                         \
            BOOST_PARAMETER_FUNCTION_DISPATCH_ARG_DEFN                       \
          , 0                                                                \
          , BOOST_PARAMETER_FUNCTION_DISPATCH_SPLIT_ARGS(x)                  \
        )                                                                    \
    ) BOOST_PP_EXPR_IF(BOOST_PARAMETER_FUNCTION_DISPATCH_IS_CONST(x), const)
/**/

#endif  // include guard

