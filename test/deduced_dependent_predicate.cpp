// Copyright Daniel Wallin 2006.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/parameter/parameters.hpp>
#include <boost/parameter/name.hpp>
#include <boost/parameter/binding.hpp>
#include <boost/type_traits.hpp>
#include "deduced.hpp"

namespace test {

    BOOST_PARAMETER_NAME(x)
    BOOST_PARAMETER_NAME(y)
    BOOST_PARAMETER_NAME(z)
} // namespace test

int main()
{
    test::check<
        boost::parameter::parameters<
            test::tag::x
          , boost::parameter::optional<
                boost::parameter::deduced<test::tag::y>
#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))    
              , boost::is_same<
                    boost::mpl::_1
                  , boost::remove_reference<
                        boost::parameter::binding<boost::mpl::_2,test::tag::x>
                    >
                >
#else
              , boost::is_same<
                    boost::add_lvalue_reference<boost::mpl::_1>
                  , boost::parameter::binding<boost::mpl::_2,test::tag::x>
                >
#endif
            >
        >
    >((test::_x = 0, test::_y = 1), 0, 1);

    test::check<
        boost::parameter::parameters<
            test::tag::x
          , boost::parameter::optional<
                boost::parameter::deduced<test::tag::y>
#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))    
              , boost::is_same<
                    boost::mpl::_1
                  , boost::remove_reference<
                        boost::parameter::binding<boost::mpl::_2,test::tag::x>
                    >
                >
#else
              , boost::is_same<
                    boost::add_lvalue_reference<boost::mpl::_1>
                  , boost::parameter::binding<boost::mpl::_2,test::tag::x>
                >
#endif
            >
        >
    >((test::_x = 0U, test::_y = 1U), 0U, 1U);

    test::check<
        boost::parameter::parameters<
            test::tag::x
          , boost::parameter::optional<
                boost::parameter::deduced<test::tag::y>
              , boost::is_convertible<boost::mpl::_1,test::tag::x::_>
            >
        >
    >((test::_x = 0U, test::_y = 1U), 0U, 1U);

    test::check<
        boost::parameter::parameters<
            test::tag::x
          , boost::parameter::optional<
                boost::parameter::deduced<test::tag::y>
              , boost::is_convertible<boost::mpl::_1,test::tag::x::_1>
            >
        >
    >((test::_x = 0U, test::_y = 1U), 0U, 1U);

    return boost::report_errors();
}

