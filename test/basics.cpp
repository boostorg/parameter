// Copyright David Abrahams, Daniel Wallin 2003.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/parameter/config.hpp>

#if !defined(BOOST_GCC) || ( \
        defined(__MINGW32__) && (1 == __MINGW32__) \
    ) || BOOST_WORKAROUND(BOOST_GCC, < 40900) || ( \
        !defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING) && \
        BOOST_WORKAROUND(BOOST_GCC, >= 40900) && \
        BOOST_WORKAROUND(BOOST_GCC, < 50000) \
    ) || ( \
        defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING) && \
        BOOST_WORKAROUND(BOOST_GCC, >= 50000) \
    )
#define LIBS_PARAMETER_TEST_WILL_NOT_ICE
#endif

#include <boost/core/lightweight_test.hpp>
#include <boost/config/pragma_message.hpp>

#if defined LIBS_PARAMETER_TEST_WILL_NOT_ICE

#include <boost/parameter.hpp>
#include <string.h>
#include <boost/bind.hpp>

#include "basics.hpp"

namespace test {

    // A separate function for getting the "value" key, so we can deduce F
    // and use lazy_binding on it.
    template <typename Params, typename F>
    typename boost::parameter::lazy_binding<Params,tag::value,F>::type
        extract_value(Params const& p, F const& f)
    {
        typename boost::parameter::lazy_binding<
            Params,test::tag::value,F
        >::type v = p[test::_value || f];
        return v;
    }

    template <typename Params>
    int f_impl(Params const& p)
    {
        typename boost::parameter::binding<Params,test::tag::name>::type
            n = p[test::_name];

        typename boost::parameter::binding<
            Params,test::tag::value,double
        >::type v = test::extract_value(p, boost::bind(&test::value_default));

        typename boost::parameter::binding<Params,test::tag::index,int>::type
            i = p[test::_index | 999];

        p[test::_tester](n, v, i);

        return 1;
    }

#if defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)
    template <typename ...Args>
    int f(Args const&... args)
    {
        return test::f_impl(test::f_parameters()(args...));
    }
#else
    template <typename A0, typename A1, typename A2, typename A3>
    int f(A0 const& a0, A1 const& a1, A2 const& a2, A3 const& a3)
    {
        return test::f_impl(test::f_parameters()(a0, a1, a2, a3));
    }

    template <typename A0, typename A1, typename A2>
    int f(A0 const& a0, A1 const& a1, A2 const& a2)
    {
        return test::f_impl(test::f_parameters()(a0, a1, a2));
    }

    template <typename A0, typename A1>
    int f(A0 const& a0, A1 const& a1)
    {
        return test::f_impl(test::f_parameters()(a0, a1));
    }
#endif // BOOST_PARAMETER_HAS_PERFECT_FORWARDING

    template <class Params>
    int f_list(Params const& params)
    {
        return test::f_impl(params);
    }
}

#include <boost/ref.hpp>

BOOST_PRAGMA_MESSAGE("Test should compile.");
#else
BOOST_PRAGMA_MESSAGE("Test not compiled.");
#endif // Compiler won't ICE.

int main()
{
#if defined LIBS_PARAMETER_TEST_WILL_NOT_ICE
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

#if defined(LIBS_PARAMETER_TEST_COMPILE_FAILURE)
//#if defined(__MINGW32__) && (1 == __MINGW32__)
//#error test::f(test::_index = 56, test::_name = 55) should not compile.
//#else
    test::f(test::_index = 56, test::_name = 55); // won't compile
//#endif
#endif
#endif // Compiler won't ICE.
    return boost::report_errors();
}

