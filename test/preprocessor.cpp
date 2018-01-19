// Copyright Daniel Wallin 2006.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/parameter/preprocessor.hpp>
#include <boost/parameter/keyword.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <string>
#include "basics.hpp"

#if !defined BOOST_NO_SFINAE
#include <boost/core/enable_if.hpp>
#endif

namespace test {

    BOOST_PARAMETER_BASIC_FUNCTION((int), f, test::tag,
        (required
            (tester, *)
            (name, *)
        )
        (optional
            (value, *)
            (index, (int))
        )
    )
    {
        typedef typename boost::parameter::binding<
            Args,test::tag::index,int&
        >::type index_type;

        BOOST_MPL_ASSERT((boost::is_same<index_type,int&>));

        args[test::_tester](
            args[test::_name]
          , args[test::_value | 1.f]
          , args[test::_index | 2]
        );

        return 1;
    }

    BOOST_PARAMETER_BASIC_FUNCTION((int), g, test::tag,
        (required
            (tester, *)
            (name, *)
        )
        (optional
            (value, *)
            (index, (int))
        )
    )
    {
        typedef typename boost::parameter::value_type<
            Args,test::tag::index,int
        >::type index_type;

        BOOST_MPL_ASSERT((boost::is_convertible<index_type,int>));

        args[test::_tester](
            args[test::_name]
          , args[test::_value | 1.f]
          , args[test::_index | 2]
        );

        return 1;
    }

    BOOST_PARAMETER_FUNCTION((int), h, test::tag,
        (required
            (tester, *)
            (name, *)
        )
        (optional
            (value, *, 1.f)
            (index, (int), 2)
        )
    )
    {
#if !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564)) && \
    !BOOST_WORKAROUND(BOOST_MSVC, <= 1300)
        BOOST_MPL_ASSERT((boost::is_same<index_type,int>));
#endif

        tester(name, value, index);

        return 1;
    }

    BOOST_PARAMETER_FUNCTION((int), h2, test::tag,
        (required
            (tester, *)
            (name, *)
        )
        (optional
            (value, *, 1.f)
            (index, (int), static_cast<int>(value * 2))
        )
    )
    {
#if !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564)) && \
    !BOOST_WORKAROUND(BOOST_MSVC, <= 1300)
        BOOST_MPL_ASSERT((boost::is_convertible<index_type,int>));
#endif

        tester(name, value, index);

        return 1;
    }

    struct base
    {
        template <class Args>
        base(Args const& args)
        {
            args[test::_tester](
                args[test::_name]
              , args[test::_value | 1.f]
              , args[test::_index | 2]
            );
        }
    };

    struct class_ : test::base
    {
        BOOST_PARAMETER_CONSTRUCTOR(class_, (test::base), test::tag,
            (required
                (tester, *)
                (name, *)
            )
            (optional
                (value, *)
                (index, *)
            )
        )

        BOOST_PARAMETER_BASIC_MEMBER_FUNCTION((int), f, test::tag,
            (required
                (tester, *)
                (name, *)
            )
            (optional
                (value, *)
                (index, *)
            )
        )
        {
            args[test::_tester](
                args[test::_name]
              , args[test::_value | 1.f]
              , args[test::_index | 2]
            );

            return 1;
        }

        BOOST_PARAMETER_BASIC_CONST_MEMBER_FUNCTION((int), f, test::tag,
            (required
                (tester, *)
                (name, *)
            )
            (optional
                (value, *)
                (index, *)
            )
        )
        {
            args[test::_tester](
                args[test::_name]
              , args[test::_value | 1.f]
              , args[test::_index | 2]
            );

            return 1;
        }

        BOOST_PARAMETER_MEMBER_FUNCTION((int), f2, test::tag,
            (required
                (tester, *)
                (name, *)
            )
            (optional
                (value, *, 1.f)
                (index, *, 2)
            )
        )
        {
            tester(name, value, index);
            return 1;
        }

        BOOST_PARAMETER_CONST_MEMBER_FUNCTION((int), f2, test::tag,
            (required
                (tester, *)
                (name, *)
            )
            (optional
                (value, *, 1.f)
                (index, *, 2)
            )
        )
        {
            tester(name, value, index);
            return 1;
        }

        BOOST_PARAMETER_MEMBER_FUNCTION((int), static f_static, test::tag,
            (required
                (tester, *)
                (name, *)
            )
            (optional
                (value, *, 1.f)
                (index, *, 2)
            )
        )
        {
            tester(name, value, index);
            return 1;
        }
    };

    BOOST_PARAMETER_FUNCTION((int), sfinae, test::tag,
        (required
            (name, (std::string))
        )
    )
    {
        return 1;
    }

#if !defined BOOST_NO_SFINAE
    // On compilers that actually support SFINAE, add another overload
    // that is an equally good match and can only be in the overload set
    // when the others are not.  This tests that the SFINAE is actually
    // working.  On all other compilers we're just checking that everything
    // about SFINAE-enabled code will work, except of course the SFINAE.
    template <class A0>
    typename boost::enable_if<boost::is_same<int,A0>,int>::type
    sfinae(A0 const& a0)
    {
        return 0;
    }
#endif

#if BOOST_WORKAROUND(__SUNPRO_CC, BOOST_TESTED_AT(0x580))

    // Sun has problems with this syntax:
    //
    //   template1< r* ( template2<x> ) >
    //
    // Workaround: factor template2<x> into a separate typedef.
    typedef boost::is_convertible<boost::mpl::_,std::string> predicate;

    BOOST_PARAMETER_FUNCTION((int), sfinae1, test::tag,
        (required
            (name, *(test::predicate))
        )
    )
    {
        return 1;
    }

#else // !BOOST_WORKAROUND(__SUNPRO_CC, BOOST_TESTED_AT(0x580))

    BOOST_PARAMETER_FUNCTION((int), sfinae1, test::tag,
        (required
           (name, *(boost::is_convertible<boost::mpl::_,std::string>))
        )
    )
    {
        return 1;
    }

#endif // SunProCC workarounds needed.

#if !defined BOOST_NO_SFINAE
    // On compilers that actually support SFINAE, add another overload
    // that is an equally good match and can only be in the overload set
    // when the others are not.  This tests that the SFINAE is actually
    // working.  On all other compilers we're just checking that everything
    // about SFINAE-enabled code will work, except of course the SFINAE.
    template <class A0>
    typename boost::enable_if<boost::is_same<int,A0>,int>::type
    sfinae1(A0 const& a0)
    {
        return 0;
    }
#endif

    template <class T>
    T const& as_lvalue(T const& x)
    {
        return x;
    }

    struct udt
    {
        udt(int foo_, int bar_) : foo(foo_), bar(bar_)
        {
        }

        int foo;
        int bar;
    };

    BOOST_PARAMETER_FUNCTION((int), lazy_defaults, test::tag,
        (required
            (name, *)
        )
        (optional
            (value, *, name.foo)
            (index, *, name.bar)
        )
    )
    {
        return 0;
    }
} // namespace test

int main()
{
    using namespace test;

    test::f(test::values(S("foo"), 1.f, 2), S("foo"));
    test::f(
        test::_tester = test::values(S("foo"), 1.f, 2), test::_name = S("foo")
    );

    int index_lvalue = 2;

    test::f(
        test::_tester = test::values(S("foo"), 1.f, 2)
      , test::_name = S("foo")
      , test::_value = 1.f
      , test::_index = index_lvalue
    );

    test::f(
        test::values(S("foo"), 1.f, 2)
      , S("foo")
      , 1.f
      , index_lvalue
    );

    test::g(
        test::values(S("foo"), 1.f, 2)
      , S("foo")
      , 1.f
#if BOOST_WORKAROUND(BOOST_MSVC, == 1300)
      , test::as_lvalue(2)
#else
      , 2
#endif
    );

    test::h(
        test::values(S("foo"), 1.f, 2)
      , S("foo")
      , 1.f
#if BOOST_WORKAROUND(BOOST_MSVC, == 1300)
      , test::as_lvalue(2)
#else
      , 2
#endif
    );

    test::h2(
        test::_tester = values(S("foo"), 1.f, 2)
      , test::_name = S("foo")
      , test::_value = 1.f
    );

    test::class_ x(test::values(S("foo"), 1.f, 2), S("foo"), test::_index = 2);

    x.f(test::values(S("foo"), 1.f, 2), S("foo"));
    x.f(
        test::_tester = test::values(S("foo"), 1.f, 2), test::_name = S("foo")
    );
    x.f2(test::values(S("foo"), 1.f, 2), S("foo"));
    x.f2(
        test::_tester = test::values(S("foo"), 1.f, 2), test::_name = S("foo")
    );

    test::class_ const& x_const = x;

    x_const.f(test::values(S("foo"), 1.f, 2), S("foo"));
    x_const.f(
        test::_tester = test::values(S("foo"), 1.f, 2), test::_name = S("foo")
    );
    x_const.f2(test::values(S("foo"), 1.f, 2), S("foo"));
    x_const.f2(
        test::_tester = test::values(S("foo"), 1.f, 2), test::_name = S("foo")
    );
    test::class_::f_static(test::values(S("foo"), 1.f, 2), S("foo"));
    test::class_::f_static(
        test::_tester = test::values(S("foo"), 1.f, 2), test::_name = S("foo")
    );

#if !defined(BOOST_NO_SFINAE) && \
    !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x592))
    BOOST_TEST(test::sfinae("foo") == 1);
    BOOST_TEST(test::sfinae(1) == 0);

#if !BOOST_WORKAROUND(__SUNPRO_CC, BOOST_TESTED_AT(0x580))
    // Sun actually eliminates the desired overload for some reason.
    // Disabling this part of the test because SFINAE abilities are
    // not the point of this test.
    BOOST_TEST(test::sfinae1("foo") == 1);
#endif
    
    BOOST_TEST(test::sfinae1(1) == 0);
#endif

    test::lazy_defaults(test::_name = test::udt(0,1));
    test::lazy_defaults(test::_name = 0, test::_value = 1, test::_index = 2);

    return boost::report_errors();
}

