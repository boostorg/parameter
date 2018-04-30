// Copyright Daniel Wallin 2006.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/parameter/config.hpp>

#if !defined(BOOST_GCC) || BOOST_WORKAROUND(BOOST_GCC, < 40800) || ( \
        defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING) && \
        BOOST_WORKAROUND(BOOST_GCC, >= 40900) \
    )
#define LIBS_PARAMETER_TEST_WILL_NOT_ICE
#endif

#include <boost/config/pragma_message.hpp>

#if defined LIBS_PARAMETER_TEST_WILL_NOT_ICE

#include <boost/parameter/preprocessor.hpp>
#include <boost/parameter/name.hpp>
#include <boost/tuple/tuple.hpp>
#include <string>
#include "basics.hpp"

#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
#include <boost/type_traits/is_convertible.hpp>
#else
#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <type_traits>
#endif

#if !defined(BOOST_NO_SFINAE)
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
    // Workaround: factor template2<x> into separate typedefs
    struct predicate1
    {
        template <typename From, typename Args>
        struct apply
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
          : boost::is_convertible<From,int>
#else
          : boost::mpl::if_<
                std::is_convertible<From,int>
              , boost::mpl::true_
              , boost::mpl::false_
            >
#endif
        {
        };
    };

    struct predicate2
    {
        template <typename From, typename Args>
        struct apply
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
          : boost::is_convertible<From,std::string>
#else
          : boost::mpl::if_<
                std::is_convertible<From,std::string>
              , boost::mpl::true_
              , boost::mpl::false_
            >
#endif
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
#else // !BOOST_WORKAROUND(__SUNPRO_CC, BOOST_TESTED_AT(0x580))
    template <typename To>
    struct predicate
    {
        template <typename From, typename Args>
        struct apply
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
          : boost::is_convertible<From,To>
#else
          : boost::mpl::if_<
                std::is_convertible<From,To>
              , boost::mpl::true_
              , boost::mpl::false_
            >
#endif
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
    struct predicate3
    {
        template <typename From, typename Args>
        struct apply
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
          : boost::is_convertible<From,test::X>
#else
          : boost::mpl::if_<
                std::is_convertible<From,test::X>
              , boost::mpl::true_
              , boost::mpl::false_
            >
#endif
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
#else // !BOOST_WORKAROUND(__SUNPRO_CC, BOOST_TESTED_AT(0x580))
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
                (x, *(test::predicate<std::string>))
            )
        )
    )
#endif
    {
        return 1;
    }

#if !defined(BOOST_NO_SFINAE)
    // On compilers that actually support SFINAE, add another overload
    // that is an equally good match and can only be in the overload set
    // when the others are not.  This tests that the SFINAE is actually
    // working.  On all other compilers we're just checking that everything
    // about SFINAE-enabled code will work, except of course the SFINAE.
    template <typename A0>
    typename boost::enable_if<
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
        boost::is_same<int,A0>
#else
        typename boost::mpl::if_<
            std::is_same<int,A0>
          , boost::mpl::true_
          , boost::mpl::false_
        >::type
#endif
      , int
    >::type
        sfinae(A0 const& a0)
    {
        return 0;
    }
#endif // BOOST_NO_SFINAE

    // make_tuple doesn't work with char arrays.
    char const* str(char const* s)
    {
        return s;
    }
} // namespace test

BOOST_PRAGMA_MESSAGE("Test should compile.");
#else
BOOST_PRAGMA_MESSAGE("Test not compiled.");
#include <boost/core/lightweight_test.hpp>
#endif // Compiler won't ICE.

int main()
{
#if defined LIBS_PARAMETER_TEST_WILL_NOT_ICE
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

#if !defined(BOOST_NO_SFINAE)
    BOOST_TEST(test::sfinae("foo") == 1);
    BOOST_TEST(test::sfinae(0) == 0);
#endif
#endif // Compiler won't ICE.

    return boost::report_errors();
}

