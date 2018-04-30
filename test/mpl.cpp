// Copyright David Abrahams 2006.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/parameter/config.hpp>

#if !defined(BOOST_GCC) || ( \
        defined(__MINGW32__) && (1 == __MINGW32__) \
    ) || BOOST_WORKAROUND(BOOST_GCC, < 40800) || ( \
        defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING) && \
        BOOST_WORKAROUND(BOOST_GCC, >= 50000) \
    )
#define LIBS_PARAMETER_TEST_WILL_NOT_ICE
#endif

#include <boost/core/lightweight_test.hpp>
#include <boost/config/pragma_message.hpp>

#if defined LIBS_PARAMETER_TEST_WILL_NOT_ICE

#include "basics.hpp"
#include <boost/mpl/list.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/contains.hpp>
#include <boost/mpl/assert.hpp>

#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
#include <boost/type_traits/add_pointer.hpp>
#else
#include <type_traits>
#endif

namespace test {

    template <typename Set>
    struct assert_in_set
    {
        template <typename T>
        void operator()(T*)
        {
            BOOST_MPL_ASSERT((boost::mpl::contains<Set,T>));
        }
    };

    template <typename Expected, typename Params>
    void f_impl(Params const& p BOOST_APPEND_EXPLICIT_TEMPLATE_TYPE(Expected))
    {
        BOOST_MPL_ASSERT_RELATION(
            boost::mpl::size<Expected>::value
          , ==
          , boost::mpl::size<Params>::value
        );

        boost::mpl::for_each<
            Params
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
          , boost::add_pointer<boost::mpl::_1>
#else
          , std::add_pointer<boost::mpl::_1>
#endif
        >(
            test::assert_in_set<Expected>()
        );
    }

    template <
        typename Expected
      , typename Tester
      , typename Name
      , typename Value
      , typename Index
    >
    void f(
        Tester const& t
      , Name const& name_
      , Value const& value_
      , Index const& index_
        BOOST_APPEND_EXPLICIT_TEMPLATE_TYPE(Expected)
    )
    {
        test::f_impl<Expected>(
            test::f_parameters()(t, name_, value_, index_)
        );
    }

    template <
        typename Expected
      , typename Tester
      , typename Name
      , typename Value
    >
    void f(
        Tester const& t
      , const Name& name_
      , Value const& value_
        BOOST_APPEND_EXPLICIT_TEMPLATE_TYPE(Expected)
    )
    {
        test::f_impl<Expected>(test::f_parameters()(t, name_, value_));
    }

    template <typename Expected, typename Tester, typename Name>
    void f(
        Tester const& t
      , Name const& name_ BOOST_APPEND_EXPLICIT_TEMPLATE_TYPE(Expected)
    )
    {
        test::f_impl<Expected>(test::f_parameters()(t, name_));
    }

    void run()
    {
        typedef test::tag::tester tester_;
        typedef test::tag::name name_;
        typedef test::tag::value value_;
        typedef test::tag::index index_;

        test::f<boost::mpl::list4<tester_,name_,value_,index_> >(1, 2, 3, 4);
        test::f<boost::mpl::list3<tester_,name_,index_> >(
            1, 2, test::_index = 3
        );
        test::f<boost::mpl::list3<tester_,name_,index_> >(
            1, test::_index = 2, test::_name = 3
        );
        test::f<boost::mpl::list2<name_,value_> >(
            test::_name = 3, test::_value = 4
        );
        test::f_impl<boost::mpl::list1<value_> >(test::_value = 4);
    }
}

BOOST_PRAGMA_MESSAGE("Test should compile.");
#else
BOOST_PRAGMA_MESSAGE("Test not compiled.");
#endif // Compiler won't ICE.

int main()
{
#if defined LIBS_PARAMETER_TEST_WILL_NOT_ICE
    test::run();
#endif
    return boost::report_errors();
}

