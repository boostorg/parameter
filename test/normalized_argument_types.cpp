// Copyright Daniel Wallin 2006.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/parameter/config.hpp>

#if !defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING) && \
    (BOOST_PARAMETER_MAX_ARITY < 2)
#error Define BOOST_PARAMETER_MAX_ARITY as 2 or greater.
#endif

#if !defined(BOOST_GCC) || ( \
        defined(__MINGW32__) && (1 == __MINGW32__) \
    ) || BOOST_WORKAROUND(BOOST_GCC, < 40800) || ( \
        defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING) && \
        BOOST_WORKAROUND(BOOST_GCC, >= 40900) \
    )
#define LIBS_PARAMETER_TEST_WILL_NOT_ICE
#endif

#include <boost/core/lightweight_test.hpp>
#include <boost/config/pragma_message.hpp>

#if defined LIBS_PARAMETER_TEST_WILL_NOT_ICE

#include <boost/parameter.hpp>

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

        template <typename T>
        count_instances(T const&)
        {
            ++count_instances::count;
        }

        ~count_instances()
        {
            --count_instances::count;
        }

        static std::size_t count;

        void noop() const
        {
        }
    };

    std::size_t count_instances::count = 0;

    BOOST_PARAMETER_NAME(x)
    BOOST_PARAMETER_NAME(y)
} // namespace test

#include <boost/mpl/assert.hpp>

#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
#include <boost/type_traits/is_convertible.hpp>
#else
#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <type_traits>
#endif

namespace test {

    BOOST_PARAMETER_FUNCTION((int), f, tag,
        (required
            (x, (long))
        )
        (optional
            (y, (long), 2L)
        )
    )
    {
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
        BOOST_MPL_ASSERT((boost::is_convertible<x_type,long>));
        BOOST_MPL_ASSERT((boost::is_convertible<y_type,long>));
#else
        BOOST_MPL_ASSERT((
            typename boost::mpl::if_<
                std::is_convertible<x_type,long>
              , boost::mpl::true_
              , boost::mpl::false_
            >::type
        ));
        BOOST_MPL_ASSERT((
            typename boost::mpl::if_<
                std::is_convertible<y_type,long>
              , boost::mpl::true_
              , boost::mpl::false_
            >::type
        ));
#endif // BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS
        return 0;
    }

    BOOST_PARAMETER_FUNCTION((int), g, tag,
        (required
            (x, (test::count_instances))
        )
    )
    {
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
        BOOST_MPL_ASSERT((
            boost::is_convertible<x_type,test::count_instances>
        ));
#else
        BOOST_MPL_ASSERT((
            typename boost::mpl::if_<
                std::is_convertible<x_type,test::count_instances>
              , boost::mpl::true_
              , boost::mpl::false_
            >::type
        ));
#endif
        x.noop();
        BOOST_TEST_LT(0, test::count_instances::count);
        return 0;
    }

    BOOST_PARAMETER_FUNCTION((int), h, tag,
        (required
            (x, (test::count_instances const&))
        )
    )
    {
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
        BOOST_MPL_ASSERT((
            boost::is_convertible<x_type,test::count_instances const>
        ));
#else
        BOOST_MPL_ASSERT((
            typename boost::mpl::if_<
                std::is_convertible<x_type,test::count_instances const>
              , boost::mpl::true_
              , boost::mpl::false_
            >::type
        ));
#endif
        x.noop();
        BOOST_TEST_EQ(1, test::count_instances::count);
        return 0;
    }
} // namespace test

BOOST_PRAGMA_MESSAGE("Test should compile.");
#else
BOOST_PRAGMA_MESSAGE("Test not compiled.");
#endif // Compiler won't ICE.

int main()
{
#if defined LIBS_PARAMETER_TEST_WILL_NOT_ICE
    test::f(1, 2);
    test::f(1., 2.f);
    test::f(1U);
    test::g(0);
    test::h(0);
#endif
    return boost::report_errors();
}

