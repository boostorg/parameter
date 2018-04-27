// Copyright David Abrahams, Daniel Wallin 2003.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETERS_031014_HPP
#define BOOST_PARAMETERS_031014_HPP

namespace boost {

    template <typename T>
    class reference_wrapper;
} // namespace boost

namespace boost { namespace parameter { namespace aux {

    struct use_default
    {
    };

    // Tag type passed to MPL lambda.
    struct lambda_tag;
}}} // namespace boost::parameter::aux

namespace boost { namespace parameter {

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
    template <
        typename Tag
      , typename Predicate = ::boost::parameter::aux::use_default
    >
    struct required
    {
        typedef Tag key_type;
        typedef Predicate predicate;
    };

    template <
        typename Tag
      , typename Predicate = ::boost::parameter::aux::use_default
    >
    struct optional
    {
        typedef Tag key_type;
        typedef Predicate predicate;
    };

    template <typename Tag>
    struct deduced
    {
        typedef Tag key_type;
    };
}} // namespace boost::parameter

#include <boost/mpl/bool.hpp>

namespace boost { namespace parameter { namespace aux {

    template <typename T>
    struct is_required : ::boost::mpl::false_
    {
    };

    template <typename Tag, typename Predicate>
    struct is_required< ::boost::parameter::required<Tag,Predicate> >
      : ::boost::mpl::true_
    {
    };

    template <typename T>
    struct is_optional : ::boost::mpl::false_
    {
    };

    template <typename Tag, typename Predicate>
    struct is_optional< ::boost::parameter::optional<Tag,Predicate> >
      : ::boost::mpl::true_
    {
    };

    template <typename T>
    struct is_deduced_aux : ::boost::mpl::false_
    {
    };

    template <typename Tag>
    struct is_deduced_aux< ::boost::parameter::deduced<Tag> >
      : ::boost::mpl::true_
    {
    };

    template <typename T>
    struct is_deduced0
      : ::boost::parameter::aux::is_deduced_aux<typename T::key_type>::type
    {
    };
}}} // namespace boost::parameter::aux

#include <boost/mpl/if.hpp>

namespace boost { namespace parameter { namespace aux {

    //
    // tag_type, has_default, and predicate --
    //
    // These metafunctions accept a ParameterSpec and extract the
    // keyword tag, whether or not a default is supplied for the
    // parameter, and the predicate that the corresponding actual
    // argument type is required match.
    //
    // a ParameterSpec is a specialization of either keyword<...>,
    // required<...>, optional<...>
    //

    template <typename T>
    struct has_default
      : ::boost::mpl::if_<
            ::boost::parameter::aux::is_required<T>
          , ::boost::mpl::false_
          , ::boost::mpl::true_
        >::type
    {
    };

    template <typename T>
    struct is_deduced
      : ::boost::mpl::if_<
            typename ::boost::mpl::if_<
                ::boost::parameter::aux::is_optional<T>
              , ::boost::mpl::true_
              , ::boost::parameter::aux::is_required<T>
            >::type
          , ::boost::parameter::aux::is_deduced0<T>
          , ::boost::mpl::false_
        >::type
    {
    };
}}} // namespace boost::parameter::aux

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>

namespace boost { namespace parameter { namespace aux {

    // helper for tag_type<...>, below.
    template <typename T>
    struct get_tag_type0
    {
        typedef typename T::key_type type;
    };

    template <typename T>
    struct get_tag_type
      : ::boost::mpl::eval_if<
            ::boost::parameter::aux::is_deduced_aux<typename T::key_type>
          , ::boost::parameter::aux::get_tag_type0<typename T::key_type>
          , ::boost::mpl::identity<typename T::key_type>
        >
    {
    };

    template <typename T>
    struct tag_type
      : ::boost::mpl::eval_if<
            typename ::boost::mpl::if_<
                ::boost::parameter::aux::is_optional<T>
              , ::boost::mpl::true_
              , ::boost::parameter::aux::is_required<T>
            >::type
          , ::boost::parameter::aux::get_tag_type<T>
          , ::boost::mpl::identity<T>
        >
    {
    };
}}} // namespace boost::parameter::aux

#include <boost/mpl/always.hpp>

namespace boost { namespace parameter { namespace aux {

    // helper for get_predicate<...>, below
    template <typename T>
    struct get_predicate_or_default
    {
        typedef T type;
    };

    template <>
    struct get_predicate_or_default< ::boost::parameter::aux::use_default>
    {
        typedef ::boost::mpl::always< ::boost::mpl::true_> type;
    };

    // helper for predicate<...>, below
    template <typename T>
    struct get_predicate
      : ::boost::parameter::aux::get_predicate_or_default<
            typename T::predicate
        >
    {
    };

    template <typename T>
    struct predicate
      : ::boost::mpl::eval_if<
            typename ::boost::mpl::if_<
                ::boost::parameter::aux::is_optional<T>
              , ::boost::mpl::true_
              , ::boost::parameter::aux::is_required<T>
            >::type
          , ::boost::parameter::aux::get_predicate<T>
          , ::boost::mpl::identity<
                ::boost::mpl::always< ::boost::mpl::true_>
            >
        >
    {
    };
}}} // namespace boost::parameter::aux

#include <boost/parameter/aux_/parameter_requirements.hpp>

namespace boost { namespace parameter { namespace aux {

    // Converts a ParameterSpec into a specialization of
    // parameter_requirements.  We need to do this in order to get the
    // tag_type into the type in a way that can be conveniently matched
    // by a satisfies(...) member function in arg_list.
    template <typename ParameterSpec>
    struct as_parameter_requirements
    {
        typedef ::boost::parameter::aux::parameter_requirements<
            typename ::boost::parameter::aux::tag_type<ParameterSpec>::type
          , typename ::boost::parameter::aux::predicate<ParameterSpec>::type
          , ::boost::parameter::aux::has_default<ParameterSpec>
        > type;
    };
}}} // namespace boost::parameter::aux

#include <boost/parameter/aux_/tag.hpp>

namespace boost { namespace parameter { namespace aux {

    struct tag_keyword_arg
    {
        template <typename K, typename T>
        struct apply : ::boost::parameter::aux::tag<K,T>
        {
        };
    };
}}} // namespace boost::parameter::aux

#include <boost/parameter/aux_/template_keyword.hpp>

namespace boost { namespace parameter { namespace aux {

    struct tag_template_keyword_arg
    {
        template <typename K, typename T>
        struct apply
        {
            typedef ::boost::parameter::template_keyword<K,T> type;
        };
    };
}}} // namespace boost::parameter::aux

#include <boost/parameter/aux_/is_tagged_argument.hpp>

namespace boost { namespace parameter { namespace aux {

    template <typename T>
    struct is_named_argument
      : ::boost::mpl::if_<
            ::boost::parameter::aux::is_template_keyword<T>
          , ::boost::mpl::true_
          , ::boost::parameter::aux::is_tagged_argument<T>
        >::type
    {
    };
}}} // namespace boost::parameter::aux

#include <boost/parameter/aux_/augment_predicate.hpp>
#include <boost/parameter/aux_/yesno.hpp>
#include <boost/parameter/aux_/void.hpp>
#include <boost/parameter/config.hpp>
#include <boost/mpl/apply_wrap.hpp>

#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
#include <boost/type_traits/is_same.hpp>
#else
#include <type_traits>
#endif

#if !BOOST_WORKAROUND(BOOST_MSVC, == 1310)
#include <boost/tti/detail/dnullptr.hpp>
#endif

namespace boost { namespace parameter { namespace aux {

    // Returns mpl::true_ iff the given ParameterRequirements are satisfied by
    // ArgList.
    template <typename ArgList, typename ParameterRequirements>
    struct satisfies
    {
#if BOOST_WORKAROUND(BOOST_MSVC, == 1310)
        // VC7.1 can't handle the sizeof() implementation below,
        // so we use this instead.
        typedef typename ::boost::mpl::apply_wrap3<
            typename ArgList::binding
          , typename ParameterRequirements::keyword
          , ::boost::parameter::void_
          , ::boost::mpl::false_
        >::type bound;

        typedef typename ::boost::mpl::eval_if<
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
            ::boost::is_same<bound,::boost::parameter::void_>
#else
            ::std::is_same<bound,::boost::parameter::void_>
#endif
          , typename ParameterRequirements::has_default
          , ::boost::mpl::apply_wrap2<
                ::boost::parameter::aux::augment_predicate<
                    typename ParameterRequirements::predicate
                  , typename ArgList::reference
                  , typename ArgList::key_type
                >
              , bound
              , ArgList
            >
        >::type type;
#else // !BOOST_WORKAROUND(BOOST_MSVC, == 1310)
        BOOST_STATIC_CONSTANT(
            bool, value = (
                sizeof(
                    ::boost::parameter::aux::to_yesno(
                        ArgList::satisfies(
                            static_cast<ParameterRequirements*>(
                                BOOST_TTI_DETAIL_NULLPTR
                            )
                          , static_cast<ArgList*>(BOOST_TTI_DETAIL_NULLPTR)
                        )
                    )
                ) == sizeof(::boost::parameter::aux::yes_tag)
            )
        );

        typedef ::boost::mpl::bool_<satisfies::value> type;
#endif // MSVC 7.1 workarounds needed.
    };

    // Returns mpl::true_ if the requirements of the given ParameterSpec
    // are satisfied by ArgList.
    template <typename ArgList, typename ParameterSpec>
    struct satisfies_requirements_of
      : ::boost::parameter::aux::satisfies<
            ArgList
          , typename ::boost::parameter::aux::as_parameter_requirements<
                ParameterSpec
            >::type
        >
    {
    };
}}} // namespace boost::parameter::aux

#include <boost/mpl/assert.hpp>

namespace parameter_ {

    template <typename T>
    struct unmatched_argument
    {
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
        BOOST_MPL_ASSERT((::boost::is_same<T,void>));
#else
        BOOST_MPL_ASSERT((
            typename ::boost::mpl::if_<
                ::std::is_same<T,void>
              , ::boost::mpl::true_
              , ::boost::mpl::false_
            >::type
        ));
#endif
        typedef int type;
    }; 
} // namespace parameter_

#include <boost/parameter/aux_/set.hpp>

namespace boost { namespace parameter { namespace aux {

    // Inserts Tagged::key_type into the UserArgs set.
    // Extra indirection to lazily evaluate Tagged::key_type.
    template <typename UsedArgs, typename Tagged>
    struct insert_tagged
      : ::boost::parameter::aux::insert_<UsedArgs,typename Tagged::key_type>
    {
    };
}}} // namespace boost::parameter::aux

#include <boost/mpl/pair.hpp>

namespace boost { namespace parameter { namespace aux {

    // Tags a deduced argument Arg with the keyword tag of Spec using TagFn.
    // Returns the tagged argument and the mpl::set<> UsedArgs with the
    // tag of Spec inserted.
    template <typename UsedArgs, typename Spec, typename Arg, typename TagFn>
    struct tag_deduced
    {
        typedef ::boost::mpl::pair<
            typename ::boost::mpl::apply_wrap2<
                TagFn
              , typename ::boost::parameter::aux::tag_type<Spec>::type
              , Arg
            >::type
          , typename ::boost::parameter::aux::insert_<
                UsedArgs
              , typename ::boost::parameter::aux::tag_type<Spec>::type
            >::type
        > type;
    };

    template <
        typename Argument
      , typename ArgumentPack
      , typename DeducedArgs
      , typename UsedArgs
      , typename TagFn
    >
    struct deduce_tag;

    // Helper for deduce_tag<...>, below.
    template <
        typename Argument
      , typename ArgumentPack
      , typename DeducedArgs
      , typename UsedArgs
      , typename TagFn
    >
    struct deduce_tag0
    {
        typedef typename DeducedArgs::spec spec;

        typedef typename ::boost::mpl::apply_wrap2<
            typename ::boost::mpl::lambda<
                typename spec::predicate
              , ::boost::parameter::aux::lambda_tag
            >::type
          , Argument
          , ArgumentPack
        >::type condition;

        // Deduced parameter matches several arguments.
        BOOST_MPL_ASSERT((
            typename ::boost::mpl::eval_if<
                typename ::boost::parameter::aux::has_key_<
                    UsedArgs
                  , typename ::boost::parameter::aux::tag_type<spec>::type
                >::type
              , ::boost::mpl::if_<
                    condition
                  , ::boost::mpl::false_
                  , ::boost::mpl::true_
                >
              , ::boost::mpl::true_
            >::type
        ));

        typedef typename ::boost::mpl::eval_if<
            condition
          , ::boost::parameter::aux::tag_deduced<UsedArgs,spec,Argument,TagFn>
          , ::boost::parameter::aux::deduce_tag<
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
        typename Argument
      , typename ArgumentPack
      , typename DeducedArgs
      , typename UsedArgs
      , typename TagFn
    >
    struct deduce_tag
      : ::boost::mpl::eval_if<
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
            ::boost::is_same<DeducedArgs,::boost::parameter::void_>
#else
            ::std::is_same<DeducedArgs,::boost::parameter::void_>
#endif
          , ::boost::mpl::pair< ::boost::parameter::void_,UsedArgs>
          , ::boost::parameter::aux::deduce_tag0<
                Argument
              , ArgumentPack
              , DeducedArgs
              , UsedArgs
              , TagFn
            >
        >
    {
    };
}}} // namespace boost::parameter::aux

#include <boost/parameter/aux_/arg_list.hpp>

#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/remove_reference.hpp>
#endif

namespace boost { namespace parameter { namespace aux {

    template <
        typename List
      , typename DeducedArgs
      , typename TagFn
      , typename Positional
      , typename UsedArgs
      , typename ArgumentPack
      , typename Error
    >
    struct make_arg_list_aux;

    // Borland needs the insane extra-indirection workaround below so that
    // it doesn't magically drop the const qualifier from the argument type.
    template <
        typename List
      , typename DeducedArgs
      , typename TagFn
      , typename Positional
      , typename UsedArgs
      , typename ArgumentPack
#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
      , typename argument
#endif
      , typename Error
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
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
        typedef typename ::boost::remove_const<
            typename ::boost::remove_reference<argument>::type
        >::type arg_type;
#else
        typedef typename ::std::remove_const<
            typename ::std::remove_reference<argument>::type
        >::type arg_type;
#endif
        typedef typename List::spec parameter_spec;
        typedef typename ::boost::parameter::aux::tag_type<
            parameter_spec
        >::type tag_;
        typedef ::boost::parameter::aux::is_named_argument<
            argument
        > is_tagged;

        // If this argument is either explicitly tagged or a deduced
        // parameter, then turn off positional matching.
        typedef typename ::boost::mpl::eval_if<
            Positional
          , ::boost::mpl::eval_if<
                ::boost::parameter::aux::is_deduced<parameter_spec>
              , ::boost::mpl::false_
              , ::boost::mpl::if_<
                    is_tagged
                  , ::boost::mpl::false_
                  , ::boost::mpl::true_
                >
            >
          , ::boost::mpl::false_
        >::type positional;

        // If this parameter is explicitly tagged, then add it to the
        // used-parmeters set.  We only really need to add parameters
        // that are deduced, but we would need a way to check if
        // a given tag corresponds to a deduced parameter spec.
        typedef typename ::boost::mpl::eval_if<
            is_tagged
          , ::boost::parameter::aux::insert_tagged<UsedArgs,arg_type>
          , ::boost::mpl::identity<UsedArgs>
        >::type used_args;

        // If this parameter is neither explicitly tagged nor positionally
        // matched, then deduce the tag from the deduced parameter specs.
        typedef typename ::boost::mpl::eval_if<
            typename ::boost::mpl::if_<
                is_tagged
              , ::boost::mpl::true_
              , positional
            >::type
          , ::boost::mpl::pair< ::boost::parameter::void_,used_args>
          , ::boost::parameter::aux::deduce_tag<
                argument
              , ArgumentPack
              , DeducedArgs
              , used_args
              , TagFn
            >
        >::type deduced_data;

        // If this parameter is explicitly tagged ...
        typedef typename ::boost::mpl::eval_if<
            is_tagged
            // ... just use it
          , ::boost::mpl::identity<arg_type>
            // ... else ...
          , ::boost::mpl::eval_if<
                // if positional matching is turned on ...
                positional
                // ... tag it positionally
              , ::boost::mpl::apply_wrap2<TagFn,tag_,argument>
                // ... else, use the deduced tag
              , ::boost::mpl::first<deduced_data>
            >
        >::type tagged;

        // Build the arg_list incrementally, prepending new nodes.
        typedef typename ::boost::mpl::if_<
            typename ::boost::mpl::if_<
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
                ::boost::is_same<Error,::boost::parameter::void_>
              , ::boost::is_same<tagged,::boost::parameter::void_>
#else
                ::std::is_same<Error,::boost::parameter::void_>
              , ::std::is_same<tagged,::boost::parameter::void_>
#endif
              , ::boost::mpl::false_
            >::type
          , ::parameter_::unmatched_argument<argument>
          , ::boost::parameter::void_
        >::type error;

        typedef typename ::boost::mpl::if_<
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
            ::boost::is_same<tagged,::boost::parameter::void_>
#else
            ::std::is_same<tagged,::boost::parameter::void_>
#endif
          , ArgumentPack
          , ::boost::parameter::aux::arg_list<tagged,ArgumentPack>
        >::type argument_pack;

        typedef typename ::boost::parameter::aux::make_arg_list_aux<
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
        typename List
      , typename DeducedArgs
      , typename TagFn
      , typename Positional
      , typename UsedArgs
      , typename ArgumentPack
      , typename Error
    >
    struct make_arg_list0
    {
        typedef typename ::boost::mpl::eval_if<
            typename List::is_arg_const
          , ::boost::parameter::aux::make_arg_list00<
                List
              , DeducedArgs
              , TagFn
              , Positional
              , UsedArgs
              , ArgumentPack
              , typename List::arg const
              , Error
            >
          , ::boost::parameter::aux::make_arg_list00<
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
        typename List
      , typename DeducedArgs
      , typename TagFn
      , typename Positional
      , typename DeducedSet
      , typename ArgumentPack
      , typename Error
    >
    struct make_arg_list_aux
      : ::boost::mpl::eval_if<
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
            ::boost::is_same<List,::boost::parameter::void_>
#else
            ::std::is_same<List,::boost::parameter::void_>
#endif
          , ::boost::mpl::identity< ::boost::mpl::pair<ArgumentPack,Error> >
          , ::boost::parameter::aux::make_arg_list0<
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
        typename List
      , typename DeducedArgs
      , typename TagFn
      , typename EmitErrors = ::boost::mpl::true_
    >
    struct make_arg_list
      : ::boost::parameter::aux::make_arg_list_aux<
            List
          , DeducedArgs
          , TagFn
          , ::boost::mpl::true_
          , ::boost::parameter::aux::set0
          , ::boost::parameter::aux::empty_arg_list
          , ::boost::parameter::void_
        >
    {
    };
}}} // namespace boost::parameter::aux

#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
#include <boost/type_traits/is_const.hpp>
#endif
#endif

namespace boost { namespace parameter { namespace aux {

    // A parameter spec item typelist.
    template <
        typename Spec
      , typename Arg
      , typename Tail = ::boost::parameter::void_
    >
    struct item
    {
        typedef Spec spec;
#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
        typedef ::boost::is_const<
            typename ::boost::remove_reference<Arg>::type
        > is_arg_const;
#else
        typedef ::std::is_const<
            typename ::std::remove_reference<Arg>::type
        > is_arg_const;
#endif
#endif
        typedef Arg arg;
        typedef Tail tail;
    };

    template <typename Spec, typename Arg, typename Tail>
    struct make_item
    {
        typedef boost::parameter::aux::item<
            Spec
          , Arg
          , typename Tail::type
        > type;
    };

    // Creates a item typelist.
    template <typename Spec, typename Arg, typename Tail>
    struct make_items
      : ::boost::mpl::eval_if<
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
            ::boost::is_same<Arg,::boost::parameter::void_>
#else
            ::std::is_same<Arg,::boost::parameter::void_>
#endif
          , ::boost::mpl::identity< ::boost::parameter::void_>
          , ::boost::parameter::aux::make_item<Spec,Arg,Tail>
        >
    {
    };

    // A typelist that stored deduced parameter specs.
    template <
        typename ParameterSpec
      , typename Tail = ::boost::parameter::void_
    >
    struct deduced_item
    {
        typedef ParameterSpec spec;
        typedef Tail tail;
    };

    // Evaluate Tail and construct deduced_item list.
    template <typename Spec, typename Tail>
    struct make_deduced_item
    {
        typedef ::boost::parameter::aux::deduced_item<
            Spec
          , typename Tail::type
        > type;
    };

    template <typename Spec, typename Tail>
    struct make_deduced_items
      : ::boost::mpl::eval_if<
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
            ::boost::is_same<Spec,::boost::parameter::void_>
#else
            ::std::is_same<Spec,::boost::parameter::void_>
#endif
          , ::boost::mpl::identity< ::boost::parameter::void_>
          , ::boost::mpl::eval_if<
                ::boost::parameter::aux::is_deduced<Spec>
              , ::boost::parameter::aux::make_deduced_item<Spec,Tail>
              , Tail
            >
        >
    {
    };
}}} // namespace boost::parameter::aux

#if defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)

namespace boost { namespace parameter { namespace aux {

    // This recursive metafunction forwards successive elements of
    // parameters::parameter_spec to make_deduced_items<>.
    // -- Cromwell D. Enage
    template <typename SpecDeque>
    struct make_deduced_list;
}}} // namespace boost::parameter::aux

#include <boost/mpl/front.hpp>
#include <boost/mpl/pop_front.hpp>

namespace boost { namespace parameter { namespace aux {

    template <typename SpecDeque>
    struct make_deduced_list_not_empty
      : ::boost::parameter::aux::make_deduced_items<
            typename boost::mpl::front<SpecDeque>::type
          , ::boost::parameter::aux::make_deduced_list<
                typename boost::mpl::pop_front<SpecDeque>::type
            >
        >
    {
    };
}}} // namespace boost::parameter::aux

#include <boost/mpl/empty.hpp>

namespace boost { namespace parameter { namespace aux {

    template <typename SpecDeque>
    struct make_deduced_list
      : ::boost::mpl::eval_if<
            ::boost::mpl::empty<SpecDeque>
          , ::boost::mpl::identity< ::boost::parameter::void_>
          , ::boost::parameter::aux::make_deduced_list_not_empty<SpecDeque>
        >
    {
    };

    template <typename ArgumentPackAndError>
    struct is_arg_pack_error_void
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
      : ::boost::is_same<
#else
      : ::boost::mpl::if_<
            ::std::is_same<
#endif
                typename ::boost::mpl::second<ArgumentPackAndError>::type
              , ::boost::parameter::void_
            >
#if !defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
          , ::boost::mpl::true_
          , ::boost::mpl::false_
        >::type
#endif
    {
    };

    // Checks if the arguments match the criteria of overload resolution.
    template <typename ArgumentPackAndError, typename SpecDeque>
    struct match_parameters_base_cond;

    // Helper for match_parameters_base_cond<...>, below.
    template <typename ArgumentPackAndError, typename SpecDeque>
    struct match_parameters_base_cond_helper
      : ::boost::mpl::eval_if<
            ::boost::parameter::aux::satisfies_requirements_of<
                typename ::boost::mpl::first<ArgumentPackAndError>::type
              , typename ::boost::mpl::front<SpecDeque>::type
            >
          , ::boost::parameter::aux::match_parameters_base_cond<
                ArgumentPackAndError
              , typename ::boost::mpl::pop_front<SpecDeque>::type
            >
          , ::boost::mpl::false_
        >
    {
    };

    // If NamedList satisfies the PS0, PS1, ..., this is a metafunction
    // returning parameters.  Otherwise it has no nested ::type.
    template <typename ArgumentPackAndError, typename SpecDeque>
    struct match_parameters_base_cond
      : ::boost::mpl::eval_if<
            ::boost::mpl::empty<SpecDeque>
          , ::boost::parameter::aux::is_arg_pack_error_void<
                ArgumentPackAndError
            >
          , ::boost::parameter::aux::match_parameters_base_cond_helper<
                ArgumentPackAndError
              , SpecDeque
            >
        >
    {
    };

    // Helper metafunction for make_parameter_spec_items<...>, below.
    template <typename SpecDeque, typename ...Args>
    struct make_parameter_spec_items_helper;

    // This parameters item chaining metafunction class does not require
    // the lengths of the SpecDeque and of Args parameter pack to match.
    // Used by argument_pack to build the items in the resulting arg_list.
    // -- Cromwell D. Enage
    template <typename SpecDeque, typename ...Args>
    struct make_parameter_spec_items
      : ::boost::mpl::eval_if<
            ::boost::mpl::empty<SpecDeque>
          , ::boost::mpl::identity< ::boost::parameter::void_>
          , ::boost::parameter::aux::make_parameter_spec_items_helper<
                SpecDeque
              , Args...
            >
        >
    {
    };

    template <typename SpecDeque>
    struct make_parameter_spec_items_helper<SpecDeque>
      : ::boost::mpl::identity< ::boost::parameter::void_>
    {
    };

    template <typename SpecDeque, typename A0, typename ...Args>
    struct make_parameter_spec_items_helper<SpecDeque,A0,Args...>
      : ::boost::parameter::aux::make_items<
            typename ::boost::mpl::front<SpecDeque>::type
          , A0
          , ::boost::parameter::aux::make_parameter_spec_items<
                typename ::boost::mpl::pop_front<SpecDeque>::type
              , Args...
            >
        >
    {
    };
}}} // namespace boost::parameter::aux

#include <boost/move/utility_core.hpp>

namespace boost { namespace parameter { namespace aux {

    // The make_arg_list<> metafunction produces a reversed arg_list,
    // so pass the arguments to the arg_list constructor reversed in turn.
    template <typename ArgList, typename ...Args>
    struct arg_list_factory;

    // TODO: Reduce template code bloat. -- Cromwell D. Enage
    template <typename ArgList>
    struct arg_list_factory<ArgList>
    {
        template <typename ...ReversedArgs>
        static inline ArgList reverse(ReversedArgs&&... reversed_args)
        {
            return ArgList(
                typename ::boost::mpl::if_<
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
                    ::boost::is_same<
#else
                    ::std::is_same<
#endif
                        typename ArgList::tagged_arg::value_type
                      , ::boost::parameter::void_
                    >
                  , ::boost::parameter::aux::value_type_is_void
                  , ::boost::parameter::aux::value_type_is_not_void
                >::type()
              , ::boost::forward<ReversedArgs>(reversed_args)...
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
            return ::boost::parameter::aux::arg_list_factory<
                ArgList
              , Args...
            >::reverse(
                ::boost::forward<Args>(args)...
              , ::boost::forward<A0>(a0)
              , ::boost::forward<ReversedArgs>(reversed_args)...
            );
        }
    };
}}} // namespace boost::parameter::aux

#include <boost/fusion/container/deque.hpp>
#include <boost/fusion/mpl.hpp>

namespace boost { namespace parameter {

    template <typename ...Spec>
    struct parameters
    {
        typedef ::boost::fusion::deque<Spec...> parameter_spec;

        typedef typename ::boost::parameter::aux::make_deduced_list<
            parameter_spec
        >::type deduced_list;

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
              , ::boost::mpl::identity<parameters>
              , ::boost::parameter::void_
            >
        {
        };

        // Specializations are to be used as an optional argument
        // to eliminate overloads via SFINAE.
        template <typename ...Args>
        struct match
          : match_base<
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

            typedef typename ::boost::mpl::first<
                list_error_pair
            >::type result_type;

            typedef typename ::boost::mpl::second<
                list_error_pair
            >::type error;

            error();

            return ::boost::parameter::aux::arg_list_factory<
                result_type
              , A0
              , Args...
            >::reverse(
                ::boost::forward<A0>(a0)
              , ::boost::forward<Args>(args)...
            );
        }
    };
}} // namespace boost::parameter

#else // !defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)

#define BOOST_PARAMETER_right_angle(z, n, _) >
/**/

#include <boost/preprocessor/seq/elem.hpp>
#include <boost/preprocessor/cat.hpp>

// Generates:
//
//   make<
//       parameter_spec##0, argument_type##0
//     , make<
//           parameter_spec##1, argument_type##1
//         , ... boost::mpl::identity<boost::parameter::aux::empty_arg_list>
//    ...>
//   >
#define BOOST_PARAMETER_make_arg_list(z, n, names)                           \
    BOOST_PP_SEQ_ELEM(0, names)<                                             \
        BOOST_PP_CAT(BOOST_PP_SEQ_ELEM(1, names), n),                        \
        BOOST_PP_CAT(BOOST_PP_SEQ_ELEM(2, names), n),
/**/

#include <boost/preprocessor/repetition/repeat.hpp>

#define BOOST_PARAMETER_build_arg_list(n, make, param_spec, arg_type)        \
    BOOST_PP_REPEAT(                                                         \
        n, BOOST_PARAMETER_make_arg_list, (make)(param_spec)(arg_type)       \
    )                                                                        \
    ::boost::mpl::identity< ::boost::parameter::void_>                       \
    BOOST_PP_REPEAT(n, BOOST_PARAMETER_right_angle, _)
/**/

#define BOOST_PARAMETER_make_deduced_list(z, n, names)                       \
    BOOST_PP_SEQ_ELEM(0, names)<BOOST_PP_CAT(BOOST_PP_SEQ_ELEM(1, names), n),
/**/

#define BOOST_PARAMETER_build_deduced_list(n, make, parameter_spec)          \
    BOOST_PP_REPEAT(                                                         \
        n, BOOST_PARAMETER_make_deduced_list, (make)(parameter_spec)         \
    )                                                                        \
    ::boost::mpl::identity< ::boost::parameter::void_>                       \
    BOOST_PP_REPEAT(n, BOOST_PARAMETER_right_angle, _)
/**/

#define BOOST_PARAMETER_forward_typedef(z, n, names)                         \
    typedef BOOST_PP_CAT(BOOST_PP_SEQ_ELEM(0, names), n)                     \
    BOOST_PP_CAT(BOOST_PP_SEQ_ELEM(1, names), n);
/**/

#define BOOST_PARAMETER_template_args(z, n, prefix)                          \
    class BOOST_PP_CAT(prefix, n) = ::boost::parameter::void_
/**/

#define BOOST_PARAMETER_make_arg_items_Z(z, n, prefix)                       \
    ::boost::parameter::aux::item<                                           \
        BOOST_PP_CAT(prefix, n), BOOST_PP_CAT(A, n) const&,
/**/

#include <boost/preprocessor/control/iif.hpp>

#define BOOST_PARAMETER_make_arg_items_R(r, prefix, i, elem)                 \
    ::boost::parameter::aux::item<BOOST_PP_CAT(prefix, i),                   \
    BOOST_PP_CAT(A, i) BOOST_PP_IIF(elem, &, const&),
/**/

#define BOOST_PARAMETER_function_call_arg_list_Z(z, n, prefix)               \
    ::boost::parameter::aux::make_arg_list<                                  \
        BOOST_PP_CAT(BOOST_PP_REPEAT_, z)(                                   \
            n, BOOST_PARAMETER_make_arg_items_Z, prefix                      \
        )                                                                    \
        ::boost::parameter::void_                                            \
        BOOST_PP_CAT(BOOST_PP_REPEAT_, z)(n, BOOST_PARAMETER_right_angle, _) \
      , deduced_list                                                         \
      , ::boost::parameter::aux::tag_keyword_arg                             \
    >
/**/

#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/seq/size.hpp>

#define BOOST_PARAMETER_function_call_arg_list_R(r, seq)                     \
    ::boost::parameter::aux::make_arg_list<                                  \
        BOOST_PP_SEQ_FOR_EACH_I_R(                                           \
            r, BOOST_PARAMETER_make_arg_items_R, PS, seq                     \
        )                                                                    \
        ::boost::parameter::void_                                            \
        BOOST_PP_REPEAT(                                                     \
            BOOST_PP_SEQ_SIZE(seq), BOOST_PARAMETER_right_angle, _           \
        )                                                                    \
      , deduced_list                                                         \
      , ::boost::parameter::aux::tag_keyword_arg                             \
    >
/**/

#include <boost/preprocessor/arithmetic/sub.hpp>

#define BOOST_PARAMETER_function_call_arg_pack_init(z, n, limit)             \
    BOOST_PP_CAT(a, BOOST_PP_SUB(limit, n))
/**/

#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>

#define BOOST_PARAMETER_function_call_op_overload_Z(z, n, prefix)            \
    template <BOOST_PP_ENUM_PARAMS_Z(z, n, typename A)>                      \
    inline typename ::boost::mpl::first<                                     \
        typename BOOST_PARAMETER_function_call_arg_list_Z(                   \
            z, n, prefix                                                     \
        )::type                                                              \
    >::type                                                                  \
        operator()(                                                          \
            BOOST_PP_ENUM_BINARY_PARAMS_Z(z, n, A, const& a)                 \
        ) const                                                              \
    {                                                                        \
        typedef typename BOOST_PARAMETER_function_call_arg_list_Z(           \
            z, n, prefix                                                     \
        )::type result;                                                      \
        typedef typename ::boost::mpl::first<result>::type result_type;      \
        typedef typename ::boost::mpl::second<result>::type error;           \
        error();                                                             \
        return result_type(                                                  \
            BOOST_PP_CAT(BOOST_PP_ENUM_, z)(                                 \
                n                                                            \
              , BOOST_PARAMETER_function_call_arg_pack_init                  \
              , BOOST_PP_DEC(n)                                              \
            )                                                                \
            BOOST_PP_ENUM_TRAILING_PARAMS_Z(                                 \
                z                                                            \
              , BOOST_PP_SUB(BOOST_PARAMETER_MAX_ARITY, n)                   \
              , ::boost::parameter::aux::void_reference() BOOST_PP_INTERCEPT \
            )                                                                \
        );                                                                   \
    }
/**/

#include <boost/parameter/aux_/preprocessor/binary_seq_to_args.hpp>
#include <boost/preprocessor/seq/seq.hpp>

#define BOOST_PARAMETER_function_call_op_overload_R(r, seq)                  \
    template <                                                               \
        BOOST_PP_ENUM_PARAMS(                                                \
            BOOST_PP_DEC(BOOST_PP_SEQ_SIZE(seq))                             \
          , typename A                                                       \
        )                                                                    \
    >                                                                        \
    inline typename ::boost::mpl::first<                                     \
        typename BOOST_PARAMETER_function_call_arg_list_R(                   \
            r, BOOST_PP_SEQ_TAIL(seq)                                        \
        )::type                                                              \
    >::type                                                                  \
        operator()(                                                          \
            BOOST_PARAMETER_AUX_PP_BINARY_SEQ_TO_ARGS(                       \
                BOOST_PP_SEQ_TAIL(seq), (A)(a)                               \
            )                                                                \
        ) const                                                              \
    {                                                                        \
        typedef typename BOOST_PARAMETER_function_call_arg_list_R(           \
            r, BOOST_PP_SEQ_TAIL(seq)                                        \
        )::type result;                                                      \
        typedef typename ::boost::mpl::first<result>::type result_type;      \
        typedef typename ::boost::mpl::second<result>::type error;           \
        error();                                                             \
        return result_type(                                                  \
            BOOST_PP_ENUM(                                                   \
                BOOST_PP_DEC(BOOST_PP_SEQ_SIZE(seq))                         \
              , BOOST_PARAMETER_function_call_arg_pack_init                  \
              , BOOST_PP_DEC(BOOST_PP_DEC(BOOST_PP_SEQ_SIZE(seq)))           \
            )                                                                \
            BOOST_PP_ENUM_TRAILING_PARAMS(                                   \
                BOOST_PP_SUB(                                                \
                    BOOST_PARAMETER_MAX_ARITY                                \
                  , BOOST_PP_DEC(BOOST_PP_SEQ_SIZE(seq))                     \
                )                                                            \
              , ::boost::parameter::aux::void_reference() BOOST_PP_INTERCEPT \
            )                                                                \
        );                                                                   \
    }
/**/

#define BOOST_PARAMETER_satisfies_begin(z, n, prefix)                        \
    ::boost::mpl::eval_if<                                                   \
        ::boost::parameter::aux::satisfies_requirements_of<                  \
            typename ::boost::mpl::first<ArgumentPackAndError>::type         \
          , BOOST_PP_CAT(prefix, n)                                          \
        >,
/**/

#define BOOST_PARAMETER_satisfies_end(z, n, false_t) ,false_t>
/**/

#include <boost/parameter/aux_/preprocessor/binary_seq_for_each.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/repetition/enum_shifted.hpp>
#include <boost/preprocessor/repetition/for.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/selection/min.hpp>

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
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
                ::boost::is_same<
#else
                ::std::is_same<
#endif
                    typename ::boost::mpl::second<
                        ArgumentPackAndError
                    >::type
                  , ::boost::parameter::void_
                >
                BOOST_PP_REPEAT(
                    BOOST_PARAMETER_MAX_ARITY
                  , BOOST_PARAMETER_satisfies_end
                  , ::boost::mpl::false_
                )::type
              , ::boost::mpl::identity<parameters>
              , ::boost::parameter::void_
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
          : match_base<
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
#endif // SFINAE enabled, not Borland.

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
        inline ::boost::parameter::aux::empty_arg_list operator()() const
        {
            return ::boost::parameter::aux::empty_arg_list();
        }

        BOOST_PP_REPEAT(
            BOOST_PP_MIN(
                BOOST_PP_INC(BOOST_PARAMETER_MAX_ARITY)
              , BOOST_PARAMETER_ALL_CONST_THRESHOLD_ARITY
            )
          , BOOST_PARAMETER_AUX_PP_BINARY_SEQ_FOR_EACH_Z
          , (BOOST_PARAMETER_function_call_op_overload_R)(_)
        )
#if (BOOST_PARAMETER_ALL_CONST_THRESHOLD_ARITY < BOOST_PARAMETER_MAX_ARITY)
        BOOST_PP_REPEAT_FROM_TO(
            BOOST_PARAMETER_ALL_CONST_THRESHOLD_ARITY
          , BOOST_PP_INC(BOOST_PARAMETER_MAX_ARITY)
          , BOOST_PARAMETER_function_call_op_overload_Z
          , PS
        )
#endif
    };
}} // namespace boost::parameter

#undef BOOST_PARAMETER_satisfies_end
#undef BOOST_PARAMETER_satisfies_begin
#undef BOOST_PARAMETER_function_call_op_overload_R
#undef BOOST_PARAMETER_function_call_op_overload_Z
#undef BOOST_PARAMETER_function_call_arg_pack_init
#undef BOOST_PARAMETER_function_call_arg_list_R
#undef BOOST_PARAMETER_function_call_arg_list_Z
#undef BOOST_PARAMETER_make_arg_items_R
#undef BOOST_PARAMETER_make_arg_items_Z
#undef BOOST_PARAMETER_template_args
#undef BOOST_PARAMETER_forward_typedef
//#undef BOOST_PARAMETER_build_deduced_list
//#undef BOOST_PARAMETER_make_deduced_list
//#undef BOOST_PARAMETER_build_arg_list
//#undef BOOST_PARAMETER_make_arg_list
//#undef BOOST_PARAMETER_right_angle

#endif // BOOST_PARAMETER_HAS_PERFECT_FORWARDING

#endif // BOOST_PARAMETERS_031014_HPP

