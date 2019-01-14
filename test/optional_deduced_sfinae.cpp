// Copyright Daniel Wallin 2006.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/parameter/config.hpp>

#if (BOOST_PARAMETER_MAX_ARITY < 2)
#error Define BOOST_PARAMETER_MAX_ARITY as 2 or greater.
#endif
#if !defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING) && \
    (BOOST_PARAMETER_EXPONENTIAL_OVERLOAD_THRESHOLD_ARITY < 2)
#error Define BOOST_PARAMETER_EXPONENTIAL_OVERLOAD_THRESHOLD_ARITY \
as 2 or greater.
#endif

#include <boost/parameter/preprocessor.hpp>
#include <boost/parameter/name.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <boost/tti/detail/dnullptr.hpp>
#include <boost/core/enable_if.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <string>

namespace test {

    BOOST_PARAMETER_NAME(x)

    struct predicate
    {
        template <typename T, typename Args>
        struct apply
          : boost::mpl::if_<
                boost::is_convertible<T,char const*>
              , boost::mpl::true_
              , boost::mpl::false_
            >
        {
        };
    };

    BOOST_PARAMETER_FUNCTION((int), sfinae, test::tag,
        (deduced
            (optional
                (x
                  , *(test::predicate)
                  , static_cast<char const*>(BOOST_TTI_DETAIL_NULLPTR)
                )
            )
        )
    )
    {
        return 1;
    }

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
    BOOST_TEST_EQ(1, test::sfinae());
    BOOST_TEST_EQ(1, test::sfinae("foo"));
    BOOST_TEST_EQ(0, test::sfinae(1));
    return boost::report_errors();
}

