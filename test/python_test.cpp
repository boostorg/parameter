// Copyright Daniel Wallin 2006.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/python.hpp>
#include <boost/parameter/preprocessor.hpp>
#include <boost/parameter/name.hpp>
#include <boost/parameter/python.hpp>
#include <boost/config.hpp>
#include <cmath>

#if !defined(BOOST_NO_SFINAE)
#include <boost/tti/detail/dnullptr.hpp>
#include <boost/core/enable_if.hpp>
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
#include <boost/type_traits/is_base_of.hpp>
#else
#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <type_traits>
#endif
#endif

namespace test {

    BOOST_PARAMETER_NAME(x)
    BOOST_PARAMETER_NAME(y)
    BOOST_PARAMETER_NAME(z)

    struct Xbase
    {
        template <typename Args>
        Xbase(
            Args const& args
#if !defined(BOOST_NO_SFINAE)
            // We need the disable_if part for VC7.1/8.0.
          , typename boost::disable_if<
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
                boost::is_base_of<Xbase,Args>
#else
                typename boost::mpl::if_<
                    std::is_base_of<Xbase,Args>
                  , boost::mpl::true_
                  , boost::mpl::false_
                >::type
#endif
            >::type* = BOOST_TTI_DETAIL_NULLPTR
#endif // BOOST_NO_SFINAE
        ) : value(
                std::string(args[test::_x | "foo"]) + args[test::_y | "bar"]
            )
        {
        }

        std::string value;
    };

    struct X : test::Xbase
    {
        BOOST_PARAMETER_CONSTRUCTOR(X, (test::Xbase), test::tag,
            (optional
                (x, *)
                (y, *)
            )
        )

        BOOST_PARAMETER_BASIC_MEMBER_FUNCTION((int), f, test::tag,
            (required
                (x, *)
                (y, *)
            )
            (optional
                (z, *)
            )
        )
        {
            return args[test::_x] + args[test::_y] + args[test::_z | 0];
        }

        BOOST_PARAMETER_BASIC_MEMBER_FUNCTION((std::string), g, test::tag,
            (optional
                (x, *)
                (y, *)
            )
        )
        {
            return std::string(
                args[test::_x | "foo"]
            ) + args[test::_y | "bar"];
        }

        BOOST_PARAMETER_MEMBER_FUNCTION((X&), h, test::tag,
            (optional (x, *, "") (y, *, ""))
        )
        {
            return *this;
        }

        template <class A0>
        X& operator()(A0 const& a0)
        {
            return *this;
        }
    };
} // namespace test

struct f_fwd
{
    template <typename R, typename T, typename A0, typename A1, typename A2>
    R
        operator()(
            boost::type<R>
          , T& self
          , A0 const& a0
          , A1 const& a1
          , A2 const& a2
        )
    {
        return self.f(a0, a1, a2);
    }
};

struct g_fwd
{
    template <typename R, typename T, typename A0, typename A1>
    R operator()(boost::type<R>, T& self, A0 const& a0, A1 const& a1)
    {
        return self.g(a0, a1);
    }
};

struct h_fwd
{
    template <typename R, typename T>
    R operator()(boost::type<R>, T& self)
    {
        return self.h();
    }

    template <typename R, typename T, typename A0>
    R operator()(boost::type<R>, T& self, A0 const& a0)
    {
        return self.h(a0);
    }

    template <typename R, typename T, typename A0, typename A1>
    R operator()(boost::type<R>, T& self, A0 const& a0, A1 const& a1)
    {
        return self.h(a0, a1);
    }
};

BOOST_PYTHON_MODULE(python_test_ext)
{
    namespace mpl = boost::mpl;
    using namespace test;
    using namespace boost::python;

    class_<X>("X")
        .def(
            boost::parameter::python::init<
                mpl::vector<
                    tag::x*(std::string), tag::y*(std::string)
                >
            >()
        )
        .def(
            "f"
          , boost::parameter::python::function<
                f_fwd
              , mpl::vector<
                    int, tag::x(int), tag::y(int), tag::z*(int)
                >
            >()
        )
        .def(
            "g"
          , boost::parameter::python::function<
                g_fwd
              , mpl::vector<
                    std::string, tag::x*(std::string), tag::y*(std::string)
                >
            >()
        )
        .def(
            "h"
          , boost::parameter::python::function<
                h_fwd
              , mpl::vector<
                    X&, tag::x**(std::string), tag::y**(std::string)
                >
            >()
          , return_arg<>()
        )
        .def(
            boost::parameter::python::call<
                mpl::vector<
                    X&, tag::x(int)
                >
            >() [ return_arg<>() ]
        )
        .def_readonly("value", &X::value);
}

