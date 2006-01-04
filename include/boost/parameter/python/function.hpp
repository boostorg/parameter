// Copyright Daniel Wallin 2005. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_FUNCTION_051130_HPP
# define BOOST_PARAMETER_FUNCTION_051130_HPP

# include <boost/preprocessor/cat.hpp>
# include <boost/preprocessor/for.hpp>
# include <boost/preprocessor/detail/split.hpp>
# include <boost/preprocessor/facilities/is_empty.hpp>
# include <boost/preprocessor/tuple/eat.hpp>
# include <boost/preprocessor/logical/not.hpp>
# include <boost/preprocessor/repeat.hpp>
# include <boost/preprocessor/punctuation/comma_if.hpp>
# include <boost/preprocessor/seq/for_each.hpp>
# include <boost/preprocessor/seq/for_each_i.hpp>
# include <boost/preprocessor/seq/fold_left.hpp>
# include <boost/preprocessor/arithmetic/inc.hpp>
# include <boost/preprocessor/arithmetic/add.hpp>
# include <boost/preprocessor/control/if.hpp>
# include <boost/preprocessor/repetition/repeat_from_to.hpp>
# include <boost/preprocessor/repetition/enum_params.hpp>
# include <boost/preprocessor/repetition/enum_binary_params.hpp>

# include <boost/implicit_cast.hpp>
# include <boost/parameter.hpp>
# include <boost/type_traits/is_convertible.hpp>
# include <boost/mpl/vector.hpp>
# include <boost/mpl/bool.hpp>

# define ZKB_KEYWORD(tag_namespace,name) \
    BOOST_PARAMETER_KEYWORD(tag_namespace,name) \
    namespace tag_namespace \
    { \
      struct name \
      { \
          static inline char const* keyword() \
          { \
              return #name; \
          } \
      }; \
    }

//
// A ``spec`` is a 4-tuple defined as:
//
// ( qualifier , name , type, default )
//
// Where qualifier is one of ``required`` or ``optional``.
//
// Below are access macros to the different components of a spec
//

# define ZKB_spec_qualifier(spec) BOOST_PP_TUPLE_ELEM(4,0,spec)
# define ZKB_spec_name(spec) BOOST_PP_TUPLE_ELEM(4,1,spec)
# define ZKB_spec_type(spec) BOOST_PP_TUPLE_ELEM(4,2,spec)
# define ZKB_spec_default(spec) BOOST_PP_TUPLE_ELEM(4,3,spec)

//
// Split a sublist into qualifier and arg-list.
// The form for the sublist w is:
//
//    [optional|required] (x,y ..)
//
// ZKB_sublist_qualifier(w) will return 'optional' or 'required'
// ZKB_sublist_args(w) will return (x,y ..)
//

# define ZKB_sublist_required(args)
# define ZKB_sublist_optional(args)

# define ZKB_sublist_split_required required,
# define ZKB_sublist_split_optional optional,

# define ZKB_sublist_split(sub) \
    BOOST_PP_CAT(ZKB_sublist_split_, sub)

# define ZKB_sublist_qualifier(sub) \
    BOOST_PP_SPLIT(0, ZKB_sublist_split(sub))

# define ZKB_sublist_args(sub) \
    BOOST_PP_SPLIT(1, ZKB_sublist_split(sub))

# define ZKB_sublist(sub) \
    [ ZKB_sublist_qualifier(sub) : ZKB_sublist_args(sub) ]

//
// Iteration through n-ary tuples seqs
//

# define ZKB_for_each_head_aux2(x,y) (x,y), ~
# define ZKB_for_each_head_aux3(x,y,z) (x,y,z), ~
# define ZKB_for_each_head_aux4(x,y,z,u) (x,y,z,u), ~
# define ZKB_for_each_head(n,x) \
    BOOST_PP_SPLIT(0, BOOST_PP_CAT(ZKB_for_each_head_aux,n) x)

# define ZKB_for_each_pred_aux_ZKB_END_SENTINEL
# define ZKB_for_each_pred_aux_check(x) \
    BOOST_PP_NOT(BOOST_PP_IS_EMPTY(BOOST_PP_CAT(ZKB_for_each_pred_aux_, x))), ~

# define ZKB_for_each_pred_aux2(x,y) ZKB_for_each_pred_aux_check(x)
# define ZKB_for_each_pred_aux3(x,y,z) ZKB_for_each_pred_aux_check(x)
# define ZKB_for_each_pred_aux4(x,y,z,u) ZKB_for_each_pred_aux_check(x)

# define ZKB_for_each_pred_aux0(n,x) BOOST_PP_CAT(ZKB_for_each_pred_aux,n) x

# define ZKB_for_each_pred(r, state) \
    BOOST_PP_SPLIT( \
        0 \
      , ZKB_for_each_pred_aux0( \
            BOOST_PP_TUPLE_ELEM(5,3,state) \
          , BOOST_PP_TUPLE_ELEM(5,0,state) \
        ) \
    )

# define ZKB_for_each_op(r, state) \
    ( \
        BOOST_PP_TUPLE_EAT(BOOST_PP_TUPLE_ELEM(5,3,state)) \
          BOOST_PP_TUPLE_ELEM(5,0,state) \
      , BOOST_PP_TUPLE_ELEM(5,1,state) \
      , BOOST_PP_TUPLE_ELEM(5,2,state) \
      , BOOST_PP_TUPLE_ELEM(5,3,state) \
      , BOOST_PP_INC(BOOST_PP_TUPLE_ELEM(5,4,state)) \
    )

# define ZKB_for_each_macro(r, state) \
    BOOST_PP_TUPLE_ELEM(5,2,state)( \
        r \
      , BOOST_PP_TUPLE_ELEM(5,4,state) \
      , ZKB_for_each_head( \
            BOOST_PP_TUPLE_ELEM(5,3,state) \
          , BOOST_PP_TUPLE_ELEM(5,0,state) \
        ) \
      , BOOST_PP_TUPLE_ELEM(5,1,state) \
    )

# define ZKB_for_each_build_end_sentinel(z,n,text) BOOST_PP_COMMA_IF(n) ZKB_END_SENTINEL
# define ZKB_for_each_build_end_sentinel_tuple(arity) \
    ( \
        BOOST_PP_REPEAT(arity, ZKB_for_each_build_end_sentinel, _) \
    )

# define ZKB_for_each_seq(arity, list, data, macro) \
    BOOST_PP_FOR( \
        (list ZKB_for_each_build_end_sentinel_tuple(arity), data, macro, arity, 0) \
      , ZKB_for_each_pred \
      , ZKB_for_each_op \
      , ZKB_for_each_macro \
    )

//
// ZKB_build_parameters
//
// Build a parameters<> specialization. tag_namespace is the namespace
// where the keywords reside. args is a sequence of spec's.
//
# define ZKB_build_parameters_aux(r,tag_namespace,i,elem) \
    BOOST_PP_COMMA_IF(i) \
    boost::parameter::ZKB_spec_qualifier(elem)< \
        tag_namespace::ZKB_spec_name(elem) \
      , boost::is_convertible<boost::mpl::_, ZKB_spec_type(elem)> \
    >

# define ZKB_build_parameters(tag_namespace, args) \
    boost::parameter::parameters< \
        BOOST_PP_SEQ_FOR_EACH_I(ZKB_build_parameters_aux, tag_namespace, args) \
    >

# define ZKB_formal_args(r, data, i, elem) \
    BOOST_PP_COMMA_IF(i) ZKB_spec_type(elem) ZKB_spec_name(elem)

# define ZKB_build_function_declaration(result,name,args) \
    result name( \
        BOOST_PP_SEQ_FOR_EACH_I(ZKB_formal_args, _, args) \
    )

# define ZKB_build_fwd_declaration(result,name,args) \
    ZKB_build_function_declaration(result,name,args);

# define ZKB_actual_args_maybe_default_optional(spec) | ZKB_spec_default(spec)
# define ZKB_actual_args_maybe_default_required(spec)
# define ZKB_actual_args_maybe_default(spec) \
    BOOST_PP_CAT( \
        ZKB_actual_args_maybe_default_ \
      , ZKB_spec_qualifier(spec) \
    )(spec)

# define ZKB_actual_args(r, data, i, elem) \
    BOOST_PP_COMMA_IF(i) \
    boost::implicit_cast<ZKB_spec_type(elem)>( \
        args[ZKB_spec_name(elem) ZKB_actual_args_maybe_default(elem)] \
    )

# define ZKB_build_dispatch(result,name,args_) \
    template <class Args> \
    result BOOST_PP_CAT(name,_dispatch)(Args const& args) \
    { \
        return name( \
            BOOST_PP_SEQ_FOR_EACH_I(ZKB_actual_args, _, args_) \
        ); \
    }

# define ZKB_calculate_arity_range_op_required 1
# define ZKB_calculate_arity_range_op_optional 0
# define ZKB_calculate_arity_range_op(s, state, spec) \
    BOOST_PP_IF( \
        BOOST_PP_CAT(ZKB_calculate_arity_range_op_, ZKB_spec_qualifier(spec)) \
      , BOOST_PP_INC(state) \
      , state \
    )

# define ZKB_calculate_arity_range(args) \
    ( \
        BOOST_PP_SEQ_FOLD_LEFT(ZKB_calculate_arity_range_op, 0, args) \
      , BOOST_PP_SEQ_SIZE(args) \
    )

# define ZKB_build_forwarding_functions_arity_nullary(n,data) \
    inline BOOST_PP_TUPLE_ELEM(2,0,data) BOOST_PP_TUPLE_ELEM(2,1,data)() \
    { \
        return BOOST_PP_CAT( \
            BOOST_PP_TUPLE_ELEM(2,1,data) \
          , _dispatch \
        )( \
            BOOST_PP_CAT(BOOST_PP_TUPLE_ELEM(2,1,data), _parameters)() \
        ); \
    }

# define ZKB_build_forwarding_functions_arity_nary(n,data) \
    template<BOOST_PP_ENUM_PARAMS(n, class A)> \
    BOOST_PP_TUPLE_ELEM(2,0,data) BOOST_PP_TUPLE_ELEM(2,1,data)( \
        BOOST_PP_ENUM_BINARY_PARAMS(n, A, const& a) \
    ) \
    { \
        return BOOST_PP_CAT( \
            BOOST_PP_TUPLE_ELEM(2,1,data) \
          , _dispatch \
        )( \
            BOOST_PP_CAT( \
                BOOST_PP_TUPLE_ELEM(2,1,data) \
              , _parameters \
            )()(BOOST_PP_ENUM_PARAMS(n, a)) \
        ); \
    }

# define ZKB_build_forwarding_functions_arity(z,n,data) \
    BOOST_PP_IF( \
        n \
      , ZKB_build_forwarding_functions_arity_nary \
      , ZKB_build_forwarding_functions_arity_nullary \
    )(n,data)

# define ZKB_build_forwarding_functions0(result,name,arity_tuple) \
    BOOST_PP_REPEAT_FROM_TO( \
        BOOST_PP_TUPLE_ELEM(2,0,arity_tuple) \
      , BOOST_PP_INC(BOOST_PP_TUPLE_ELEM(2,1,arity_tuple)) \
      , ZKB_build_forwarding_functions_arity \
      , (result,name) \
    )

# define ZKB_build_forwarding_functions(result,name,args) \
    ZKB_build_forwarding_functions0(result,name,ZKB_calculate_arity_range(args))

# define ZKB_build_meta_arg(r,data,i,spec) \
    BOOST_PP_COMMA_IF(i) ZKB_spec_type(spec) BOOST_PP_CAT(a,i)

# define ZKB_build_meta_is_required_optional 0
# define ZKB_build_meta_is_required_required 1
# define ZKB_build_meta_is_required(spec) \
    BOOST_PP_CAT(ZKB_build_meta_is_required_, ZKB_spec_qualifier(spec))

# define ZKB_build_meta_keywords(r,tag_namespace,i,spec) \
    BOOST_PP_COMMA_IF(i) boost::mpl::pair< \
        boost::mpl::bool_<ZKB_build_meta_is_required(spec)> \
      , tag_namespace::ZKB_spec_name(spec) \
    >

# define ZKB_build_meta_types(r,data,spec) , ZKB_spec_type(spec)

# define ZKB_build_meta_default(tag_namespace,spec) \
    static ZKB_spec_type(spec) default_(tag_namespace::ZKB_spec_name(spec)) \
    { \
        return ZKB_spec_default(spec); \
    }
                               
# define ZKB_build_meta_defaults(r,tag_namespace,spec) \
    BOOST_PP_IF( \
        ZKB_build_meta_is_required(spec) \
      , BOOST_PP_TUPLE_EAT(2) \
      , ZKB_build_meta_default \
    )(tag_namespace,spec)
                                   
# define ZKB_build_meta(result,name,meta_name,tag_namespace,args) \
    struct meta_name \
    { \
        result operator()(BOOST_PP_SEQ_FOR_EACH_I(ZKB_build_meta_arg, ~, args)) const \
        { \
            return name(BOOST_PP_ENUM_PARAMS(BOOST_PP_SEQ_SIZE(args), a)); \
        } \
\
        typedef BOOST_PP_CAT(boost::mpl::vector, BOOST_PP_SEQ_SIZE(args))< \
            BOOST_PP_SEQ_FOR_EACH_I(ZKB_build_meta_keywords, tag_namespace, args) \
        > keywords; \
\
        typedef BOOST_PP_CAT(boost::mpl::vector, BOOST_PP_INC(BOOST_PP_SEQ_SIZE(args)))< \
            result BOOST_PP_SEQ_FOR_EACH(ZKB_build_meta_types, ~, args) \
        > signature; \
\
        BOOST_PP_SEQ_FOR_EACH(ZKB_build_meta_defaults, tag_namespace, args) \
    };

# define ZKB_FUNCTION_IMPL_AUX(result,name,meta_name,args,tag_namespace,with_fwd_declaration) \
    BOOST_PP_IF( \
        with_fwd_declaration \
      , ZKB_build_fwd_declaration \
      , BOOST_PP_TUPLE_EAT(3) \
    )(result,name,args) \
    typedef ZKB_build_parameters(tag_namespace,args) BOOST_PP_CAT(name,_parameters); \
    ZKB_build_dispatch(result,name,args) \
    ZKB_build_forwarding_functions(result,name,args) \
    ZKB_build_meta(result,name,meta_name,tag_namespace,args) \
    ZKB_build_function_declaration(result,name,args)
                                   
# define ZKB_FUNCTION_IMPL(result,name,meta_name,args,tag_namespace,with_fwd_declaration) \
    ZKB_FUNCTION_IMPL_AUX( \
        result \
      , name \
      , meta_name \
      , ZKB_flatten_arg_spec(args) \
      , tag_namespace \
      , with_fwd_declaration \
    )

# define ZKB_FUNCTION(result,name,meta_name,tag_namespace,args) \
    ZKB_FUNCTION_IMPL(result,name,meta_name,args,tag_namespace,1)

# define ZKB_MEMBER(class_,result,name,meta_name,tag_namespace,args) \
    ZKB_FUNCTION_IMPL(result,name,meta_name,args,tag_namespace,0)

//
// ZKB_flatten_arg_spec
//
// Flattens an argument list into normalized 4 arity tuple sequence form:
//
// (qualifier, name, type, default)
//
// Example:
//
// (required (x, int) (y, float))
// (optional (z, int, 0))
//
// Is converted to:
//
// ((required, x, int, ~)) ((required, y, float, ~)) ((optional, z, int, 0))
//
// This makes processing much simpler.
//

# define ZKB_qualifier_arity_required 2
# define ZKB_qualifier_arity_optional 3
# define ZKB_qualifier_arity(q) BOOST_PP_CAT(ZKB_qualifier_arity_,q)

# define ZKB_flatten_arg_spec_aux_required(r,n,elem,_) \
    (( \
        required \
      , BOOST_PP_TUPLE_REM(2) elem \
      , ~ \
    ))
 
# define ZKB_flatten_arg_spec_aux_optional(r,n,elem,_) \
    (( \
        optional \
      , BOOST_PP_TUPLE_REM(3) elem \
    ))

# define ZKB_flatten_arg_spec_aux0(z,data,elem) \
    ZKB_for_each_seq( \
        ZKB_qualifier_arity(ZKB_sublist_qualifier(elem)) \
      , ZKB_sublist_args(elem) \
      , ~ \
      , BOOST_PP_CAT(ZKB_flatten_arg_spec_aux_, ZKB_sublist_qualifier(elem)) \
    )

# define ZKB_flatten_arg_spec(args) \
    BOOST_PP_SEQ_FOR_EACH(ZKB_flatten_arg_spec_aux0, _, args)
                                  
// ?_FUNCTION() generates
//
// void f(int value, char const* name, float scale);
//
// template<class Args>
// void f_dispatch(Args const& args)
// {
//     f(args[value], args[name], args[scale | 20.f]);
// }
//
// template<class A0>
// void f(A0 const& a0)
// {
//     f_dispatch(f_parameters()(a0));
// }
//
// template<class A0, class A1>
// void f(A0 const& a0, A1 const& a1)
// {
//     f_dispatch(f_parameters()(a0,a1));
// }
//
// template<class A0, class A1, class A2>
// void f(A0 const& a0, A1 const& a1, A2 const& a2)
// {
//     f_dispatch(f_parameters()(a0,a1,a2));
// }
//
// void f(int value, char const* name, float scale)
//
// ---------------------------------------------------------------------------
//
// _MEMBER() generates
//
// template<class Args>
// void f_dispatch(Args const& args)
// {
//     f(args[value], args[name], args[scale | 20.f]);
// }
//
// template<class A0>
// void f(A0 const& a0)
// {
//     f_dispatch(f_parameters()(a0));
// }
//
// template<class A0, class A1>
// void f(A0 const& a0, A1 const& a1)
// {
//     f_dispatch(f_parameters()(a0,a1));
// }
//
// template<class A0, class A1, class A2>
// void f(A0 const& a0, A1 const& a1, A2 const& a2)
// {
//     f_dispatch(f_parameters()(a0,a1,a2));
// }
//
// void f(int value, char const* name, float scale)
//
// ---------------------------------------------------------------------------
//
// _CONSTRUCTOR() generates
//
// template<class A0>
// type(A0 const& a0)
// {
//     this->init_dispatch(f_parameters()(a0));
// }
//
// template<class A0, class A1>
// type(A0 const& a0, A1 const& a1)
// {
//     this->init_dispatch(f_parameters()(a0,a1));
// }
//
// template<class Args>
// void init_dispatch(Args const& args)
// {
//     this->init(args[value], args[name], args[scale | 20.f]);
// }
//
// private:
//   void init(int value, char const* name, float scale)
//
// *** OR ***
//
// template<class A0>
// type(A0 const& a0)
//   : base(
//         f_parameters()(a0)[value]
//       , f_parameters()(a0)[name]
//       , f_parameters()(a0)[scale | 20.f]
//     )
// {}
//
// template<class A0, class A1>
// type(A0 const& a0, A1 const& a1)
//   : base(
//         f_parameters()(a0,a1)[value]
//       , f_parameters()(a0,a1)[name]
//       , f_parameters()(a0,a1)[scale | 20.f]
//     )
// {}

#endif // BOOST_PARAMETER_FUNCTION_051130_HPP

