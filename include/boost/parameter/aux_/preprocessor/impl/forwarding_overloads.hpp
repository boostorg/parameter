// Copyright Daniel Wallin 2006.
// Copyright Cromwell D. Enage 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_AUX_PREPROCESSOR_IMPL_FORWARDING_OVERLOADS_HPP
#define BOOST_PARAMETER_AUX_PREPROCESSOR_IMPL_FORWARDING_OVERLOADS_HPP

#include <boost/parameter/aux_/preprocessor/impl/function_name.hpp>

// Defines the no-spec implementation function header.
#define BOOST_PARAMETER_NO_SPEC_FUNCTION_IMPL_HEAD(name, is_const)           \
    template <typename ResultType, typename Args>                            \
    BOOST_PARAMETER_MEMBER_FUNCTION_STATIC(name) ResultType                  \
        BOOST_PARAMETER_NO_SPEC_FUNCTION_IMPL_NAME(                          \
            name, is_const                                                   \
        )(ResultType(*)(), Args const& args)
/**/

#include <boost/parameter/config.hpp>

#if defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)

#include <boost/parameter/aux_/preprocessor/impl/parenthesized_type.hpp>

// Expands to the result metafunction for the specified no-spec function.
#define BOOST_PARAMETER_NO_SPEC_FUNCTION_HEAD(result, name, is_const)        \
    template <typename TaggedArg0, typename ...TaggedArgs>                   \
    struct BOOST_PARAMETER_NO_SPEC_FUNCTION_RESULT_NAME(name, is_const)      \
    {                                                                        \
        typedef typename BOOST_PARAMETER_PARENTHESIZED_TYPE(result) type;    \
    };
/**/

#include <boost/parameter/parameters.hpp>
#include <boost/parameter/are_tagged_arguments.hpp>
#include <boost/core/enable_if.hpp>

// Exapnds to a variadic constructor that is enabled if and only if all its
// arguments are tagged arguments.  The enclosing class must inherit from the
// specified base class, which in turn must implement a constructor that takes
// in the argument pack that this one passes on.
#define BOOST_PARAMETER_NO_SPEC_CONSTRUCTOR(class_, base)                    \
    template <                                                               \
        typename TaggedArg0                                                  \
      , typename ...TaggedArgs                                               \
      , typename = typename ::boost::enable_if<                              \
            ::boost::parameter                                               \
            ::are_tagged_arguments<TaggedArg0,TaggedArgs...>                 \
        >::type                                                              \
    > inline explicit                                                        \
    class_(TaggedArg0 const& arg0, TaggedArgs const&... args)                \
      : BOOST_PARAMETER_PARENTHESIZED_TYPE(base)(                            \
            ::boost::parameter::parameters<>()(arg0, args...)                \
        )                                                                    \
    {                                                                        \
    }
/**/

// Exapnds to a variadic constructor that is enabled if and only if all its
// arguments are tagged arguments.  The specified function must be able to
// take in the argument pack that this constructor passes on.
#define BOOST_PARAMETER_NO_SPEC_NO_BASE_CONSTRUCTOR(class_, func)            \
    template <                                                               \
        typename TaggedArg0                                                  \
      , typename ...TaggedArgs                                               \
      , typename = typename ::boost::enable_if<                              \
            ::boost::parameter                                               \
            ::are_tagged_arguments<TaggedArg0,TaggedArgs...>                 \
        >::type                                                              \
    > inline explicit                                                        \
    class_(TaggedArg0 const& arg0, TaggedArgs const&... args)                \
    {                                                                        \
        func(::boost::parameter::parameters<>()(arg0, args...));             \
    }
/**/

#include <boost/tti/detail/dnullptr.hpp>
#include <boost/preprocessor/control/expr_if.hpp>

// Exapnds to a variadic function that is enabled if and only if
// all its arguments are tagged arguments.
#define BOOST_PARAMETER_NO_SPEC_FUNCTION_OVERLOAD(name, impl, is_m, c)       \
    template <typename TaggedArg0, typename ...TaggedArgs>                   \
    BOOST_PARAMETER_MEMBER_FUNCTION_STATIC(impl)                             \
    inline typename ::boost::lazy_enable_if<                                 \
        ::boost::parameter                                                   \
        ::are_tagged_arguments<TaggedArg0,TaggedArgs...>                     \
      , BOOST_PARAMETER_NO_SPEC_FUNCTION_RESULT_NAME(                        \
            impl, c                                                          \
        )<TaggedArg0,TaggedArgs...>                                          \
    >::type BOOST_PARAMETER_MEMBER_FUNCTION_NAME(name)                       \
    (TaggedArg0 const& arg0, TaggedArgs const&... args)                      \
    BOOST_PP_EXPR_IF(c, const)                                               \
    {                                                                        \
        return BOOST_PP_EXPR_IF(is_m, this->)                                \
        BOOST_PARAMETER_NO_SPEC_FUNCTION_IMPL_NAME(impl, c)(                 \
            static_cast<                                                     \
                typename BOOST_PARAMETER_NO_SPEC_FUNCTION_RESULT_NAME(       \
                    impl, c                                                  \
                )<TaggedArg0,TaggedArgs...>::type(*)()                       \
            >(BOOST_TTI_DETAIL_NULLPTR)                                      \
          , ::boost::parameter::parameters<>()(arg0, args...)                \
        );                                                                   \
    }
/**/

#include <boost/preprocessor/cat.hpp>

// Expands to a forwarding parameter for a constructor or forwarding function.
#define BOOST_PARAMETER_FUNCTION_ARG_TYPE_Z(z, n, type_prefix)               \
    BOOST_PP_CAT(type_prefix, n)&&
/**/

#include <utility>

// Expands to an argument passed from a forwarding function to the front-end
// implementation function, or from a constructor to its delegate.
#define BOOST_PARAMETER_FUNCTION_FORWARD_PARAM_Z(z, n, type_prefix)          \
    ::std::forward<BOOST_PP_CAT(type_prefix, n)>(BOOST_PP_CAT(a, n))
/**/

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

// Expands to a 0-arity forwarding function, whose job is to pass an empty
// pack to the front-end implementation function.
#define BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_0_Z(z, n, data)            \
    BOOST_PARAMETER_MEMBER_FUNCTION_STATIC(BOOST_PP_TUPLE_ELEM(4, 1, data))  \
    inline BOOST_PARAMETER_FUNCTION_RESULT_NAME(                             \
        BOOST_PP_TUPLE_ELEM(4, 1, data)                                      \
      , BOOST_PP_TUPLE_ELEM(4, 3, data)                                      \
    )<                                                                       \
        ::boost::parameter::aux::argument_pack<                              \
            BOOST_PARAMETER_FUNCTION_SPECIFICATION_NAME(                     \
                BOOST_PP_TUPLE_ELEM(4, 1, data)                              \
              , BOOST_PP_TUPLE_ELEM(4, 3, data)                              \
            )                                                                \
        >::type                                                              \
    >::type                                                                  \
    BOOST_PARAMETER_MEMBER_FUNCTION_NAME(BOOST_PP_TUPLE_ELEM(3, 0, data))()  \
    BOOST_PP_EXPR_IF(BOOST_PP_TUPLE_ELEM(4, 3, data), const)                 \
    {                                                                        \
        return BOOST_PP_EXPR_IF(BOOST_PP_TUPLE_ELEM(4, 2, data), this->)     \
        BOOST_PARAMETER_FUNCTION_IMPL_NAME(                                  \
            BOOST_PP_TUPLE_ELEM(4, 1, data)                                  \
          , BOOST_PP_TUPLE_ELEM(4, 3, data)                                  \
        )(                                                                   \
            BOOST_PARAMETER_FUNCTION_SPECIFICATION_NAME(                     \
                BOOST_PP_TUPLE_ELEM(4, 1, data)                              \
              , BOOST_PP_TUPLE_ELEM(4, 3, data)                              \
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
    BOOST_PARAMETER_MEMBER_FUNCTION_STATIC(BOOST_PP_TUPLE_ELEM(4, 1, data))  \
    inline typename BOOST_PARAMETER_FUNCTION_RESULT_NAME(                    \
        BOOST_PP_TUPLE_ELEM(4, 1, data)                                      \
      , BOOST_PP_TUPLE_ELEM(4, 3, data)                                      \
    )<                                                                       \
        typename ::boost::parameter::aux::argument_pack<                     \
            BOOST_PARAMETER_FUNCTION_SPECIFICATION_NAME(                     \
                BOOST_PP_TUPLE_ELEM(4, 1, data)                              \
              , BOOST_PP_TUPLE_ELEM(4, 3, data)                              \
            )                                                                \
          , BOOST_PP_CAT(BOOST_PP_ENUM_, z)(                                 \
                n                                                            \
              , BOOST_PARAMETER_FUNCTION_ARG_TYPE_Z                          \
              , ParameterArgumentType                                        \
            )                                                                \
        >::type                                                              \
    >::type                                                                  \
    BOOST_PARAMETER_MEMBER_FUNCTION_NAME(BOOST_PP_TUPLE_ELEM(4, 0, data))(   \
        BOOST_PP_ENUM_BINARY_PARAMS_Z(z, n, ParameterArgumentType, && a)     \
        BOOST_PARAMETER_FUNCTION_FORWARD_MATCH_Z(                            \
            z                                                                \
          , BOOST_PARAMETER_FUNCTION_SPECIFICATION_NAME(                     \
                BOOST_PP_TUPLE_ELEM(4, 1, data)                              \
              , BOOST_PP_TUPLE_ELEM(4, 3, data)                              \
            )                                                                \
          , n                                                                \
          , ParameterArgumentType                                            \
        )                                                                    \
    ) BOOST_PP_EXPR_IF(BOOST_PP_TUPLE_ELEM(4, 3, data), const)               \
    {                                                                        \
        return BOOST_PP_EXPR_IF(BOOST_PP_TUPLE_ELEM(4, 2, data), this->)     \
        BOOST_PARAMETER_FUNCTION_IMPL_NAME(                                  \
            BOOST_PP_TUPLE_ELEM(4, 1, data)                                  \
          , BOOST_PP_TUPLE_ELEM(4, 3, data)                                  \
        )(                                                                   \
            BOOST_PARAMETER_FUNCTION_SPECIFICATION_NAME(                     \
                BOOST_PP_TUPLE_ELEM(4, 1, data)                              \
              , BOOST_PP_TUPLE_ELEM(4, 3, data)                              \
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
        BOOST_PARAMETER_FUNCTION_FORWARD_MATCH_Z(                            \
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
#define BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOADS_AUX(nm, impl, r, is_m, c) \
    BOOST_PP_REPEAT_FROM_TO(                                                 \
        BOOST_PP_TUPLE_ELEM(2, 0, r)                                         \
      , BOOST_PP_TUPLE_ELEM(2, 1, r)                                         \
      , BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_Z                          \
      , (                                                                    \
            nm                                                               \
          , impl                                                             \
          , BOOST_PP_IF(                                                     \
                BOOST_PARAMETER_MEMBER_FUNCTION_IS_STATIC(nm)                \
              , 0                                                            \
              , is_m                                                         \
            )                                                                \
          , c                                                                \
        )                                                                    \
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
#define BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOADS(name, impl, a, is_m, c)   \
    BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOADS_AUX(                          \
        name, impl, BOOST_PARAMETER_ARITY_RANGE(a), is_m, c                  \
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

#include <boost/parameter/aux_/void.hpp>
#include <boost/parameter/aux_/preprocessor/impl/parenthesized_type.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>

// Expands to the result metafunction for the specified no-spec function.
#define BOOST_PARAMETER_NO_SPEC_FUNCTION_HEAD(result, name, is_const)        \
    template <                                                               \
        BOOST_PP_ENUM_BINARY_PARAMS(                                         \
            BOOST_PARAMETER_MAX_ARITY                                        \
          , typename TaggedArg                                               \
          , = ::boost::parameter::void_ BOOST_PP_INTERCEPT                   \
        )                                                                    \
    >                                                                        \
    struct BOOST_PARAMETER_NO_SPEC_FUNCTION_RESULT_NAME(name, is_const)      \
    {                                                                        \
        typedef typename BOOST_PARAMETER_PARENTHESIZED_TYPE(result) type;    \
    };
/**/

#include <boost/parameter/parameters.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/control/expr_if.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

#if defined(BOOST_NO_SFINAE)

// Exapnds to a tagged-argument constructor overload that passes the argument
// pack to the base class delegate constructor.
#define BOOST_PARAMETER_NO_SPEC_CONSTRUCTOR_OVERLOAD_Z(z, n, data)           \
    template <BOOST_PP_ENUM_PARAMS_Z(z, n, typename TaggedArg)>              \
    BOOST_PP_EXPR_IF(BOOST_PP_EQUAL(n, 1), explicit) inline                  \
    BOOST_PP_TUPLE_ELEM(2, 0, data)(                                         \
        BOOST_PP_ENUM_BINARY_PARAMS_Z(z, n, TaggedArg, const& arg)           \
    ) : BOOST_PARAMETER_PARENTHESIZED_TYPE(BOOST_PP_TUPLE_ELEM(2, 1, data))( \
            ::boost::parameter::parameters<>()(                              \
                BOOST_PP_ENUM_PARAMS_Z(z, n, arg)                            \
            )                                                                \
        )                                                                    \
    {                                                                        \
    }
/**/

// Exapnds to a tagged-argument constructor overload that passes the argument
// pack to the delegate function.
#define BOOST_PARAMETER_NO_SPEC_NO_BASE_CONSTRUCTOR_OVERLOAD_Z(z, n, data)   \
    template <BOOST_PP_ENUM_PARAMS_Z(z, n, typename TaggedArg)>              \
    BOOST_PP_EXPR_IF(BOOST_PP_EQUAL(n, 1), explicit) inline                  \
    BOOST_PP_TUPLE_ELEM(2, 0, data)(                                         \
        BOOST_PP_ENUM_BINARY_PARAMS_Z(z, n, TaggedArg, const& arg)           \
    )                                                                        \
    {                                                                        \
        BOOST_PP_TUPLE_ELEM(2, 1, data)(                                     \
            ::boost::parameter::parameters<>()(                              \
                BOOST_PP_ENUM_PARAMS_Z(z, n, arg)                            \
            )                                                                \
        );                                                                   \
    }
/**/

#include <boost/tti/detail/dnullptr.hpp>

// Exapnds to a tagged-argument function overload.
#define BOOST_PARAMETER_NO_SPEC_FUNCTION_OVERLOAD_Z(z, n, data)              \
    template <BOOST_PP_ENUM_PARAMS_Z(z, n, typename TaggedArg)>              \
    BOOST_PARAMETER_MEMBER_FUNCTION_STATIC(BOOST_PP_TUPLE_ELEM(4, 1, data))  \
    inline typename BOOST_PARAMETER_NO_SPEC_FUNCTION_RESULT_NAME(            \
        BOOST_PP_TUPLE_ELEM(4, 1, data)                                      \
      , BOOST_PP_TUPLE_ELEM(4, 3, data)                                      \
    )<BOOST_PP_ENUM_PARAMS_Z(z, n, TaggedArg)>::type                         \
        BOOST_PARAMETER_MEMBER_FUNCTION_NAME(                                \
            BOOST_PP_TUPLE_ELEM(4, 0, data)                                  \
        )(BOOST_PP_ENUM_BINARY_PARAMS_Z(z, n, TaggedArg, const& arg))        \
        BOOST_PP_EXPR_IF(BOOST_PP_TUPLE_ELEM(4, 3, data), const)             \
    {                                                                        \
        return BOOST_PP_EXPR_IF(BOOST_PP_TUPLE_ELEM(4, 2, data), this->)     \
        BOOST_PARAMETER_NO_SPEC_FUNCTION_IMPL_NAME(                          \
            BOOST_PP_TUPLE_ELEM(4, 1, data)                                  \
          , BOOST_PP_TUPLE_ELEM(4, 3, data)                                  \
        )(                                                                   \
            static_cast<                                                     \
                typename BOOST_PARAMETER_NO_SPEC_FUNCTION_RESULT_NAME(       \
                    BOOST_PP_TUPLE_ELEM(4, 1, data)                          \
                  , BOOST_PP_TUPLE_ELEM(4, 3, data)                          \
                )<BOOST_PP_ENUM_PARAMS_Z(z, n, TaggedArg)>::type(*)()        \
            >(BOOST_TTI_DETAIL_NULLPTR)                                      \
          , ::boost::parameter::parameters<>()(                              \
                BOOST_PP_ENUM_PARAMS_Z(z, n, arg)                            \
            )                                                                \
        );                                                                   \
    }
/**/

#else   // !defined(BOOST_NO_SFINAE)

#include <boost/parameter/are_tagged_arguments.hpp>
#include <boost/tti/detail/dnullptr.hpp>
#include <boost/core/enable_if.hpp>

// Exapnds to a tagged-argument constructor overload that passes the argument
// pack to the base class delegate constructor.  This constructor is enabled
// if and only if all its arguments are tagged arguments.
#define BOOST_PARAMETER_NO_SPEC_CONSTRUCTOR_OVERLOAD_Z(z, n, data)           \
    template <BOOST_PP_ENUM_PARAMS_Z(z, n, typename TaggedArg)>              \
    BOOST_PP_EXPR_IF(BOOST_PP_EQUAL(n, 1), explicit) inline                  \
    BOOST_PP_TUPLE_ELEM(2, 0, data)(                                         \
        BOOST_PP_ENUM_BINARY_PARAMS_Z(z, n, TaggedArg, const& arg)           \
      , typename ::boost::enable_if<                                         \
            ::boost::parameter::are_tagged_arguments<                        \
                BOOST_PP_ENUM_PARAMS_Z(z, n, TaggedArg)                      \
            >                                                                \
        >::type* = BOOST_TTI_DETAIL_NULLPTR                                  \
    ) : BOOST_PARAMETER_PARENTHESIZED_TYPE(BOOST_PP_TUPLE_ELEM(2, 1, data))( \
            ::boost::parameter::parameters<>()(                              \
                BOOST_PP_ENUM_PARAMS_Z(z, n, arg)                            \
            )                                                                \
        )                                                                    \
    {                                                                        \
    }
/**/

// Exapnds to a tagged-argument constructor overload that passes the argument
// pack to the delegate function.  This constructor is enabled if and only if
// all its arguments are tagged arguments.
#define BOOST_PARAMETER_NO_SPEC_NO_BASE_CONSTRUCTOR_OVERLOAD_Z(z, n, data)   \
    template <BOOST_PP_ENUM_PARAMS_Z(z, n, typename TaggedArg)>              \
    BOOST_PP_EXPR_IF(BOOST_PP_EQUAL(n, 1), explicit) inline                  \
    BOOST_PP_TUPLE_ELEM(2, 0, data)(                                         \
        BOOST_PP_ENUM_BINARY_PARAMS_Z(z, n, TaggedArg, const& arg)           \
      , typename ::boost::enable_if<                                         \
            ::boost::parameter::are_tagged_arguments<                        \
                BOOST_PP_ENUM_PARAMS_Z(z, n, TaggedArg)                      \
            >                                                                \
        >::type* = BOOST_TTI_DETAIL_NULLPTR                                  \
    )                                                                        \
    {                                                                        \
        BOOST_PP_TUPLE_ELEM(2, 1, data)(                                     \
            ::boost::parameter::parameters<>()(                              \
                BOOST_PP_ENUM_PARAMS_Z(z, n, arg)                            \
            )                                                                \
        );                                                                   \
    }
/**/

// Exapnds to a function overload that is enabled if and only if
// all its arguments are tagged arguments.
#define BOOST_PARAMETER_NO_SPEC_FUNCTION_OVERLOAD_Z(z, n, data)              \
    template <BOOST_PP_ENUM_PARAMS_Z(z, n, typename TaggedArg)>              \
    BOOST_PARAMETER_MEMBER_FUNCTION_STATIC(BOOST_PP_TUPLE_ELEM(4, 1, data))  \
    inline typename ::boost::lazy_enable_if<                                 \
        ::boost::parameter                                                   \
        ::are_tagged_arguments<BOOST_PP_ENUM_PARAMS_Z(z, n, TaggedArg)>      \
      , BOOST_PARAMETER_NO_SPEC_FUNCTION_RESULT_NAME(                        \
            BOOST_PP_TUPLE_ELEM(4, 1, data)                                  \
          , BOOST_PP_TUPLE_ELEM(4, 3, data)                                  \
        )<BOOST_PP_ENUM_PARAMS_Z(z, n, TaggedArg)>                           \
    >::type BOOST_PARAMETER_MEMBER_FUNCTION_NAME(                            \
        BOOST_PP_TUPLE_ELEM(4, 0, data)                                      \
    )(BOOST_PP_ENUM_BINARY_PARAMS_Z(z, n, TaggedArg, const& arg))            \
    BOOST_PP_EXPR_IF(BOOST_PP_TUPLE_ELEM(4, 3, data), const)                 \
    {                                                                        \
        return BOOST_PP_EXPR_IF(BOOST_PP_TUPLE_ELEM(4, 2, data), this->)     \
        BOOST_PARAMETER_NO_SPEC_FUNCTION_IMPL_NAME(                          \
            BOOST_PP_TUPLE_ELEM(4, 1, data)                                  \
          , BOOST_PP_TUPLE_ELEM(4, 3, data)                                  \
        )(                                                                   \
            static_cast<                                                     \
                typename BOOST_PARAMETER_NO_SPEC_FUNCTION_RESULT_NAME(       \
                    BOOST_PP_TUPLE_ELEM(4, 1, data)                          \
                  , BOOST_PP_TUPLE_ELEM(4, 3, data)                          \
                )<BOOST_PP_ENUM_PARAMS_Z(z, n, TaggedArg)>::type(*)()        \
            >(BOOST_TTI_DETAIL_NULLPTR)                                      \
          , ::boost::parameter::parameters<>()(                              \
                BOOST_PP_ENUM_PARAMS_Z(z, n, arg)                            \
            )                                                                \
        );                                                                   \
    }
/**/

#endif  // BOOST_NO_SFINAE

#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>

// Emulates a variadic constructor that is enabled if and only if all its
// arguments are tagged arguments.  The enclosing class must inherit from the
// specified base class, which in turn must implement a constructor that takes
// in the argument pack that this one passes on.
#define BOOST_PARAMETER_NO_SPEC_CONSTRUCTOR(class_, base)                    \
    BOOST_PP_REPEAT_FROM_TO(                                                 \
        1                                                                    \
      , BOOST_PP_INC(BOOST_PARAMETER_MAX_ARITY)                              \
      , BOOST_PARAMETER_NO_SPEC_CONSTRUCTOR_OVERLOAD_Z                       \
      , (class_, base)                                                       \
    )
/**/

// Emulates a variadic constructor that is enabled if and only if all its
// arguments are tagged arguments.  The specified function must be able to
// take in the argument pack that this constructor passes on.
#define BOOST_PARAMETER_NO_SPEC_NO_BASE_CONSTRUCTOR(class_, func)            \
    BOOST_PP_REPEAT_FROM_TO(                                                 \
        1                                                                    \
      , BOOST_PP_INC(BOOST_PARAMETER_MAX_ARITY)                              \
      , BOOST_PARAMETER_NO_SPEC_NO_BASE_CONSTRUCTOR_OVERLOAD_Z               \
      , (class_, func)                                                       \
    )
/**/

// Emulates a variadic function that is enabled if and only if
// all its arguments are tagged arguments.
#define BOOST_PARAMETER_NO_SPEC_FUNCTION_OVERLOAD(name, impl, is_m, c)       \
    BOOST_PP_REPEAT_FROM_TO(                                                 \
        1                                                                    \
      , BOOST_PP_INC(BOOST_PARAMETER_MAX_ARITY)                              \
      , BOOST_PARAMETER_NO_SPEC_FUNCTION_OVERLOAD_Z                          \
      , (name, impl, is_m, c)                                                \
    )
/**/

#include <boost/preprocessor/seq/seq.hpp>
#include <boost/preprocessor/cat.hpp>

// Expands to the default constructor, whose job is to pass an empty argument
// pack back to the delegate constructor of the base class.
#define BOOST_PARAMETER_DEFAULT_CONSTRUCTOR(z, n, seq)                       \
    inline                                                                   \
    BOOST_PP_TUPLE_ELEM(2, 0, BOOST_PP_SEQ_HEAD(BOOST_PP_SEQ_TAIL(seq)))()   \
      : BOOST_PARAMETER_PARENTHESIZED_TYPE(                                  \
            BOOST_PP_TUPLE_ELEM(                                             \
                2, 1, BOOST_PP_SEQ_HEAD(BOOST_PP_SEQ_TAIL(seq))              \
            )                                                                \
        )(BOOST_PP_CAT(constructor_parameters, __LINE__)()())                \
    {                                                                        \
    }
/**/

#include <boost/parameter/aux_/pp_impl/argument_pack.hpp>

// Expands to a 0-arity forwarding function, whose job is to pass an empty
// argument pack to the front-end implementation function.
#define BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_0_ARITY(z, n, seq)         \
    BOOST_PARAMETER_MEMBER_FUNCTION_STATIC(                                  \
        BOOST_PP_TUPLE_ELEM(                                                 \
            4, 1, BOOST_PP_SEQ_HEAD(BOOST_PP_SEQ_TAIL(seq))                  \
        )                                                                    \
    )                                                                        \
    inline BOOST_PARAMETER_FUNCTION_RESULT_NAME(                             \
        BOOST_PP_TUPLE_ELEM(                                                 \
            4, 1, BOOST_PP_SEQ_HEAD(BOOST_PP_SEQ_TAIL(seq))                  \
        )                                                                    \
      , BOOST_PP_TUPLE_ELEM(                                                 \
            4, 3, BOOST_PP_SEQ_HEAD(BOOST_PP_SEQ_TAIL(seq))                  \
        )                                                                    \
    )<                                                                       \
        ::boost::parameter::aux::argument_pack<                              \
            BOOST_PARAMETER_FUNCTION_SPECIFICATION_NAME(                     \
                BOOST_PP_TUPLE_ELEM(                                         \
                    4, 1, BOOST_PP_SEQ_HEAD(BOOST_PP_SEQ_TAIL(seq))          \
                )                                                            \
              , BOOST_PP_TUPLE_ELEM(                                         \
                    4, 3, BOOST_PP_SEQ_HEAD(BOOST_PP_SEQ_TAIL(seq))          \
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
        return BOOST_PP_EXPR_IF(                                             \
            BOOST_PP_TUPLE_ELEM(                                             \
                4, 2, BOOST_PP_SEQ_HEAD(BOOST_PP_SEQ_TAIL(seq))              \
            )                                                                \
          , this->                                                           \
        ) BOOST_PARAMETER_FUNCTION_IMPL_NAME(                                \
            BOOST_PP_TUPLE_ELEM(                                             \
                4, 1, BOOST_PP_SEQ_HEAD(BOOST_PP_SEQ_TAIL(seq))              \
            )                                                                \
          , BOOST_PP_TUPLE_ELEM(                                             \
                4, 3, BOOST_PP_SEQ_HEAD(BOOST_PP_SEQ_TAIL(seq))              \
            )                                                                \
        )(                                                                   \
            BOOST_PARAMETER_FUNCTION_SPECIFICATION_NAME(                     \
                BOOST_PP_TUPLE_ELEM(                                         \
                    4, 1, BOOST_PP_SEQ_HEAD(BOOST_PP_SEQ_TAIL(seq))          \
                )                                                            \
              , BOOST_PP_TUPLE_ELEM(                                         \
                    4, 3, BOOST_PP_SEQ_HEAD(BOOST_PP_SEQ_TAIL(seq))          \
                )                                                            \
            )()()                                                            \
        );                                                                   \
    }
/**/

#include <boost/parameter/aux_/preprocessor/binary_seq_to_args.hpp>
#include <boost/parameter/aux_/preprocessor/impl/function_forward_match.hpp>
#include <boost/preprocessor/seq/size.hpp>

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
    BOOST_PP_EXPR_IF(                                                        \
        BOOST_PP_EQUAL(BOOST_PP_SEQ_SIZE(BOOST_PP_SEQ_TAIL(seq)), 1)         \
      , explicit                                                             \
    )                                                                        \
    inline BOOST_PP_TUPLE_ELEM(2, 0, BOOST_PP_SEQ_HEAD(seq))(                \
        BOOST_PARAMETER_AUX_PP_BINARY_SEQ_TO_ARGS(                           \
            BOOST_PP_SEQ_TAIL(seq), (ParameterArgumentType)(a)               \
        )                                                                    \
        BOOST_PARAMETER_FUNCTION_FORWARD_MATCH(                              \
            BOOST_PP_CAT(constructor_parameters, __LINE__)                   \
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
    BOOST_PARAMETER_MEMBER_FUNCTION_STATIC(                                  \
        BOOST_PP_TUPLE_ELEM(4, 1, BOOST_PP_SEQ_HEAD(seq))                    \
    )                                                                        \
    inline typename BOOST_PARAMETER_FUNCTION_RESULT_NAME(                    \
        BOOST_PP_TUPLE_ELEM(4, 1, BOOST_PP_SEQ_HEAD(seq))                    \
      , BOOST_PP_TUPLE_ELEM(4, 3, BOOST_PP_SEQ_HEAD(seq))                    \
    )<                                                                       \
        typename ::boost::parameter::aux::argument_pack<                     \
            BOOST_PARAMETER_FUNCTION_SPECIFICATION_NAME(                     \
                BOOST_PP_TUPLE_ELEM(4, 1, BOOST_PP_SEQ_HEAD(seq))            \
              , BOOST_PP_TUPLE_ELEM(4, 3, BOOST_PP_SEQ_HEAD(seq))            \
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
        BOOST_PARAMETER_FUNCTION_FORWARD_MATCH(                              \
            BOOST_PARAMETER_FUNCTION_SPECIFICATION_NAME(                     \
                BOOST_PP_TUPLE_ELEM(4, 1, BOOST_PP_SEQ_HEAD(seq))            \
              , BOOST_PP_TUPLE_ELEM(4, 3, BOOST_PP_SEQ_HEAD(seq))            \
            )                                                                \
          , BOOST_PP_SEQ_SIZE(BOOST_PP_SEQ_TAIL(seq))                        \
          , ParameterArgumentType                                            \
        )                                                                    \
    ) BOOST_PP_EXPR_IF(                                                      \
        BOOST_PP_TUPLE_ELEM(4, 3, BOOST_PP_SEQ_HEAD(seq)), const             \
    )                                                                        \
    {                                                                        \
        return BOOST_PP_EXPR_IF(                                             \
            BOOST_PP_TUPLE_ELEM(4, 2, BOOST_PP_SEQ_HEAD(seq))                \
          , this->                                                           \
        ) BOOST_PARAMETER_FUNCTION_IMPL_NAME(                                \
            BOOST_PP_TUPLE_ELEM(4, 1, BOOST_PP_SEQ_HEAD(seq))                \
          , BOOST_PP_TUPLE_ELEM(4, 3, BOOST_PP_SEQ_HEAD(seq))                \
        )(                                                                   \
            BOOST_PARAMETER_FUNCTION_SPECIFICATION_NAME(                     \
                BOOST_PP_TUPLE_ELEM(4, 1, BOOST_PP_SEQ_HEAD(seq))            \
              , BOOST_PP_TUPLE_ELEM(4, 3, BOOST_PP_SEQ_HEAD(seq))            \
            )()(                                                             \
                BOOST_PP_ENUM_PARAMS(                                        \
                    BOOST_PP_SEQ_SIZE(BOOST_PP_SEQ_TAIL(seq)), a             \
                )                                                            \
            )                                                                \
        );                                                                   \
    }
/**/

#include <boost/parameter/aux_/preprocessor/binary_seq_for_each.hpp>
#include <boost/preprocessor/control/if.hpp>

// Expands to all constructors that take in n arguments.  Enables
// BOOST_PARAMETER_CONSTRUCTOR_OVERLOADS_AUX to use
// BOOST_PARAMETER_CONSTRUCTOR_OVERLOAD_Z within BOOST_PP_REPEAT_FROM_TO.
#define BOOST_PARAMETER_CONSTRUCTOR_OVERLOAD_Z(z, n, data)                   \
    BOOST_PP_IF(                                                             \
        n                                                                    \
      , BOOST_PARAMETER_AUX_PP_BINARY_SEQ_FOR_EACH_Z                         \
      , BOOST_PARAMETER_DEFAULT_CONSTRUCTOR                                  \
    )(z, n, (BOOST_PARAMETER_CONSTRUCTOR_OVERLOAD_R)(data))
/**/

// Expands to all forwarding functions that take in n arguments.  Enables
// BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOADS_AUX to use
// BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_Z within BOOST_PP_REPEAT_FROM_TO.
#define BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_Z(z, n, data)              \
    BOOST_PP_IF(                                                             \
        n                                                                    \
      , BOOST_PARAMETER_AUX_PP_BINARY_SEQ_FOR_EACH_Z                         \
      , BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_0_ARITY                    \
    )(z, n, (BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_R)(data))
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

// Helper macro for BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOADS.
#define BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOADS_AUX(nm, impl, r, is_m, c) \
    BOOST_PP_REPEAT_FROM_TO(                                                 \
        BOOST_PP_TUPLE_ELEM(2, 0, r)                                         \
      , BOOST_PP_TUPLE_ELEM(2, 1, r)                                         \
      , BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOAD_Z                          \
      , (                                                                    \
            nm                                                               \
          , impl                                                             \
          , BOOST_PP_IF(                                                     \
                BOOST_PARAMETER_MEMBER_FUNCTION_IS_STATIC(impl)              \
              , 0                                                            \
              , is_m                                                         \
            )                                                                \
          , c                                                                \
        )                                                                    \
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
    )
/**/

// Expands to the layer of forwarding functions for the function with the
// specified name, whose arguments determine the range of arities.
#define BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOADS(name, impl, a, is_m, c)   \
    BOOST_PARAMETER_FUNCTION_FORWARD_OVERLOADS_AUX(                          \
        name                                                                 \
      , impl                                                                 \
      , BOOST_PARAMETER_ARITY_RANGE(a)                                       \
      , is_m                                                                 \
      , c                                                                    \
    )
/**/

#endif  // BOOST_PARAMETER_HAS_PERFECT_FORWARDING
#endif  // include guard

