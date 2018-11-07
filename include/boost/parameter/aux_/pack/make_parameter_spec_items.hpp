// Copyright Cromwell D. Enage 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_AUX_PACK_MAKE_PARAMETER_SPEC_ITEMS_HPP
#define BOOST_PARAMETER_AUX_PACK_MAKE_PARAMETER_SPEC_ITEMS_HPP

namespace boost { namespace parameter { namespace aux {

    // This recursive metafunction forwards successive elements of
    // parameters::parameter_spec to make_deduced_items<>.
    // -- Cromwell D. Enage
    template <typename SpecSeq>
    struct make_deduced_list;

    // Checks if the arguments match the criteria of overload resolution.
    template <typename ArgumentPackAndError, typename SpecSeq>
    struct match_parameters_base_cond;

    // Helper metafunction for make_parameter_spec_items<...>, below.
    template <typename SpecSeq, typename ...Args>
    struct make_parameter_spec_items_helper;
}}} // namespace boost::parameter::aux

#include <boost/parameter/aux_/void.hpp>
#include <boost/mpl/identity.hpp>

namespace boost { namespace parameter { namespace aux {

    template <typename SpecSeq>
    struct make_parameter_spec_items_helper<SpecSeq>
      : ::boost::mpl::identity< ::boost::parameter::void_>
    {
    };
}}} // namespace boost::parameter::aux

#include <boost/parameter/aux_/pack/make_deduced_items.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/pop_front.hpp>

namespace boost { namespace parameter { namespace aux {

    template <typename SpecSeq>
    struct make_deduced_list_not_empty
      : ::boost::parameter::aux::make_deduced_items<
            typename boost::mpl::front<SpecSeq>::type
          , ::boost::parameter::aux::make_deduced_list<
                typename boost::mpl::pop_front<SpecSeq>::type
            >
        >
    {
    };
}}} // namespace boost::parameter::aux

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/empty.hpp>

namespace boost { namespace parameter { namespace aux {

    template <typename SpecSeq>
    struct make_deduced_list
      : ::boost::mpl::eval_if<
            ::boost::mpl::empty<SpecSeq>
          , ::boost::mpl::identity< ::boost::parameter::void_>
          , ::boost::parameter::aux::make_deduced_list_not_empty<SpecSeq>
        >
    {
    };
}}} // namespace boost::parameter::aux

#include <boost/mpl/bool.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace parameter { namespace aux {

    template <typename ArgumentPackAndError>
    struct is_arg_pack_error_void
      : ::boost::mpl::if_<
            ::boost::is_same<
                typename ::boost::mpl::second<ArgumentPackAndError>::type
              , ::boost::parameter::void_
            >
          , ::boost::mpl::true_
          , ::boost::mpl::false_
        >::type
    {
    };
}}} // namespace boost::parameter::aux

#include <boost/parameter/aux_/pack/satisfies.hpp>

namespace boost { namespace parameter { namespace aux {

    // Helper for match_parameters_base_cond<...>, below.
    template <typename ArgumentPackAndError, typename SpecSeq>
    struct match_parameters_base_cond_helper
      : ::boost::mpl::eval_if<
            ::boost::parameter::aux::satisfies_requirements_of<
                typename ::boost::mpl::first<ArgumentPackAndError>::type
              , typename ::boost::mpl::front<SpecSeq>::type
            >
          , ::boost::parameter::aux::match_parameters_base_cond<
                ArgumentPackAndError
              , typename ::boost::mpl::pop_front<SpecSeq>::type
            >
          , ::boost::mpl::false_
        >
    {
    };

    // If NamedList satisfies the PS0, PS1, ..., this is a metafunction
    // returning parameters.  Otherwise it has no nested ::type.
    template <typename ArgumentPackAndError, typename SpecSeq>
    struct match_parameters_base_cond
      : ::boost::mpl::eval_if<
            ::boost::mpl::empty<SpecSeq>
          , ::boost::parameter::aux
            ::is_arg_pack_error_void<ArgumentPackAndError>
          , ::boost::parameter::aux::match_parameters_base_cond_helper<
                ArgumentPackAndError
              , SpecSeq
            >
        >
    {
    };

    // This parameters item chaining metafunction class does not require
    // the lengths of the SpecSeq and of Args parameter pack to match.
    // Used by argument_pack to build the items in the resulting arg_list.
    // -- Cromwell D. Enage
    template <typename SpecSeq, typename ...Args>
    struct make_parameter_spec_items
      : ::boost::mpl::eval_if<
            ::boost::mpl::empty<SpecSeq>
          , ::boost::mpl::identity< ::boost::parameter::void_>
          , ::boost::parameter::aux
            ::make_parameter_spec_items_helper<SpecSeq,Args...>
        >
    {
    };
}}} // namespace boost::parameter::aux

#include <boost/parameter/aux_/pack/make_items.hpp>

namespace boost { namespace parameter { namespace aux {

    template <typename SpecSeq, typename A0, typename ...Args>
    struct make_parameter_spec_items_helper<SpecSeq,A0,Args...>
      : ::boost::parameter::aux::make_items<
            typename ::boost::mpl::front<SpecSeq>::type
          , A0
          , ::boost::parameter::aux::make_parameter_spec_items<
                typename ::boost::mpl::pop_front<SpecSeq>::type
              , Args...
            >
        >
    {
    };
}}} // namespace boost::parameter::aux

#endif  // include guard

