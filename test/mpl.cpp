// Copyright David Abrahams 2006.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "basics.hpp"
#include <boost/type_traits/add_pointer.hpp>
#include <boost/mpl/list.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/contains.hpp>
#include <boost/mpl/assert.hpp>

namespace test {

    template <class Set>
    struct assert_in_set
    {
        template <class T>
        void operator()(T*)
        {
            BOOST_MPL_ASSERT((boost::mpl::contains<Set,T>));
        }
    };

    template <class Expected, class Params>
    void f_impl(Params const& p BOOST_APPEND_EXPLICIT_TEMPLATE_TYPE(Expected))
    {
        BOOST_MPL_ASSERT_RELATION(
            boost::mpl::size<Expected>::value
          , ==
          , boost::mpl::size<Params>::value
        );

        boost::mpl::for_each<Params, boost::add_pointer<boost::mpl::_1> >(
            test::assert_in_set<Expected>()
        );
    }

    template <
        class Expected, class Tester, class Name, class Value, class Index
    >
    void f(
        Tester const& t, Name const& name_, Value const& value_
      , const Index& index_ BOOST_APPEND_EXPLICIT_TEMPLATE_TYPE(Expected)
    )
    {
        test::f_impl<Expected>(
            test::f_parameters()(t, name_, value_, index_)
        );
    }

    template <class Expected, class Tester, class Name, class Value>
    void f(
        Tester const& t, const Name& name_
      , Value const& value_ BOOST_APPEND_EXPLICIT_TEMPLATE_TYPE(Expected)
    )
    {
        test::f_impl<Expected>(test::f_parameters()(t, name_, value_));
    }

    template <class Expected, class Tester, class Name>
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

int main()
{
    test::run();
    return boost::report_errors();
}

