// Copyright Cromwell D. Enage 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/parameter/config.hpp>

#if !defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING) && \
    (BOOST_PARAMETER_MAX_ARITY < 16)
#error Define BOOST_PARAMETER_MAX_ARITY as 16 or greater.
#endif

#include <boost/parameter.hpp>

namespace test {

    BOOST_PARAMETER_NAME((_lrc0, keywords) in(lrc0))
    BOOST_PARAMETER_NAME((_lrc1, keywords) in(lrc1))
    BOOST_PARAMETER_NAME((_lrc2, keywords) in(lrc2))
    BOOST_PARAMETER_NAME((_lrc3, keywords) in(lrc3))
    BOOST_PARAMETER_NAME((_lrc4, keywords) in(lrc4))
    BOOST_PARAMETER_NAME((_lrc5, keywords) in(lrc5))
    BOOST_PARAMETER_NAME((_lrc6, keywords) in(lrc6))
    BOOST_PARAMETER_NAME((_lrc7, keywords) in(lrc7))
    BOOST_PARAMETER_NAME((_rrc0, keywords) in(rrc0))
    BOOST_PARAMETER_NAME((_rrc1, keywords) in(rrc1))
    BOOST_PARAMETER_NAME((_rrc2, keywords) in(rrc2))
    BOOST_PARAMETER_NAME((_rrc3, keywords) in(rrc3))
    BOOST_PARAMETER_NAME((_rrc4, keywords) in(rrc4))
    BOOST_PARAMETER_NAME((_rrc5, keywords) in(rrc5))
    BOOST_PARAMETER_NAME((_rrc6, keywords) in(rrc6))
    BOOST_PARAMETER_NAME((_rrc7, keywords) in(rrc7))

    struct h_parameters
      : boost::parameter::parameters<
            boost::parameter::required<test::keywords::lrc0>
          , boost::parameter::required<test::keywords::rrc0>
          , boost::parameter::required<test::keywords::lrc1>
          , boost::parameter::required<test::keywords::rrc1>
          , boost::parameter::required<test::keywords::lrc2>
          , boost::parameter::required<test::keywords::rrc2>
          , boost::parameter::required<test::keywords::lrc3>
          , boost::parameter::required<test::keywords::rrc3>
          , boost::parameter::optional<test::keywords::lrc4>
          , boost::parameter::optional<test::keywords::rrc4>
          , boost::parameter::optional<test::keywords::lrc5>
          , boost::parameter::optional<test::keywords::rrc5>
          , boost::parameter::optional<test::keywords::lrc6>
          , boost::parameter::optional<test::keywords::rrc6>
          , boost::parameter::optional<test::keywords::lrc7>
          , boost::parameter::optional<test::keywords::rrc7>
        >
    {
    };
} // namespace test

#include <boost/core/lightweight_test.hpp>
#include "evaluate_category.hpp"

namespace test {

    struct D
    {
        template <typename Args>
        static void evaluate(Args const& args)
        {
            BOOST_TEST_EQ(
                test::passed_by_lvalue_reference_to_const
              , U::evaluate_category<0>(args[test::_lrc0])
            );
            BOOST_TEST_EQ(
                test::passed_by_lvalue_reference_to_const
              , U::evaluate_category<1>(args[test::_lrc1])
            );
            BOOST_TEST_EQ(
                test::passed_by_lvalue_reference_to_const
              , U::evaluate_category<2>(args[test::_lrc2])
            );
            BOOST_TEST_EQ(
                test::passed_by_lvalue_reference_to_const
              , U::evaluate_category<3>(args[test::_lrc3])
            );
            BOOST_TEST_EQ(
                test::passed_by_lvalue_reference_to_const
              , U::evaluate_category<4>(
                    args[test::_lrc4 | test::lvalue_const_bitset<4>()]
                )
            );
            BOOST_TEST_EQ(
                test::passed_by_lvalue_reference_to_const
              , U::evaluate_category<5>(
                    args[
                        test::_lrc5 || test::lvalue_const_bitset_function<5>()
                    ]
                )
            );
            BOOST_TEST_EQ(
                test::passed_by_lvalue_reference_to_const
              , U::evaluate_category<6>(
                    args[test::_lrc6 | test::lvalue_const_bitset<6>()]
                )
            );
            BOOST_TEST_EQ(
                test::passed_by_lvalue_reference_to_const
              , U::evaluate_category<7>(
                    args[
                        test::_lrc7 || test::lvalue_const_bitset_function<7>()
                    ]
                )
            );
#if defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)
            BOOST_TEST_EQ(
                test::passed_by_rvalue_reference_to_const
              , U::evaluate_category<0>(args[test::_rrc0])
            );
            BOOST_TEST_EQ(
                test::passed_by_rvalue_reference_to_const
              , U::evaluate_category<1>(args[test::_rrc1])
            );
            BOOST_TEST_EQ(
                test::passed_by_rvalue_reference_to_const
              , U::evaluate_category<2>(args[test::_rrc2])
            );
            BOOST_TEST_EQ(
                test::passed_by_rvalue_reference_to_const
              , U::evaluate_category<3>(args[test::_rrc3])
            );
            BOOST_TEST_EQ(
                test::passed_by_rvalue_reference_to_const
              , U::evaluate_category<4>(
                    args[test::_rrc4 | test::rvalue_const_bitset<4>()]
                )
            );
            BOOST_TEST_EQ(
                test::passed_by_rvalue_reference_to_const
              , U::evaluate_category<5>(
                    args[
                        test::_rrc5 || test::rvalue_const_bitset_function<5>()
                    ]
                )
            );
            BOOST_TEST_EQ(
                test::passed_by_rvalue_reference_to_const
              , U::evaluate_category<6>(
                    args[test::_rrc6 | test::rvalue_const_bitset<6>()]
                )
            );
            BOOST_TEST_EQ(
                test::passed_by_rvalue_reference_to_const
              , U::evaluate_category<7>(
                    args[
                        test::_rrc7 || test::rvalue_const_bitset_function<7>()
                    ]
                )
            );
#else // !defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)
            BOOST_TEST_EQ(
                test::passed_by_lvalue_reference_to_const
              , U::evaluate_category<0>(args[test::_rrc0])
            );
            BOOST_TEST_EQ(
                test::passed_by_lvalue_reference_to_const
              , U::evaluate_category<1>(args[test::_rrc1])
            );
            BOOST_TEST_EQ(
                test::passed_by_lvalue_reference_to_const
              , U::evaluate_category<2>(args[test::_rrc2])
            );
            BOOST_TEST_EQ(
                test::passed_by_lvalue_reference_to_const
              , U::evaluate_category<3>(args[test::_rrc3])
            );
            BOOST_TEST_EQ(
                test::passed_by_lvalue_reference_to_const
              , U::evaluate_category<4>(
                    args[test::_rrc4 | test::rvalue_const_bitset<4>()]
                )
            );
            BOOST_TEST_EQ(
                test::passed_by_lvalue_reference_to_const
              , U::evaluate_category<5>(
                    args[
                        test::_rrc5 || test::rvalue_const_bitset_function<5>()
                    ]
                )
            );
            BOOST_TEST_EQ(
                test::passed_by_lvalue_reference_to_const
              , U::evaluate_category<6>(
                    args[test::_rrc6 | test::rvalue_const_bitset<6>()]
                )
            );
            BOOST_TEST_EQ(
                test::passed_by_lvalue_reference_to_const
              , U::evaluate_category<7>(
                    args[
                        test::_rrc7 || test::rvalue_const_bitset_function<7>()
                    ]
                )
            );
#endif // BOOST_PARAMETER_HAS_PERFECT_FORWARDING
        }
    };
} // namespace test

int main()
{
    test::D::evaluate(
        test::h_parameters()(
            test::lvalue_const_bitset<0>()
          , test::rvalue_const_bitset<0>()
          , test::lvalue_const_bitset<1>()
          , test::rvalue_const_bitset<1>()
          , test::lvalue_const_bitset<2>()
          , test::rvalue_const_bitset<2>()
          , test::lvalue_const_bitset<3>()
          , test::rvalue_const_bitset<3>()
        )
    );
    test::D::evaluate(
        test::h_parameters()(
            test::lvalue_const_bitset<0>()
          , test::rvalue_const_bitset<0>()
          , test::lvalue_const_bitset<1>()
          , test::rvalue_const_bitset<1>()
          , test::lvalue_const_bitset<2>()
          , test::rvalue_const_bitset<2>()
          , test::lvalue_const_bitset<3>()
          , test::rvalue_const_bitset<3>()
          , test::lvalue_const_bitset<4>()
          , test::rvalue_const_bitset<4>()
          , test::lvalue_const_bitset<5>()
          , test::rvalue_const_bitset<5>()
          , test::lvalue_const_bitset<6>()
          , test::rvalue_const_bitset<6>()
          , test::lvalue_const_bitset<7>()
          , test::rvalue_const_bitset<7>()
        )
    );
    return boost::report_errors();
}

