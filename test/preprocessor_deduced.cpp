// Copyright Daniel Wallin 2006.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/parameter/preprocessor.hpp>
#include <boost/parameter/name.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/tuple/tuple.hpp>
#include <string>
#include "basics.hpp"

#if !defined BOOST_NO_SFINAE
#include <boost/core/enable_if.hpp>
#endif

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
    // Workaround: factor template2<x> into a separate typedef
    typedef boost::is_convertible<boost::mpl::_,int> predicate1;
    typedef boost::is_convertible<boost::mpl::_,std::string> predicate2;

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
#else // !BOOST_WORKAROUND(__SUNPRO_CC, BOOST_TESTED_AT(0x580))
    BOOST_PARAMETER_FUNCTION((int), f, test::tag,
        (required
            (expected, *)
        )
        (deduced
            (required
                (x, *(boost::is_convertible<boost::mpl::_,int>))
                (y, *(boost::is_convertible<boost::mpl::_,std::string>))
            )
        )
    )
#endif // SunPro CC workarounds needed.
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
    typedef boost::is_convertible<boost::mpl::_,X> predicate3;

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
                (z, *(test::predicate3), X())
            )
        )
    )
#else // !BOOST_WORKAROUND(__SUNPRO_CC, BOOST_TESTED_AT(0x580))
    BOOST_PARAMETER_FUNCTION((int), g, test::tag,
        (required
            (expected, *)
        )
        (deduced
            (required
                (x, *(boost::is_convertible<boost::mpl::_,int>))
                (y, *(boost::is_convertible<boost::mpl::_,std::string>))
            )
            (optional
                (z, *(boost::is_convertible<boost::mpl::_,X>), X())
            )
        )
    )
#endif // SunPro CC workarounds needed.
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
                (x, *(boost::is_convertible<boost::mpl::_,std::string>))
            )
        )
    )
#endif
    {
        return 1;
    }

#if !defined BOOST_NO_SFINAE
    // On compilers that actually support SFINAE, add another overload
    // that is an equally good match and can only be in the overload set
    // when the others are not.  This tests that the SFINAE is actually
    // working.  On all other compilers we're just checking that everything
    // about SFINAE-enabled code will work, except of course the SFINAE.
    template <class A0>
    typename boost::enable_if<boost::is_same<int,A0>,int>::type
    sfinae(A0 const& a0)
    {
        return 0;
    }
#endif

    // make_tuple doesn't work with char arrays.
    char const* str(char const* s)
    {
        return s;
    }
} // namespace test

int main()
{
    test::f(
        boost::make_tuple(0, test::str("foo")), test::_x = 0, test::_y = "foo"
    );
    test::f(
        boost::make_tuple(0, test::str("foo")), test::_x = 0, test::_y = "foo"
    );
    test::f(boost::make_tuple(0, test::str("foo")), 0, "foo");
    test::f(boost::make_tuple(0, test::str("foo")), "foo", 0);
    test::f(boost::make_tuple(0, test::str("foo")), test::_y = "foo", 0);
    test::f(boost::make_tuple(0, test::str("foo")), test::_x = 0, "foo");
    test::f(boost::make_tuple(0, test::str("foo")), 0, test::_y = "foo");
    test::g(
        boost::make_tuple(0, test::str("foo"), test::X())
      , test::_x = 0
      , test::_y = "foo"
    );
    test::g(boost::make_tuple(0, test::str("foo"), test::X()), 0, "foo");
    test::g(boost::make_tuple(0, test::str("foo"), test::X()), "foo", 0);
    test::g(
        boost::make_tuple(0, test::str("foo"), test::X()), test::_y = "foo", 0
    );
    test::g(
        boost::make_tuple(0, test::str("foo"), test::X()), test::_x = 0, "foo"
    );
    test::g(
        boost::make_tuple(0, test::str("foo"), test::X())
      , 0
      , test::_y = "foo"
    );
    test::g(
        boost::make_tuple(0, test::str("foo"), test::X(1))
      , 0
      , test::_y = "foo"
      , test::X(1)
    );
    test::g(
        boost::make_tuple(0, test::str("foo"), test::X(1))
      , test::X(1)
      , 0
      , test::_y = "foo"
    );

#if !defined BOOST_NO_SFINAE
    BOOST_TEST(test::sfinae("foo") == 1);
    BOOST_TEST(test::sfinae(0) == 0);
#endif

    return boost::report_errors();
}

