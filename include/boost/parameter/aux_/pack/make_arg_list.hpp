// Copyright David Abrahams, Daniel Wallin 2003.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_AUX_PACK_MAKE_ARG_LIST_HPP
#define BOOST_PARAMETER_AUX_PACK_MAKE_ARG_LIST_HPP

namespace boost { namespace parameter { namespace aux {

    template <
        typename List
      , typename DeducedArgs
      , typename TagFn
      , typename IsPositional
      , typename UsedArgs
      , typename ArgumentPack
      , typename Error
      , typename EmitsErrors
    >
    struct make_arg_list_aux;
}}} // namespace boost::parameter::aux

#include <boost/parameter/aux_/arg_list.hpp>
#include <boost/parameter/aux_/void.hpp>
#include <boost/parameter/aux_/pack/unmatched_argument.hpp>
#include <boost/parameter/aux_/pack/tag_type.hpp>
#include <boost/parameter/aux_/pack/is_named_argument.hpp>
#include <boost/parameter/aux_/pack/insert_tagged.hpp>
#include <boost/parameter/aux_/pack/deduce_tag.hpp>
#include <boost/parameter/deduced.hpp>
#include <boost/parameter/config.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/apply_wrap.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/remove_reference.hpp>

namespace boost { namespace parameter { namespace aux {

    // Borland needs the insane extra-indirection workaround below so that
    // it doesn't magically drop the const qualifier from the argument type.
    template <
        typename List
      , typename DeducedArgs
      , typename TagFn
      , typename IsPositional
      , typename UsedArgs
      , typename ArgumentPack
#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
      , typename _argument
#endif
      , typename Error
      , typename EmitsErrors
    >
#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
    class make_arg_list00
#else
    class make_arg_list0
#endif
    {
#if !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
        typedef typename List::arg _argument;
#endif
        typedef typename ::boost::remove_const<
            typename ::boost::remove_reference<_argument>::type
        >::type _arg_type;
        typedef typename List::spec _parameter_spec;
        typedef typename ::boost::parameter::aux
        ::tag_type<_parameter_spec>::type _tag;
        typedef ::boost::parameter::aux
        ::is_named_argument<_argument> _is_tagged;

        // If this argument is either explicitly tagged or a deduced
        // parameter, then turn off positional matching.
        typedef typename ::boost::mpl::eval_if<
            IsPositional
          , ::boost::mpl::eval_if<
                ::boost::parameter::aux::is_deduced<_parameter_spec>
              , ::boost::mpl::false_
              , ::boost::mpl::if_<
                    _is_tagged
                  , ::boost::mpl::false_
                  , ::boost::mpl::true_
                >
            >
          , ::boost::mpl::false_
        >::type _is_positional;

        // If this parameter is explicitly tagged, then add it to the
        // used-parmeters set.  We only really need to add parameters
        // that are deduced, but we would need a way to check if
        // a given tag corresponds to a deduced parameter spec.
        typedef typename ::boost::mpl::eval_if<
            _is_tagged
          , ::boost::parameter::aux::insert_tagged<UsedArgs,_arg_type>
          , ::boost::mpl::identity<UsedArgs>
        >::type _used_args;

        // If this parameter is neither explicitly tagged nor positionally
        // matched, then deduce the tag from the deduced parameter specs.
        typedef typename ::boost::mpl::eval_if<
            typename ::boost::mpl::if_<
                _is_tagged
              , ::boost::mpl::true_
              , _is_positional
            >::type
          , ::boost::mpl::pair< ::boost::parameter::void_,_used_args>
          , ::boost::parameter::aux::deduce_tag<
                _argument
              , ArgumentPack
              , DeducedArgs
              , _used_args
              , TagFn
              , EmitsErrors
            >
        >::type _deduced_data;

        // If this parameter is explicitly tagged ...
        typedef typename ::boost::mpl::eval_if<
            _is_tagged
            // ... just use it
          , ::boost::mpl::identity<_arg_type>
            // ... else ...
          , ::boost::mpl::eval_if<
                // if positional matching is turned on ...
                _is_positional
                // ... tag it positionally
              , ::boost::mpl::apply_wrap2<TagFn,_tag,_argument>
                // ... else, use the deduced tag
              , ::boost::mpl::first<_deduced_data>
            >
        >::type _tagged;

        // Build the arg_list incrementally, prepending new nodes.
        typedef typename ::boost::mpl::if_<
            typename ::boost::mpl::if_<
                ::boost::is_same<Error,::boost::parameter::void_>
              , ::boost::is_same<_tagged,::boost::parameter::void_>
              , ::boost::mpl::false_
            >::type
          , ::boost::parameter::aux::unmatched_argument<_argument>
          , ::boost::parameter::void_
        >::type _error;

        typedef typename ::boost::mpl::if_<
            ::boost::is_same<_tagged,::boost::parameter::void_>
          , ArgumentPack
#if defined(BOOST_NO_SFINAE) || BOOST_WORKAROUND(BOOST_MSVC, < 1800)
          , ::boost::parameter::aux::arg_list<_tagged,ArgumentPack>
#else
          , ::boost::parameter::aux
            ::arg_list<_tagged,ArgumentPack,EmitsErrors>
#endif
        >::type _argument_pack;

     public:
        typedef typename ::boost::parameter::aux::make_arg_list_aux<
            typename List::tail
          , DeducedArgs
          , TagFn
          , _is_positional
          , typename _deduced_data::second
          , _argument_pack
          , _error
          , EmitsErrors
        >::type type;
    };

#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
    template <
        typename List
      , typename DeducedArgs
      , typename TagFn
      , typename IsPositional
      , typename UsedArgs
      , typename ArgumentPack
      , typename Error
      , typename EmitsErrors
    >
    struct make_arg_list0
    {
        typedef typename ::boost::mpl::eval_if<
            typename List::is_arg_const
          , ::boost::parameter::aux::make_arg_list00<
                List
              , DeducedArgs
              , TagFn
              , IsPositional
              , UsedArgs
              , ArgumentPack
              , typename List::arg const
              , Error
              , EmitsErrors
            >
          , ::boost::parameter::aux::make_arg_list00<
                List
              , DeducedArgs
              , TagFn
              , IsPositional
              , UsedArgs
              , ArgumentPack
              , typename List::arg
              , Error
              , EmitsErrors
            >
        >::type type;
    };
#endif  // Borland workarounds needed.

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
    //   IsPositional: An mpl::bool_<> specialization indicating if positional
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
      , typename IsPositional
      , typename DeducedSet
      , typename ArgumentPack
      , typename Error
      , typename EmitsErrors
    >
    struct make_arg_list_aux
      : ::boost::mpl::eval_if<
            ::boost::is_same<List,::boost::parameter::void_>
          , ::boost::mpl::identity< ::boost::mpl::pair<ArgumentPack,Error> >
          , ::boost::parameter::aux::make_arg_list0<
                List
              , DeducedArgs
              , TagFn
              , IsPositional
              , DeducedSet
              , ArgumentPack
              , Error
              , EmitsErrors
            >
        >
    {
    };
}}} // namespace boost::parameter::aux

#include <boost/parameter/aux_/set.hpp>

namespace boost { namespace parameter { namespace aux {

    // VC6.5 was choking on the default parameters for make_arg_list_aux,
    // so this just forwards to that adding in the defaults.
    template <
        typename List
      , typename DeducedArgs
      , typename TagFn
      , typename EmitsErrors = ::boost::mpl::true_
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
          , EmitsErrors
        >
    {
    };
}}} // namespace boost::parameter::aux

#endif  // include guard

