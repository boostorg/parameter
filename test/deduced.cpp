// Copyright Daniel Wallin 2006.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/parameter/parameters.hpp>
#include <boost/parameter/name.hpp>
#include <boost/parameter/binding.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include "deduced.hpp"

#if defined LIBS_PARAMETER_TEST_COMPILE_FAILURE
#include <boost/tti/detail/dnullptr.hpp>
#endif

namespace test {

    BOOST_PARAMETER_NAME(x)
    BOOST_PARAMETER_NAME(y)
    BOOST_PARAMETER_NAME(z)
} // namespace test

int main()
{
    test::check<
        boost::parameter::parameters<test::tag::x,test::tag::y>
    >((test::_x = 0, test::_y = 1), 0, 1);

    test::check<
        boost::parameter::parameters<
            test::tag::x
          , boost::parameter::required<
                boost::parameter::deduced<test::tag::y>
              , boost::is_convertible<boost::mpl::_,int>
            >
          , boost::parameter::optional<
                boost::parameter::deduced<test::tag::z>
              , boost::is_convertible<boost::mpl::_,char const*>
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
              , boost::is_convertible<boost::mpl::_,int>
            >
          , boost::parameter::optional<
                boost::parameter::deduced<test::tag::z>
              , boost::is_convertible<boost::mpl::_,char const*>
            >
        >
    >((test::_x = 0, test::_y = 1, test::_z = "foo"), 0, "foo", 1);

    test::check<
        boost::parameter::parameters<
            test::tag::x
          , boost::parameter::required<
                boost::parameter::deduced<test::tag::y>
              , boost::is_convertible<boost::mpl::_,int>
            >
          , boost::parameter::optional<
                boost::parameter::deduced<test::tag::z>
              , boost::is_convertible<boost::mpl::_,char const*>
            >
        >
    >((test::_x = 0, test::_y = 1, test::_z = "foo"), 0, 1, "foo");

    test::check<
        boost::parameter::parameters<
            test::tag::x
          , boost::parameter::required<
                boost::parameter::deduced<test::tag::y>
              , boost::is_convertible<boost::mpl::_,int>
            >
          , boost::parameter::optional<
                boost::parameter::deduced<test::tag::z>
              , boost::is_convertible<boost::mpl::_,char const*>
            >
        >
    >((test::_x = 0, test::_y = 1, test::_z = "foo"), 0, test::_y = 1, "foo");

    test::check<
        boost::parameter::parameters<
            test::tag::x
          , boost::parameter::required<
                boost::parameter::deduced<test::tag::y>
              , boost::is_convertible<boost::mpl::_,int>
            >
          , boost::parameter::optional<
                boost::parameter::deduced<test::tag::z>
              , boost::is_convertible<boost::mpl::_,char const*>
            >
        >
    >(
        (test::_x = 0, test::_y = 1, test::_z = "foo")
      , test::_z = "foo"
      , test::_x = 0
      , 1
    );

#if defined LIBS_PARAMETER_TEST_COMPILE_FAILURE
    // Fails because boost::parameter::aux::make_arg_list<> evaluates
    // boost::parameter::aux::is_named_argument<> to boost::mpl::false_
    // for static_cast<long*>(BOOST_TTI_DETAIL_NULLPTR).
    test::check<
        boost::parameter::parameters<
            test::tag::x
          , boost::parameter::required<
                boost::parameter::deduced<test::tag::y>
              , boost::is_convertible<boost::mpl::_,int>
            >
          , boost::parameter::optional<
                boost::parameter::deduced<test::tag::z>
              , boost::is_convertible<boost::mpl::_,char const*>
            >
        >
    >(
        (test::_x = 0, test::_y = 1, test::_z = "foo")
      , test::_x = 0
      , static_cast<long*>(BOOST_TTI_DETAIL_NULLPTR)
      , 1
    );
#endif

    return boost::report_errors();
}

