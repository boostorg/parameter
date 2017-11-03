// Copyright Daniel Wallin 2006.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DEDUCED_060920_HPP
#define BOOST_DEDUCED_060920_HPP

#include <boost/mpl/for_each.hpp>
#include "basics.hpp"

struct not_present_tag {};
not_present_tag not_present;

template <class E, class ArgPack>
struct assert_expected
{
    assert_expected(E const& e, ArgPack const& args_)
      : expected(e), args(args_)
    {
    }

    template <class T>
    bool check_not_present(T const&) const
    {
        BOOST_MPL_ASSERT((boost::is_same<T,not_present_tag>));
        return true;
    }

    template <class K>
    bool check1(K const& k, not_present_tag const&, long) const
    {
        return check_not_present(args[k | not_present]);
    }

    template <class K, class Expected>
    bool check1(K const& k, Expected const& expected, int) const
    {
        return test::equal(args[k], expected);
    }

    template <class K>
    void operator()(K) const
    {
        boost::parameter::keyword<K> const&
            k = boost::parameter::keyword<K>::instance;
        BOOST_TEST(check1(k, expected[k], 0L));
    }

    E const& expected;
    ArgPack const& args;
};

template <class E, class ArgPack>
void check0(E const& e, ArgPack const& args)
{
    boost::mpl::for_each<E>(assert_expected<E,ArgPack>(e, args));
}

#if defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
template <class P, class E, class ...Args>
void check(E const& e, Args const&... args)
{
    check0(e, P()(args...));
}
#else
template <class P, class E, class A0>
void check(E const& e, A0 const& a0)
{
    check0(e, P()(a0));
}

template <class P, class E, class A0, class A1>
void check(E const& e, A0 const& a0, A1 const& a1)
{
    check0(e, P()(a0, a1));
}

template <class P, class E, class A0, class A1, class A2>
void check(E const& e, A0 const& a0, A1 const& a1, A2 const& a2)
{
    check0(e, P()(a0, a1, a2));
}
#endif // BOOST_PARAMETER_HAS_PERFECT_FORWARDING
#endif // BOOST_DEDUCED_060920_HPP

