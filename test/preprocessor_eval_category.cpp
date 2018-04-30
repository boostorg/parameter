// Copyright Cromwell D. Enage 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/parameter/config.hpp>

#if !defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)
#if (BOOST_PARAMETER_MAX_ARITY < 4)
#error Define BOOST_PARAMETER_MAX_ARITY as 4 or greater.
#endif
#if (BOOST_PARAMETER_ALL_CONST_THRESHOLD_ARITY < 5)
#error Define BOOST_PARAMETER_ALL_CONST_THRESHOLD_ARITY as 5 or greater.
#endif
#endif

#if !defined(BOOST_GCC) || ( \
        defined(__MINGW32__) && (1 == __MINGW32__) \
    ) || BOOST_WORKAROUND(BOOST_GCC, < 40800) || ( \
        defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING) && \
        BOOST_WORKAROUND(BOOST_GCC, >= 40900) \
    )
#define LIBS_PARAMETER_TEST_WILL_NOT_ICE
#endif

#include <boost/core/lightweight_test.hpp>
#include <boost/config/pragma_message.hpp>

#if defined LIBS_PARAMETER_TEST_WILL_NOT_ICE

#include <boost/parameter.hpp>

namespace test {

    BOOST_PARAMETER_NAME((_lrc0, kw) in(lrc0))
    BOOST_PARAMETER_NAME((_lr0, kw) in_out(lr0))
    BOOST_PARAMETER_NAME((_rrc0, kw) in(rrc0))
#if defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)
    BOOST_PARAMETER_NAME((_rr0, kw) consume(rr0))
#else
    BOOST_PARAMETER_NAME((_rr0, kw) rr0)
#endif
} // namespace test

#include "evaluate_category.hpp"

#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
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
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
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
#else // !defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
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
#endif // BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS

#if defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)
        if (
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
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
#endif // BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS
        )
        {
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
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
                >::evaluate_category(boost::forward<rrc0_type>(rrc0))
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
                >::evaluate_category(boost::forward<rr0_type>(rr0))
            );
#else // !defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
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
                >::evaluate_category(boost::forward<rrc0_type>(rrc0))
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
                >::evaluate_category(boost::forward<rr0_type>(rr0))
            );
#endif // BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS
        }
        else // rrc0's value type isn't scalar
        {
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
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
#else // !defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
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
#endif // BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS
        }
#else // !defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
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
#else // !defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
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
#endif // BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS
#endif // BOOST_PARAMETER_HAS_PERFECT_FORWARDING

        return true;
    }
} // namespace test

#if !defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#endif

#if !defined(BOOST_NO_SFINAE)
#include <boost/tti/detail/dnullptr.hpp>
#include <boost/core/enable_if.hpp>

#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
#include <boost/type_traits/is_base_of.hpp>
#endif
#endif

namespace test {

    char const* baz = "baz";

    struct B
    {
#if BOOST_WORKAROUND(BOOST_MSVC, >= 1910) && \
    BOOST_WORKAROUND(BOOST_MSVC, < 1920)
        B()
        {
        }
#endif

        template <typename Args>
        explicit B(
            Args const& args
#if !defined(BOOST_NO_SFINAE)
          , typename boost::disable_if<
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
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
                    (lr0, (char const*), test::baz)
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
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
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
#else // !defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
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
#endif // BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS

#if defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
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
#else // !defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
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
#endif // BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS
#else // !defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
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
#else // !defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
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
#endif // BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS
#endif // BOOST_PARAMETER_HAS_PERFECT_FORWARDING

            return true;
        }
    };

    struct C : B
    {
#if BOOST_WORKAROUND(BOOST_MSVC, >= 1910) && \
    BOOST_WORKAROUND(BOOST_MSVC, < 1920)
        C() : B()
        {
        }
#endif

        BOOST_PARAMETER_CONSTRUCTOR(C, (B), kw,
            (required
                (lrc0, *)
                (lr0, *)
                (rrc0, *)
                (rr0, *)
            )
        )
    };
} // namespace test

BOOST_PRAGMA_MESSAGE("Test should compile.");
#else
BOOST_PRAGMA_MESSAGE("Test not compiled.");
#endif // Compiler won't ICE.

int main()
{
#if defined LIBS_PARAMETER_TEST_WILL_NOT_ICE
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

#if BOOST_WORKAROUND(BOOST_MSVC, >= 1910) && \
    BOOST_WORKAROUND(BOOST_MSVC, < 1920)
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
    test::C::evaluate(
        test::lvalue_const_str()[0]
      , test::rvalue_const_float()
    );

#if BOOST_WORKAROUND(BOOST_MSVC, >= 1910) && \
    BOOST_WORKAROUND(BOOST_MSVC, < 1920)
    // MSVC 14.1 on AppVeyor treats static_cast<char_arr&&>(baz_arr)
    // as an lvalue.
    test::C cp0;
    test::C cp1;
#else
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
#endif // MSVC 14.1

    cp0.evaluate(
        test::lvalue_const_str()[0]
      , test::lvalue_char_ptr()
      , test::rvalue_str()
      , test::rvalue_const_float()
    );
    cp1.evaluate(
        test::lvalue_const_str()[0]
      , test::rvalue_str()
      , test::rvalue_const_float()
      , test::lvalue_char_ptr()
    );
#endif // Compiler won't ICE.

    return boost::report_errors();
}

