// Copyright Cromwell D. Enage 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/parameter.hpp>

namespace test {

    BOOST_PARAMETER_NAME(a0)
    BOOST_PARAMETER_NAME(a1)
    BOOST_PARAMETER_NAME(a2)
    BOOST_PARAMETER_NAME(a3)

    struct f_parameters
      : boost::parameter::parameters<
            tag::a0
          , tag::a1
          , tag::a2
          , tag::a3
        >
    {
    };
} // namespace test

#include <boost/type_traits/is_scalar.hpp>
#include <boost/detail/lightweight_test.hpp>
#include "evaluate_category.hpp"

namespace test {

    template <typename T>
    struct B
    {
        template <typename Args>
        static void evaluate(Args const& args)
        {
            BOOST_TEST(
                passed_by_lvalue_reference_to_const == A<
                    T
                >::evaluate_category(args[_a0])
            );
            BOOST_TEST(
                passed_by_lvalue_reference == A<
                    T
                >::evaluate_category(args[_a1])
            );

#if defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
            if (boost::is_scalar<T>::value)
            {
                BOOST_TEST(
                    passed_by_rvalue_reference == A<
                        T
                    >::evaluate_category(args[_a2])
                );
            }
            else
            {
                BOOST_TEST(
                    passed_by_rvalue_reference_to_const == A<
                        T
                    >::evaluate_category(args[_a2])
                );
            }

            BOOST_TEST(
                passed_by_rvalue_reference == A<
                    T
                >::evaluate_category(args[_a3])
            );
#else // !defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
            BOOST_TEST(
                passed_by_lvalue_reference_to_const == A<
                    T
                >::evaluate_category(args[_a2])
            );
            BOOST_TEST(
                passed_by_lvalue_reference_to_const == A<
                    T
                >::evaluate_category(args[_a3])
            );
#endif // BOOST_PARAMETER_HAS_PERFECT_FORWARDING
        }
    };
} // namespace test

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
        test::_a1 = test::lvalue_float()
      , test::_a2 = test::rvalue_const_float()
      , test::_a3 = test::rvalue_float()
      , test::_a0 = test::lvalue_const_float()
    ));
    test::B<char const*>::evaluate((
        test::_a1 = test::lvalue_char_ptr()
      , test::_a2 = test::rvalue_const_char_ptr()
      , test::_a3 = test::rvalue_char_ptr()
      , test::_a0 = test::lvalue_const_char_ptr()
    ));
    test::B<std::string>::evaluate((
        test::_a1 = test::lvalue_str()
      , test::_a2 = test::rvalue_const_str()
      , test::_a3 = test::rvalue_str()
      , test::_a0 = test::lvalue_const_str()
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
        test::_a1 = baz_arr
#if defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
      , test::_a2 = static_cast<char_arr const&&>("baz")
      , test::_a3 = static_cast<char_arr&&>(baz_arr)
#else
      , test::_a2 = "baz"
      , test::_a3 = "baz"
#endif
      , test::_a0 = "baz"
    ));
    return boost::report_errors();
}

