// Copyright Cromwell D. Enage 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/parameter/config.hpp>

#if !defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)
#if (BOOST_PARAMETER_MAX_ARITY < 10)
#error Define BOOST_PARAMETER_MAX_ARITY as 10 or greater.
#endif
#if (BOOST_PARAMETER_ALL_CONST_THRESHOLD_ARITY < 11)
#error Define BOOST_PARAMETER_ALL_CONST_THRESHOLD_ARITY as 11 or greater.
#endif
#endif

#if !defined(LIBS_PARAMETER_TEST_WILL_NOT_ICE)
#if defined(BOOST_GCC)
#if (defined(__MINGW32__) && (1 == __MINGW32__)) || ( \
        BOOST_WORKAROUND(BOOST_GCC, < 40700) && ( \
            BOOST_WORKAROUND(BOOST_GCC, >= 40500) || \
            !defined(__GXX_EXPERIMENTAL_CXX0X__) \
        ) \
    ) || ( \
        BOOST_WORKAROUND(BOOST_GCC, >= 40900) && \
        defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING) \
    )
#define LIBS_PARAMETER_TEST_WILL_NOT_ICE
#endif
#else // !defined(BOOST_GCC)
#if defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING) || \
    !defined(BOOST_CLANG) || !(1 == BOOST_CLANG) || defined(__APPLE_CC__)
#define LIBS_PARAMETER_TEST_WILL_NOT_ICE
#endif
#endif // BOOST_GCC
#endif // !defined(LIBS_PARAMETER_TEST_WILL_NOT_ICE)

#include <boost/core/lightweight_test.hpp>
#include <boost/config/pragma_message.hpp>

#if defined(LIBS_PARAMETER_TEST_WILL_NOT_ICE)

#include <boost/parameter.hpp>

namespace test {

    BOOST_PARAMETER_NAME(in(lrc0))
    BOOST_PARAMETER_NAME(out(lr0))
    BOOST_PARAMETER_NAME(in(rrc0))
    BOOST_PARAMETER_NAME(rr0)
    BOOST_PARAMETER_NAME(in(lrc1))
    BOOST_PARAMETER_NAME(out(lr1))
    BOOST_PARAMETER_NAME(in(rrc1))
    BOOST_PARAMETER_NAME(in(lrc2))
    BOOST_PARAMETER_NAME(out(lr2))
    BOOST_PARAMETER_NAME(rr2)

    struct g_parameters
      : boost::parameter::parameters<
            boost::parameter::required<test::tag::lrc0>
          , boost::parameter::required<test::tag::lr0>
          , boost::parameter::required<test::tag::rrc0>
          , boost::parameter::required<test::tag::rr0>
          , boost::parameter::required<test::tag::lrc1>
          , boost::parameter::required<test::tag::lr1>
          , boost::parameter::required<test::tag::rrc1>
          , boost::parameter::optional<test::tag::lrc2>
          , boost::parameter::optional<test::tag::lr2>
          , boost::parameter::optional<test::tag::rr2>
        >
    {
    };
} // namespace test

#include <boost/parameter/macros.hpp>
#include <boost/core/lightweight_test.hpp>
#include "evaluate_category.hpp"

namespace test {

    struct C
    {
        BOOST_PARAMETER_MEMFUN(static int, evaluate, 7, 10, g_parameters)
        {
            BOOST_TEST_EQ(
                test::passed_by_lvalue_reference_to_const
              , U::evaluate_category<0>(p[test::_lrc0])
            );
            BOOST_TEST_EQ(
                test::passed_by_lvalue_reference
              , U::evaluate_category<0>(p[test::_lr0])
            );
            BOOST_TEST_EQ(
                test::passed_by_lvalue_reference_to_const
              , U::evaluate_category<1>(p[test::_lrc1])
            );
            BOOST_TEST_EQ(
                test::passed_by_lvalue_reference
              , U::evaluate_category<1>(p[test::_lr1])
            );
            BOOST_TEST_EQ(
                test::passed_by_lvalue_reference_to_const
              , U::evaluate_category<2>(
                    p[test::_lrc2 | test::lvalue_const_bitset<2>()]
                )
            );
            BOOST_TEST_EQ(
                test::passed_by_lvalue_reference
              , U::evaluate_category<2>(
                    p[test::_lr2 || test::lvalue_bitset_function<2>()]
                )
            );
#if defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)
            BOOST_TEST_EQ(
                test::passed_by_rvalue_reference_to_const
              , U::evaluate_category<0>(p[test::_rrc0])
            );
            BOOST_TEST_EQ(
                test::passed_by_rvalue_reference
              , U::evaluate_category<0>(p[test::_rr0])
            );
            BOOST_TEST_EQ(
                test::passed_by_rvalue_reference_to_const
              , U::evaluate_category<1>(p[test::_rrc1])
            );
            BOOST_TEST_EQ(
                test::passed_by_rvalue_reference
              , U::evaluate_category<2>(
                    p[test::_rr2 || test::rvalue_bitset_function<2>()]
                )
            );
#else // !defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)
            BOOST_TEST_EQ(
                test::passed_by_lvalue_reference_to_const
              , U::evaluate_category<0>(p[test::_rrc0])
            );
            BOOST_TEST_EQ(
                test::passed_by_lvalue_reference_to_const
              , U::evaluate_category<0>(p[test::_rr0])
            );
            BOOST_TEST_EQ(
                test::passed_by_lvalue_reference_to_const
              , U::evaluate_category<1>(p[test::_rrc1])
            );
            BOOST_TEST_EQ(
                test::passed_by_lvalue_reference_to_const
              , U::evaluate_category<2>(
                    p[test::_rr2 || test::rvalue_bitset_function<2>()]
                )
            );
#endif // BOOST_PARAMETER_HAS_PERFECT_FORWARDING

            return 0;
        }
    };
} // namespace test

#else
BOOST_PRAGMA_MESSAGE("Test not compiled.");
#endif // Compiler won't ICE.

int main()
{
#if defined(LIBS_PARAMETER_TEST_WILL_NOT_ICE)
    test::C::evaluate(
        test::lvalue_const_bitset<0>()
      , test::lvalue_bitset<0>()
      , test::rvalue_const_bitset<0>()
      , test::rvalue_bitset<0>()
      , test::lvalue_const_bitset<1>()
      , test::lvalue_bitset<1>()
      , test::rvalue_const_bitset<1>()
    );
    test::C::evaluate(
        test::lvalue_const_bitset<0>()
      , test::lvalue_bitset<0>()
      , test::rvalue_const_bitset<0>()
      , test::rvalue_bitset<0>()
      , test::lvalue_const_bitset<1>()
      , test::lvalue_bitset<1>()
      , test::rvalue_const_bitset<1>()
      , test::lvalue_const_bitset<2>()
      , test::lvalue_bitset<2>()
      , test::rvalue_bitset<2>()
    );
#endif // Compiler won't ICE.
    return boost::report_errors();
}

