// Copyright David Abrahams, Daniel Wallin 2005.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/parameter.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/detail/lightweight_test.hpp>

namespace test {

BOOST_PARAMETER_NAME(w)
BOOST_PARAMETER_NAME(x)
BOOST_PARAMETER_NAME(y)
BOOST_PARAMETER_NAME(z)

using namespace boost::parameter;
using namespace boost::mpl::placeholders;

struct f_parameters // vc6 is happier with inheritance than with a typedef
  : parameters<
        required<tag::w>
      , optional<tag::x,boost::is_convertible<_,int> >
      , optional<tag::y,boost::is_convertible<_,int> >
      , optional<tag::z,boost::is_convertible<_,int> >
    >
{};

#ifdef BOOST_NO_VOID_RETURNS
BOOST_PARAMETER_FUN(int, f, 1, 4, f_parameters)
#else 
BOOST_PARAMETER_FUN(void, f, 1, 4, f_parameters)
#endif 
{
    BOOST_TEST(p[_w][0] == p[_x | -1]);
    BOOST_TEST(p[_w][1] == p[_y | -2]);
    BOOST_TEST(p[_w][2] == p[_z | -3]);
#ifdef BOOST_NO_VOID_RETURNS
    return 0;
#endif 
}

}

int main()
{
    using namespace test;
    int a[3];
    a[0] = 1;
    a[1] = 2;
    a[2] = 3;
    f(_x = 1, _y = 2, _z = 3, _w = a);
    a[1] = -2;
    a[2] = -3;
    f(_x = 1, _w = a);
    a[0] = -1;
    a[1] = 2;
    f(_y = 2, _w = a);
    a[1] = -2;
    a[2] = 3;
    f(_z = 3, _w = a);
    a[0] = 1;
    f(_z = 3, _x = 1, _w = a);
    return boost::report_errors();
}

