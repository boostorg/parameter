// Copyright Daniel Wallin 2005.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

namespace test {

    struct default_src
    {
        typedef int result_type;

        int operator()() const
        {
            return 0;
        }
    };
} // namespace test

#include <boost/parameter/name.hpp>

namespace test {

    BOOST_PARAMETER_NAME(x)
    BOOST_PARAMETER_NAME(y)
} // namespace test

#include <boost/core/lightweight_test.hpp>

namespace test {

    template <typename ArgumentPack, typename K, typename T>
    void check(ArgumentPack const& p, K const& kw, T const& value)
    {
        BOOST_TEST_EQ(p[kw], value);
    }
} // namespace test

int main()
{
    test::check(test::_x = 20, test::_x, 20);
    test::check(test::_y = 20, test::_y, 20);

    test::check(test::_x = 20, test::_x | 0, 20);
    test::check(test::_y = 20, test::_y | 0, 20);

    test::check(test::_x = 20, test::_x || test::default_src(), 20);
    test::check(test::_y = 20, test::_y || test::default_src(), 20);

    test::check(test::_y = 20, test::_x | 0, 0);
    test::check(test::_y = 20, test::_x || test::default_src(), 0);

    return boost::report_errors();
}

