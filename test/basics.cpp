// Copyright David Abrahams, Daniel Wallin 2003.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/parameter.hpp>
#include <string.h>
#include <boost/bind.hpp>

#include "basics.hpp"

namespace test {

    // A separate function for getting the "value" key, so we can deduce F
    // and use lazy_binding on it.
    template <class Params, class F>
    typename boost::parameter::lazy_binding<Params,tag::value,F>::type
    extract_value(Params const& p, F const& f)
    {
        typename boost::parameter::lazy_binding<Params,tag::value,F>::type
            v = p[_value || f];
        return v;
    }

    template <class Params>
    int f_impl(Params const& p)
    {
        typename boost::parameter::binding<Params,tag::name>::type
            n = p[_name];

        typename boost::parameter::binding<Params,tag::value,double>::type
            v = extract_value(p, boost::bind(&value_default));

        typename boost::parameter::binding<Params,tag::index,int>::type
#if BOOST_WORKAROUND(__DECCXX_VER, BOOST_TESTED_AT(60590042))
            i = p[test::_index | 999];
#else
            i = p[_index | 999];
#endif

        p[_tester](n, v, i);

        return 1;
    }

#if defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
    template <class ...Args>
    int f(Args const&... args)
    {
        return f_impl(f_parameters()(args...));
    }
#else
    template <class A0, class A1, class A2, class A3>
    int f(A0 const& a0, A1 const& a1, A2 const& a2, A3 const& a3)
    {
        return f_impl(f_parameters()(a0, a1, a2, a3));
    }

    template <class A0, class A1, class A2>
    int f(A0 const& a0, A1 const& a1, A2 const& a2)
    {
        return f_impl(f_parameters()(a0, a1, a2));
    }

    template <class A0, class A1>
    int f(A0 const& a0, A1 const& a1)
    {
        return f_impl(f_parameters()(a0, a1));
    }
#endif // BOOST_PARAMETER_HAS_PERFECT_FORWARDING

    template <class Params>
    int f_list(Params const& params)
    {
        return f_impl(params);
    }
}

#include <boost/ref.hpp>

int main()
{
    test::f(
        test::values(S("foo"), S("bar"), S("baz"))
      , S("foo")
      , S("bar")
      , S("baz")
    );

    int x = 56;
    test::f(
        test::values("foo", 666.222, 56)
      , test::_index = boost::ref(x)
      , test::_name = "foo"
    );

#if !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
    x = 56;
    test::f_list((
        test::_tester = test::values("foo", 666.222, 56)
      , test::_index = boost::ref(x)
      , test::_name = "foo"
    ));
#endif // No comma operator available on Borland.
   
    //f(index = 56, name = 55); // won't compile

    return boost::report_errors();
}

