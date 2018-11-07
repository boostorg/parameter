// Copyright Daniel Wallin 2006.
// Copyright Cromwell D. Enage 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DEDUCED_060920_HPP
#define BOOST_DEDUCED_060920_HPP

#include <boost/mpl/for_each.hpp>
#include "basics.hpp"

namespace test {

    struct not_present_tag
    {
    };

    not_present_tag not_present;

    template <typename E, typename ArgPack>
    struct assert_expected
    {
        assert_expected(E const& e, ArgPack const& args_)
          : expected(e), args(args_)
        {
        }

        template <typename T>
        bool check_not_present(T const&) const
        {
            BOOST_MPL_ASSERT((
                typename boost::mpl::if_<
                    boost::is_same<T,test::not_present_tag>
                  , boost::mpl::true_
                  , boost::mpl::false_
                >::type
            ));
            return true;
        }

        template <typename K>
        bool check1(K const& k, test::not_present_tag const& t, long) const
        {
            return check_not_present(args[k | t]);
        }

        template <typename K, typename Expected>
        bool check1(K const& k, Expected const& expected, int) const
        {
            return test::equal(args[k], expected);
        }

        template <typename K>
        void operator()(K) const
        {
            boost::parameter::keyword<K> const&
                k = boost::parameter::keyword<K>::instance;
            BOOST_TEST(check1(k, expected[k], 0L));
        }

        E const& expected;
        ArgPack const& args;
    };

    template <typename E, typename ArgPack>
    void check0(E const& e, ArgPack const& args)
    {
        boost::mpl::for_each<E>(test::assert_expected<E,ArgPack>(e, args));
    }

    template <typename P, typename E, typename ...Args>
    void check(E const& e, Args const&... args)
    {
        test::check0(e, P()(args...));
    }
} // namespace test

#endif  // include guard

