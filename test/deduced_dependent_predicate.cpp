// Copyright Daniel Wallin 2006.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/parameter/config.hpp>
#include <boost/parameter/parameters.hpp>
#include <boost/parameter/name.hpp>
#include <boost/parameter/binding.hpp>
#include <boost/mpl/placeholders.hpp>
#include "deduced.hpp"

#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
#include <boost/type_traits.hpp>
#else
#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <type_traits>
#endif

namespace test {

    BOOST_PARAMETER_NAME(x)
    BOOST_PARAMETER_NAME(y)
    BOOST_PARAMETER_NAME(z)
} // namespace test

#include <boost/core/lightweight_test.hpp>

int main()
{
    test::check<
        boost::parameter::parameters<
            test::tag::x
          , boost::parameter::optional<
                boost::parameter::deduced<test::tag::y>
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
              , boost::is_same<
#else
              , boost::mpl::if_<
                    std::is_same<
#endif
#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
                        boost::mpl::_1
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
                      , boost::remove_reference<
#else
                      , std::remove_reference<
#endif
                            boost::parameter::binding<
                                boost::mpl::_2
                              , test::tag::x
                            >
                        >
#else // !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
                        boost::add_lvalue_reference<boost::mpl::_1>
#else
                        std::add_lvalue_reference<boost::mpl::_1>
#endif
                      , boost::parameter::binding<boost::mpl::_2,test::tag::x>
#endif // Borland workarounds needed.
                    >
#if !defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
                  , boost::mpl::true_
                  , boost::mpl::false_
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
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
              , boost::is_same<
#else
              , boost::mpl::if_<
                    std::is_same<
#endif
#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
                        boost::mpl::_1
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
                      , boost::remove_reference<
#else
                      , std::remove_reference<
#endif
                            boost::parameter::binding<
                                boost::mpl::_2
                              , test::tag::x
                            >
                        >
#else // !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
                        boost::add_lvalue_reference<boost::mpl::_1>
#else
                        std::add_lvalue_reference<boost::mpl::_1>
#endif
                      , boost::parameter::binding<boost::mpl::_2,test::tag::x>
#endif // Borland workarounds needed.
                    >
#if !defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
                  , boost::mpl::true_
                  , boost::mpl::false_
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
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
              , boost::is_convertible<boost::mpl::_1,test::tag::x::_>
#else
              , boost::mpl::if_<
                    std::is_convertible<boost::mpl::_1,test::tag::x::_>
                  , boost::mpl::true_
                  , boost::mpl::false_
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
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
              , boost::is_convertible<boost::mpl::_1,test::tag::x::_1>
#else
              , boost::mpl::if_<
                    std::is_convertible<boost::mpl::_1,test::tag::x::_1>
                  , boost::mpl::true_
                  , boost::mpl::false_
                >
#endif
            >
        >
    >((test::_x = 0U, test::_y = 1U), 0U, 1U);

    return boost::report_errors();
}

