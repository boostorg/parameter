// Copyright Daniel Wallin 2006.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/parameter/config.hpp>

#if !defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING) && \
    (BOOST_PARAMETER_MAX_ARITY < 4)
#error Define BOOST_PARAMETER_MAX_ARITY as 4 or greater.
#endif

namespace test {

    struct X
    {
    };

    struct Y : X
    {
    };
} // namespace test

#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/remove_reference.hpp>
#else
#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <type_traits>
#endif

namespace test {

    struct Z
    {
        template <class T, class Args>
        struct apply
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
          : boost::is_base_of<
                X
              , typename boost::remove_const<
                    typename boost::remove_reference<T>::type
                >::type
            >
#else
          : boost::mpl::if_<
                std::is_base_of<
                    X
                  , typename std::remove_const<
                        typename std::remove_reference<T>::type
                    >::type
                >
              , boost::mpl::true_
              , boost::mpl::false_
            >::type
#endif
        {
        };
    };
} // namespace test

#include <boost/parameter.hpp>

namespace test {

    template <class T = int>
    struct a0_is : boost::parameter::template_keyword<a0_is<>,T>
    {
    };

    template <class T = int>
    struct a1_is : boost::parameter::template_keyword<a1_is<>,T>
    {
    };

    template <class T = int>
    struct a2_is : boost::parameter::template_keyword<a2_is<>,T>
    {
    };

    template <class T = int>
    struct a3_is : boost::parameter::template_keyword<a3_is<>,T>
    {
    };

    template <
        class A0 = boost::parameter::void_
      , class A1 = boost::parameter::void_
      , class A2 = boost::parameter::void_
      , class A3 = boost::parameter::void_
    >
    struct with_ntp
    {
        typedef typename boost::parameter::parameters<
            test::a0_is<>
          , test::a1_is<>
          , test::a2_is<>
          , boost::parameter::optional<
                boost::parameter::deduced<test::a3_is<> >
              , Z
            >
        >::BOOST_NESTED_TEMPLATE bind<
            A0
          , A1
          , A2
          , A3
#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
          , boost::parameter::void_
#endif
        >::type args;
        typedef typename boost::parameter::binding<
            args
          , test::a0_is<>
          , void*
        >::type a0;
        typedef typename boost::parameter::binding<
            args
          , test::a1_is<>
          , void*
        >::type a1;
        typedef typename boost::parameter::binding<
            args
          , test::a2_is<>
          , void*
        >::type a2;
        typedef typename boost::parameter::binding<
            args
          , test::a3_is<>
          , void*
        >::type a3;
        typedef void(*type)(a0, a1, a2, a3);
    };
} // namespace test

#include <boost/mpl/assert.hpp>
#include <boost/mpl/aux_/test.hpp>

#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
#include <boost/type_traits/is_same.hpp>
#endif

MPL_TEST_CASE()
{
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
    BOOST_MPL_ASSERT((
        boost::is_same<
            test::with_ntp<>::type
          , void(*)(void*, void*, void*, void*)
        >
    ));
    BOOST_MPL_ASSERT((
        boost::is_same<
            test::with_ntp<test::a2_is<int> >::type
          , void(*)(void*, void*, int, void*)
        >
    ));
    BOOST_MPL_ASSERT((
        boost::is_same<
            test::with_ntp<test::a1_is<int> >::type
          , void(*)(void*, int, void*, void*)
        >
    ));
    BOOST_MPL_ASSERT((
        boost::is_same<
            test::with_ntp<test::a2_is<int const>,test::a1_is<float> >::type
          , void(*)(void*, float, int const, void*)
        >
    ));
    BOOST_MPL_ASSERT((
        boost::is_same<
            test::with_ntp<int const>::type
          , void(*)(int const, void*, void*, void*)
        >
    ));
    BOOST_MPL_ASSERT((
        boost::is_same<
            test::with_ntp<int,float>::type
          , void(*)(int, float, void*, void*)
        >
    ));
    BOOST_MPL_ASSERT((
        boost::is_same<
            test::with_ntp<int,float,char>::type
          , void(*)(int, float, char, void*)
        >
    ));
    BOOST_MPL_ASSERT((
        boost::is_same<
            test::with_ntp<test::a0_is<int>,test::Y>::type
          , void(*)(int, void*, void*, test::Y)
        >
    ));
    BOOST_MPL_ASSERT((
        boost::is_same<
            test::with_ntp<int&,test::a2_is<char>,test::Y>::type
          , void(*)(int&, void*, char, test::Y)
        >
    ));
#else // !defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
    BOOST_MPL_ASSERT((
        boost::mpl::if_<
            std::is_same<
                test::with_ntp<>::type
              , void(*)(void*, void*, void*, void*)
            >
          , boost::mpl::true_
          , boost::mpl::false_
        >::type
    ));
    BOOST_MPL_ASSERT((
        boost::mpl::if_<
            std::is_same<
                test::with_ntp<test::a2_is<int> >::type
              , void(*)(void*, void*, int, void*)
            >
          , boost::mpl::true_
          , boost::mpl::false_
        >::type
    ));
    BOOST_MPL_ASSERT((
        boost::mpl::if_<
            std::is_same<
                test::with_ntp<test::a1_is<int> >::type
              , void(*)(void*, int, void*, void*)
            >
          , boost::mpl::true_
          , boost::mpl::false_
        >::type
    ));
    BOOST_MPL_ASSERT((
        boost::mpl::if_<
            std::is_same<
                test::with_ntp<
                    test::a2_is<int const>
                  , test::a1_is<float>
                >::type
              , void(*)(void*, float, int const, void*)
            >
          , boost::mpl::true_
          , boost::mpl::false_
        >::type
    ));
    BOOST_MPL_ASSERT((
        boost::mpl::if_<
            std::is_same<
                test::with_ntp<int const>::type
              , void(*)(int const, void*, void*, void*)
            >
          , boost::mpl::true_
          , boost::mpl::false_
        >::type
    ));
    BOOST_MPL_ASSERT((
        boost::mpl::if_<
            std::is_same<
                test::with_ntp<int,float>::type
              , void(*)(int, float, void*, void*)
            >
          , boost::mpl::true_
          , boost::mpl::false_
        >::type
    ));
    BOOST_MPL_ASSERT((
        boost::mpl::if_<
            std::is_same<
                test::with_ntp<int,float,char>::type
              , void(*)(int, float, char, void*)
            >
          , boost::mpl::true_
          , boost::mpl::false_
        >::type
    ));
    BOOST_MPL_ASSERT((
        boost::mpl::if_<
            std::is_same<
                test::with_ntp<test::a0_is<int>,test::Y>::type
              , void(*)(int, void*, void*, test::Y)
            >
          , boost::mpl::true_
          , boost::mpl::false_
        >::type
    ));
    BOOST_MPL_ASSERT((
        boost::mpl::if_<
            std::is_same<
                test::with_ntp<int&,test::a2_is<char>,test::Y>::type
              , void(*)(int&, void*, char, test::Y)
            >
          , boost::mpl::true_
          , boost::mpl::false_
        >::type
    ));
#endif // BOOST_NO_CXX11_HDR_TYPE_TRAITS
}

