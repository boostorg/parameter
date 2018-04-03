// Copyright Cromwell D. Enage 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/parameter.hpp>

#if !defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
#if BOOST_PARAMETER_MAX_ARITY < 4
#error Define BOOST_PARAMETER_MAX_ARITY as 4 or greater.
#endif
#if BOOST_PARAMETER_ALL_CONST_THRESHOLD_ARITY < 5
#error Define BOOST_PARAMETER_ALL_CONST_THRESHOLD_ARITY as 5 or greater.
#endif
#endif

namespace test {

    BOOST_PARAMETER_NAME((_lrc0, keywords) in(lrc0))
    BOOST_PARAMETER_NAME((_lr0, keywords) in_out(lr0))
    BOOST_PARAMETER_NAME((_rrc0, keywords) in(rrc0))
#if defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
    BOOST_PARAMETER_NAME((_rr0, keywords) consume(rr0))
#else
    BOOST_PARAMETER_NAME((_rr0, keywords) rr0)
#endif

    struct f_parameters
      : boost::parameter::parameters<
            boost::parameter::required<test::keywords::lrc0>
          , boost::parameter::required<test::keywords::lr0>
          , boost::parameter::required<test::keywords::rrc0>
          , boost::parameter::required<test::keywords::rr0>
        >
    {
    };
} // namespace test

#if defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING || ( \
        (10 < BOOST_PARAMETER_ALL_CONST_THRESHOLD_ARITY) && \
        (9 < BOOST_PARAMETER_MAX_ARITY) \
    )

namespace test {

    BOOST_PARAMETER_NAME((_lrc1, keywords) in(lrc1))
    BOOST_PARAMETER_NAME((_lr1, keywords) out(lr1))
    BOOST_PARAMETER_NAME((_rrc1, keywords) in(rrc1))
    BOOST_PARAMETER_NAME((_lrc2, keywords) in(lrc2))
    BOOST_PARAMETER_NAME((_lr2, keywords) out(lr2))
    BOOST_PARAMETER_NAME((_rr2, keywords) rr2)

    struct g_parameters
      : boost::parameter::parameters<
            boost::parameter::required<test::keywords::lrc0>
          , boost::parameter::required<test::keywords::lr0>
          , boost::parameter::required<test::keywords::rrc0>
          , boost::parameter::required<test::keywords::rr0>
          , boost::parameter::required<test::keywords::lrc1>
          , boost::parameter::required<test::keywords::lr1>
          , boost::parameter::required<test::keywords::rrc1>
          , boost::parameter::optional<test::keywords::lrc2>
          , boost::parameter::optional<test::keywords::lr2>
          , boost::parameter::optional<test::keywords::rr2>
        >
    {
    };
} // namespace test

#endif // Test with many arguments.

#if defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING || \
    15 < BOOST_PARAMETER_MAX_ARITY

namespace test {

    BOOST_PARAMETER_NAME((_lrc3, keywords) in(lrc3))
    BOOST_PARAMETER_NAME((_lrc4, keywords) in(lrc4))
    BOOST_PARAMETER_NAME((_lrc5, keywords) in(lrc5))
    BOOST_PARAMETER_NAME((_lrc6, keywords) in(lrc6))
    BOOST_PARAMETER_NAME((_lrc7, keywords) in(lrc7))
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

#endif // Test with more arguments.

#include <boost/type_traits/is_scalar.hpp>
#include <boost/core/lightweight_test.hpp>
#include "evaluate_category.hpp"

namespace test {

    template <typename T>
    struct B
    {
        template <typename Args>
        static void evaluate(Args const& args)
        {
            BOOST_TEST_EQ(
                test::passed_by_lvalue_reference_to_const
              , A<T>::evaluate_category(args[test::_lrc0])
            );
            BOOST_TEST_EQ(
                test::passed_by_lvalue_reference
              , A<T>::evaluate_category(args[test::_lr0])
            );
#if defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING

            if (boost::is_scalar<T>::value)
            {
                BOOST_TEST_EQ(
                    test::passed_by_rvalue_reference
                  , A<T>::evaluate_category(args[test::_rrc0])
                );
            }
            else
            {
                BOOST_TEST_EQ(
                    test::passed_by_rvalue_reference_to_const
                  , A<T>::evaluate_category(args[test::_rrc0])
                );
            }

            BOOST_TEST_EQ(
                test::passed_by_rvalue_reference
              , A<T>::evaluate_category(args[test::_rr0])
            );
#else // !defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
            BOOST_TEST_EQ(
                test::passed_by_lvalue_reference_to_const
              , A<T>::evaluate_category(args[test::_rrc0])
            );
            BOOST_TEST_EQ(
                test::passed_by_lvalue_reference_to_const
              , A<T>::evaluate_category(args[test::_rr0])
            );
#endif // BOOST_PARAMETER_HAS_PERFECT_FORWARDING
        }
    };
} // namespace test

#if defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING || ( \
        (10 < BOOST_PARAMETER_ALL_CONST_THRESHOLD_ARITY) && \
        (9 < BOOST_PARAMETER_MAX_ARITY) \
    )

namespace test {

    struct C
    {
        template <typename Args>
        static void evaluate(Args const& args)
        {
            BOOST_TEST_EQ(
                test::passed_by_lvalue_reference_to_const
              , U::evaluate_category<0>(args[test::_lrc0])
            );
            BOOST_TEST_EQ(
                test::passed_by_lvalue_reference
              , U::evaluate_category<0>(args[test::_lr0])
            );
            BOOST_TEST_EQ(
                test::passed_by_lvalue_reference_to_const
              , U::evaluate_category<1>(args[test::_lrc1])
            );
            BOOST_TEST_EQ(
                test::passed_by_lvalue_reference
              , U::evaluate_category<1>(args[test::_lr1])
            );
            BOOST_TEST_EQ(
                test::passed_by_lvalue_reference_to_const
              , U::evaluate_category<2>(
                    args[test::_lrc2 | test::lvalue_const_bitset<2>()]
                )
            );
            BOOST_TEST_EQ(
                test::passed_by_lvalue_reference
              , U::evaluate_category<2>(
                    args[test::_lr2 || test::lvalue_bitset_function<2>()]
                )
            );
#if defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
            BOOST_TEST_EQ(
                test::passed_by_rvalue_reference_to_const
              , U::evaluate_category<0>(args[test::_rrc0])
            );
            BOOST_TEST_EQ(
                test::passed_by_rvalue_reference
              , U::evaluate_category<0>(args[test::_rr0])
            );
            BOOST_TEST_EQ(
                test::passed_by_rvalue_reference_to_const
              , U::evaluate_category<1>(args[test::_rrc1])
            );
            BOOST_TEST_EQ(
                test::passed_by_rvalue_reference
              , U::evaluate_category<2>(
                    args[test::_rr2 || test::rvalue_bitset_function<2>()]
                )
            );
#else // !defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
            BOOST_TEST_EQ(
                test::passed_by_lvalue_reference_to_const
              , U::evaluate_category<0>(args[test::_rrc0])
            );
            BOOST_TEST_EQ(
                test::passed_by_lvalue_reference_to_const
              , U::evaluate_category<0>(args[test::_rr0])
            );
            BOOST_TEST_EQ(
                test::passed_by_lvalue_reference_to_const
              , U::evaluate_category<1>(args[test::_rrc1])
            );
            BOOST_TEST_EQ(
                test::passed_by_lvalue_reference_to_const
              , U::evaluate_category<2>(
                    args[test::_rr2 || test::rvalue_bitset_function<2>()]
                )
            );
#endif // BOOST_PARAMETER_HAS_PERFECT_FORWARDING
        }
    };
} // namespace test

#endif // Test with many arguments.

#if defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING || \
    15 < BOOST_PARAMETER_MAX_ARITY

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
#if defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
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
#else // !defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
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

#endif // Test with more arguments.

int main()
{
    test::B<float>::evaluate(
        test::f_parameters()(
            test::lvalue_const_float()
          , test::lvalue_float()
          , test::rvalue_const_float()
          , test::rvalue_float()
        )
    );
    test::B<char const*>::evaluate(
        test::f_parameters()(
            test::lvalue_const_char_ptr()
          , test::lvalue_char_ptr()
          , test::rvalue_const_char_ptr()
          , test::rvalue_char_ptr()
        )
    );
    test::B<std::string>::evaluate(
        test::f_parameters()(
            test::lvalue_const_str()
          , test::lvalue_str()
          , test::rvalue_const_str()
          , test::rvalue_str()
        )
    );
    test::B<float>::evaluate((
        test::_lr0 = test::lvalue_float()
      , test::_rrc0 = test::rvalue_const_float()
      , test::_rr0 = test::rvalue_float()
      , test::_lrc0 = test::lvalue_const_float()
    ));
    test::B<char const*>::evaluate((
        test::_lr0 = test::lvalue_char_ptr()
      , test::_rrc0 = test::rvalue_const_char_ptr()
      , test::_rr0 = test::rvalue_char_ptr()
      , test::_lrc0 = test::lvalue_const_char_ptr()
    ));
    test::B<std::string>::evaluate((
        test::_lr0 = test::lvalue_str()
      , test::_rrc0 = test::rvalue_const_str()
      , test::_rr0 = test::rvalue_str()
      , test::_lrc0 = test::lvalue_const_str()
    ));

    char baz_arr[4] = "baz";
    typedef char char_arr[4];

    test::B<char_arr>::evaluate(
        test::f_parameters()(
            "baz"
          , baz_arr
#if defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
          , static_cast<char_arr const&&>("baz")
          , static_cast<char_arr&&>(baz_arr)
#else
          , "baz"
          , "baz"
#endif
        )
    );
    test::B<char_arr>::evaluate((
        test::_lr0 = baz_arr
#if defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
      , test::_rrc0 = static_cast<char_arr const&&>("baz")
      , test::_rr0 = static_cast<char_arr&&>(baz_arr)
#else
      , test::_rrc0 = "baz"
      , test::_rr0 = "baz"
#endif
      , test::_lrc0 = "baz"
    ));

#if defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING || ( \
        (10 < BOOST_PARAMETER_ALL_CONST_THRESHOLD_ARITY) && \
        (9 < BOOST_PARAMETER_MAX_ARITY) \
    )
    test::C::evaluate(
        test::g_parameters()(
            test::lvalue_const_bitset<0>()
          , test::lvalue_bitset<0>()
          , test::rvalue_const_bitset<0>()
          , test::rvalue_bitset<0>()
          , test::lvalue_const_bitset<1>()
          , test::lvalue_bitset<1>()
          , test::rvalue_const_bitset<1>()
        )
    );
    test::C::evaluate(
        test::g_parameters()(
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
        )
    );
#endif // Test with many arguments.

#if defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING || \
    15 < BOOST_PARAMETER_MAX_ARITY
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
#endif // Test with more arguments.

    return boost::report_errors();
}

