// Copyright Cromwell D. Enage 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/parameter.hpp>

namespace test {

    BOOST_PARAMETER_NAME(lrc)
    BOOST_PARAMETER_NAME(lr)
    BOOST_PARAMETER_NAME(rrc_in)
    BOOST_PARAMETER_NAME(rrc)
    BOOST_PARAMETER_NAME(rr)
} // namespace test

#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/is_scalar.hpp>
#include <boost/detail/lightweight_test.hpp>
#include "evaluate_category.hpp"

namespace test {

    BOOST_PARAMETER_BASIC_FUNCTION((bool), evaluate, tag,
        (required
            (lrc, *)
            (out(lr), *)
            (rrc_in, *)
            (out(rrc), *)
            (out(rr), *)
        )
    )
    {
        BOOST_TEST((
            passed_by_lvalue_reference_to_const == A<
                typename boost::remove_const<
                    typename boost::parameter::value_type<
                        Args
                      , tag::lrc
                    >::type
                >::type
            >::evaluate_category(args[_lrc])
        ));
        BOOST_TEST((
            passed_by_lvalue_reference == A<
                typename boost::remove_const<
                    typename boost::parameter::value_type<
                        Args
                      , tag::lr
                    >::type
                >::type
            >::evaluate_category(args[_lr])
        ));
        BOOST_TEST((
            passed_by_lvalue_reference_to_const == A<
                typename boost::remove_const<
                    typename boost::parameter::value_type<
                        Args
                      , tag::lrc
                    >::type
                >::type
            >::evaluate_category(args[_rrc_in])
        ));

#if defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
        if (
            boost::is_scalar<
                typename boost::remove_const<
                    typename boost::parameter::value_type<
                        Args
                      , tag::rrc
                    >::type
                >::type
            >::value
        )
        {
            BOOST_TEST((
                passed_by_rvalue_reference == A<
                    typename boost::remove_const<
                        typename boost::parameter::value_type<
                            Args
                          , tag::rrc
                        >::type
                    >::type
                >::evaluate_category(args[_rrc])
            ));
        }
        else
        {
            BOOST_TEST((
                passed_by_rvalue_reference_to_const == A<
                    typename boost::remove_const<
                        typename boost::parameter::value_type<
                            Args
                          , tag::rrc
                        >::type
                    >::type
                >::evaluate_category(args[_rrc])
            ));
        }

        BOOST_TEST((
            passed_by_rvalue_reference == A<
                typename boost::remove_const<
                    typename boost::parameter::value_type<
                        Args
                      , tag::rr
                    >::type
                >::type
            >::evaluate_category(args[_rr])
        ));
#else // !defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
        BOOST_TEST((
            passed_by_lvalue_reference_to_const == A<
                typename boost::remove_const<
                    typename boost::parameter::value_type<
                        Args
                      , tag::rrc
                    >::type
                >::type
            >::evaluate_category(args[_rrc])
        ));
        BOOST_TEST((
            passed_by_lvalue_reference_to_const == A<
                typename boost::remove_const<
                    typename boost::parameter::value_type<
                        Args
                      , tag::rr
                    >::type
                >::type
            >::evaluate_category(args[_rr])
        ));
#endif // BOOST_PARAMETER_HAS_PERFECT_FORWARDING

        return true;
    }

    struct B
    {
        template <typename Args>
        explicit B(Args const& args)
        {
            evaluate(
                args[_lrc], args[_lr], args[_rrc_in], args[_rrc], args[_rr]
            );
        }
    };

    struct C : B
    {
        BOOST_PARAMETER_CONSTRUCTOR(C, (B), tag,
            (required
                (lrc, *)
                (in_out(lr), *)
                (rrc_in, *)
                (in_out(rrc), *)
                (in_out(rr), *)
            )
        )
    };
} // namespace test

int main()
{
    test::evaluate(
        test::lvalue_const_float()
      , test::lvalue_float()
      , test::rvalue_const_float()
      , test::rvalue_const_float()
      , test::rvalue_float()
    );
    test::evaluate(
        test::lvalue_const_char_ptr()
      , test::lvalue_char_ptr()
      , test::rvalue_const_char_ptr()
      , test::rvalue_const_char_ptr()
      , test::rvalue_char_ptr()
    );
    test::evaluate(
        test::lvalue_const_str()
      , test::lvalue_str()
      , test::rvalue_const_str()
      , test::rvalue_const_str()
      , test::rvalue_str()
    );
    test::evaluate(
        test::_lr = test::lvalue_float()
      , test::_rrc_in = test::rvalue_const_float()
      , test::_rrc = test::rvalue_const_float()
      , test::_rr = test::rvalue_float()
      , test::_lrc = test::lvalue_const_float()
    );
    test::evaluate(
        test::_lr = test::lvalue_char_ptr()
      , test::_rrc_in = test::rvalue_const_char_ptr()
      , test::_rrc = test::rvalue_const_char_ptr()
      , test::_rr = test::rvalue_char_ptr()
      , test::_lrc = test::lvalue_const_char_ptr()
    );
    test::evaluate(
        test::_lr = test::lvalue_str()
      , test::_rrc_in = test::rvalue_const_str()
      , test::_rrc = test::rvalue_const_str()
      , test::_rr = test::rvalue_str()
      , test::_lrc = test::lvalue_const_str()
    );

    test::C cf0(
        test::lvalue_const_float()
      , test::lvalue_float()
      , test::rvalue_const_float()
      , test::rvalue_const_float()
      , test::rvalue_float()
    );
    test::C cc0(
        test::lvalue_const_char_ptr()
      , test::lvalue_char_ptr()
      , test::rvalue_const_char_ptr()
      , test::rvalue_const_char_ptr()
      , test::rvalue_char_ptr()
    );
    test::C cs0(
        test::lvalue_const_str()
      , test::lvalue_str()
      , test::rvalue_const_str()
      , test::rvalue_const_str()
      , test::rvalue_str()
    );
    test::C cf1(
        test::_lr = test::lvalue_float()
      , test::_rrc_in = test::rvalue_const_float()
      , test::_rrc = test::rvalue_const_float()
      , test::_rr = test::rvalue_float()
      , test::_lrc = test::lvalue_const_float()
    );
    test::C cc1(
        test::_lr = test::lvalue_char_ptr()
      , test::_rrc_in = test::rvalue_const_char_ptr()
      , test::_rrc = test::rvalue_const_char_ptr()
      , test::_rr = test::rvalue_char_ptr()
      , test::_lrc = test::lvalue_const_char_ptr()
    );
    test::C cs1(
        test::_lr = test::lvalue_str()
      , test::_rrc_in = test::rvalue_const_str()
      , test::_rrc = test::rvalue_const_str()
      , test::_rr = test::rvalue_str()
      , test::_lrc = test::lvalue_const_str()
    );

    char baz_arr[4] = "baz";
    typedef char char_arr[4];

    test::evaluate(
        "baz"
      , baz_arr
#if defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
      , static_cast<char_arr const&&>("baz")
      , static_cast<char_arr const&&>("baz")
      , static_cast<char_arr&&>(baz_arr)
#else
      , "baz"
      , "baz"
      , "baz"
#endif
    );
    test::evaluate(
        test::_lr = baz_arr
#if defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
      , test::_rrc_in = static_cast<char_arr const&&>("baz")
      , test::_rrc = static_cast<char_arr const&&>("baz")
      , test::_rr = static_cast<char_arr&&>(baz_arr)
#else
      , test::_rrc_in = "baz"
      , test::_rrc = "baz"
      , test::_rr = "baz"
#endif
      , test::_lrc = "baz"
    );

    test::C cp0(
        "baz"
      , baz_arr
#if defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
      , static_cast<char_arr const&&>("baz")
      , static_cast<char_arr const&&>("baz")
      , static_cast<char_arr&&>(baz_arr)
#else
      , "baz"
      , "baz"
      , "baz"
#endif
    );
    test::C cp1(
        test::_lr = baz_arr
#if defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
      , test::_rrc_in = static_cast<char_arr const&&>("baz")
      , test::_rrc = static_cast<char_arr const&&>("baz")
      , test::_rr = static_cast<char_arr&&>(baz_arr)
#else
      , test::_rrc_in = "baz"
      , test::_rrc = "baz"
      , test::_rr = "baz"
#endif
      , test::_lrc = "baz"
    );

    return boost::report_errors();
}

