// Copyright Daniel Wallin 2005. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/python.hpp>
#include <boost/parameter/python/general.hpp>
#include <boost/parameter/parameters.hpp>

namespace mpl = boost::mpl;

BOOST_PARAMETER_KEYWORD(tag, x)
BOOST_PARAMETER_KEYWORD(tag, y)
BOOST_PARAMETER_KEYWORD(tag, z)

struct tag::x
{
    static char const* keyword()
    {
        return "x";
    }
};


struct tag::y
{
    static char const* keyword()
    {
        return "y";
    }
};

struct tag::z
{
    static char const* keyword()
    {
        return "z";
    }
};

typedef boost::parameter::parameters<
    tag::x
  , tag::y
> f_parameters;

template <class Args>
float f_impl(Args const& args)
{
    return args[x | 1] / args[y | 1];
}

template <class A0>
float f(A0 const& a0)
{
    return f_impl(f_parameters()(a0));
}

template <class A0, class A1>
float f(A0 const& a0, A1 const& a1)
{
    return f_impl(f_parameters()(a0,a1));
}

template <class A0, class A1, class A2>
float f(A0 const& a0, A1 const& a1, A2 const& a2)
{
    return f_impl(f_parameters()(a0,a1,a2));
}

struct meta
{
    typedef mpl::vector3<
        mpl::pair<tag::x, mpl::true_>
      , mpl::pair<tag::y, mpl::false_>
      , mpl::pair<tag::z, mpl::false_>
    > keywords;

    template <class R, class A0>
    R operator()(boost::type<R>, A0 const& a0)
    {
        return f(a0);
    }

    template <class R, class A0, class A1>
    R operator()(boost::type<R>, A0 const& a0, A1 const& a1)
    {
        return f(a0,a1);
    }

    template <class R, class A0, class A1, class A2>
    R operator()(boost::type<R>, A0 const& a0, A1 const& a1, A2 const& a2)
    {
        return f(a0,a1,a2);
    }
};

BOOST_PYTHON_MODULE(general)
{ 
    boost::parameter::python::def<meta>("f", mpl::vector4<float,float,float,float>());
}

