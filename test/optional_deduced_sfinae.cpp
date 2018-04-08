// Copyright Daniel Wallin 2006.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/parameter/preprocessor.hpp>
#include <boost/parameter/name.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/tti/detail/dnullptr.hpp>
#include <boost/core/enable_if.hpp>
#include "basics.hpp"
#include <string>

namespace test {

    BOOST_PARAMETER_NAME(x)

#if BOOST_WORKAROUND(__SUNPRO_CC, BOOST_TESTED_AT(0x580))
    // Sun has problems with this syntax:
    //
    //   template1< r* ( template2<x> ) >
    //
    // Workaround: factor template2<x> into a separate typedef
    typedef boost::is_convertible<boost::mpl::_,char const*> predicate;

    BOOST_PARAMETER_FUNCTION((int), sfinae, test::tag,
        (deduced
            (optional (x, *(test::predicate), BOOST_TTI_DETAIL_NULLPTR))
        )
    )
    {
        return 1;
    }
#else
    BOOST_PARAMETER_FUNCTION((int), sfinae, test::tag,
        (deduced
            (optional
                (x, *(boost::is_convertible<boost::mpl::_,char const*>),
                    BOOST_TTI_DETAIL_NULLPTR
                )
            )
        )
    )
    {
        return 1;
    }
#endif // SunPro CC workarounds needed.

    template <class A0>
    typename boost::enable_if<boost::is_same<int,A0>,int>::type
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

