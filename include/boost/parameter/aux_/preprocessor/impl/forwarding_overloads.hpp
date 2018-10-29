// Copyright Daniel Wallin 2006.
// Copyright Cromwell D. Enage 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_AUX_PREPROCESSOR_IMPL_FORWARDING_OVERLOADS_HPP
#define BOOST_PARAMETER_AUX_PREPROCESSOR_IMPL_FORWARDING_OVERLOADS_HPP

#include <boost/parameter/config.hpp>

#if defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)

#include <boost/preprocessor/cat.hpp>

// Expands to a forwarding parameter for a constructor or forwarding function.
#define BOOST_PARAMETER_FUNCTION_ARG_TYPE_Z(z, n, type_prefix)               \
    BOOST_PP_CAT(type_prefix, n)&&
/**/

#include <boost/move/utility_core.hpp>

// Expands to an argument passed from a forwarding function to the front-end
// implementation function, or from a constructor to its delegate.
#define BOOST_PARAMETER_FUNCTION_FORWARD_PARAM_Z(z, n, type_prefix)          \
    ::boost::forward<BOOST_PP_CAT(type_prefix, n)>(BOOST_PP_CAT(a, n))
/**/

#include <boost/parameter/aux_/preprocessor/impl/parenthesized_type.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

// Expands to the default constructor, whose job is to pass an empty back to
// the delegate constructor of the base class.
#define BOOST_PARAMETER_CONSTRUCTOR_OVERLOAD_0_Z(z, n, data)                 \
    inline BOOST_PP_TUPLE_ELEM(2, 0, data)()                                 \
      : BOOST_PARAMETER_PARENTHESIZED_TYPE(BOOST_PP_TUPLE_ELEM(2, 1, data))( \
            BOOST_PP_CAT(constructor_parameters, __LINE__)()()               \
        )                                                                    \
    {                                                                        \
    }
/**/

#include <boost/parameter/aux_/pp_impl/argument_pack.hpp>
#include <boost/parameter/aux_/preprocessor/impl/function_name.hpp>
#include <boost/preprocessor/control/expr_if.hpp>

// Expands to a 0-arity forwarding function, whose job is to pass an empty
// pack to the front-end implementation function.
#define BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_0_Z(z, n, data)            \
    BOOST_PARAMETER_MEMBER_FUNCTION_STATIC(BOOST_PP_TUPLE_ELEM(3, 1, data))  \
    inline                                                                   \
    BOOST_PARAMETER_FUNCTION_RESULT_NAME(BOOST_PP_TUPLE_ELEM(3, 1, data))<   \
        ::boost::parameter::aux::argument_pack<                              \
            BOOST_PARAMETER_FUNCTION_SPECIFICATION_NAME(                     \
                BOOST_PP_TUPLE_ELEM(3, 1, data)                              \
            )                                                                \
        >::type                                                              \
    >::type                                                                  \
    BOOST_PARAMETER_MEMBER_FUNCTION_NAME(BOOST_PP_TUPLE_ELEM(3, 0, data))(   \
    ) BOOST_PP_EXPR_IF(BOOST_PP_TUPLE_ELEM(3, 2, data), const)               \
    {                                                                        \
        return BOOST_PARAMETER_FUNCTION_IMPL_NAME(                           \
            BOOST_PP_TUPLE_ELEM(3, 1, data)                                  \
        )(                                                                   \
            BOOST_PARAMETER_FUNCTION_SPECIFICATION_NAME(                     \
                BOOST_PP_TUPLE_ELEM(3, 1, data)                              \
            )()()                                                            \
        );                                                                   \
    }
/**/

#include <boost/parameter/aux_/preprocessor/impl/function_forward_match.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>

// Expands to a forwarding function, whose job is to consolidate its arguments
// into a pack for the front-end implementation function to take in.
#define BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_1_Z(z, n, data)            \
    template <BOOST_PP_ENUM_PARAMS_Z(z, n, typename ParameterArgumentType)>  \
    BOOST_PARAMETER_MEMBER_FUNCTION_STATIC(BOOST_PP_TUPLE_ELEM(3, 1, data))  \
    inline typename                                                          \
    BOOST_PARAMETER_FUNCTION_RESULT_NAME(BOOST_PP_TUPLE_ELEM(3, 1, data))<   \
        typename ::boost::parameter::aux::argument_pack<                     \
            BOOST_PARAMETER_FUNCTION_SPECIFICATION_NAME(                     \
                BOOST_PP_TUPLE_ELEM(3, 1, data)                              \
            )                                                                \
          , BOOST_PP_CAT(BOOST_PP_ENUM_, z)(                                 \
                n                                                            \
              , BOOST_PARAMETER_FUNCTION_ARG_TYPE_Z                          \
              , ParameterArgumentType                                        \
            )                                                                \
        >::type                                                              \
    >::type                                                                  \
    BOOST_PARAMETER_MEMBER_FUNCTION_NAME(BOOST_PP_TUPLE_ELEM(3, 0, data))(   \
        BOOST_PP_ENUM_BINARY_PARAMS_Z(z, n, ParameterArgumentType, && a)     \
      , BOOST_PARAMETER_FUNCTION_FORWARD_MATCH_Z(                            \
            z                                                                \
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

#include <boost/preprocessor/comparison/equal.hpp>

// Expands to a constructor whose job is to consolidate its arguments into a
// pack for the delegate constructor of the base class to take in.
#define BOOST_PARAMETER_CONSTRUCTOR_OVERLOAD_1_Z(z, n, data)                 \
    template <BOOST_PP_ENUM_PARAMS_Z(z, n, typename ParameterArgumentType)>  \
    BOOST_PP_EXPR_IF(BOOST_PP_EQUAL(n, 1), explicit)                         \
    inline BOOST_PP_TUPLE_ELEM(2, 0, data)(                                  \
        BOOST_PP_ENUM_BINARY_PARAMS_Z(z, n, ParameterArgumentType, && a)     \
      , BOOST_PARAMETER_FUNCTION_FORWARD_MATCH_Z(                            \
            z                                                                \
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

#include <boost/preprocessor/control/if.hpp>

#define BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_Z(z, n, data)              \
    BOOST_PP_IF(                                                             \
        n                                                                    \
      , BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_1_Z                        \
      , BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_0_Z                        \
    )(z, n, data)
/**/

#define BOOST_PARAMETER_CONSTRUCTOR_OVERLOAD_Z(z, n, data)                   \
    BOOST_PP_IF(                                                             \
        n                                                                    \
      , BOOST_PARAMETER_CONSTRUCTOR_OVERLOAD_1_Z                             \
      , BOOST_PARAMETER_CONSTRUCTOR_OVERLOAD_0_Z                             \
    )(z, n, data)
/**/

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

// Helper macro for BOOST_PARAMETER_CONSTRUCTOR_OVERLOADS.
#define BOOST_PARAMETER_CONSTRUCTOR_OVERLOADS_AUX(class_, base, range)       \
    BOOST_PP_REPEAT_FROM_TO(                                                 \
        BOOST_PP_TUPLE_ELEM(2, 0, range)                                     \
      , BOOST_PP_TUPLE_ELEM(2, 1, range)                                     \
      , BOOST_PARAMETER_CONSTRUCTOR_OVERLOAD_Z                               \
      , (class_, base)                                                       \
    )
/**/

#include <boost/parameter/aux_/preprocessor/impl/arity_range.hpp>

// Expands to the layer of forwarding functions for the function with the
// specified name, whose arguments determine the range of arities.
#define BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOADS(name, impl, args, const_) \
    BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOADS_AUX(                          \
        name, impl, BOOST_PARAMETER_ARITY_RANGE(args), const_                \
    )
/**/

// Expands to the layer of forwarding functions for the constructor in the
// specified class, whose arguments determine the range of arities.
#define BOOST_PARAMETER_CONSTRUCTOR_OVERLOADS(class_, base, args)            \
    BOOST_PARAMETER_CONSTRUCTOR_OVERLOADS_AUX(                               \
        class_, base, BOOST_PARAMETER_ARITY_RANGE(args)                      \
    )
/**/

#else   // !defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)

#define BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_PRODUCT(r, product)        \
    (product)
/**/

#include <boost/preprocessor/cat.hpp>

#if defined(BOOST_NO_FUNCTION_TEMPLATE_ORDERING)
// No partial ordering.  This feature doesn't work.
#define BOOST_PARAMETER_FUNCTION_FORWARD_COMBINATION(r, _, i, elem)          \
    ((BOOST_PP_CAT(ParameterArgumentType, i)))
/**/
#else
#define BOOST_PARAMETER_FUNCTION_FORWARD_COMBINATION(r, _, i, elem)          \
    (                                                                        \
        (BOOST_PP_CAT(ParameterArgumentType, i) const)                       \
        (BOOST_PP_CAT(ParameterArgumentType, i))                             \
    )
/**/
#endif  // BOOST_NO_FUNCTION_TEMPLATE_ORDERING

#include <boost/preprocessor/punctuation/comma_if.hpp>

// Expands to a definition of a constructor or forwarding function argument
// passed by lvalue reference.  Used by
// BOOST_PARAMETER_CONSTRUCTOR_OVERLOAD_IMPL and
// BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_IMPL.
#define BOOST_PARAMETER_FUNCTION_ARGUMENT(r, _, i, elem)                     \
    BOOST_PP_COMMA_IF(i) elem& BOOST_PP_CAT(a, i)
/**/

#include <boost/preprocessor/seq/for_each_i.hpp>

#define BOOST_PARAMETER_FUNCTION_FORWARD_COMBINATIONS(args)                  \
    BOOST_PP_SEQ_FOR_EACH_I(                                                 \
        BOOST_PARAMETER_FUNCTION_FORWARD_COMBINATION, ~, args                \
    )
/**/

#include <boost/preprocessor/repetition/enum_params.hpp>

#define BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_TEMPLATE_HEADER_Z(z, n, p) \
    template <BOOST_PP_ENUM_PARAMS_Z(z, n, typename p)>
/**/

#include <boost/parameter/aux_/preprocessor/impl/function_forward_match.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/control/expr_if.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/tuple/eat.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

#define BOOST_PARAMETER_CONSTRUCTOR_OVERLOAD_IMPL(z, n, r, data, elem)       \
    BOOST_PP_IF(                                                             \
        n                                                                    \
      , BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_TEMPLATE_HEADER_Z          \
      , BOOST_PP_TUPLE_EAT(3)                                                \
    )(z, n, ParameterArgumentType)                                           \
    BOOST_PP_EXPR_IF(BOOST_PP_EQUAL(n, 1), explicit)                         \
    BOOST_PP_TUPLE_ELEM(5, 2, data)(                                         \
        BOOST_PP_IF(                                                         \
            n, BOOST_PP_SEQ_FOR_EACH_I_R, BOOST_PP_TUPLE_EAT(4)              \
        )(r, BOOST_PARAMETER_FUNCTION_ARGUMENT, ~, elem)                     \
        BOOST_PP_IF(                                                         \
            n, BOOST_PARAMETER_FUNCTION_FWD_MATCH_Z, BOOST_PP_TUPLE_EAT(4)   \
        )(                                                                   \
            z                                                                \
          , BOOST_PP_CAT(constructor_parameters, __LINE__)                   \
          , n                                                                \
          , ParameterArgumentType                                            \
        )                                                                    \
    ) : BOOST_PARAMETER_PARENTHESIZED_TYPE(                                  \
            BOOST_PP_TUPLE_ELEM(5, 3, data)                                  \
        )(                                                                   \
            BOOST_PP_CAT(constructor_parameters, __LINE__)()(                \
                BOOST_PP_ENUM_PARAMS_Z(z, n, a)                              \
            )                                                                \
        )                                                                    \
    {                                                                        \
    }
/**/

#include <boost/parameter/aux_/pp_impl/argument_pack.hpp>
#include <boost/parameter/aux_/preprocessor/impl/function_name.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/seq/seq.hpp>

#define BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_IMPL(z, n, r, data, elem)  \
    BOOST_PP_IF(                                                             \
        n                                                                    \
      , BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_TEMPLATE_HEADER_Z          \
      , BOOST_PP_TUPLE_EAT(3)                                                \
    )(z, n, ParameterArgumentType)                                           \
    BOOST_PARAMETER_MEMBER_FUNCTION_STATIC(BOOST_PP_TUPLE_ELEM(6, 3, data))  \
    inline BOOST_PP_EXPR_IF(n, typename)                                     \
    BOOST_PARAMETER_FUNCTION_RESULT_NAME(BOOST_PP_TUPLE_ELEM(6, 3, data))<   \
        BOOST_PP_EXPR_IF(n, typename)                                        \
        ::boost::parameter::aux::argument_pack<                              \
            BOOST_PARAMETER_FUNCTION_SPECIFICATION_NAME(                     \
                BOOST_PP_TUPLE_ELEM(6, 3, data)                              \
            )                                                                \
            BOOST_PP_COMMA_IF(n)                                             \
            BOOST_PP_IF(                                                     \
                n, BOOST_PP_SEQ_ENUM, BOOST_PP_TUPLE_EAT(1)                  \
            )(elem)                                                          \
        >::type                                                              \
    >::type                                                                  \
    BOOST_PARAMETER_MEMBER_FUNCTION_NAME(BOOST_PP_TUPLE_ELEM(6, 2, data))(   \
        BOOST_PP_IF(                                                         \
            n, BOOST_PP_SEQ_FOR_EACH_I_R, BOOST_PP_TUPLE_EAT(4)              \
        )(r, BOOST_PARAMETER_FUNCTION_ARGUMENT, ~, elem)                     \
        BOOST_PP_IF(                                                         \
            n, BOOST_PARAMETER_FUNCTION_FWD_MATCH_Z, BOOST_PP_TUPLE_EAT(4)   \
        )(                                                                   \
            z                                                                \
          , BOOST_PARAMETER_FUNCTION_SPECIFICATION_NAME(                     \
                BOOST_PP_TUPLE_ELEM(6, 3, data)                              \
            )                                                                \
          , n                                                                \
          , ParameterArgumentType                                            \
        )                                                                    \
    ) BOOST_PP_EXPR_IF(BOOST_PP_TUPLE_ELEM(6, 5, data), const)               \
    {                                                                        \
        return BOOST_PARAMETER_FUNCTION_IMPL_NAME(                           \
            BOOST_PP_TUPLE_ELEM(6, 3, data)                                  \
        )(                                                                   \
            BOOST_PARAMETER_FUNCTION_SPECIFICATION_NAME(                     \
                BOOST_PP_TUPLE_ELEM(6, 3, data)                              \
            )()(BOOST_PP_ENUM_PARAMS_Z(z, n, a))                             \
        );                                                                   \
    }
/**/

#define BOOST_PARAMETER_CONSTRUCTOR_OVERLOAD_IMPL_R(r, data, elem)           \
    BOOST_PARAMETER_CONSTRUCTOR_OVERLOAD_IMPL(                               \
        BOOST_PP_TUPLE_ELEM(5, 0, data)                                      \
      , BOOST_PP_TUPLE_ELEM(5, 1, data)                                      \
      , r                                                                    \
      , data                                                                 \
      , elem                                                                 \
    )
/**/

#define BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_IMPL_R(r, data, elem)      \
    BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_IMPL(                          \
        BOOST_PP_TUPLE_ELEM(6, 0, data)                                      \
      , BOOST_PP_TUPLE_ELEM(6, 1, data)                                      \
      , r                                                                    \
      , data                                                                 \
      , elem                                                                 \
    )
/**/

#include <boost/preprocessor/repetition/deduce_r.hpp>
#include <boost/preprocessor/tuple/rem.hpp>

#define BOOST_PARAMETER_CONSTRUCTOR_OVERLOAD_IMPL_ARITY_0(z, n, data)        \
    BOOST_PARAMETER_CONSTRUCTOR_OVERLOAD_IMPL(                               \
        z                                                                    \
      , n                                                                    \
      , BOOST_PP_DEDUCE_R()                                                  \
      , (z, n, BOOST_PP_TUPLE_REM(3) data)                                   \
      , ~                                                                    \
    )
/**/

#define BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_IMPL_ARITY_0(z, n, data)   \
    BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_IMPL(                          \
        z                                                                    \
      , n                                                                    \
      , BOOST_PP_DEDUCE_R()                                                  \
      , (z, n, BOOST_PP_TUPLE_REM(4) data)                                   \
      , ~                                                                    \
    )
/**/

#include <boost/preprocessor/seq/first_n.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/for_each_product.hpp>

#define BOOST_PARAMETER_CONSTRUCTOR_OVERLOAD_IMPL_ARITY_N(z, n, data)        \
    BOOST_PP_SEQ_FOR_EACH(                                                   \
        BOOST_PARAMETER_CONSTRUCTOR_OVERLOAD_IMPL_R                          \
      , (z, n, BOOST_PP_TUPLE_REM(3) data)                                   \
      , BOOST_PP_SEQ_FOR_EACH_PRODUCT(                                       \
            BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_PRODUCT                \
          , BOOST_PP_SEQ_FIRST_N(n, BOOST_PP_TUPLE_ELEM(3, 2, data))         \
        )                                                                    \
    )
/**/

#define BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_IMPL_ARITY_N(z, n, data)   \
    BOOST_PP_SEQ_FOR_EACH(                                                   \
        BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_IMPL_R                     \
      , (z, n, BOOST_PP_TUPLE_REM(4) data)                                   \
      , BOOST_PP_SEQ_FOR_EACH_PRODUCT(                                       \
            BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_PRODUCT                \
          , BOOST_PP_SEQ_FIRST_N(n, BOOST_PP_TUPLE_ELEM(4, 2, data))         \
        )                                                                    \
    )
/**/

// Expands to a forwarding function, whose job is to consolidate its arguments
// into a pack for the front-end implementation function to take in.  Used by
// BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_Z when the number of arguments
// the forwarding function will take in is greater than or equal to
// BOOST_PARAMETER_EXPONENTIAL_OVERLOAD_THRESHOLD_ARITY, in which case
// all the arguments will be passed by lvalue reference.
#define BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_0(z, n, data)              \
    BOOST_PP_IF(                                                             \
        n                                                                    \
      , BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_IMPL_ARITY_N               \
      , BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_IMPL_ARITY_0               \
    )(z, n, data)
/**/

// Expands to a constructor whose job is to consolidate its arguments into a
// pack for the delegate to take in.  Used by
// BOOST_PARAMETER_CONSTRUCTOR_OVERLOAD_Z when the number of arguments
// the forwarding constructor will take in is greater than or equal to
// BOOST_PARAMETER_EXPONENTIAL_OVERLOAD_THRESHOLD_ARITY, in which case
// all the arguments will be passed by lvalue reference.
#define BOOST_PARAMETER_CONSTRUCTOR_OVERLOAD_0(z, n, data)                   \
    BOOST_PP_IF(                                                             \
        n                                                                    \
      , BOOST_PARAMETER_CONSTRUCTOR_OVERLOAD_IMPL_ARITY_N                    \
      , BOOST_PARAMETER_CONSTRUCTOR_OVERLOAD_IMPL_ARITY_0                    \
    )(z, n, data)
/**/

// Expands to a 0-arity forwarding function, whose job is to pass an empty
// argument pack to the front-end implementation function.
#define BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_0_ARITY(z, n, seq)         \
    inline BOOST_PARAMETER_MEMBER_FUNCTION_STATIC(                           \
        BOOST_PP_TUPLE_ELEM(                                                 \
            4, 1, BOOST_PP_SEQ_HEAD(BOOST_PP_SEQ_TAIL(seq))                  \
        )                                                                    \
    )                                                                        \
    BOOST_PARAMETER_FUNCTION_RESULT_NAME(                                    \
        BOOST_PP_TUPLE_ELEM(                                                 \
            4, 1, BOOST_PP_SEQ_HEAD(BOOST_PP_SEQ_TAIL(seq))                  \
        )                                                                    \
    )<                                                                       \
        ::boost::parameter::aux::argument_pack<                              \
            BOOST_PARAMETER_FUNCTION_SPECIFICATION_NAME(                     \
                BOOST_PP_TUPLE_ELEM(                                         \
                    4, 1, BOOST_PP_SEQ_HEAD(BOOST_PP_SEQ_TAIL(seq))          \
                )                                                            \
            )                                                                \
        >::type                                                              \
    >::type                                                                  \
    BOOST_PARAMETER_MEMBER_FUNCTION_NAME(                                    \
        BOOST_PP_TUPLE_ELEM(                                                 \
            4, 0, BOOST_PP_SEQ_HEAD(BOOST_PP_SEQ_TAIL(seq))                  \
        )                                                                    \
    )() BOOST_PP_EXPR_IF(                                                    \
        BOOST_PP_TUPLE_ELEM(                                                 \
            4, 3, BOOST_PP_SEQ_HEAD(BOOST_PP_SEQ_TAIL(seq))                  \
        )                                                                    \
      , const                                                                \
    )                                                                        \
    {                                                                        \
        return BOOST_PARAMETER_FUNCTION_IMPL_NAME(                           \
            BOOST_PP_TUPLE_ELEM(                                             \
                4, 1, BOOST_PP_SEQ_HEAD(BOOST_PP_SEQ_TAIL(seq))              \
            )                                                                \
        )(                                                                   \
            BOOST_PARAMETER_FUNCTION_SPECIFICATION_NAME(                     \
                BOOST_PP_TUPLE_ELEM(                                         \
                    4, 1, BOOST_PP_SEQ_HEAD(BOOST_PP_SEQ_TAIL(seq))          \
                )                                                            \
            )()()                                                            \
        );                                                                   \
    }
/**/

// Expands to the default constructor, whose job is to pass an empty argument
// pack back to the delegate constructor of the base class.
#define BOOST_PARAMETER_DEFAULT_CONSTRUCTOR(z, n, seq)                       \
    inline                                                                   \
    BOOST_PP_TUPLE_ELEM(3, 0, BOOST_PP_SEQ_HEAD(BOOST_PP_SEQ_TAIL(seq)))()   \
      : BOOST_PARAMETER_PARENTHESIZED_TYPE(                                  \
            BOOST_PP_TUPLE_ELEM(                                             \
                3, 1, BOOST_PP_SEQ_HEAD(BOOST_PP_SEQ_TAIL(seq))              \
            )                                                                \
        )(BOOST_PP_CAT(constructor_parameters, __LINE__)()())                \
    {                                                                        \
    }
/**/

#include <boost/parameter/aux_/preprocessor/binary_seq_to_args.hpp>

// Expands to a forwarding function, whose job is to consolidate its arguments
// into a pack for the front-end implementation function to take in.  Each
// element in BOOST_PP_SEQ_TAIL(seq) determines the const-ness of the
// corresponding argument.
#define BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_R(r, seq)                  \
    template <                                                               \
        BOOST_PP_ENUM_PARAMS(                                                \
            BOOST_PP_SEQ_SIZE(BOOST_PP_SEQ_TAIL(seq))                        \
          , typename ParameterArgumentType                                   \
        )                                                                    \
    >                                                                        \
    inline BOOST_PARAMETER_MEMBER_FUNCTION_STATIC(                           \
        BOOST_PP_TUPLE_ELEM(4, 1, BOOST_PP_SEQ_HEAD(seq))                    \
    )                                                                        \
    typename BOOST_PARAMETER_FUNCTION_RESULT_NAME(                           \
        BOOST_PP_TUPLE_ELEM(4, 1, BOOST_PP_SEQ_HEAD(seq))                    \
    )<                                                                       \
        typename ::boost::parameter::aux::argument_pack<                     \
            BOOST_PARAMETER_FUNCTION_SPECIFICATION_NAME(                     \
                BOOST_PP_TUPLE_ELEM(4, 1, BOOST_PP_SEQ_HEAD(seq))            \
            )                                                                \
          , BOOST_PARAMETER_AUX_PP_BINARY_SEQ_TO_ARGS(                       \
                BOOST_PP_SEQ_TAIL(seq), (ParameterArgumentType)              \
            )                                                                \
        >::type                                                              \
    >::type                                                                  \
    BOOST_PARAMETER_MEMBER_FUNCTION_NAME(                                    \
        BOOST_PP_TUPLE_ELEM(4, 0, BOOST_PP_SEQ_HEAD(seq))                    \
    )(                                                                       \
        BOOST_PARAMETER_AUX_PP_BINARY_SEQ_TO_ARGS(                           \
            BOOST_PP_SEQ_TAIL(seq), (ParameterArgumentType)(a)               \
        )                                                                    \
      , BOOST_PARAMETER_FUNCTION_FORWARD_MATCH(                              \
            BOOST_PARAMETER_FUNCTION_SPECIFICATION_NAME(                     \
                BOOST_PP_TUPLE_ELEM(4, 1, BOOST_PP_SEQ_HEAD(seq))            \
            )                                                                \
          , BOOST_PP_SEQ_SIZE(BOOST_PP_SEQ_TAIL(seq))                        \
          , ParameterArgumentType                                            \
        )                                                                    \
    ) BOOST_PP_EXPR_IF(                                                      \
        BOOST_PP_TUPLE_ELEM(4, 3, BOOST_PP_SEQ_HEAD(seq)), const             \
    )                                                                        \
    {                                                                        \
        return BOOST_PARAMETER_FUNCTION_IMPL_NAME(                           \
            BOOST_PP_TUPLE_ELEM(4, 1, BOOST_PP_SEQ_HEAD(seq))                \
        )(                                                                   \
            BOOST_PARAMETER_FUNCTION_SPECIFICATION_NAME(                     \
                BOOST_PP_TUPLE_ELEM(4, 1, BOOST_PP_SEQ_HEAD(seq))            \
            )()(                                                             \
                BOOST_PP_ENUM_PARAMS(                                        \
                    BOOST_PP_SEQ_SIZE(BOOST_PP_SEQ_TAIL(seq)), a             \
                )                                                            \
            )                                                                \
        );                                                                   \
    }
/**/

// Expands to a constructor whose job is to consolidate its arguments into a
// pack for the delegate constructor of the base class to take in.  Each
// element in BOOST_PP_SEQ_TAIL(seq) determines the const-ness of the
// corresponding argument.
#define BOOST_PARAMETER_CONSTRUCTOR_OVERLOAD_R(r, seq)                       \
    template <                                                               \
        BOOST_PP_ENUM_PARAMS(                                                \
            BOOST_PP_SEQ_SIZE(BOOST_PP_SEQ_TAIL(seq))                        \
          , typename ParameterArgumentType                                   \
        )                                                                    \
    >                                                                        \
    inline BOOST_PP_EXPR_IF(                                                 \
        BOOST_PP_EQUAL(BOOST_PP_SEQ_SIZE(BOOST_PP_SEQ_TAIL(seq)), 1)         \
      , explicit                                                             \
    )                                                                        \
    BOOST_PP_TUPLE_ELEM(3, 0, BOOST_PP_SEQ_HEAD(seq))(                       \
        BOOST_PARAMETER_AUX_PP_BINARY_SEQ_TO_ARGS(                           \
            BOOST_PP_SEQ_TAIL(seq), (ParameterArgumentType)(a)               \
        )                                                                    \
      , BOOST_PARAMETER_FUNCTION_FORWARD_MATCH(                              \
            BOOST_PP_CAT(constructor_parameters, __LINE__)                   \
          , BOOST_PP_SEQ_SIZE(BOOST_PP_SEQ_TAIL(seq))                        \
          , ParameterArgumentType                                            \
        )                                                                    \
    ) : BOOST_PARAMETER_PARENTHESIZED_TYPE(                                  \
            BOOST_PP_TUPLE_ELEM(3, 1, BOOST_PP_SEQ_HEAD(seq))                \
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
#include <boost/preprocessor/control/if.hpp>

// Expands to all forwarding functions that take in n arguments.  Used by
// BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_Z when n is less than
// BOOST_PARAMETER_ALL_CONST_THRESHOLD_ARITY.
#define BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_1(z, n, data)              \
    BOOST_PP_IF(                                                             \
        n                                                                    \
      , BOOST_PARAMETER_AUX_PP_BINARY_SEQ_FOR_EACH_Z                         \
      , BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_0_ARITY                    \
    )(z, n, (BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_R)(data))
/**/

// Expands to all constructors that take in n arguments.  Used by
// BOOST_PARAMETER_CONSTRUCTOR_OVERLOAD_Z when n is less than
// BOOST_PARAMETER_ALL_CONST_THRESHOLD_ARITY.
#define BOOST_PARAMETER_CONSTRUCTOR_OVERLOAD_1(z, n, data)                   \
    BOOST_PP_IF(                                                             \
        n                                                                    \
      , BOOST_PARAMETER_AUX_PP_BINARY_SEQ_FOR_EACH_Z                         \
      , BOOST_PARAMETER_DEFAULT_CONSTRUCTOR                                  \
    )(z, n, (BOOST_PARAMETER_CONSTRUCTOR_OVERLOAD_R)(data))
/**/

#include <boost/preprocessor/comparison/less.hpp>

// Enables BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOADS_AUX to use either
// BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_0 or
// BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_1 within BOOST_PP_REPEAT_FROM_TO.
#define BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_Z(z, n, data)              \
    BOOST_PP_CAT(                                                            \
        BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_                           \
      , BOOST_PP_LESS(                                                       \
            n                                                                \
          , BOOST_PARAMETER_EXPONENTIAL_OVERLOAD_THRESHOLD_ARITY             \
        )                                                                    \
    )(z, n, data)
/**/

// Enables BOOST_PARAMETER_CONSTRUCTOR_OVERLOADS_AUX to use either
// BOOST_PARAMETER_CONSTRUCTOR_OVERLOAD_0 or
// BOOST_PARAMETER_CONSTRUCTOR_OVERLOAD_1 within BOOST_PP_REPEAT_FROM_TO.
#define BOOST_PARAMETER_CONSTRUCTOR_OVERLOAD_Z(z, n, data)                   \
    BOOST_PP_CAT(                                                            \
        BOOST_PARAMETER_CONSTRUCTOR_OVERLOAD_                                \
      , BOOST_PP_LESS(                                                       \
            n                                                                \
          , BOOST_PARAMETER_EXPONENTIAL_OVERLOAD_THRESHOLD_ARITY             \
        )                                                                    \
    )(z, n, data)
/**/

#include <boost/preprocessor/repetition/repeat_from_to.hpp>

// Helper macro for BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOADS.
#define BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOADS_AUX(n, i, r, combo, c)    \
    BOOST_PP_REPEAT_FROM_TO(                                                 \
        BOOST_PP_TUPLE_ELEM(2, 0, r)                                         \
      , BOOST_PP_TUPLE_ELEM(2, 1, r)                                         \
      , BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_Z                          \
      , (n, i, combo, c)                                                     \
    )
/**/

// Helper macro for BOOST_PARAMETER_CONSTRUCTOR_OVERLOADS.
#define BOOST_PARAMETER_CONSTRUCTOR_OVERLOADS_AUX(class_, base, r, combo)    \
    BOOST_PP_REPEAT_FROM_TO(                                                 \
        BOOST_PP_TUPLE_ELEM(2, 0, r)                                         \
      , BOOST_PP_TUPLE_ELEM(2, 1, r)                                         \
      , BOOST_PARAMETER_CONSTRUCTOR_OVERLOAD_Z                               \
      , (class_, base, combo)                                                \
    )
/**/

#include <boost/parameter/aux_/preprocessor/impl/arity_range.hpp>

// Expands to the layer of forwarding functions for the function with the
// specified name, whose arguments determine the range of arities.
#define BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOADS(name, impl, args, const_) \
    BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOADS_AUX(                          \
        name                                                                 \
      , impl                                                                 \
      , BOOST_PARAMETER_ARITY_RANGE(args)                                    \
      , BOOST_PARAMETER_FUNCTION_FORWARD_COMBINATIONS(args)                  \
      , const_                                                               \
    )
/**/

// Expands to the layer of forwarding functions for the constructor in the
// specified class, whose arguments determine the range of arities.
#define BOOST_PARAMETER_CONSTRUCTOR_OVERLOADS(class_, base, args)            \
    BOOST_PARAMETER_CONSTRUCTOR_OVERLOADS_AUX(                               \
        class_                                                               \
      , base                                                                 \
      , BOOST_PARAMETER_ARITY_RANGE(args)                                    \
      , BOOST_PARAMETER_FUNCTION_FORWARD_COMBINATIONS(args)                  \
    )
/**/

#endif  // BOOST_PARAMETER_HAS_PERFECT_FORWARDING
#endif  // include guard

