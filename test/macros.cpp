// Copyright David Abrahams, Daniel Wallin 2003.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/parameter/config.hpp>

#if !defined(LIBS_PARAMETER_TEST_WILL_NOT_ICE)
/*
#if !defined(BOOST_GCC) || ( \
        defined(__MINGW32__) && (1 == __MINGW32__) \
    ) || BOOST_WORKAROUND(BOOST_GCC, < 40800) || ( \
        BOOST_WORKAROUND(BOOST_GCC, >= 40900) && \
        defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING) \
    )
*/
#define LIBS_PARAMETER_TEST_WILL_NOT_ICE
//#endif
#endif

#include <boost/core/lightweight_test.hpp>
#include <boost/config/pragma_message.hpp>

#if defined(LIBS_PARAMETER_TEST_WILL_NOT_ICE)

#include <boost/parameter.hpp>
#include <boost/parameter/macros.hpp>
#include <boost/bind.hpp>
#include <cstring>

#include "basics.hpp"

namespace test {

    BOOST_PARAMETER_FUN(int, f, 2, 4, f_parameters)
    {
        p[test::_tester](
            p[test::_name]
          , p[test::_value || boost::bind(&test::value_default)]
          , p[test::_index | 999]
        );

        return 1;
    }

    BOOST_PARAMETER_NAME(foo)
    BOOST_PARAMETER_NAME(bar)

    struct baz_parameters
      : boost::parameter::parameters<
            boost::parameter::optional<test::tag::foo>
          , boost::parameter::optional<test::tag::bar>
        >
    {
    };

    BOOST_PARAMETER_FUN(int, baz, 0, 2, baz_parameters)
    {
        return 1;
    }
} // namespace test

#include <boost/ref.hpp>

#else
BOOST_PRAGMA_MESSAGE("Test not compiled.");
#endif // Compiler won't ICE.

int main()
{
#if defined(LIBS_PARAMETER_TEST_WILL_NOT_ICE)
    test::f(
        test::values(S("foo"), S("bar"), S("baz"))
      , S("foo"), S("bar"), S("baz")
    );
    BOOST_TEST_EQ(1, test::baz());

    int x = 56;
    test::f(
        test::values("foo", 666.222, 56)
      , test::_index = boost::ref(x)
      , test::_name = "foo"
    );

#endif
    return boost::report_errors();
}

