// Copyright Cromwell D. Enage 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/parameter.hpp>

#if !defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)
#if (BOOST_PARAMETER_MAX_ARITY < 8)
#error Define BOOST_PARAMETER_MAX_ARITY as 8 or greater.
#endif
#if (BOOST_PARAMETER_ALL_CONST_THRESHOLD_ARITY < 9)
#error Define BOOST_PARAMETER_ALL_CONST_THRESHOLD_ARITY as 9 or greater.
#endif
#endif

namespace test {

    BOOST_PARAMETER_NAME((_lrc0, kw) in(lrc0))
    BOOST_PARAMETER_NAME((_lr0, kw) in_out(lr0))
    BOOST_PARAMETER_NAME((_rrc0, kw) in(rrc0))
#if defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)
    BOOST_PARAMETER_NAME((_rr0, kw) consume(rr0))
#else
    BOOST_PARAMETER_NAME((_rr0, kw) rr0)
#endif
    BOOST_PARAMETER_NAME((_lrc1, kw) in(lrc1))
    BOOST_PARAMETER_NAME((_lr1, kw) out(lr1))
    BOOST_PARAMETER_NAME((_rrc1, kw) in(rrc1))
    BOOST_PARAMETER_NAME((_rr1, kw) rr1)
} // namespace test

#include <boost/core/lightweight_test.hpp>
#include "evaluate_category.hpp"

#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
#include <boost/type_traits/is_scalar.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/remove_reference.hpp>
#else
#include <type_traits>
#endif

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
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
        BOOST_TEST((
            test::passed_by_lvalue_reference_to_const == test::A<
                typename boost::remove_const<
                    typename boost::parameter::value_type<
                        Args
                      , test::kw::lrc0
                    >::type
                >::type
            >::evaluate_category(args[test::_lrc0])
        ));
        BOOST_TEST_EQ(
            test::passed_by_lvalue_reference_to_const
          , test::A<
                typename boost::remove_const<
                    typename boost::remove_reference<lrc0_type>::type
                >::type
            >::evaluate_category(lrc0)
        );
        BOOST_TEST((
            test::passed_by_lvalue_reference == test::A<
                typename boost::remove_const<
                    typename boost::parameter::value_type<
                        Args
                      , test::kw::lr0
                    >::type
                >::type
            >::evaluate_category(args[test::_lr0])
        ));
        BOOST_TEST_EQ(
            test::passed_by_lvalue_reference
          , test::A<
                typename boost::remove_const<
                    typename boost::remove_reference<lr0_type>::type
                >::type
            >::evaluate_category(lr0)
        );
#else // !defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
        BOOST_TEST((
            test::passed_by_lvalue_reference_to_const == test::A<
                typename std::remove_const<
                    typename boost::parameter::value_type<
                        Args
                      , test::kw::lrc0
                    >::type
                >::type
            >::evaluate_category(args[test::_lrc0])
        ));
        BOOST_TEST_EQ(
            test::passed_by_lvalue_reference_to_const
          , test::A<
                typename std::remove_const<
                    typename std::remove_reference<lrc0_type>::type
                >::type
            >::evaluate_category(lrc0)
        );
        BOOST_TEST((
            test::passed_by_lvalue_reference == test::A<
                typename std::remove_const<
                    typename boost::parameter::value_type<
                        Args
                      , test::kw::lr0
                    >::type
                >::type
            >::evaluate_category(args[test::_lr0])
        ));
        BOOST_TEST_EQ(
            test::passed_by_lvalue_reference
          , test::A<
                typename std::remove_const<
                    typename std::remove_reference<lr0_type>::type
                >::type
            >::evaluate_category(lr0)
        );
#endif // BOOST_NO_CXX11_HDR_TYPE_TRAITS

#if defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)
        if (
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
            boost::is_scalar<
                typename boost::remove_const<
                    typename boost::remove_reference<rrc0_type>::type
                >::type
            >::value
#else
            std::is_scalar<
                typename std::remove_const<
                    typename std::remove_reference<rrc0_type>::type
                >::type
            >::value
#endif // BOOST_NO_CXX11_HDR_TYPE_TRAITS
        )
        {
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
            BOOST_TEST((
                test::passed_by_rvalue_reference == test::A<
                    typename boost::remove_const<
                        typename boost::parameter::value_type<
                            Args
                          , test::kw::rrc0
                        >::type
                    >::type
                >::evaluate_category(args[test::_rrc0])
            ));
            BOOST_TEST_EQ(
                test::passed_by_rvalue_reference
              , test::A<
                    typename boost::remove_const<
                        typename boost::remove_reference<rrc0_type>::type
                    >::type
                >::evaluate_category(boost::forward<rrc0_type>(rrc0))
            );
#else // !defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
            BOOST_TEST((
                test::passed_by_rvalue_reference == test::A<
                    typename std::remove_const<
                        typename boost::parameter::value_type<
                            Args
                          , test::kw::rrc0
                        >::type
                    >::type
                >::evaluate_category(args[test::_rrc0])
            ));
            BOOST_TEST_EQ(
                test::passed_by_rvalue_reference
              , test::A<
                    typename std::remove_const<
                        typename std::remove_reference<rrc0_type>::type
                    >::type
                >::evaluate_category(boost::forward<rrc0_type>(rrc0))
            );
#endif // BOOST_NO_CXX11_HDR_TYPE_TRAITS
        }
        else // rrc0's value type isn't scalar
        {
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
            BOOST_TEST((
                test::passed_by_rvalue_reference_to_const == test::A<
                    typename boost::remove_const<
                        typename boost::parameter::value_type<
                            Args
                          , test::kw::rrc0
                        >::type
                    >::type
                >::evaluate_category(args[test::_rrc0])
            ));
            BOOST_TEST_EQ(
                test::passed_by_rvalue_reference_to_const
              , test::A<
                    typename boost::remove_const<
                        typename boost::remove_reference<rrc0_type>::type
                    >::type
                >::evaluate_category(boost::forward<rrc0_type>(rrc0))
            );
#else // !defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
            BOOST_TEST((
                test::passed_by_rvalue_reference_to_const == test::A<
                    typename std::remove_const<
                        typename boost::parameter::value_type<
                            Args
                          , test::kw::rrc0
                        >::type
                    >::type
                >::evaluate_category(args[test::_rrc0])
            ));
            BOOST_TEST_EQ(
                test::passed_by_rvalue_reference_to_const
              , test::A<
                    typename std::remove_const<
                        typename std::remove_reference<rrc0_type>::type
                    >::type
                >::evaluate_category(boost::forward<rrc0_type>(rrc0))
            );
#endif // BOOST_NO_CXX11_HDR_TYPE_TRAITS
        }

#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
        BOOST_TEST((
            test::passed_by_rvalue_reference == test::A<
                typename boost::remove_const<
                    typename boost::parameter::value_type<
                        Args
                      , test::kw::rr0
                    >::type
                >::type
            >::evaluate_category(args[test::_rr0])
        ));
        BOOST_TEST_EQ(
            test::passed_by_rvalue_reference
          , test::A<
                typename boost::remove_const<
                    typename boost::remove_reference<rr0_type>::type
                >::type
            >::evaluate_category(boost::forward<rr0_type>(rr0))
        );
#else // !defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
        BOOST_TEST((
            test::passed_by_rvalue_reference == test::A<
                typename std::remove_const<
                    typename boost::parameter::value_type<
                        Args
                      , test::kw::rr0
                    >::type
                >::type
            >::evaluate_category(args[test::_rr0])
        ));
        BOOST_TEST_EQ(
            test::passed_by_rvalue_reference
          , test::A<
                typename std::remove_const<
                    typename std::remove_reference<rr0_type>::type
                >::type
            >::evaluate_category(boost::forward<rr0_type>(rr0))
        );
#endif // BOOST_NO_CXX11_HDR_TYPE_TRAITS
#else // !defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
        BOOST_TEST((
            test::passed_by_lvalue_reference_to_const == test::A<
                typename boost::remove_const<
                    typename boost::parameter::value_type<
                        Args
                      , test::kw::rrc0
                    >::type
                >::type
            >::evaluate_category(args[test::_rrc0])
        ));
        BOOST_TEST_EQ(
            test::passed_by_lvalue_reference_to_const
          , test::A<
                typename boost::remove_const<
                    typename boost::remove_reference<rrc0_type>::type
                >::type
            >::evaluate_category(rrc0)
        );
        BOOST_TEST((
            test::passed_by_lvalue_reference_to_const == test::A<
                typename boost::remove_const<
                    typename boost::parameter::value_type<
                        Args
                      , test::kw::rr0
                    >::type
                >::type
            >::evaluate_category(args[test::_rr0])
        ));
        BOOST_TEST_EQ(
            test::passed_by_lvalue_reference_to_const
          , test::A<
                typename boost::remove_const<
                    typename boost::remove_reference<rr0_type>::type
                >::type
            >::evaluate_category(rr0)
        );
#else // !defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
        BOOST_TEST((
            test::passed_by_lvalue_reference_to_const == test::A<
                typename std::remove_const<
                    typename boost::parameter::value_type<
                        Args
                      , test::kw::rrc0
                    >::type
                >::type
            >::evaluate_category(args[test::_rrc0])
        ));
        BOOST_TEST_EQ(
            test::passed_by_lvalue_reference_to_const
          , test::A<
                typename std::remove_const<
                    typename std::remove_reference<rrc0_type>::type
                >::type
            >::evaluate_category(rrc0)
        );
        BOOST_TEST((
            test::passed_by_lvalue_reference_to_const == test::A<
                typename std::remove_const<
                    typename boost::parameter::value_type<
                        Args
                      , test::kw::rr0
                    >::type
                >::type
            >::evaluate_category(args[test::_rr0])
        ));
        BOOST_TEST_EQ(
            test::passed_by_lvalue_reference_to_const
          , test::A<
                typename std::remove_const<
                    typename std::remove_reference<rr0_type>::type
                >::type
            >::evaluate_category(rr0)
        );
#endif // BOOST_NO_CXX11_HDR_TYPE_TRAITS
#endif // BOOST_PARAMETER_HAS_PERFECT_FORWARDING

        return true;
    }
} // namespace test

#if !defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#endif

#if !defined(BOOST_NO_SFINAE)
#include <boost/tti/detail/dnullptr.hpp>
#include <boost/core/enable_if.hpp>

#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
#include <boost/type_traits/is_base_of.hpp>
#endif
#endif

namespace test {

    struct B
    {
        static char const* baz;

        template <class Args>
        explicit B(
            Args const& args
#if !defined(BOOST_NO_SFINAE)
          , typename boost::disable_if<
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
                boost::is_base_of<B,Args>
#else
                typename boost::mpl::if_<
                    std::is_base_of<B,Args>
                  , boost::mpl::true_
                  , boost::mpl::false_
                >::type
#endif
            >::type* = BOOST_TTI_DETAIL_NULLPTR
#endif // BOOST_NO_SFINAE
        )
        {
            test::evaluate(
                args[test::_lrc0]
              , args[test::_lr0]
              , args[test::_rrc0]
              , args[test::_rr0]
            );
        }

#if BOOST_WORKAROUND(__SUNPRO_CC, BOOST_TESTED_AT(0x580))
        typedef test::string_predicate<test::kw::lr0> rr0_pred;

        BOOST_PARAMETER_MEMBER_FUNCTION((bool), static evaluate, kw,
            (required
                (lrc0, (char))
            )
            (deduced
                (optional
                    (rrc0, (float), 0.0f)
                    (lr0, (char const*), baz)
                    (rr0, *(rr0_pred), std::string(lr0))
                )
            )
        )
#else // !BOOST_WORKAROUND(__SUNPRO_CC, BOOST_TESTED_AT(0x580))
        BOOST_PARAMETER_MEMBER_FUNCTION((bool), static evaluate, kw,
            (required
                (lrc0, (char))
            )
            (deduced
                (optional
                    (rrc0, (float), 0.0f)
                    (lr0, (char const*), baz)
                    (rr0
                      , *(test::string_predicate<test::kw::lr0>)
                      , std::string(lr0)
                    )
                )
            )
        )
#endif // SunPro CC workarounds needed.
        {
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
            BOOST_TEST((
                test::passed_by_lvalue_reference_to_const == test::A<
                    typename boost::remove_const<
                        typename boost::parameter::value_type<
                            Args
                          , test::kw::lrc0
                        >::type
                    >::type
                >::evaluate_category(args[test::_lrc0])
            ));
            BOOST_TEST_EQ(
                test::passed_by_lvalue_reference_to_const
              , test::A<
                    typename boost::remove_const<
                        typename boost::remove_reference<lrc0_type>::type
                    >::type
                >::evaluate_category(lrc0)
            );
            BOOST_TEST((
                test::passed_by_lvalue_reference == test::A<
                    typename boost::remove_const<
                        typename boost::parameter::value_type<
                            Args
                          , test::kw::lr0
                        >::type
                    >::type
                >::evaluate_category(args[test::_lr0])
            ));
            BOOST_TEST_EQ(
                test::passed_by_lvalue_reference
              , test::A<
                    typename boost::remove_const<
                        typename boost::remove_reference<lr0_type>::type
                    >::type
                >::evaluate_category(lr0)
            );
#else // !defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
            BOOST_TEST((
                test::passed_by_lvalue_reference_to_const == test::A<
                    typename std::remove_const<
                        typename boost::parameter::value_type<
                            Args
                          , test::kw::lrc0
                        >::type
                    >::type
                >::evaluate_category(args[test::_lrc0])
            ));
            BOOST_TEST_EQ(
                test::passed_by_lvalue_reference_to_const
              , test::A<
                    typename std::remove_const<
                        typename std::remove_reference<lrc0_type>::type
                    >::type
                >::evaluate_category(lrc0)
            );
            BOOST_TEST((
                test::passed_by_lvalue_reference == test::A<
                    typename std::remove_const<
                        typename boost::parameter::value_type<
                            Args
                          , test::kw::lr0
                        >::type
                    >::type
                >::evaluate_category(args[test::_lr0])
            ));
            BOOST_TEST_EQ(
                test::passed_by_lvalue_reference
              , test::A<
                    typename std::remove_const<
                        typename std::remove_reference<lr0_type>::type
                    >::type
                >::evaluate_category(lr0)
            );
#endif // BOOST_NO_CXX11_HDR_TYPE_TRAITS

#if defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
            BOOST_TEST((
                test::passed_by_rvalue_reference == test::A<
                    typename boost::remove_const<
                        typename boost::parameter::value_type<
                            Args
                          , test::kw::rrc0
                        >::type
                    >::type
                >::evaluate_category(args[test::_rrc0])
            ));
            BOOST_TEST_EQ(
                test::passed_by_rvalue_reference
              , test::A<
                    typename boost::remove_const<
                        typename boost::remove_reference<rrc0_type>::type
                    >::type
                >::evaluate_category(boost::forward<rrc0_type>(rrc0))
            );
            BOOST_TEST((
                test::passed_by_rvalue_reference == test::A<
                    typename boost::remove_const<
                        typename boost::parameter::value_type<
                            Args
                          , test::kw::rr0
                        >::type
                    >::type
                >::evaluate_category(args[test::_rr0])
            ));
            BOOST_TEST_EQ(
                test::passed_by_rvalue_reference
              , test::A<
                    typename boost::remove_const<
                        typename boost::remove_reference<rr0_type>::type
                    >::type
                >::evaluate_category(boost::forward<rr0_type>(rr0))
            );
#else // !defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
            BOOST_TEST((
                test::passed_by_rvalue_reference == test::A<
                    typename std::remove_const<
                        typename boost::parameter::value_type<
                            Args
                          , test::kw::rrc0
                        >::type
                    >::type
                >::evaluate_category(args[test::_rrc0])
            ));
            BOOST_TEST_EQ(
                test::passed_by_rvalue_reference
              , test::A<
                    typename std::remove_const<
                        typename std::remove_reference<rrc0_type>::type
                    >::type
                >::evaluate_category(boost::forward<rrc0_type>(rrc0))
            );
            BOOST_TEST((
                test::passed_by_rvalue_reference == test::A<
                    typename std::remove_const<
                        typename boost::parameter::value_type<
                            Args
                          , test::kw::rr0
                        >::type
                    >::type
                >::evaluate_category(args[test::_rr0])
            ));
            BOOST_TEST_EQ(
                test::passed_by_rvalue_reference
              , test::A<
                    typename std::remove_const<
                        typename std::remove_reference<rr0_type>::type
                    >::type
                >::evaluate_category(boost::forward<rr0_type>(rr0))
            );
#endif // BOOST_NO_CXX11_HDR_TYPE_TRAITS
#else // !defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
            BOOST_TEST((
                test::passed_by_lvalue_reference_to_const == test::A<
                    typename boost::remove_const<
                        typename boost::parameter::value_type<
                            Args
                          , test::kw::rrc0
                        >::type
                    >::type
                >::evaluate_category(args[test::_rrc0])
            ));
            BOOST_TEST_EQ(
                test::passed_by_lvalue_reference_to_const
              , test::A<
                    typename boost::remove_const<
                        typename boost::remove_reference<rrc0_type>::type
                    >::type
                >::evaluate_category(rrc0)
            );
            BOOST_TEST((
                test::passed_by_lvalue_reference_to_const == test::A<
                    typename boost::remove_const<
                        typename boost::parameter::value_type<
                            Args
                          , test::kw::rr0
                        >::type
                    >::type
                >::evaluate_category(args[test::_rr0])
            ));
            BOOST_TEST_EQ(
                test::passed_by_lvalue_reference_to_const
              , test::A<
                    typename boost::remove_const<
                        typename boost::remove_reference<rr0_type>::type
                    >::type
                >::evaluate_category(rr0)
            );
#else // !defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
            BOOST_TEST((
                test::passed_by_lvalue_reference_to_const == test::A<
                    typename std::remove_const<
                        typename boost::parameter::value_type<
                            Args
                          , test::kw::rrc0
                        >::type
                    >::type
                >::evaluate_category(args[test::_rrc0])
            ));
            BOOST_TEST_EQ(
                test::passed_by_lvalue_reference_to_const
              , test::A<
                    typename std::remove_const<
                        typename std::remove_reference<rrc0_type>::type
                    >::type
                >::evaluate_category(rrc0)
            );
            BOOST_TEST((
                test::passed_by_lvalue_reference_to_const == test::A<
                    typename std::remove_const<
                        typename boost::parameter::value_type<
                            Args
                          , test::kw::rr0
                        >::type
                    >::type
                >::evaluate_category(args[test::_rr0])
            ));
            BOOST_TEST_EQ(
                test::passed_by_lvalue_reference_to_const
              , test::A<
                    typename std::remove_const<
                        typename std::remove_reference<rr0_type>::type
                    >::type
                >::evaluate_category(rr0)
            );
#endif // BOOST_NO_CXX11_HDR_TYPE_TRAITS
#endif // BOOST_PARAMETER_HAS_PERFECT_FORWARDING

            return true;
        }
    };

    char const* B::baz = "baz";
} // namespace test

#include <boost/mpl/placeholders.hpp>

namespace test {

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

#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
        typedef boost::is_convertible<boost::mpl::_,std::bitset<1> > bs0_pred;
        typedef boost::is_convertible<boost::mpl::_,std::bitset<2> > bs1_pred;
        typedef boost::is_convertible<boost::mpl::_,std::bitset<3> > bs2_pred;
        typedef boost::is_convertible<boost::mpl::_,std::bitset<4> > bs3_pred;
        typedef boost::is_convertible<boost::mpl::_,std::bitset<5> > bs4_pred;
        typedef boost::is_convertible<boost::mpl::_,std::bitset<6> > bs5_pred;
        typedef boost::is_convertible<boost::mpl::_,std::bitset<7> > bs6_pred;
        typedef boost::is_convertible<boost::mpl::_,std::bitset<8> > bs7_pred;
#else
        typedef boost::mpl::if_<
            std::is_convertible<boost::mpl::_,std::bitset<1> >
          , boost::mpl::true_
          , boost::mpl::false_
        > bs0_pred;
        typedef boost::mpl::if_<
            std::is_convertible<boost::mpl::_,std::bitset<2> >
          , boost::mpl::true_
          , boost::mpl::false_
        > bs1_pred;
        typedef boost::mpl::if_<
            std::is_convertible<boost::mpl::_,std::bitset<3> >
          , boost::mpl::true_
          , boost::mpl::false_
        > bs2_pred;
        typedef boost::mpl::if_<
            std::is_convertible<boost::mpl::_,std::bitset<4> >
          , boost::mpl::true_
          , boost::mpl::false_
        > bs3_pred;
        typedef boost::mpl::if_<
            std::is_convertible<boost::mpl::_,std::bitset<5> >
          , boost::mpl::true_
          , boost::mpl::false_
        > bs4_pred;
        typedef boost::mpl::if_<
            std::is_convertible<boost::mpl::_,std::bitset<6> >
          , boost::mpl::true_
          , boost::mpl::false_
        > bs5_pred;
        typedef boost::mpl::if_<
            std::is_convertible<boost::mpl::_,std::bitset<7> >
          , boost::mpl::true_
          , boost::mpl::false_
        > bs6_pred;
        typedef boost::mpl::if_<
            std::is_convertible<boost::mpl::_,std::bitset<8> >
          , boost::mpl::true_
          , boost::mpl::false_
        > bs7_pred;
#endif // BOOST_NO_CXX11_HDR_TYPE_TRAITS

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
#if defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)
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
#else // !defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)
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

    char baz_arr[4] = "qux";
    typedef char char_arr[4];

#if defined(BOOST_MSVC)// && (BOOST_MSVC >= 1910) && (BOOST_MSVC < 1912)
    // MSVC 14.1 on AppVeyor treats static_cast<char_arr&&>(baz_arr)
    // as an lvalue.
#else
    test::evaluate(
        "q2x"
      , baz_arr
#if defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)
      , static_cast<char_arr const&&>("mos")
      , static_cast<char_arr&&>(baz_arr)
#else
      , "crg"
      , "uir"
#endif
    );
    test::evaluate(
        test::_lr0 = baz_arr
#if defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)
      , test::_rrc0 = static_cast<char_arr const&&>("def")
      , test::_rr0 = static_cast<char_arr&&>(baz_arr)
#else
      , test::_rrc0 = "grl"
      , test::_rr0 = "grp"
#endif
      , test::_lrc0 = "wld"
    );
#endif // MSVC 14.1
    test::B::evaluate(test::lvalue_const_str()[0]);
    test::B::evaluate(
        test::lvalue_const_str()[0]
      , test::rvalue_const_float()
    );
    test::B::evaluate(
        test::lvalue_const_str()[0]
      , test::lvalue_char_ptr()
      , test::rvalue_str()
      , test::rvalue_const_float()
    );
    test::B::evaluate(
        test::lvalue_const_str()[0]
      , test::rvalue_str()
      , test::rvalue_const_float()
      , test::lvalue_char_ptr()
    );

    test::C cp0(
        "frd"
      , baz_arr
#if defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)
      , static_cast<char_arr const&&>("dfs")
      , static_cast<char_arr&&>(baz_arr)
#else
      , "plg"
      , "thd"
#endif
    );
    test::C cp1(
        test::_lr0 = baz_arr
#if defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)
      , test::_rrc0 = static_cast<char_arr const&&>("dgx")
      , test::_rr0 = static_cast<char_arr&&>(baz_arr)
#else
      , test::_rrc0 = "hnk"
      , test::_rr0 = "xzz"
#endif
      , test::_lrc0 = "zts"
    );

    cp0(
        test::lvalue_const_bitset<4>()
      , test::lvalue_const_bitset<0>()
      , test::lvalue_bitset<1>()
      , test::rvalue_const_bitset<2>()
      , test::rvalue_bitset<3>()
    );
    cp0(
        test::lvalue_const_bitset<4>()
      , test::lvalue_const_bitset<0>()
      , test::rvalue_const_bitset<6>()
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

