// Copyright Daniel Wallin 2006.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/parameter/config.hpp>

#if !defined(LIBS_PARAMETER_TEST_WILL_NOT_ICE)
/*
#if !defined(BOOST_GCC) || ( \
        defined(__MINGW32__) && (1 == __MINGW32__) \
    ) || BOOST_WORKAROUND(BOOST_GCC, < 40800) || ( \
        defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING) && \
        BOOST_WORKAROUND(BOOST_GCC, >= 40800) && \
        BOOST_WORKAROUND(BOOST_GCC, < 40900) \
    ) || ( \
        BOOST_WORKAROUND(BOOST_GCC, >= 40900) && \
        BOOST_WORKAROUND(BOOST_GCC, < 50000) \
    )
*/
#define LIBS_PARAMETER_TEST_WILL_NOT_ICE
//#endif
#endif

#include <boost/config/pragma_message.hpp>

#if defined(LIBS_PARAMETER_TEST_WILL_NOT_ICE)

#include <boost/parameter/preprocessor.hpp>
#include <boost/parameter/name.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/tti/detail/dnullptr.hpp>
#include <boost/core/enable_if.hpp>
#include "basics.hpp"
#include <string>

#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
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
        template <typename T, typename Args>
        struct apply
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
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
} // namespace test

#else
BOOST_PRAGMA_MESSAGE("Test not compiled.");
#include <boost/core/lightweight_test.hpp>
#endif // Compiler won't ICE.

int main()
{
#if defined(LIBS_PARAMETER_TEST_WILL_NOT_ICE)
    BOOST_TEST_EQ(1, test::sfinae());
    BOOST_TEST_EQ(1, test::sfinae("foo"));
    BOOST_TEST_EQ(0, test::sfinae(1));
#endif
    return boost::report_errors();
}

