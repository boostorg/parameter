// Copyright Cromwell D. Enage 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/parameter.hpp>

#if !defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
#if BOOST_PARAMETER_MAX_ARITY < 8
#error Define BOOST_PARAMETER_MAX_ARITY as 8 or greater.
#endif
#if BOOST_PARAMETER_ALL_CONST_THRESHOLD_ARITY < 9
#error Define BOOST_PARAMETER_ALL_CONST_THRESHOLD_ARITY as 9 or greater.
#endif
#endif

namespace test {

    BOOST_PARAMETER_NAME((_lrc0, kw) in(lrc0))
    BOOST_PARAMETER_NAME((_lr0, kw) in_out(lr0))
    BOOST_PARAMETER_NAME((_rrc0, kw) in(rrc0))
#if defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
    BOOST_PARAMETER_NAME((_rr0, kw) consume(rr0))
#else
    BOOST_PARAMETER_NAME((_rr0, kw) rr0)
#endif
    BOOST_PARAMETER_NAME((_lrc1, kw) in(lrc1))
    BOOST_PARAMETER_NAME((_lr1, kw) out(lr1))
    BOOST_PARAMETER_NAME((_rrc1, kw) in(rrc1))
    BOOST_PARAMETER_NAME((_rr1, kw) rr1)
} // namespace test

#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/is_scalar.hpp>
#include <boost/core/lightweight_test.hpp>
#include "evaluate_category.hpp"

namespace test {

    BOOST_PARAMETER_FUNCTION((bool), evaluate, kw,
        (required
            (lrc0, *)
            (lr0, *)
            (rrc0, *)
            (rr0, *)
        )
    )
    {
        BOOST_TEST((
            passed_by_lvalue_reference_to_const == A<
                typename boost::remove_const<
                    typename boost::remove_reference<lrc0_type>::type
                >::type
            >::evaluate_category(args[_lrc0])
        ));
        BOOST_TEST((
            passed_by_lvalue_reference == A<
                typename boost::remove_const<
                    typename boost::remove_reference<lr0_type>::type
                >::type
            >::evaluate_category(args[_lr0])
        ));
#if defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING

        if (
            boost::is_scalar<
                typename boost::remove_const<
                    typename boost::remove_reference<rrc0_type>::type
                >::type
            >::value
        )
        {
            BOOST_TEST((
                passed_by_rvalue_reference == A<
                    typename boost::remove_const<
                        typename boost::remove_reference<rrc0_type>::type
                    >::type
                >::evaluate_category(args[_rrc0])
            ));
        }
        else
        {
            BOOST_TEST((
                passed_by_rvalue_reference_to_const == A<
                    typename boost::remove_const<
                        typename boost::remove_reference<rrc0_type>::type
                    >::type
                >::evaluate_category(args[_rrc0])
            ));
        }

        BOOST_TEST((
            passed_by_rvalue_reference == A<
                typename boost::remove_const<
                    typename boost::remove_reference<rr0_type>::type
                >::type
            >::evaluate_category(args[_rr0])
        ));
#else // !defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
        BOOST_TEST((
            passed_by_lvalue_reference_to_const == A<
                typename boost::remove_const<
                    typename boost::remove_reference<rrc0_type>::type
                >::type
            >::evaluate_category(args[_rrc0])
        ));
        BOOST_TEST((
            passed_by_lvalue_reference_to_const == A<
                typename boost::remove_const<
                    typename boost::remove_reference<rr0_type>::type
                >::type
            >::evaluate_category(args[_rr0])
        ));
#endif // BOOST_PARAMETER_HAS_PERFECT_FORWARDING

        return true;
    }
} // namespace test

#include <boost/type_traits/remove_pointer.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/mpl/placeholders.hpp>

namespace test {

    struct rr0_pred
    {
        template <typename Arg, typename Args>
        struct apply
          : boost::is_convertible<
                Arg
              , std::basic_string<
                    typename boost::remove_const<
                        typename boost::remove_pointer<
                            typename boost::parameter::value_type<
                                Args
                              , test::kw::lr0
                              , char const*
                            >::type
                        >::type
                    >::type
                >
            >
        {
        };
    };

    struct B
    {
        template <typename Args>
        explicit B(Args const& args)
        {
            test::evaluate(args[_lrc0], args[_lr0], args[_rrc0], args[_rr0]);
        }

#if BOOST_WORKAROUND(__SUNPRO_CC, BOOST_TESTED_AT(0x580))
        typedef boost::is_convertible<boost::mpl::_,float> lrc0_pred;
        typedef boost::is_convertible<boost::mpl::_,char const*> lr0_pred;

        BOOST_PARAMETER_MEMBER_FUNCTION((bool), static evaluate_deduced, kw,
            (deduced
                (required
                    (lrc0, *(lrc0_pred))
                    (lr0, *(lr0_pred))
                    (rr0, *(rr0_pred))
                )
            )
        )
#else // !BOOST_WORKAROUND(__SUNPRO_CC, BOOST_TESTED_AT(0x580))
        BOOST_PARAMETER_MEMBER_FUNCTION((bool), static evaluate_deduced, kw,
            (deduced
                (required
                    (lrc0, *(boost::is_convertible<boost::mpl::_,float>))
                    (lr0, *(boost::is_convertible<boost::mpl::_,char const*>))
                    (rr0, *(rr0_pred))
                )
            )
        )
#endif // SunPro CC workarounds needed.
        {
            BOOST_TEST((
                passed_by_lvalue_reference_to_const == A<
                    typename boost::remove_const<
                        typename boost::remove_reference<lrc0_type>::type
                    >::type
                >::evaluate_category(args[_lrc0])
            ));
            BOOST_TEST((
                passed_by_lvalue_reference == A<
                    typename boost::remove_const<
                        typename boost::remove_reference<lr0_type>::type
                    >::type
                >::evaluate_category(args[_lr0])
            ));

#if defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
            BOOST_TEST((
                passed_by_rvalue_reference == A<
                    typename boost::remove_const<
                        typename boost::remove_reference<rr0_type>::type
                    >::type
                >::evaluate_category(args[_rr0])
            ));
#else
            BOOST_TEST((
                passed_by_lvalue_reference_to_const == A<
                    typename boost::remove_const<
                        typename boost::remove_reference<rr0_type>::type
                    >::type
                >::evaluate_category(args[_rr0])
            ));
#endif // BOOST_PARAMETER_HAS_PERFECT_FORWARDING

            return true;
        }
    };

    struct C : B
    {
        BOOST_PARAMETER_CONSTRUCTOR(C, (B), kw,
            (required
                (lrc0, *)
                (lr0, *)
                (rrc0, *)
                (rr0, *)
            )
        )

        typedef boost::is_convertible<boost::mpl::_,std::bitset<1> > bs0_pred;
        typedef boost::is_convertible<boost::mpl::_,std::bitset<2> > bs1_pred;
        typedef boost::is_convertible<boost::mpl::_,std::bitset<3> > bs2_pred;
        typedef boost::is_convertible<boost::mpl::_,std::bitset<4> > bs3_pred;
        typedef boost::is_convertible<boost::mpl::_,std::bitset<5> > bs4_pred;
        typedef boost::is_convertible<boost::mpl::_,std::bitset<6> > bs5_pred;
        typedef boost::is_convertible<boost::mpl::_,std::bitset<7> > bs6_pred;
        typedef boost::is_convertible<boost::mpl::_,std::bitset<8> > bs7_pred;

        BOOST_PARAMETER_CONST_FUNCTION_CALL_OPERATOR((bool), kw,
            (deduced
                (required
                    (lrc0, *(bs0_pred))
                    (lr0, *(bs1_pred))
                    (rrc0, *(bs2_pred))
                    (rr0, *(bs3_pred))
                    (lrc1, *(bs4_pred))
                )
                (optional
                    (lr1, *(bs5_pred), test::lvalue_bitset<5>())
                    (rrc1, *(bs6_pred), test::rvalue_const_bitset<6>())
                    (rr1, *(bs7_pred), test::rvalue_bitset<7>())
                )
            )
        )
        {
            BOOST_TEST_EQ(
                passed_by_lvalue_reference_to_const
              , U::evaluate_category<0>(lrc0)
            );
            BOOST_TEST_EQ(
                passed_by_lvalue_reference
              , U::evaluate_category<1>(lr0)
            );
            BOOST_TEST_EQ(
                passed_by_lvalue_reference_to_const
              , U::evaluate_category<4>(lrc1)
            );
            BOOST_TEST_EQ(
                passed_by_lvalue_reference
              , U::evaluate_category<5>(lr1)
            );
#if defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
            BOOST_TEST_EQ(
                passed_by_rvalue_reference_to_const
              , U::evaluate_category<2>(boost::forward<rrc0_type>(rrc0))
            );
            BOOST_TEST_EQ(
                passed_by_rvalue_reference
              , U::evaluate_category<3>(boost::forward<rr0_type>(rr0))
            );
            BOOST_TEST_EQ(
                passed_by_rvalue_reference_to_const
              , U::evaluate_category<6>(boost::forward<rrc1_type>(rrc1))
            );
            BOOST_TEST_EQ(
                passed_by_rvalue_reference
              , U::evaluate_category<7>(boost::forward<rr1_type>(rr1))
            );
#else // !defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
            BOOST_TEST_EQ(
                passed_by_lvalue_reference_to_const
              , U::evaluate_category<2>(rrc0)
            );
            BOOST_TEST_EQ(
                passed_by_lvalue_reference_to_const
              , U::evaluate_category<3>(rr0)
            );
            BOOST_TEST_EQ(
                passed_by_lvalue_reference_to_const
              , U::evaluate_category<6>(rrc1)
            );
            BOOST_TEST_EQ(
                passed_by_lvalue_reference_to_const
              , U::evaluate_category<7>(rr1)
            );
#endif // BOOST_PARAMETER_HAS_PERFECT_FORWARDING

            return true;
        }
    };
} // namespace test

int main()
{
    test::evaluate(
        test::lvalue_const_float()
      , test::lvalue_float()
      , test::rvalue_const_float()
      , test::rvalue_float()
    );
    test::evaluate(
        test::lvalue_const_char_ptr()
      , test::lvalue_char_ptr()
      , test::rvalue_const_char_ptr()
      , test::rvalue_char_ptr()
    );
    test::evaluate(
        test::lvalue_const_str()
      , test::lvalue_str()
      , test::rvalue_const_str()
      , test::rvalue_str()
    );
    test::evaluate(
        test::_lr0 = test::lvalue_float()
      , test::_rrc0 = test::rvalue_const_float()
      , test::_rr0 = test::rvalue_float()
      , test::_lrc0 = test::lvalue_const_float()
    );
    test::evaluate(
        test::_lr0 = test::lvalue_char_ptr()
      , test::_rrc0 = test::rvalue_const_char_ptr()
      , test::_rr0 = test::rvalue_char_ptr()
      , test::_lrc0 = test::lvalue_const_char_ptr()
    );
    test::evaluate(
        test::_lr0 = test::lvalue_str()
      , test::_rrc0 = test::rvalue_const_str()
      , test::_rr0 = test::rvalue_str()
      , test::_lrc0 = test::lvalue_const_str()
    );

    test::C cf0(
        test::lvalue_const_float()
      , test::lvalue_float()
      , test::rvalue_const_float()
      , test::rvalue_float()
    );
    test::C cc0(
        test::lvalue_const_char_ptr()
      , test::lvalue_char_ptr()
      , test::rvalue_const_char_ptr()
      , test::rvalue_char_ptr()
    );
    test::C cs0(
        test::lvalue_const_str()
      , test::lvalue_str()
      , test::rvalue_const_str()
      , test::rvalue_str()
    );
    test::C cf1(
        test::_lr0 = test::lvalue_float()
      , test::_rrc0 = test::rvalue_const_float()
      , test::_rr0 = test::rvalue_float()
      , test::_lrc0 = test::lvalue_const_float()
    );
    test::C cc1(
        test::_lr0 = test::lvalue_char_ptr()
      , test::_rrc0 = test::rvalue_const_char_ptr()
      , test::_rr0 = test::rvalue_char_ptr()
      , test::_lrc0 = test::lvalue_const_char_ptr()
    );
    test::C cs1(
        test::_lr0 = test::lvalue_str()
      , test::_rrc0 = test::rvalue_const_str()
      , test::_rr0 = test::rvalue_str()
      , test::_lrc0 = test::lvalue_const_str()
    );

    char baz_arr[4] = "baz";
    typedef char char_arr[4];

    test::evaluate(
        "baz"
      , baz_arr
#if defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
      , static_cast<char_arr const&&>("baz")
      , static_cast<char_arr&&>(baz_arr)
#else
      , "baz"
      , "baz"
#endif
    );
    test::evaluate(
        test::_lr0 = baz_arr
#if defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
      , test::_rrc0 = static_cast<char_arr const&&>("baz")
      , test::_rr0 = static_cast<char_arr&&>(baz_arr)
#else
      , test::_rrc0 = "baz"
      , test::_rr0 = "baz"
#endif
      , test::_lrc0 = "baz"
    );
    test::B::evaluate_deduced(
        test::lvalue_char_ptr()
      , test::rvalue_str()
      , test::lvalue_const_float()
    );
    test::B::evaluate_deduced(
        test::rvalue_str()
      , test::lvalue_const_float()
      , test::lvalue_char_ptr()
    );

    test::C cp0(
        "baz"
      , baz_arr
#if defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
      , static_cast<char_arr const&&>("baz")
      , static_cast<char_arr&&>(baz_arr)
#else
      , "baz"
      , "baz"
#endif
    );
    test::C cp1(
        test::_lr0 = baz_arr
#if defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
      , test::_rrc0 = static_cast<char_arr const&&>("baz")
      , test::_rr0 = static_cast<char_arr&&>(baz_arr)
#else
      , test::_rrc0 = "baz"
      , test::_rr0 = "baz"
#endif
      , test::_lrc0 = "baz"
    );

    cp0(
        test::lvalue_const_bitset<4>()
      , test::lvalue_const_bitset<0>()
      , test::lvalue_bitset<1>()
      , test::rvalue_const_bitset<2>()
      , test::rvalue_bitset<3>()
    );
    cp1(
        test::lvalue_bitset<1>()
      , test::rvalue_const_bitset<2>()
      , test::rvalue_bitset<3>()
      , test::lvalue_const_bitset<4>()
      , test::lvalue_bitset<5>()
      , test::rvalue_const_bitset<6>()
      , test::rvalue_bitset<7>()
      , test::lvalue_const_bitset<0>()
    );

    return boost::report_errors();
}

