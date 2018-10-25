// Copyright Daniel Wallin 2006.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_AUX_PREPROCESSOR_IMPL_FORWARDING_OVERLOADS_HPP
#define BOOST_PARAMETER_AUX_PREPROCESSOR_IMPL_FORWARDING_OVERLOADS_HPP

#define BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_PRODUCT(r, product)        \
    (product)
/**/

#include <boost/preprocessor/repetition/enum_params.hpp>

#define BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_TEMPLATE_HEADER_Z(z, n, p) \
    template <BOOST_PP_ENUM_PARAMS_Z(z, n, typename p)>
/**/

#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/cat.hpp>

#define BOOST_PARAMETER_FUNCTION_ARGUMENT(r, _, i, elem)                     \
    BOOST_PP_COMMA_IF(i) elem& BOOST_PP_CAT(a, i)
/**/

#include <boost/parameter/aux_/preprocessor/qualifier.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/config.hpp>

#if defined(BOOST_NO_FUNCTION_TEMPLATE_ORDERING)
// No partial ordering.  This feature doesn't work.
#define BOOST_PARAMETER_FUNCTION_FORWARD_COMBINATION(r, _, i, elem)          \
    (BOOST_PP_IF(                                                            \
        BOOST_PARAMETER_IS_QUALIFIER(                                        \
            BOOST_PARAMETER_FN_ARG_KEYWORD(elem)                             \
        )                                                                    \
      , (BOOST_PP_CAT(ParameterArgumentType, i))                             \
      , (BOOST_PP_CAT(ParameterArgumentType, i) const)                       \
    ))
/**/
#else
#define BOOST_PARAMETER_FUNCTION_FORWARD_COMBINATION(r, _, i, elem)          \
    (BOOST_PP_IF(                                                            \
        BOOST_PARAMETER_IS_QUALIFIER(                                        \
            BOOST_PARAMETER_FN_ARG_KEYWORD(elem)                             \
        )                                                                    \
      , (BOOST_PP_CAT(ParameterArgumentType, i) const)                       \
        (BOOST_PP_CAT(ParameterArgumentType, i))                             \
      , (BOOST_PP_CAT(ParameterArgumentType, i) const)                       \
    ))
/**/
#endif  // BOOST_NO_FUNCTION_TEMPLATE_ORDERING

#include <boost/preprocessor/seq/for_each_i.hpp>

#define BOOST_PARAMETER_FUNCTION_FORWARD_COMBINATIONS(args)                  \
    BOOST_PP_SEQ_FOR_EACH_I(                                                 \
        BOOST_PARAMETER_FUNCTION_FORWARD_COMBINATION, ~, args                \
    )
/**/

#include <boost/parameter/aux_/preprocessor/impl/function_forward_match.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/control/expr_if.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
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
#include <boost/preprocessor/seq/enum.hpp>

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

#define BOOST_PARAMETER_CONSTRUCTOR_OVERLOAD_Z(z, n, data)                   \
    BOOST_PP_IF(                                                             \
        n                                                                    \
      , BOOST_PARAMETER_CONSTRUCTOR_OVERLOAD_IMPL_ARITY_N                    \
      , BOOST_PARAMETER_CONSTRUCTOR_OVERLOAD_IMPL_ARITY_0                    \
    )(z, n, data)
/**/

#define BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_Z(z, n, data)              \
    BOOST_PP_IF(                                                             \
        n                                                                    \
      , BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_IMPL_ARITY_N               \
      , BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_IMPL_ARITY_0               \
    )(z, n, data)
/**/

#include <boost/preprocessor/repetition/repeat_from_to.hpp>

// Helper macro for BOOST_PARAMETER_CONSTRUCTOR_OVERLOADS.
#define BOOST_PARAMETER_CONSTRUCTOR_OVERLOADS_AUX(class_, base, r, combo)    \
    BOOST_PP_REPEAT_FROM_TO(                                                 \
        BOOST_PP_TUPLE_ELEM(2, 0, r)                                         \
      , BOOST_PP_TUPLE_ELEM(2, 1, r)                                         \
      , BOOST_PARAMETER_CONSTRUCTOR_OVERLOAD_Z                               \
      , (class_, base, combo)                                                \
    )
/**/

// Helper macro for BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOADS.
#define BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOADS_AUX(n, i, r, combo, c)    \
    BOOST_PP_REPEAT_FROM_TO(                                                 \
        BOOST_PP_TUPLE_ELEM(2, 0, r)                                         \
      , BOOST_PP_TUPLE_ELEM(2, 1, r)                                         \
      , BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_Z                          \
      , (n, i, combo, c)                                                     \
    )
/**/

#include <boost/parameter/aux_/preprocessor/impl/arity_range.hpp>

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

#endif  // include guard

