// Copyright Daniel Wallin 2006.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/parameter.hpp>

#if !defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING && \
    BOOST_PARAMETER_MAX_ARITY < 2
#error Define BOOST_PARAMETER_MAX_ARITY as 2 or greater.
#endif

#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/core/lightweight_test.hpp>

namespace test {

    struct count_instances
    {
        count_instances()
        {
            ++count_instances::count;
        }

        count_instances(count_instances const&)
        {
            ++count_instances::count;
        }

        template <class T>
        count_instances(T const&)
        {
            ++count_instances::count;
        }

        ~count_instances()
        {
            --count_instances::count;
        }

        static std::size_t count;
    };

    std::size_t count_instances::count = 0;

    BOOST_PARAMETER_NAME(x)
    BOOST_PARAMETER_NAME(y)

    BOOST_PARAMETER_FUNCTION((int), f, tag,
        (required
            (x, (int))
            (y, (int))
        )
    )
    {
        BOOST_MPL_ASSERT((boost::is_same<x_type,int>));
        BOOST_MPL_ASSERT((boost::is_same<y_type,int>));
        return 0;
    }

    BOOST_PARAMETER_FUNCTION((int), g, tag,
        (required
            (x, (test::count_instances))
        )
    )
    {
        BOOST_MPL_ASSERT((boost::is_same<x_type,test::count_instances>));
        BOOST_TEST_LT(0, test::count_instances::count);
        return 0;
    }

    BOOST_PARAMETER_FUNCTION((int), h, tag,
        (required
            (x, (test::count_instances const&))
        )
    )
    {
        BOOST_MPL_ASSERT((
            boost::is_convertible<x_type,test::count_instances const>
        ));
        BOOST_TEST_EQ(1, test::count_instances::count);
        return 0;
    }
} // namespace test

int main()
{
    test::f(1, 2);
    test::f(1., 2.f);
#if defined BOOST_CLANG && (1 == BOOST_CLANG) && (__clang_major__ < 6)
    // Travis Cl on Linux reports substitution errors involving suffix I.
#elif !defined BOOST_MSVC
    // Appveyor reports MSVC declaring suffix I illegal.
    test::f(1U, 2I);
#endif
    test::g(0);
    test::h(0);
    return boost::report_errors();
}

