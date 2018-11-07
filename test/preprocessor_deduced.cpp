// Copyright Daniel Wallin 2006.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/parameter/config.hpp>
#include <boost/parameter/preprocessor.hpp>
#include <boost/parameter/name.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <boost/core/enable_if.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <string>
#include "basics.hpp"

namespace test {

    BOOST_PARAMETER_NAME(expected)
    BOOST_PARAMETER_NAME(x)
    BOOST_PARAMETER_NAME(y)
    BOOST_PARAMETER_NAME(z)

#if BOOST_WORKAROUND(__SUNPRO_CC, BOOST_TESTED_AT(0x580))
    // Sun has problems with this syntax:
    //
    //   template1< r* ( template2<x> ) >
    //
    // Workaround: factor template2<x> into separate typedefs
    struct predicate1
    {
        template <typename From, typename Args>
        struct apply
          : boost::mpl::if_<
                boost::is_convertible<From,int>
              , boost::mpl::true_
              , boost::mpl::false_
            >
        {
        };
    };

    struct predicate2
    {
        template <typename From, typename Args>
        struct apply
          : boost::mpl::if_<
                boost::is_convertible<From,std::string>
              , boost::mpl::true_
              , boost::mpl::false_
            >
        {
        };
    };

    BOOST_PARAMETER_FUNCTION((int), f, test::tag,
        (required
            (expected, *)
        )
        (deduced
            (required
                (x, *(test::predicate1))
                (y, *(test::predicate2))
            )
        )
    )
#else   // !BOOST_WORKAROUND(__SUNPRO_CC, BOOST_TESTED_AT(0x580))
    template <typename To>
    struct predicate
    {
        template <typename From, typename Args>
        struct apply
          : boost::mpl::if_<
                boost::is_convertible<From,To>
              , boost::mpl::true_
              , boost::mpl::false_
            >
        {
        };
    };

    BOOST_PARAMETER_FUNCTION((int), f, test::tag,
        (required
            (expected, *)
        )
        (deduced
            (required
                (x, *(test::predicate<int>))
                (y, *(test::predicate<std::string>))
            )
        )
    )
#endif  // SunPro CC workarounds needed.
    {
        BOOST_TEST(test::equal(x, boost::tuples::get<0>(expected)));
        BOOST_TEST(test::equal(y, boost::tuples::get<1>(expected)));
        return 1;
    }

    struct X 
    {
        X(int x_ = -1) : x(x_)
        {
        }

        bool operator==(X const& other) const
        {
            return this->x == other.x;
        }

        int x;
    };

#if BOOST_WORKAROUND(__SUNPRO_CC, BOOST_TESTED_AT(0x580))
    // SunPro workaround; see above
    struct predicate3
    {
        template <typename From, typename Args>
        struct apply
          : boost::mpl::if_<
                boost::is_convertible<From,test::X>
              , boost::mpl::true_
              , boost::mpl::false_
            >
        {
        };
    };

    BOOST_PARAMETER_FUNCTION((int), g, tag,
        (required
            (expected, *)
        )
        (deduced
            (required
                (x, *(test::predicate1))
                (y, *(test::predicate2))
            )
            (optional
                (z, *(test::predicate3), test::X())
            )
        )
    )
#else   // !BOOST_WORKAROUND(__SUNPRO_CC, BOOST_TESTED_AT(0x580))
    BOOST_PARAMETER_FUNCTION((int), g, test::tag,
        (required
            (expected, *)
        )
        (deduced
            (required
                (x, *(test::predicate<int>))
                (y, *(test::predicate<std::string>))
            )
            (optional
                (z, *(test::predicate<test::X>), test::X())
            )
        )
    )
#endif  // SunPro CC workarounds needed.
    {
        BOOST_TEST(test::equal(x, boost::tuples::get<0>(expected)));
        BOOST_TEST(test::equal(y, boost::tuples::get<1>(expected)));
        BOOST_TEST(test::equal(z, boost::tuples::get<2>(expected)));
        return 1;
    }

#if BOOST_WORKAROUND(__SUNPRO_CC, BOOST_TESTED_AT(0x580))
    BOOST_PARAMETER_FUNCTION((int), sfinae, test::tag,
        (deduced
            (required
                (x, *(test::predicate2))
            )
        )
    )
#else
    BOOST_PARAMETER_FUNCTION((int), sfinae, test::tag,
        (deduced
            (required
                (x, *(test::predicate<std::string>))
            )
        )
    )
#endif
    {
        return 1;
    }

    // On compilers that actually support SFINAE, add another overload
    // that is an equally good match and can only be in the overload set
    // when the others are not.  This tests that the SFINAE is actually
    // working.
    template <typename A0>
    typename boost::enable_if<
        typename boost::mpl::if_<
            boost::is_same<int,A0>
          , boost::mpl::true_
          , boost::mpl::false_
        >::type
      , int
    >::type
        sfinae(A0 const& a0)
    {
        return 0;
    }
} // namespace test

#include <boost/core/lightweight_test.hpp>

int main()
{
    test::f(
        boost::make_tuple(0, std::string("foo"))
      , test::_x = 0
      , test::_y = std::string("foo")
    );
    test::f(
        boost::make_tuple(0, std::string("foo"))
      , 0
      , std::string("foo")
    );
    test::f(
        boost::make_tuple(0, std::string("foo"))
      , std::string("foo")
      , 0
    );
    test::f(
        boost::make_tuple(0, std::string("foo"))
      , test::_y = std::string("foo")
      , 0
    );
    test::f(
        boost::make_tuple(0, std::string("foo"))
      , test::_x = 0
      , std::string("foo")
    );
    test::f(
        boost::make_tuple(0, std::string("foo"))
      , 0
      , test::_y = std::string("foo")
    );
    test::g(
        boost::make_tuple(0, std::string("foo"), test::X())
      , test::_x = 0
      , test::_y = std::string("foo")
    );
    test::g(
        boost::make_tuple(0, std::string("foo"), test::X())
      , 0
      , std::string("foo")
    );
    test::g(
        boost::make_tuple(0, std::string("foo"), test::X())
      , std::string("foo")
      , 0
    );
    test::g(
        boost::make_tuple(0, std::string("foo"), test::X())
      , test::_y = std::string("foo")
      , 0
    );
    test::g(
        boost::make_tuple(0, std::string("foo"), test::X())
      , test::_x = 0
      , std::string("foo")
    );
    test::g(
        boost::make_tuple(0, std::string("foo"), test::X())
      , 0
      , test::_y = std::string("foo")
    );
    test::g(
        boost::make_tuple(0, std::string("foo"), test::X(1))
      , 0
      , test::_y = std::string("foo")
      , test::X(1)
    );
    test::g(
        boost::make_tuple(0, std::string("foo"), test::X(1))
      , test::X(1)
      , 0
      , test::_y = std::string("foo")
    );
    BOOST_TEST(test::sfinae("foo") == 1);
    BOOST_TEST(test::sfinae(0) == 0);

    return boost::report_errors();
}

