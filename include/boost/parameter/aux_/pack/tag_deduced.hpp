// Copyright David Abrahams, Daniel Wallin 2003.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_AUX_PACK_TAG_DEDUCED_HPP
#define BOOST_PARAMETER_AUX_PACK_TAG_DEDUCED_HPP

#include <boost/parameter/aux_/set.hpp>
#include <boost/parameter/aux_/pack/tag_type.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/apply_wrap.hpp>

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
}}} // namespace boost::parameter::aux

#endif  // include guard

