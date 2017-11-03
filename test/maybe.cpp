// Copyright Daniel Wallin 2006.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/parameter/name.hpp>
#include <boost/parameter/aux_/maybe.hpp>
#include <boost/detail/lightweight_test.hpp>

namespace test {

    BOOST_PARAMETER_NAME(kw)
    BOOST_PARAMETER_NAME(unused)

    template <class Args>
    int f(Args const& args)
    {
        return args[_kw | 1.f];
    }
} // namespace test

int main()
{
    using test::_kw;
    using test::_unused;
    using test::f;
    using boost::parameter::aux::maybe;

    BOOST_TEST(f((_kw = 0, _unused = 0)) == 0);
    BOOST_TEST(f(_unused = 0) == 1);
    BOOST_TEST(f((_kw = maybe<int>(), _unused = 0)) == 1);
    BOOST_TEST(f((_kw = maybe<int>(2), _unused = 0)) == 2);
    return boost::report_errors();
}

