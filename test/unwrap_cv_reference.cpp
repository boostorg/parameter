// Copyright David Abrahams 2005.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/parameter/aux_/unwrap_cv_reference.hpp>
#include <boost/parameter/config.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/aux_/test.hpp>
#include <boost/type_traits/is_same.hpp>

MPL_TEST_CASE()
{
    BOOST_MPL_ASSERT((
        boost::mpl::if_<
            boost::is_same<
                boost::parameter::aux::unwrap_cv_reference<int>::type
              , int
            >
          , boost::mpl::true_
          , boost::mpl::false_
        >::type
    ));
    BOOST_MPL_ASSERT((
        boost::mpl::if_<
            boost::is_same<
                boost::parameter::aux::unwrap_cv_reference<int const>::type
              , int const
            >
          , boost::mpl::true_
          , boost::mpl::false_
        >::type
    ));
    BOOST_MPL_ASSERT((
        boost::mpl::if_<
            boost::is_same<
                boost::parameter::aux::unwrap_cv_reference<int volatile>::type
              , int volatile
            >
          , boost::mpl::true_
          , boost::mpl::false_
        >::type
    ));
    BOOST_MPL_ASSERT((
        boost::mpl::if_<
            boost::is_same<
                boost::parameter::aux::unwrap_cv_reference<
                    int const volatile
                >::type
              , int const volatile
            >
          , boost::mpl::true_
          , boost::mpl::false_
        >::type
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
        boost::mpl::if_<
            boost::is_same<
                boost::parameter::aux::unwrap_cv_reference<test::foo>::type
              , test::foo
            >
          , boost::mpl::true_
          , boost::mpl::false_
        >::type
    ));
    BOOST_MPL_ASSERT((
        boost::mpl::if_<
            boost::is_same<
                boost::parameter::aux::unwrap_cv_reference<
                    test::foo const
                >::type
              , test::foo const
            >
          , boost::mpl::true_
          , boost::mpl::false_
        >::type
    ));
    BOOST_MPL_ASSERT((
        boost::mpl::if_<
            boost::is_same<
                boost::parameter::aux::unwrap_cv_reference<
                    test::foo volatile
                >::type
              , test::foo volatile
            >
          , boost::mpl::true_
          , boost::mpl::false_
        >::type
    ));
    BOOST_MPL_ASSERT((
        boost::mpl::if_<
            boost::is_same<
                boost::parameter::aux::unwrap_cv_reference<
                    test::foo const volatile
                >::type
              , test::foo const volatile
            >
          , boost::mpl::true_
          , boost::mpl::false_
        >::type
    ));
}

#include <boost/ref.hpp>

MPL_TEST_CASE()
{
    BOOST_MPL_ASSERT((
        boost::mpl::if_<
            boost::is_same<
                boost::parameter::aux::unwrap_cv_reference<
                    boost::reference_wrapper<test::foo>
                >::type
              , test::foo
            >
          , boost::mpl::true_
          , boost::mpl::false_
        >::type
    ));
    BOOST_MPL_ASSERT((
        boost::mpl::if_<
            boost::is_same<
                boost::parameter::aux::unwrap_cv_reference<
                    boost::reference_wrapper<test::foo> const
                >::type
              , test::foo
            >
          , boost::mpl::true_
          , boost::mpl::false_
        >::type
    ));
    BOOST_MPL_ASSERT((
        boost::mpl::if_<
            boost::is_same<
                boost::parameter::aux::unwrap_cv_reference<
                    boost::reference_wrapper<test::foo> volatile
                >::type
              , test::foo
            >
          , boost::mpl::true_
          , boost::mpl::false_
        >::type
    ));
    BOOST_MPL_ASSERT((
        boost::mpl::if_<
            boost::is_same<
                boost::parameter::aux::unwrap_cv_reference<
                    boost::reference_wrapper<test::foo> const volatile
                >::type
              , test::foo
            >
          , boost::mpl::true_
          , boost::mpl::false_
        >::type
    ));
}

#if !defined(BOOST_NO_CXX11_HDR_FUNCTIONAL)

#include <functional>

MPL_TEST_CASE()
{
    BOOST_MPL_ASSERT((
        boost::mpl::if_<
            boost::is_same<
                boost::parameter::aux::unwrap_cv_reference<
                    std::reference_wrapper<test::foo>
                >::type
              , test::foo
            >
          , boost::mpl::true_
          , boost::mpl::false_
        >::type
    ));
    BOOST_MPL_ASSERT((
        boost::mpl::if_<
            boost::is_same<
                boost::parameter::aux::unwrap_cv_reference<
                    std::reference_wrapper<test::foo> const
                >::type
              , test::foo
            >
          , boost::mpl::true_
          , boost::mpl::false_
        >::type
    ));
    BOOST_MPL_ASSERT((
        boost::mpl::if_<
            boost::is_same<
                boost::parameter::aux::unwrap_cv_reference<
                    std::reference_wrapper<test::foo> volatile
                >::type
              , test::foo
            >
          , boost::mpl::true_
          , boost::mpl::false_
        >::type
    ));
    BOOST_MPL_ASSERT((
        boost::mpl::if_<
            boost::is_same<
                boost::parameter::aux::unwrap_cv_reference<
                    std::reference_wrapper<test::foo> const volatile
                >::type
              , test::foo
            >
          , boost::mpl::true_
          , boost::mpl::false_
        >::type
    ));
}

#endif  // BOOST_NO_CXX11_HDR_FUNCTIONAL

