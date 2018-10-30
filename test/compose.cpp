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
    BOOST_PARAMETER_NAME(in(lrc))
    BOOST_PARAMETER_NAME(out(lr))
    BOOST_PARAMETER_NAME(consume(rr))
}

#include <boost/config.hpp>
#include <boost/config/workaround.hpp>

#if defined(BOOST_NO_CXX11_HDR_FUNCTIONAL)
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
#if defined(BOOST_NO_CXX11_HDR_FUNCTIONAL)
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

    struct G
    {
        int i;
        int& j;
        int const& k;

        template <typename ArgPack>
        G(ArgPack const& args)
          : i(args[param::_rr])
          , j(args[param::_lr])
          , k(args[param::_lrc])
        {
        }
    };
} // namespace test

#include <utility>

namespace test {

    struct H
    {
        std::pair<int,int> i;
        std::pair<int,int>& j;
        std::pair<int,int> const& k;

        template <typename ArgPack>
        H(ArgPack const& args)
          : i(args[param::_rr])
          , j(args[param::_lr])
          , k(args[param::_lrc])
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
    int x = 23;
    int const y = 42;
#if defined(LIBS_PARAMETER_TEST_COMPILE_FAILURE_0)
    test::G g((param::_lr = 15, param::_rr = 16, param::_lrc = y));
#else
    test::G g((param::_lr = x, param::_rr = 16, param::_lrc = y));
#endif
    BOOST_TEST_EQ(16, g.i);
    BOOST_TEST_EQ(23, g.j);
    BOOST_TEST_EQ(42, g.k);
    x = 1;
    BOOST_TEST_EQ(1, g.j);
    std::pair<int,int> p0(7, 10);
    std::pair<int,int> p1(8, 9);
    std::pair<int,int> const p2(11, 12);
#if defined(LIBS_PARAMETER_TEST_COMPILE_FAILURE_1)
    test::H h((
        param::_lr = p2
      , param::_rr = std::make_pair(7, 10)
      , param::_lrc = p2
    ));
#else
    test::H h((
        param::_lr = p1
      , param::_rr = std::make_pair(7, 10)
      , param::_lrc = p2
    ));
#endif
    BOOST_TEST_EQ(p0.first, h.i.first);
    BOOST_TEST_EQ(p0.second, h.i.second);
    BOOST_TEST_EQ(p1.first, h.j.first);
    BOOST_TEST_EQ(p1.second, h.j.second);
    BOOST_TEST_EQ(p2.first, h.k.first);
    BOOST_TEST_EQ(p2.second, h.k.second);
    p1.first = 1;
    BOOST_TEST_EQ(p1.first, h.j.first);
    return boost::report_errors();
}

