// Copyright Rene Rivera 2006.
// Copyright Cromwell D. Enage 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/parameter/name.hpp>

namespace param {

    BOOST_PARAMETER_NAME(a0)
    BOOST_PARAMETER_NAME(a1)
    BOOST_PARAMETER_NAME(a2)
    BOOST_PARAMETER_NAME(a3)
}

#include <boost/config.hpp>

#if defined BOOST_NO_CXX11_HDR_FUNCTIONAL
#include <boost/function.hpp>
#else
#include <functional>
#endif

namespace test {

    struct A
    {
        int i;
        int j;
        
        template <typename ArgPack>
        A(ArgPack const& args) : i(args[param::_a0]), j(args[param::_a1])
        {
        }
    };

    float F()
    {
        return 4.0f;
    }

    float E()
    {
        return 4.625f;
    }

    double D()
    {
        return 198.9;
    }

    struct C
    {
        struct result_type
        {
            double operator()() const
            {
                return 2.5;
            }
        };

        result_type operator()() const
        {
            return result_type();
        }
    };

    struct B : A
    {
#if defined BOOST_NO_CXX11_HDR_FUNCTIONAL
        boost::function<float()> k;
        boost::function<double()> l;
#else
        std::function<float()> k;
        std::function<double()> l;
#endif

        template <typename ArgPack>
        B(ArgPack const& args)
          : A((args, param::_a0 = 1))
          , k(args[param::_a2 | E])
          , l(args[param::_a3 || C()])
        {
        }
    };
} // namespace test

#include <boost/core/lightweight_test.hpp>

int main()
{
    test::A a((param::_a0 = 1, param::_a1 = 13, param::_a2 = test::D));
    BOOST_TEST_EQ(1, a.i);
    BOOST_TEST_EQ(13, a.j);
    test::B b0((param::_a1 = 13, param::_a2 = test::F));
    BOOST_TEST_EQ(1, b0.i);
    BOOST_TEST_EQ(13, b0.j);
    BOOST_TEST_EQ(4.0f, b0.k());
    BOOST_TEST_EQ(2.5, b0.l());
    test::B b1((param::_a3 = test::D, param::_a1 = 13));
    BOOST_TEST_EQ(1, b1.i);
    BOOST_TEST_EQ(13, b1.j);
    BOOST_TEST_EQ(4.625f, b1.k());
    BOOST_TEST_EQ(198.9, b1.l());
    return boost::report_errors();
}

