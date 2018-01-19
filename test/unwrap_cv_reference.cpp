// Copyright David Abrahams 2005.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/parameter/aux_/unwrap_cv_reference.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/aux_/test.hpp>

MPL_TEST_CASE()
{
    BOOST_MPL_ASSERT((
        boost::is_same<
            boost::parameter::aux::unwrap_cv_reference<int>::type
          , int
        >
    ));
    BOOST_MPL_ASSERT((
        boost::is_same<
            boost::parameter::aux::unwrap_cv_reference<int const>::type
          , int const
        >
    ));
    BOOST_MPL_ASSERT((
        boost::is_same<
            boost::parameter::aux::unwrap_cv_reference<int volatile>::type
          , int volatile
        >
    ));
    BOOST_MPL_ASSERT((
        boost::is_same<
            boost::parameter::aux::unwrap_cv_reference<
                int const volatile
            >::type
          , int const volatile
        >
    ));
}

namespace test {

    struct foo
    {
    };
} // namespace test

MPL_TEST_CASE()
{
    BOOST_MPL_ASSERT((
        boost::is_same<
            boost::parameter::aux::unwrap_cv_reference<test::foo>::type
          , test::foo
        >
    ));
    BOOST_MPL_ASSERT((
        boost::is_same<
            boost::parameter::aux::unwrap_cv_reference<test::foo const>::type
          , test::foo const
        >
    ));
    BOOST_MPL_ASSERT((
        boost::is_same<
            boost::parameter::aux::unwrap_cv_reference<
                test::foo volatile
            >::type
          , test::foo volatile
        >
    ));
    BOOST_MPL_ASSERT((
        boost::is_same<
            boost::parameter::aux::unwrap_cv_reference<
                test::foo const volatile
            >::type
          , test::foo const volatile
        >
    ));
}

#include <boost/ref.hpp>

MPL_TEST_CASE()
{
    BOOST_MPL_ASSERT((
        boost::is_same<
            boost::parameter::aux::unwrap_cv_reference<
                boost::reference_wrapper<test::foo>
            >::type
          , test::foo
        >
    ));
    BOOST_MPL_ASSERT((
        boost::is_same<
            boost::parameter::aux::unwrap_cv_reference<
                boost::reference_wrapper<test::foo> const
            >::type
          , test::foo
        >
    ));
    BOOST_MPL_ASSERT((
        boost::is_same<
            boost::parameter::aux::unwrap_cv_reference<
                boost::reference_wrapper<test::foo> volatile
            >::type
          , test::foo
        >
    ));
    BOOST_MPL_ASSERT((
        boost::is_same<
            boost::parameter::aux::unwrap_cv_reference<
                boost::reference_wrapper<test::foo> const volatile
            >::type
          , test::foo
        >
    ));
}

