// Copyright Daniel Wallin 2006.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/parameter/parameters.hpp>
#include <boost/parameter/name.hpp>
#include <boost/parameter/binding.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include "deduced.hpp"

namespace parameter = boost::parameter;
namespace mpl = boost::mpl;

BOOST_PARAMETER_NAME(x)
BOOST_PARAMETER_NAME(y)
BOOST_PARAMETER_NAME(z)

int main()
{
    using namespace parameter;

    check<
        parameters<
            tag::x
          , tag::y
        >
    >(
        (_x = 0, _y = 1)
      , 0
      , 1
    );

    check<
        parameters<
            tag::x
          , required<deduced<tag::y>, boost::is_convertible<mpl::_, int> >
          , optional<deduced<tag::z>, boost::is_convertible<mpl::_, char const*> >
        >
    >(
        (_x = 0, _y = not_present, _z = "foo")
      , _x = 0
      , "foo"
    );

    check<
        parameters<
            tag::x
          , required<deduced<tag::y>, boost::is_convertible<mpl::_, int> >
          , optional<deduced<tag::z>, boost::is_convertible<mpl::_, char const*> >
        >
    >(
        (_x = 0, _y = 1, _z = "foo")
      , 0
      , "foo"
      , 1
    );

    check<
        parameters<
            tag::x
          , required<deduced<tag::y>, boost::is_convertible<mpl::_, int> >
          , optional<deduced<tag::z>, boost::is_convertible<mpl::_, char const*> >
        >
    >(
        (_x = 0, _y = 1, _z = "foo")
      , 0
      , 1
      , "foo"
    );

    check<
        parameters<
            tag::x
          , required<deduced<tag::y>, boost::is_convertible<mpl::_, int> >
          , optional<deduced<tag::z>, boost::is_convertible<mpl::_, char const*> >
        >
    >(
        (_x = 0, _y = 1, _z = "foo")
      , 0
      , _y = 1
      , "foo"
    );

    check<
        parameters<
            tag::x
          , required<deduced<tag::y>, boost::is_convertible<mpl::_, int> >
          , optional<deduced<tag::z>, boost::is_convertible<mpl::_, char const*> >
        >
    >(
        (_x = 0, _y = 1, _z = "foo")
      , _z = "foo"
      , _x = 0
      , 1
    );

#if 0
    // Fails because boost::parameter::aux::make_arg_list<> evaluates
    // boost::parameter::aux::is_named_argument<> to boost::mpl::false_
    // for (long*)0.
    check<
        parameters<
            tag::x
          , required<deduced<tag::y>, boost::is_convertible<mpl::_, int> >
          , optional<deduced<tag::z>, boost::is_convertible<mpl::_, char const*> >
        >
    >(
        (_x = 0, _y = 1, _z = "foo")
      , _x = 0
      , (long*)0
      , 1
    );
#endif

    return boost::report_errors();
}

