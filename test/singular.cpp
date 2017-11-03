// Copyright Daniel Wallin 2005.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/parameter/name.hpp>
#include <boost/detail/lightweight_test.hpp>

BOOST_PARAMETER_NAME(x)
BOOST_PARAMETER_NAME(y)
    
struct default_src
{
    typedef int result_type;

    int operator()() const
    {
        return 0;
    }
};
    
template <class ArgumentPack, class K, class T>
void check(ArgumentPack const& p, K const& kw, T const& value)
{
    BOOST_TEST(p[kw] == value);
}

int main()
{
    check(_x = 20, _x, 20);
    check(_y = 20, _y, 20);

    check(_x = 20, _x | 0, 20);
    check(_y = 20, _y | 0, 20);

    check(_x = 20, _x | default_src(), 20);
    check(_y = 20, _y | default_src(), 20);
    
    check(_y = 20, _x | 0, 0);
    check(_y = 20, _x || default_src(), 0);
    return boost::report_errors();
}

