// Copyright Frank Mori Hess 2009.
// Copyright Cromwell D. Enage 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/parameter/config.hpp>

#if !defined(BOOST_GCC) || ( \
        BOOST_WORKAROUND(BOOST_GCC, < 40900) \
    ) || ( \
        !defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING) && \
        BOOST_WORKAROUND(BOOST_GCC, >= 40900) && \
        BOOST_WORKAROUND(BOOST_GCC, < 50000) \
    ) || ( \
        BOOST_WORKAROUND(BOOST_GCC, >= 50000) \
    )
#define LIBS_PARAMETER_TEST_WILL_NOT_ICE
#endif

#if defined LIBS_PARAMETER_TEST_WILL_NOT_ICE

#include <boost/mpl/assert.hpp>
#include <boost/parameter.hpp>

#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
#include <boost/type_traits/is_same.hpp>
#else
#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <type_traits>
#endif

#if defined(BOOST_NO_CXX11_HDR_FUNCTIONAL)
#include <boost/function.hpp>
#else
#include <functional>
#endif

namespace test {
    namespace keywords {

        BOOST_PARAMETER_TEMPLATE_KEYWORD(function_type)
    } // namespace keywords

    template <typename K, typename A>
    struct X
      : boost::parameter::value_type<
            typename boost::parameter::parameters<
                boost::parameter::required<K>
            >::BOOST_NESTED_TEMPLATE bind<
                A
            >::type
          , K
        >
    {
    };

    template <typename T>
    struct Y
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
      : boost::is_same<
#else
      : boost::mpl::if_<
            std::is_same<
#endif
                typename X<
                    test::keywords::tag::function_type
                  , test::keywords::function_type<T>
                >::type
#if defined(BOOST_NO_CXX11_HDR_FUNCTIONAL)
              , boost::function<T>
#else
              , std::function<T>
#endif
            >
#if !defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
          , boost::mpl::true_
          , boost::mpl::false_
        >::type
#endif
    {
    };
} // namespace test

#endif // Compiler won't ICE.

#include <boost/mpl/aux_/test.hpp>

MPL_TEST_CASE()
{
#if defined LIBS_PARAMETER_TEST_WILL_NOT_ICE
    BOOST_MPL_ASSERT((test::Y<void()>));
    BOOST_MPL_ASSERT_NOT((test::Y<int>));
    BOOST_MPL_ASSERT((test::Y<double(double)>));
#endif
}

