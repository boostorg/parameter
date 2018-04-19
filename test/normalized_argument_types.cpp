// Copyright Daniel Wallin 2006.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/parameter.hpp>

#if !defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING) && \
    (BOOST_PARAMETER_MAX_ARITY < 2)
#error Define BOOST_PARAMETER_MAX_ARITY as 2 or greater.
#endif

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

        void noop() const
        {
        }
    };

    std::size_t count_instances::count = 0;

    BOOST_PARAMETER_NAME(x)
    BOOST_PARAMETER_NAME(y)
} // namespace test

#include <boost/mpl/assert.hpp>
#include <boost/config.hpp>

#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
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
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
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
#endif // BOOST_NO_CXX11_HDR_TYPE_TRAITS
        return 0;
    }
} // namespace test

#include <boost/core/lightweight_test.hpp>

namespace test {

    BOOST_PARAMETER_FUNCTION((int), g, tag,
        (required
            (x, (test::count_instances))
        )
    )
    {
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
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
#endif // BOOST_NO_CXX11_HDR_TYPE_TRAITS
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
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
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

int main()
{
    test::f(1, 2);
    test::f(1., 2.f);
    test::f(1U);
    test::g(0);
    test::h(0);
    return boost::report_errors();
}

