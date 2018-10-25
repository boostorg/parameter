// Copyright David Abrahams, Daniel Wallin 2003.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_AUX_PACK_DEDUCE_TAG_HPP
#define BOOST_PARAMETER_AUX_PACK_DEDUCE_TAG_HPP

namespace boost { namespace parameter { namespace aux {

    template <
        typename Argument
      , typename ArgumentPack
      , typename DeducedArgs
      , typename UsedArgs
      , typename TagFn
    >
    struct deduce_tag;
}}} // namespace boost::parameter::aux

#include <boost/parameter/aux_/lambda_tag.hpp>
#include <boost/parameter/aux_/set.hpp>
#include <boost/parameter/aux_/pack/tag_type.hpp>
#include <boost/parameter/aux_/pack/tag_deduced.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/apply_wrap.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/mpl/assert.hpp>

namespace boost { namespace parameter { namespace aux {

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
}}} // namespace boost::parameter::aux

#include <boost/parameter/aux_/void.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace parameter { namespace aux {

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
            ::boost::is_same<DeducedArgs,::boost::parameter::void_>
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

#endif  // include guard

