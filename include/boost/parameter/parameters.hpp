// Copyright David Abrahams, Daniel Wallin 2003.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETERS_031014_HPP
#define BOOST_PARAMETERS_031014_HPP

#include <boost/detail/is_xxx.hpp>

#include <boost/mpl/lambda.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/always.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/pair.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/remove_reference.hpp>

#if BOOST_WORKAROUND(BOOST_MSVC, == 1310)
#include <boost/tti/detail/dnullptr.hpp>
#endif

#include <boost/parameter/aux_/arg_list.hpp>
#include <boost/parameter/aux_/yesno.hpp>
#include <boost/parameter/aux_/void.hpp>
#include <boost/parameter/aux_/default.hpp>
#include <boost/parameter/aux_/unwrap_cv_reference.hpp>
#include <boost/parameter/aux_/is_tagged_argument.hpp>
#include <boost/parameter/aux_/tag.hpp>
#include <boost/parameter/aux_/template_keyword.hpp>
#include <boost/parameter/aux_/set.hpp>
#include <boost/parameter/config.hpp>

#if defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
#include <boost/move/utility_core.hpp>
#include <boost/fusion/container/deque.hpp>
#include <boost/fusion/mpl.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/type_traits/is_lvalue_reference.hpp>
#include <boost/type_traits/remove_const.hpp>
#else
#include <boost/parameter/aux_/preprocessor/inc_binary_seq.hpp>
#include <boost/parameter/aux_/preprocessor/convert_binary_seq.hpp>
#include <boost/parameter/aux_/preprocessor/not_equal_binary_seq.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/comparison/not_equal.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_shifted.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/repetition/enum_shifted_params.hpp>
#include <boost/preprocessor/repetition/for.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/selection/min.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/seq/elem.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/cat.hpp>

#if !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#endif

// Generates:
//
//   make<
//       parameter_spec#0, argument_type#0
//     , make<
//           parameter_spec#1, argument_type#1
//         , ... mpl::identity<aux::empty_arg_list>
//    ...>
//   >
#define BOOST_PARAMETER_make_arg_list(z, n, names)                           \
    BOOST_PP_SEQ_ELEM(0, names)<                                             \
        BOOST_PP_CAT(BOOST_PP_SEQ_ELEM(1, names), n),                        \
        BOOST_PP_CAT(BOOST_PP_SEQ_ELEM(2, names), n),
/**/

#define BOOST_PARAMETER_build_arg_list(n, make, param_spec, arg_type)        \
    BOOST_PP_REPEAT(                                                         \
        n, BOOST_PARAMETER_make_arg_list, (make)(param_spec)(arg_type)       \
    )                                                                        \
    mpl::identity<void_>                                                     \
    BOOST_PP_REPEAT(n, BOOST_PARAMETER_right_angle, _)
/**/

#define BOOST_PARAMETER_make_deduced_list(z, n, names)                       \
    BOOST_PP_SEQ_ELEM(0, names)<BOOST_PP_CAT(BOOST_PP_SEQ_ELEM(1, names), n),
/**/

#define BOOST_PARAMETER_make_arg_items(r, p, i, elem)                        \
    aux::item<BOOST_PP_CAT(p, i),                                            \
    BOOST_PP_IF(elem, BOOST_PP_CAT(A, i)&, BOOST_PP_CAT(A, i) const&),
/**/

#define BOOST_PARAMETER_right_angle(z, n, _) >
/**/

#define BOOST_PARAMETER_build_deduced_list(n, make, parameter_spec)          \
    BOOST_PP_REPEAT(                                                         \
        n, BOOST_PARAMETER_make_deduced_list, (make)(parameter_spec)         \
    )                                                                        \
    mpl::identity<void_>                                                     \
    BOOST_PP_REPEAT(n, BOOST_PARAMETER_right_angle, _)
/**/

#define BOOST_PARAMETER_forward_typedef(z, n, names)                         \
    typedef BOOST_PP_CAT(BOOST_PP_SEQ_ELEM(0, names), n)                     \
    BOOST_PP_CAT(BOOST_PP_SEQ_ELEM(1, names), n);
/**/

#define BOOST_PARAMETER_template_args(z, n, text)                            \
    class BOOST_PP_CAT(PS, n) = void_
/**/

#define BOOST_PARAMETER_function_call_arg_list(r, seq)                       \
    aux::make_arg_list<                                                      \
        BOOST_PP_SEQ_FOR_EACH_I_R(                                           \
            r, BOOST_PARAMETER_make_arg_items, PS, seq                       \
        )                                                                    \
        void_                                                                \
        BOOST_PP_REPEAT(                                                     \
            BOOST_PP_SEQ_SIZE(seq), BOOST_PARAMETER_right_angle, _           \
        )                                                                    \
      , deduced_list                                                         \
      , aux::tag_keyword_arg                                                 \
    >
/**/

#define BOOST_PARAMETER_function_call_arg_pack_init(z, n, limit)             \
    BOOST_PP_CAT(a, BOOST_PP_SUB(limit, n))
/**/

#define BOOST_PARAMETER_function_call_op_pred_7(r, seq)                      \
    BOOST_PP_NOT_EQUAL(                                                      \
        BOOST_PP_MIN(BOOST_PP_INC(BOOST_PARAMETER_MAX_ARITY), 7)             \
      , BOOST_PP_SEQ_SIZE(seq)                                               \
    )
/**/

#define BOOST_PARAMETER_function_call_op_pred_8(r, seq)                      \
    BOOST_PP_NOT_EQUAL(8, BOOST_PP_SEQ_SIZE(seq))
/**/

#define BOOST_PARAMETER_function_call_op_pred_00000001(r, seq)               \
    BOOST_PARAMETER_AUX_PP_NOT_EQUAL_BINARY_SEQ(                             \
        (0)(0)(0)(0)(0)(0)(0)(1)                                             \
      , seq                                                                  \
    )
/**/

#define BOOST_PARAMETER_function_call_op_pred_9(r, seq)                      \
    BOOST_PP_NOT_EQUAL(9, BOOST_PP_SEQ_SIZE(seq))
/**/

#define BOOST_PARAMETER_function_call_op_inc(r, seq)                         \
    BOOST_PARAMETER_AUX_PP_INC_BINARY_SEQ(seq)
/**/

#define BOOST_PARAMETER_function_call_op_overload(r, seq)                    \
    template <BOOST_PP_ENUM_PARAMS(BOOST_PP_SEQ_SIZE(seq), class A)>         \
    inline typename mpl::first<                                              \
        typename BOOST_PARAMETER_function_call_arg_list(r, seq)::type        \
    >::type                                                                  \
        operator()(                                                          \
            BOOST_PARAMETER_AUX_PP_CONVERT_BINARY_SEQ(                       \
                seq                                                          \
              , A                                                            \
              , &                                                            \
              , const&                                                       \
              , a                                                            \
            )                                                                \
        ) const                                                              \
    {                                                                        \
        typedef typename BOOST_PARAMETER_function_call_arg_list(             \
            r, seq                                                           \
        )::type result;                                                      \
        typedef typename mpl::first<result>::type result_type;               \
        typedef typename mpl::second<result>::type error;                    \
        error();                                                             \
        return result_type(                                                  \
            BOOST_PP_ENUM(                                                   \
                BOOST_PP_SEQ_SIZE(seq)                                       \
              , BOOST_PARAMETER_function_call_arg_pack_init                  \
              , BOOST_PP_DEC(BOOST_PP_SEQ_SIZE(seq))                         \
            )                                                                \
            BOOST_PP_ENUM_TRAILING_PARAMS(                                   \
                BOOST_PP_SUB(                                                \
                    BOOST_PARAMETER_MAX_ARITY                                \
                  , BOOST_PP_SEQ_SIZE(seq)                                   \
                )                                                            \
              , aux::void_reference() BOOST_PP_INTERCEPT                     \
            )                                                                \
        );                                                                   \
    }
/**/

#endif // BOOST_PARAMETER_HAS_PERFECT_FORWARDING

namespace parameter_ {

    template <class T>
    struct unmatched_argument
    {
        BOOST_MPL_ASSERT((boost::is_same<T, void>));
        typedef int type;
    }; 
} // namespace parameter_

namespace boost {

template <class T>
class reference_wrapper;

namespace parameter {

namespace aux { struct use_default {}; }

// These templates can be used to describe the treatment of particular
// named parameters for the purposes of overload elimination with
// SFINAE, by placing specializations in the parameters<...> list.  In
// order for a treated function to participate in overload resolution:
//
//   - all keyword tags wrapped in required<...> must have a matching
//     actual argument
//
//   - The actual argument type matched by every keyword tag
//     associated with a predicate must satisfy that predicate
//
// If a keyword k is specified without an optional<...> or required<...>
// wrapper, it is treated as though optional<k> were specified.
//
// If a keyword k is specified with deduced<...>, that keyword
// will be automatically deduced from the argument list.
//
template <class Tag, class Predicate = aux::use_default>
struct required
{
    typedef Tag key_type;
    typedef Predicate predicate;
};

template <class Tag, class Predicate = aux::use_default>
struct optional
{
    typedef Tag key_type;
    typedef Predicate predicate;
};

template <class Tag>
struct deduced
{
    typedef Tag key_type;
};

namespace aux {

    // Defines metafunctions, is_required and is_optional, that identify
    // required<...>, optional<...>, and deduced<...> specializations.
    BOOST_DETAIL_IS_XXX_DEF(required, required, 2)
    BOOST_DETAIL_IS_XXX_DEF(optional, optional, 2)
    BOOST_DETAIL_IS_XXX_DEF(deduced_aux, deduced, 1)

    template <class S>
    struct is_deduced0 : is_deduced_aux<typename S::key_type>::type
    {
    };

    template <class S>
    struct is_deduced
      : mpl::eval_if<
            mpl::or_<is_optional<S>, is_required<S> >
          , is_deduced0<S>
          , mpl::false_
        >::type
    {
    };

    //
    // key_type, has_default, and predicate --
    //
    // These metafunctions accept a ParameterSpec and extract the
    // keyword tag, whether or not a default is supplied for the
    // parameter, and the predicate that the corresponding actual
    // argument type is required match.
    //
    // a ParameterSpec is a specialization of either keyword<...>,
    // required<...>, optional<...>
    //

    // helper for key_type<...>, below.
    template <class T>
    struct get_tag_type0
    {
        typedef typename T::key_type type;
    };

    template <class T>
    struct get_tag_type
      : mpl::eval_if<
            is_deduced_aux<typename T::key_type>
          , get_tag_type0<typename T::key_type>
          , mpl::identity<typename T::key_type>
        >
    {
    };

    template <class T>
    struct tag_type
      : mpl::eval_if<
            mpl::or_<is_optional<T>, is_required<T> >
          , get_tag_type<T>
          , mpl::identity<T>
        >
    {
    };

    template <class T>
    struct has_default : mpl::not_<is_required<T> >
    {
    };

    // helper for get_predicate<...>, below
    template <class T>
    struct get_predicate_or_default
    {
        typedef T type;
    };

    template <>
    struct get_predicate_or_default<use_default>
    {
        typedef mpl::always<mpl::true_> type;
    };

    // helper for predicate<...>, below
    template <class T>
    struct get_predicate : get_predicate_or_default<typename T::predicate>
    {
    };

    template <class T>
    struct predicate
      : mpl::eval_if<
            mpl::or_<is_optional<T>, is_required<T> >
          , get_predicate<T>
          , mpl::identity<mpl::always<mpl::true_> >
        >
    {
    };

    // Converts a ParameterSpec into a specialization of
    // parameter_requirements.  We need to do this in order to get the
    // tag_type into the type in a way that can be conveniently matched
    // by a satisfies(...) member function in arg_list.
    template <class ParameterSpec>
    struct as_parameter_requirements
    {
        typedef parameter_requirements<
            typename tag_type<ParameterSpec>::type
          , typename predicate<ParameterSpec>::type
          , typename has_default<ParameterSpec>::type
        > type;
    };

    template <class T>
    struct is_named_argument
      : mpl::or_<is_template_keyword<T>, is_tagged_argument<T> >
    {
    };

    // Returns mpl::true_ iff the given ParameterRequirements are satisfied by
    // ArgList.
    template <class ArgList, class ParameterRequirements>
    struct satisfies
    {
#if BOOST_WORKAROUND(BOOST_MSVC, == 1310)
        // VC7.1 can't handle the sizeof() implementation below,
        // so we use this instead.
        typedef typename mpl::apply_wrap3<
            typename ArgList::binding
          , typename ParameterRequirements::keyword
          , void_
          , mpl::false_
        >::type bound;

        typedef typename mpl::eval_if<
            is_same<bound, void_>
          , typename ParameterRequirements::has_default
          , mpl::apply_wrap2<
                typename mpl::lambda<
                    typename ParameterRequirements::predicate, lambda_tag
                >::type
              , bound
              , ArgList
            >
        >::type type;
#else // !BOOST_WORKAROUND(BOOST_MSVC, == 1310)
        BOOST_STATIC_CONSTANT(
            bool, value = (
                sizeof(
                    aux::to_yesno(
                        ArgList::satisfies(
                            static_cast<ParameterRequirements*>(
                                BOOST_TTI_DETAIL_NULLPTR
                            )
                          , static_cast<ArgList*>(BOOST_TTI_DETAIL_NULLPTR)
                        )
                    )
                ) == sizeof(yes_tag)
            )
        );

        typedef mpl::bool_<satisfies::value> type;
#endif // MSVC 7.1 workarounds needed.
    };

    // Returns mpl::true_ if the requirements of the given ParameterSpec
    // are satisfied by ArgList.
    template <class ArgList, class ParameterSpec>
    struct satisfies_requirements_of
      : satisfies<
            ArgList
          , typename as_parameter_requirements<ParameterSpec>::type
        >
    {
    };

    // Tags a deduced argument Arg with the keyword tag of Spec using TagFn.
    // Returns the tagged argument and the mpl::set<> UsedArgs with the
    // tag of Spec inserted.
    template <class UsedArgs, class Spec, class Arg, class TagFn>
    struct tag_deduced
    {
        typedef mpl::pair<
            typename mpl::apply_wrap2<
                TagFn, typename tag_type<Spec>::type, Arg
            >::type
          , typename aux::insert_<
                UsedArgs, typename tag_type<Spec>::type
            >::type
        > type;
    };

    template <
        class Argument
      , class ArgumentPack
      , class DeducedArgs
      , class UsedArgs
      , class TagFn
    >
    struct deduce_tag;

    // Tag type passed to MPL lambda.
    struct lambda_tag;

    // Helper for deduce_tag<> below.
    template <
        class Argument
      , class ArgumentPack
      , class DeducedArgs
      , class UsedArgs
      , class TagFn
    >
    struct deduce_tag0
    {
        typedef typename DeducedArgs::spec spec;

        typedef typename mpl::apply_wrap2<
            typename mpl::lambda<
                typename spec::predicate, lambda_tag
            >::type
          , Argument
          , ArgumentPack
        >::type condition;

        // Deduced parameter matches several arguments.
        BOOST_MPL_ASSERT((
            mpl::not_<
                mpl::and_<
                    condition
                  , aux::has_key_<UsedArgs, typename tag_type<spec>::type>
                >
            >
        ));

        typedef typename mpl::eval_if<
            condition
          , tag_deduced<UsedArgs, spec, Argument, TagFn>
          , deduce_tag<
                Argument
              , ArgumentPack
              , typename DeducedArgs::tail
              , UsedArgs
              , TagFn
            >
        >::type type;
    };

    // Tries to deduced a keyword tag for a given Argument.
    // Returns an mpl::pair<> consisting of the tagged_argument<>,
    // and an mpl::set<> where the new tag has been inserted.
    //
    //  Argument:     The argument type to be tagged.
    //
    //  ArgumentPack: The ArgumentPack built so far.
    //
    //  DeducedArgs:  A specialization of deduced_item<> (see below).
    //                A list containing only the deduced ParameterSpecs.
    //
    //  UsedArgs:     An mpl::set<> containing the keyword tags used so far.
    //
    //  TagFn:        A metafunction class used to tag positional or deduced
    //                arguments with a keyword tag.
    template <
        class Argument
      , class ArgumentPack
      , class DeducedArgs
      , class UsedArgs
      , class TagFn
    >
    struct deduce_tag
      : mpl::eval_if<
            is_same<DeducedArgs, void_>
          , mpl::pair<void_, UsedArgs>
          , deduce_tag0<Argument, ArgumentPack, DeducedArgs, UsedArgs, TagFn>
        >
    {
    };

    template <
        class List
      , class DeducedArgs
      , class TagFn
      , class Positional
      , class UsedArgs
      , class ArgumentPack
      , class Error
    >
    struct make_arg_list_aux;

    // Inserts Tagged::key_type into the UserArgs set.
    // Extra indirection to lazily evaluate Tagged::key_type.
    template <class UsedArgs, class Tagged>
    struct insert_tagged
      : aux::insert_<UsedArgs, typename Tagged::key_type>
    {
    };

    // Borland needs the insane extra-indirection workaround below so that
    // it doesn't magically drop the const qualifier from the argument type.
    template <
        class List
      , class DeducedArgs
      , class TagFn
      , class Positional
      , class UsedArgs
      , class ArgumentPack
#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
      , class argument
#endif
      , class Error
    >
#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
    struct make_arg_list00
#else
    struct make_arg_list0
#endif
    {
#if !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
        typedef typename List::arg argument;
#endif
        typedef typename remove_const<
            typename remove_reference<argument>::type
        >::type arg_type;
        typedef typename List::spec parameter_spec;
        typedef typename tag_type<parameter_spec>::type tag_;
        typedef is_named_argument<argument> is_tagged;

        // If this argument is either explicitly tagged or a deduced
        // parameter, then turn off positional matching.
        typedef mpl::and_<
            mpl::not_<mpl::or_<is_deduced<parameter_spec>, is_tagged> >
          , Positional
        > positional;

        // If this parameter is explicitly tagged, then add it to the
        // used-parmeters set.  We only really need to add parameters
        // that are deduced, but we would need a way to check if
        // a given tag corresponds to a deduced parameter spec.
        typedef typename mpl::eval_if<
            is_tagged
          , insert_tagged<UsedArgs, arg_type>
          , mpl::identity<UsedArgs>
        >::type used_args;

        // If this parameter is neither explicitly tagged nor positionally
        // matched, then deduce the tag from the deduced parameter specs.
        typedef typename mpl::eval_if<
            mpl::or_<is_tagged, positional>
          , mpl::pair<void_, used_args>
          , deduce_tag<argument, ArgumentPack, DeducedArgs, used_args, TagFn>
        >::type deduced_data;

        // If this parameter is explicitly tagged ...
        typedef typename mpl::eval_if<
            is_tagged
            // ... just use it
          , mpl::identity<arg_type>
            // ... else ...
          , mpl::eval_if<
                // if positional matching is turned on ...
                positional
                // ... tag it positionally
              , mpl::apply_wrap2<TagFn, tag_, argument>
                // ... else, use the deduced tag
              , mpl::first<deduced_data>
            >
        >::type tagged;

        // Build the arg_list incrementally, prepending new nodes.
        typedef typename mpl::if_<
            mpl::and_<
                is_same<Error, void_>
              , is_same<tagged, void_>
            >
          , parameter_::unmatched_argument<argument>
          , void_
        >::type error;

        typedef typename mpl::if_<
            is_same<tagged, void_>
          , ArgumentPack
          , arg_list<tagged, ArgumentPack>
        >::type argument_pack;

        typedef typename make_arg_list_aux<
            typename List::tail
          , DeducedArgs
          , TagFn
          , positional
          , typename deduced_data::second
          , argument_pack
          , error
        >::type type;
    };

#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
    template <
        class List
      , class DeducedArgs
      , class TagFn
      , class Positional
      , class UsedArgs
      , class ArgumentPack
      , class Error
    >
    struct make_arg_list0
    {
        typedef typename mpl::eval_if<
            typename List::is_arg_const
          , make_arg_list00<
                List
              , DeducedArgs
              , TagFn
              , Positional
              , UsedArgs
              , ArgumentPack
              , typename List::arg const
              , Error
            >
          , make_arg_list00<
                List
              , DeducedArgs
              , TagFn
              , Positional
              , UsedArgs
              , ArgumentPack
              , typename List::arg
              , Error
            >
        >::type type;
    };
#endif // Borland workarounds needed.

    // Returns an ArgumentPack where the list of arguments has been tagged
    // with keyword tags.
    //
    //   List:         A specialization of item<> (see below). Contains both
    //                 the ordered ParameterSpecs, and the given arguments.
    //
    //   DeducedArgs:  A specialization of deduced_item<> (see below).
    //                 A list containing only the deduced ParameterSpecs.
    //
    //   TagFn:        A metafunction class used to tag positional or deduced
    //                 arguments with a keyword tag.
    //
    //   Positional:   An mpl::bool_<> specialization indicating if positional
    //                 matching is to be performed.
    //
    //   DeducedSet:   An mpl::set<> containing the keyword tags used so far.
    //
    //   ArgumentPack: The ArgumentPack built so far. This is initially an
    //                 empty_arg_list and is built incrementally.
    template <
        class List
      , class DeducedArgs
      , class TagFn
      , class Positional
      , class DeducedSet
      , class ArgumentPack
      , class Error
    >
    struct make_arg_list_aux
      : mpl::eval_if<
            is_same<List, void_>
          , mpl::identity<mpl::pair<ArgumentPack, Error> >
          , make_arg_list0<
                List
              , DeducedArgs
              , TagFn
              , Positional
              , DeducedSet
              , ArgumentPack
              , Error
            >
        >
    {
    };

    // VC6.5 was choking on the default parameters for make_arg_list_aux,
    // so this just forwards to that adding in the defaults.
    template <
        class List
      , class DeducedArgs
      , class TagFn
      , class EmitErrors = mpl::true_
    >
    struct make_arg_list
      : make_arg_list_aux<
            List
          , DeducedArgs
          , TagFn
          , mpl::true_
          , aux::set0
          , empty_arg_list
          , void_
        >
    {
    };

    // A parameter spec item typelist.
    template <class Spec, class Arg, class Tail = void_>
    struct item
    {
        typedef Spec spec;
#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
        typedef is_const<typename remove_reference<Arg>::type> is_arg_const;
#endif
        typedef Arg arg;
        typedef Tail tail;
    };

    template <class Spec, class Arg, class Tail>
    struct make_item
    {
        typedef item<Spec, Arg, typename Tail::type> type;
    };

    // Creates a item typelist.
    template <class Spec, class Arg, class Tail>
    struct make_items
      : mpl::eval_if<
            is_same<Arg, void_>
          , mpl::identity<void_>
          , make_item<Spec, Arg, Tail>
        >
    {
    };

    // A typelist that stored deduced parameter specs.
    template <class ParameterSpec, class Tail = void_>
    struct deduced_item
    {
        typedef ParameterSpec spec;
        typedef Tail tail;
    };

    // Evaluate Tail and construct deduced_item list.
    template <class Spec, class Tail>
    struct make_deduced_item
    {
        typedef deduced_item<Spec, typename Tail::type> type;
    };

    template <class Spec, class Tail>
    struct make_deduced_items
      : mpl::eval_if<
            is_same<Spec, void_>
          , mpl::identity<void_>
          , mpl::eval_if<
                is_deduced<Spec>
              , make_deduced_item<Spec, Tail>
              , Tail
            >
        >
    {
    };

    struct tag_keyword_arg
    {
        template <class K, class T>
        struct apply : tag<K, T>
        {
        };
    };

    struct tag_template_keyword_arg
    {
        template <class K, class T>
        struct apply
        {
            typedef template_keyword<K, T> type;
        };
    };
} // namespace aux

#if defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
namespace aux {

    // This recursive metafunction forwards successive elements of
    // parameters::parameter_spec to make_deduced_items<>.
    // -- Cromwell D. Enage
    template <class SpecDeque>
    struct make_deduced_list;

    template <class SpecDeque>
    struct make_deduced_list_not_empty
      : make_deduced_items<
            typename mpl::front<SpecDeque>::type
          , make_deduced_list<typename mpl::pop_front<SpecDeque>::type>
        >
    {
    };

    template <class SpecDeque>
    struct make_deduced_list
      : mpl::eval_if<
            mpl::empty<SpecDeque>
          , mpl::identity<void_>
          , make_deduced_list_not_empty<SpecDeque>
        >
    {
    };

    template <class ArgumentPackAndError>
    struct is_arg_pack_error_void
      : is_same<typename mpl::second<ArgumentPackAndError>::type, void_>
    {
    };

    // Checks if the arguments match the criteria of overload resolution.
    template <class ArgumentPackAndError, class SpecDeque>
    struct match_parameters_base_cond;

    // If NamedList satisfies the PS0, PS1, ..., this is a metafunction
    // returning parameters.  Otherwise it has no nested ::type.
    template <class ArgumentPackAndError, class SpecDeque>
    struct match_parameters_base_cond_helper
      : mpl::eval_if<
            satisfies_requirements_of<
                typename mpl::first<ArgumentPackAndError>::type
              , typename mpl::front<SpecDeque>::type
            >
          , match_parameters_base_cond<
                ArgumentPackAndError
              , typename mpl::pop_front<SpecDeque>::type
            >
          , mpl::false_
        >
    {
    };

    template <class ArgumentPackAndError, class SpecDeque>
    struct match_parameters_base_cond
      : mpl::eval_if<
            mpl::empty<SpecDeque>
          , is_arg_pack_error_void<ArgumentPackAndError>
          , match_parameters_base_cond_helper<ArgumentPackAndError, SpecDeque>
        >
    {
    };

    // Helper metafunction for make_parameter_spec_items<>.
    template <class SpecDeque, class ...Args>
    struct make_parameter_spec_items_helper;

    // This parameters item chaining metafunction class does not require
    // the lengths of the SpecDeque and of Args parameter pack to match.
    // Used by argument_pack to build the items in the resulting arg_list.
    // -- Cromwell D. Enage
    template <class SpecDeque, class ...Args>
    struct make_parameter_spec_items
      : mpl::eval_if<
            mpl::empty<SpecDeque>
          , mpl::identity<void_>
          , make_parameter_spec_items_helper<SpecDeque, Args...>
        >
    {
    };

    template <class SpecDeque>
    struct make_parameter_spec_items_helper<SpecDeque> : mpl::identity<void_>
    {
    };

    template <class SpecDeque, class A0, class ...Args>
    struct make_parameter_spec_items_helper<SpecDeque, A0, Args...>
      : make_items<
            typename mpl::front<SpecDeque>::type
          , A0
          , make_parameter_spec_items<
                typename mpl::pop_front<SpecDeque>::type
              , Args...
            >
        >
    {
    };

    // The make_arg_list<> metafunction produces a reversed arg_list,
    // so pass the arguments to the arg_list constructor reversed in turn.
    template <class ArgList, class ...Args>
    struct arg_list_factory;

    // TODO: Reduce template code bloat. -- Cromwell D. Enage
    template <class ArgList>
    struct arg_list_factory<ArgList>
    {
        template <class ...ReversedArgs>
        static inline ArgList reverse(ReversedArgs&&... reversed_args)
        {
            return ArgList(
                typename mpl::if_<
                    is_same<typename ArgList::tagged_arg::value_type, void_>
                  , value_type_is_void
                  , value_type_is_not_void
                >::type()
              , boost::forward<ReversedArgs>(reversed_args)...
            );
        }
    };

    template <class ArgList, class A0, class ...Args>
    struct arg_list_factory<ArgList, A0, Args...>
    {
        template <class ...ReversedArgs>
        static inline ArgList
        reverse(A0&& a0, Args&&... args, ReversedArgs&&... reversed_args)
        {
            return arg_list_factory<ArgList, Args...>::reverse(
                boost::forward<Args>(args)...
              , static_cast<A0&&>(a0)
              , boost::forward<ReversedArgs>(reversed_args)...
            );
        }
    };
} // namespace aux

template <class ...Spec>
struct parameters
{
    typedef fusion::deque<Spec...> parameter_spec;

    typedef typename aux::make_deduced_list<
        parameter_spec
    >::type deduced_list;

    // If the elements of NamedList match the criteria of overload resolution,
    // returns a type which can be constructed from parameters.  Otherwise,
    // this is not a valid metafunction (no nested ::type).
    template <class ArgumentPackAndError>
    struct match_base
      : mpl::if_<
            typename aux::match_parameters_base_cond<
                ArgumentPackAndError, parameter_spec
            >::type
          , mpl::identity<parameters>
          , void_
        >
    {
    };

    // Specializations are to be used as an optional argument
    // to eliminate overloads via SFINAE.
    template <class ...Args>
    struct match
      : match_base<
            typename aux::make_arg_list<
                typename aux::make_parameter_spec_items<
                    parameter_spec, Args...
                >::type
              , deduced_list
              , aux::tag_keyword_arg
              , mpl::false_  // Don't emit errors when doing SFINAE.
            >::type
        >::type
    {
    };

    // Metafunction that returns an ArgumentPack.
    template <class ...Args>
    struct bind
      : mpl::first<
            typename aux::make_arg_list<
                typename aux::make_parameter_spec_items<
                    parameter_spec, Args...
                >::type
              , deduced_list
              , aux::tag_template_keyword_arg
            >::type
        >
    {
    };

    // The function call operator is used to build an arg_list that
    // labels the positional parameters and maintains whatever other
    // tags may have been specified by the caller.
    inline aux::empty_arg_list operator()() const
    {
        return aux::empty_arg_list();
    }

    template <class A0, class ...Args>
    inline typename mpl::first<
        typename aux::make_arg_list<
            typename aux::make_parameter_spec_items<
                parameter_spec, A0, Args...
            >::type
          , deduced_list
          , aux::tag_keyword_arg
        >::type
    >::type
    operator()(A0&& a0, Args&& ...args) const
    {
        typedef typename aux::make_arg_list<
            typename aux::make_parameter_spec_items<
                parameter_spec, A0, Args...
            >::type
          , deduced_list
          , aux::tag_keyword_arg
        >::type list_error_pair;

        typedef typename mpl::first<list_error_pair>::type result_type;
        typedef typename mpl::second<list_error_pair>::type error;
        error();

        return aux::arg_list_factory<result_type, A0, Args...>::reverse(
            boost::forward<A0>(a0)
          , boost::forward<Args>(args)...
        );
    }
};
#else // !defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
template <
    class PS0
  , BOOST_PP_ENUM_SHIFTED(
        BOOST_PARAMETER_MAX_ARITY, BOOST_PARAMETER_template_args, _
    )
>
struct parameters
{
    typedef typename BOOST_PARAMETER_build_deduced_list(
        BOOST_PARAMETER_MAX_ARITY, aux::make_deduced_items, PS
    )::type deduced_list;

    // If the elements of NamedList match the criteria of overload resolution,
    // returns a type which can be constructed from parameters.  Otherwise,
    // this is not a valid metafunction (no nested ::type).
#if !defined(BOOST_NO_SFINAE) && \
    !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x592))
    // If NamedList satisfies the PS0, PS1, ..., this is a metafunction
    // returning parameters.  Otherwise it has no nested ::type.
    template <class ArgumentPackAndError>
    struct match_base
      : mpl::if_<
            // mpl::and_<
            //    aux::satisfies_requirements_of<NamedList,PS0>
            //  , mpl::and_<
            //       aux::satisfies_requirements_of<NamedList,PS1>...
            //           ..., mpl::true_
            // ...> >
            
#define BOOST_PARAMETER_satisfies(z, n, text)                                \
            mpl::and_<                                                       \
                aux::satisfies_requirements_of<                              \
                    typename mpl::first<ArgumentPackAndError>::type          \
                  , BOOST_PP_CAT(PS, n)>                                     \
                  ,
            //!
            mpl::and_<
                is_same<
                    typename mpl::second<ArgumentPackAndError>::type, void_
                >
              , BOOST_PP_REPEAT(
                    BOOST_PARAMETER_MAX_ARITY, BOOST_PARAMETER_satisfies, _
                )
                mpl::true_
                BOOST_PP_REPEAT(
                    BOOST_PARAMETER_MAX_ARITY, BOOST_PARAMETER_right_angle, _
                )
            >

#undef BOOST_PARAMETER_satisfies

          , mpl::identity<parameters>
          , void_
        >
    {
    };
#endif // SFINAE enabled, not Borland.

    // Specializations are to be used as an optional argument
    // to eliminate overloads via SFINAE.
    template <
#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
        // Borland simply can't handle default arguments in member
        // class templates.  People wishing to write portable code can
        // explicitly specify BOOST_PARAMETER_MAX_ARITY arguments.
        BOOST_PP_ENUM_PARAMS(BOOST_PARAMETER_MAX_ARITY, class A)
#else 
        BOOST_PP_ENUM_BINARY_PARAMS(
            BOOST_PARAMETER_MAX_ARITY, class A, = void_ BOOST_PP_INTERCEPT
        )
#endif
    >
    struct match
#if !defined(BOOST_NO_SFINAE) && \
    !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x592))
      : match_base<
            typename aux::make_arg_list<
                typename BOOST_PARAMETER_build_arg_list(
                    BOOST_PARAMETER_MAX_ARITY, aux::make_items, PS, A
                )::type
              , deduced_list
              , aux::tag_keyword_arg
              , mpl::false_  // Don't emit errors when doing SFINAE.
            >::type
        >::type
    {
    };
#else
    {
        typedef parameters<
            BOOST_PP_ENUM_PARAMS(BOOST_PARAMETER_MAX_ARITY, PS)
        > type;
    };
#endif // SFINAE enabled, not Borland.

    // Metafunction that returns an ArgumentPack.

    // TODO, bind has to instantiate the error type in the result
    // of make_arg_list.

    template <
#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
        // Borland simply can't handle default arguments in member
        // class templates.  People wishing to write portable code can
        // explicitly specify BOOST_PARAMETER_MAX_ARITY arguments
        BOOST_PP_ENUM_PARAMS(BOOST_PARAMETER_MAX_ARITY, class A)
#else
        BOOST_PP_ENUM_BINARY_PARAMS(
            BOOST_PARAMETER_MAX_ARITY, class A, = void_ BOOST_PP_INTERCEPT
        )
#endif
    >
    struct bind
    {
        typedef typename aux::make_arg_list<
            typename BOOST_PARAMETER_build_arg_list(
                BOOST_PARAMETER_MAX_ARITY, aux::make_items, PS, A
            )::type
          , deduced_list
          , aux::tag_template_keyword_arg
        >::type result;

        typedef typename mpl::first<result>::type type;
    };

    BOOST_PP_REPEAT(
        BOOST_PARAMETER_MAX_ARITY
      , BOOST_PARAMETER_forward_typedef
      , (PS)(parameter_spec)
    )

    // The function call operator is used to build an arg_list that
    // labels the positional parameters and maintains whatever other
    // tags may have been specified by the caller.
    //
    // !!!NOTE!!!
    //
    // The make_arg_list<> metafunction produces a reversed arg_list,
    // so pass the arguments to the arg_list constructor reversed in turn.
    inline aux::empty_arg_list operator()() const
    {
        return aux::empty_arg_list();
    }

    BOOST_PP_FOR(
        (0)
      , BOOST_PARAMETER_function_call_op_pred_7
      , BOOST_PARAMETER_function_call_op_inc
      , BOOST_PARAMETER_function_call_op_overload
    )
#if 6 < BOOST_PARAMETER_MAX_ARITY
    BOOST_PP_FOR(
        (0)(0)(0)(0)(0)(0)(0)
      , BOOST_PARAMETER_function_call_op_pred_8
      , BOOST_PARAMETER_function_call_op_inc
      , BOOST_PARAMETER_function_call_op_overload
    )
#endif
#if 7 < BOOST_PARAMETER_MAX_ARITY
    BOOST_PP_FOR(
        (0)(0)(0)(0)(0)(0)(0)(0)
      , BOOST_PARAMETER_function_call_op_pred_00000001
      , BOOST_PARAMETER_function_call_op_inc
      , BOOST_PARAMETER_function_call_op_overload
    )
    BOOST_PP_FOR(
        (0)(0)(0)(0)(0)(0)(0)(1)
      , BOOST_PARAMETER_function_call_op_pred_9
      , BOOST_PARAMETER_function_call_op_inc
      , BOOST_PARAMETER_function_call_op_overload
    )
#endif
#if 8 < BOOST_PARAMETER_MAX_ARITY
#define BOOST_PP_ITERATION_PARAMS_1 ( \
    3, (9, BOOST_PARAMETER_MAX_ARITY, <boost/parameter/aux_/overloads.hpp>) \
)
#include BOOST_PP_ITERATE()
#endif
};
#endif // BOOST_PARAMETER_HAS_PERFECT_FORWARDING

} // namespace parameter

} // namespace boost

#if !defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
#undef BOOST_PARAMETER_function_call_op_overload
#undef BOOST_PARAMETER_function_call_op_inc
#undef BOOST_PARAMETER_function_call_op_pred_9
#undef BOOST_PARAMETER_function_call_op_pred_00000001
#undef BOOST_PARAMETER_function_call_op_pred_8
#undef BOOST_PARAMETER_function_call_op_pred_7
#undef BOOST_PARAMETER_function_call_arg_pack_init
#undef BOOST_PARAMETER_function_call_arg_list
#undef BOOST_PARAMETER_template_args
#undef BOOST_PARAMETER_forward_typedef
//#undef BOOST_PARAMETER_right_angle
#undef BOOST_PARAMETER_make_arg_items
//#undef BOOST_PARAMETER_build_deduced_list
//#undef BOOST_PARAMETER_make_deduced_list
//#undef BOOST_PARAMETER_build_arg_list
//#undef BOOST_PARAMETER_make_arg_list
#endif

#endif // BOOST_PARAMETERS_031014_HPP

