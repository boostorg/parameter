// Copyright Daniel Wallin 2006.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_SET_060912_HPP
#define BOOST_PARAMETER_SET_060912_HPP

#include <boost/config/workaround.hpp>

#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))

#include <boost/mpl/bool.hpp>
#include <boost/mpl/list.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/find.hpp>
#include <boost/mpl/push_front.hpp>
#include <boost/config.hpp>

#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
#include <boost/type_traits/is_same.hpp>
#else
#include <type_traits>
#endif

namespace boost { namespace parameter { namespace aux {

    typedef ::boost::mpl::list0<> set0;

    template <typename Set, typename K>
    struct insert_ : ::boost::mpl::push_front<Set,K>
    {
    };

    template <typename Set, typename K>
    struct has_key_
    {
        typedef typename ::boost::mpl::find<Set,K>::type iter;
        typedef typename ::boost::mpl::if_<
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
            ::boost::is_same<iter,typename ::boost::mpl::end<Set>::type>
#else
            ::std::is_same<iter,typename ::boost::mpl::end<Set>::type>
#endif
          , ::boost::mpl::false_
          , ::boost::mpl::true_
        >::type type;
    };
}}} // namespace boost::parameter::aux

#else // !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))

#include <boost/mpl/insert.hpp>
#include <boost/mpl/set/set0.hpp>
#include <boost/mpl/has_key.hpp>

namespace boost { namespace parameter { namespace aux {

    typedef ::boost::mpl::set0<> set0;

    template <typename Set, typename K>
    struct insert_ : ::boost::mpl::insert<Set,K>
    {
    };

    template <typename Set, typename K>
    struct has_key_ : ::boost::mpl::has_key<Set,K>
    {
    };
}}} // namespace boost::parameter::aux

#endif // Borland workarounds needed.

#endif // BOOST_PARAMETER_SET_060912_HPP

