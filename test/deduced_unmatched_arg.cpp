// Copyright Daniel Wallin 2006.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/parameter/name.hpp>

BOOST_PARAMETER_NAME(x)

#include <boost/parameter/parameters.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/config.hpp>

#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
#include <boost/type_traits/is_convertible.hpp>
#else
#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <type_traits>
#endif

int main()
{
    using namespace parameter;
    using boost::is_convertible;
    using mpl::_;

    boost::parameter::parameters<
        boost::parameter::optional<
            boost::parameter::deduced<tag::x>
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
          , boost::is_convertible<boost::mpl::_,int>
#else
          , boost::mpl::if_<
                std::is_convertible<boost::mpl::_,int>
              , boost::mpl::true_
              , boost::mpl::false_
            >
#endif
        >
    >()("foo");

    return 0;
}

