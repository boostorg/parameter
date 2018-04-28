// Copyright David Abrahams, Daniel Wallin 2005.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BASICS_050424_HPP
#define BASICS_050424_HPP

#include <boost/parameter.hpp>

#if !defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING) && \
    (BOOST_PARAMETER_MAX_ARITY < 4)
#error Define BOOST_PARAMETER_MAX_ARITY as 4 or greater.
#endif

#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
#include <boost/type_traits/is_same.hpp>
#else
#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <type_traits>
#endif

#include <boost/assert.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/core/lightweight_test.hpp>
#include <cstring>

namespace test {

    BOOST_PARAMETER_NAME(name)
    BOOST_PARAMETER_NAME(value)
    BOOST_PARAMETER_NAME(index)
    BOOST_PARAMETER_NAME(tester)

    struct f_parameters // vc6 is happier with inheritance than with a typedef
      : boost::parameter::parameters<
            test::tag::tester
          , test::tag::name
          , test::tag::value
          , test::tag::index
        >
    {
    };

    inline double value_default()
    {
        return 666.222;
    }

    template <typename T>
    inline bool equal(T const& x, T const& y)
    {
        return x == y;
    }

    inline bool equal(char const* s1, char const* s2)
    {
        using namespace std;
        return !strcmp(s1, s2);
    }

#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
    inline bool equal(char* s1, char* s2)
    {
        using namespace std;
        return !strcmp(s1, s2);
    }
#endif 

    template <typename Name, typename Value, typename Index>
    struct values_t
    {
        values_t(Name const& n_, Value const& v_, Index const& i_)
          : n(n_), v(v_), i(i_)
        {
        }

        template <typename Name_, typename Value_, typename Index_>
        void
            operator()(
                Name_ const& n_
              , Value_ const& v_
              , Index_ const& i_
            ) const
        {
#if defined(_MSC_VER) && (                                                   \
        BOOST_WORKAROUND(BOOST_INTEL_CXX_VERSION, <= 700) ||                 \
        BOOST_WORKAROUND(BOOST_MSVC, < 1310)) ||                             \
        BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564)                \
    )
            // Only VC and its emulators fail this; they seem to have problems
            // with deducing the constness of string literal arrays.
#elif defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
            BOOST_MPL_ASSERT((boost::is_same<Index,Index_>));
            BOOST_MPL_ASSERT((boost::is_same<Value,Value_>));
            BOOST_MPL_ASSERT((boost::is_same<Name,Name_>));
#else
            BOOST_MPL_ASSERT((
                typename boost::mpl::if_<
                    std::is_same<Index,Index_>
                  , boost::mpl::true_
                  , boost::mpl::false_
                >::type
            ));
            BOOST_MPL_ASSERT((
                typename boost::mpl::if_<
                    std::is_same<Value,Value_>
                  , boost::mpl::true_
                  , boost::mpl::false_
                >::type
            ));
            BOOST_MPL_ASSERT((
                typename boost::mpl::if_<
                    std::is_same<Name,Name_>
                  , boost::mpl::true_
                  , boost::mpl::false_
                >::type
            ));
#endif // constness deduction of string literal arrays
            BOOST_TEST(test::equal(n, n_));
            BOOST_TEST(test::equal(v, v_));
            BOOST_TEST(test::equal(i, i_));
        }

        Name const& n;
        Value const& v;
        Index const& i;
    };

    template <typename Name, typename Value, typename Index>
    inline test::values_t<Name,Value,Index>
        values(Name const& n, Value const& v, Index const& i)
    {
        return test::values_t<Name,Value,Index>(n, v, i);
    }
} // namespace test

// GCC2 has a problem with char (&)[] deduction,
// so we'll cast string literals there.
#undef S
#if BOOST_WORKAROUND(__GNUC__, == 2) || \
    BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
#define S(s) (char const*)s
#else
#define S(s) s
#endif

#endif // BASICS_050424_HPP

