// Copyright Daniel Wallin 2006.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/parameter/preprocessor.hpp>
#include <boost/parameter/name.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/tti/detail/dnullptr.hpp>
#include <boost/core/enable_if.hpp>
#include "basics.hpp"
#include <string>

#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
#include <boost/type_traits/is_convertible.hpp>
#else
#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <type_traits>
#endif

namespace test {

    BOOST_PARAMETER_NAME(x)

    struct predicate
    {
        template <class T, class Args>
        struct apply
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
          : boost::is_convertible<T,char const*>
#else
          : boost::mpl::if_<
                std::is_convertible<T,char const*>
              , boost::mpl::true_
              , boost::mpl::false_
            >
#endif
        {
        };
    };

    BOOST_PARAMETER_FUNCTION((int), sfinae, test::tag,
        (deduced
            (optional (x, *(test::predicate), BOOST_TTI_DETAIL_NULLPTR))
        )
    )
    {
        return 1;
    }

    template <class A0>
    typename boost::enable_if<
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
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
} // namespace test

int main()
{
    BOOST_TEST_EQ(1, test::sfinae());
    BOOST_TEST_EQ(1, test::sfinae("foo"));
    BOOST_TEST_EQ(0, test::sfinae(1));
    return boost::report_errors();
}

