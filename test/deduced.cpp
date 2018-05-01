// Copyright Daniel Wallin 2006.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/parameter/config.hpp>

#if !defined(LIBS_PARAMETER_TEST_WILL_NOT_ICE)
/*
#if defined(LIBS_PARAMETER_TEST_COMPILE_FAILURE) || !defined(BOOST_GCC) || ( \
        defined(__MINGW32__) && (1 == __MINGW32__) \
    ) || BOOST_WORKAROUND(BOOST_GCC, < 40800) || ( \
        defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING) && \
        BOOST_WORKAROUND(BOOST_GCC, >= 40900) \
    )
*/
#define LIBS_PARAMETER_TEST_WILL_NOT_ICE
//#endif
#endif

#include <boost/config/pragma_message.hpp>

#if defined(LIBS_PARAMETER_TEST_WILL_NOT_ICE)

#include <boost/parameter/parameters.hpp>
#include <boost/parameter/name.hpp>
#include <boost/parameter/binding.hpp>
#include "deduced.hpp"

#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
#include <boost/type_traits/is_convertible.hpp>
#else
#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <type_traits>
#endif

#if defined(LIBS_PARAMETER_TEST_COMPILE_FAILURE)
#include <boost/tti/detail/dnullptr.hpp>
#endif

namespace test {

    BOOST_PARAMETER_NAME(x)
    BOOST_PARAMETER_NAME(y)
    BOOST_PARAMETER_NAME(z)

    template <typename To>
    struct predicate
    {
        template <typename From, typename Args>
        struct apply
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
          : boost::is_convertible<From,To>
#else
          : boost::mpl::if_<
                std::is_convertible<From,To>
              , boost::mpl::true_
              , boost::mpl::false_
            >
#endif
        {
        };
    };
} // namespace test

#else
BOOST_PRAGMA_MESSAGE("Test not compiled.");
#include <boost/core/lightweight_test.hpp>
#endif // Compiler won't ICE.

int main()
{
#if defined(LIBS_PARAMETER_TEST_WILL_NOT_ICE)
    test::check<
        boost::parameter::parameters<test::tag::x,test::tag::y>
    >((test::_x = 0, test::_y = 1), 0, 1);

    test::check<
        boost::parameter::parameters<
            test::tag::x
          , boost::parameter::required<
                boost::parameter::deduced<test::tag::y>
              , test::predicate<int>
            >
          , boost::parameter::optional<
                boost::parameter::deduced<test::tag::z>
              , test::predicate<char const*>
            >
        >
    >(
        (test::_x = 0, test::_y = test::not_present, test::_z = "foo")
      , test::_x = 0
      , "foo"
    );

    test::check<
        boost::parameter::parameters<
            test::tag::x
          , boost::parameter::required<
                boost::parameter::deduced<test::tag::y>
              , test::predicate<int>
            >
          , boost::parameter::optional<
                boost::parameter::deduced<test::tag::z>
              , test::predicate<char const*>
            >
        >
    >((test::_x = 0, test::_y = 1, test::_z = "foo"), 0, "foo", 1);

    test::check<
        boost::parameter::parameters<
            test::tag::x
          , boost::parameter::required<
                boost::parameter::deduced<test::tag::y>
              , test::predicate<int>
            >
          , boost::parameter::optional<
                boost::parameter::deduced<test::tag::z>
              , test::predicate<char const*>
            >
        >
    >((test::_x = 0, test::_y = 1, test::_z = "foo"), 0, 1, "foo");

    test::check<
        boost::parameter::parameters<
            test::tag::x
          , boost::parameter::required<
                boost::parameter::deduced<test::tag::y>
              , test::predicate<int>
            >
          , boost::parameter::optional<
                boost::parameter::deduced<test::tag::z>
              , test::predicate<char const*>
            >
        >
    >((test::_x = 0, test::_y = 1, test::_z = "foo"), 0, test::_y = 1, "foo");

    test::check<
        boost::parameter::parameters<
            test::tag::x
          , boost::parameter::required<
                boost::parameter::deduced<test::tag::y>
              , test::predicate<int>
            >
          , boost::parameter::optional<
                boost::parameter::deduced<test::tag::z>
              , test::predicate<char const*>
            >
        >
    >(
        (test::_x = 0, test::_y = 1, test::_z = "foo")
      , test::_z = "foo"
      , test::_x = 0
      , 1
    );

#if defined(LIBS_PARAMETER_TEST_COMPILE_FAILURE)
    // Fails because boost::parameter::aux::make_arg_list<> evaluates
    // boost::parameter::aux::is_named_argument<> to boost::mpl::false_
    // for static_cast<long*>(BOOST_TTI_DETAIL_NULLPTR).
    test::check<
        boost::parameter::parameters<
            test::tag::x
          , boost::parameter::required<
                boost::parameter::deduced<test::tag::y>
              , test::predicate<int>
            >
          , boost::parameter::optional<
                boost::parameter::deduced<test::tag::z>
              , test::predicate<char const*>
            >
        >
    >(
        (test::_x = 0, test::_y = 1, test::_z = "foo")
      , test::_x = 0
      , static_cast<long*>(BOOST_TTI_DETAIL_NULLPTR)
      , 1
    );
#endif
#endif // Compiler won't ICE.

    return boost::report_errors();
}

