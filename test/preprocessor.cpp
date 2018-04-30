// Copyright Daniel Wallin 2006.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/parameter/config.hpp>

#if !defined(BOOST_GCC) || BOOST_WORKAROUND(BOOST_GCC, < 40800) || ( \
        defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING) && \
        BOOST_WORKAROUND(BOOST_GCC, >= 40900) \
    )
#define LIBS_PARAMETER_TEST_WILL_NOT_ICE
#endif

#include <boost/config/pragma_message.hpp>

#if defined LIBS_PARAMETER_TEST_WILL_NOT_ICE

#include <boost/parameter/preprocessor.hpp>
#include <boost/parameter/keyword.hpp>
#include <string>
#include "basics.hpp"

#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
#include <boost/type_traits/is_same.hpp>
#else
#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <type_traits>
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

#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
        BOOST_MPL_ASSERT((boost::is_same<index_type,int&>));
#else
        BOOST_MPL_ASSERT((
            typename boost::mpl::if_<
                std::is_same<index_type,int&>
              , boost::mpl::true_
              , boost::mpl::false_
            >::type
        ));
#endif

        args[test::_tester](
            args[test::_name]
          , args[test::_value | 1.f]
          , args[test::_index | 2]
        );

        return 1;
    }
} // namespace test

#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
#include <boost/type_traits/remove_const.hpp>
#endif

namespace test {

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

#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
        BOOST_MPL_ASSERT((
            boost::is_same<typename boost::remove_const<index_type>::type,int>
        ));
#else
        BOOST_MPL_ASSERT((
            typename boost::mpl::if_<
                std::is_same<typename std::remove_const<index_type>::type,int>
              , boost::mpl::true_
              , boost::mpl::false_
            >::type
        ));
#endif

        args[test::_tester](
            args[test::_name]
          , args[test::_value | 1.f]
          , args[test::_index | 2]
        );

        return 1;
    }
} // namespace test

#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
#include <boost/type_traits/remove_reference.hpp>
#endif

namespace test {

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
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
        BOOST_MPL_ASSERT((
            boost::is_same<
                typename boost::remove_const<
                    typename boost::remove_reference<index_type>::type
                >::type
              , int
            >
        ));
#else
        BOOST_MPL_ASSERT((
            typename boost::mpl::if_<
                std::is_same<
                    typename std::remove_const<
                        typename std::remove_reference<index_type>::type
                    >::type
                  , int
                >
              , boost::mpl::true_
              , boost::mpl::false_
            >::type
        ));
#endif // BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS
#endif // Borland/MSVC workarounds not needed.

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
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
        BOOST_MPL_ASSERT((
            boost::is_same<
                typename boost::remove_const<
                    typename boost::remove_reference<index_type>::type
                >::type
              , int
            >
        ));
#else
        BOOST_MPL_ASSERT((
            typename boost::mpl::if_<
                std::is_same<
                    typename std::remove_const<
                        typename std::remove_reference<index_type>::type
                    >::type
                  , int
                >
              , boost::mpl::true_
              , boost::mpl::false_
            >::type
        ));
#endif // BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS
#endif // Borland/MSVC workarounds not needed.

        tester(name, value, index);

        return 1;
    }
} // namespace test

#if !defined(BOOST_NO_SFINAE)
#include <boost/tti/detail/dnullptr.hpp>
#include <boost/core/enable_if.hpp>
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
#include <boost/type_traits/is_base_of.hpp>
#endif
#endif

namespace test {

    struct base_0
    {
        float f;
        int i;

        template <typename Args>
        explicit base_0(
            Args const& args
#if !defined(BOOST_NO_SFINAE)
          , typename boost::disable_if<
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
                boost::is_base_of<base_0,Args>
#else
                typename boost::mpl::if_<
                    std::is_base_of<base_0,Args>
                  , boost::mpl::true_
                  , boost::mpl::false_
                >::type
#endif
            >::type* = BOOST_TTI_DETAIL_NULLPTR
#endif // BOOST_NO_SFINAE
        ) : f(args[test::_value | 1.f]), i(args[test::_index | 2])
        {
        }
    };

    struct class_0 : test::base_0
    {
        BOOST_PARAMETER_CONSTRUCTOR(class_0, (test::base_0), test::tag,
            (optional
                (value, *)
                (index, *)
            )
        )
    };

    struct base_1
    {
        template <typename Args>
        explicit base_1(
            Args const& args
#if !defined(BOOST_NO_SFINAE)
          , typename boost::disable_if<
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
                boost::is_base_of<base_1,Args>
#else
                typename boost::mpl::if_<
                    std::is_base_of<base_1,Args>
                  , boost::mpl::true_
                  , boost::mpl::false_
                >::type
#endif
            >::type* = BOOST_TTI_DETAIL_NULLPTR
#endif // BOOST_NO_SFINAE
        )
        {
            args[test::_tester](
                args[test::_name]
              , args[test::_value | 1.f]
              , args[test::_index | 2]
            );
        }
    };

    struct class_1 : test::base_1
    {
        BOOST_PARAMETER_CONSTRUCTOR(class_1, (test::base_1), test::tag,
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

#if !defined(BOOST_NO_SFINAE)
    // On compilers that actually support SFINAE, add another overload
    // that is an equally good match and can only be in the overload set
    // when the others are not.  This tests that the SFINAE is actually
    // working.  On all other compilers we're just checking that everything
    // about SFINAE-enabled code will work, except of course the SFINAE.
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
#endif // BOOST_NO_SFINAE

    struct predicate
    {
        template <typename T, typename Args>
        struct apply
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
          : boost::is_convertible<T,std::string>
#else
          : boost::mpl::if_<
                std::is_convertible<T,std::string>
              , boost::mpl::true_
              , boost::mpl::false_
            >
#endif
        {
        };
    };

    BOOST_PARAMETER_FUNCTION((int), sfinae1, test::tag,
        (required
            (name, *(test::predicate))
        )
    )
    {
        return 1;
    }

#if !defined(BOOST_NO_SFINAE)
    // On compilers that actually support SFINAE, add another overload
    // that is an equally good match and can only be in the overload set
    // when the others are not.  This tests that the SFINAE is actually
    // working.  On all other compilers we're just checking that everything
    // about SFINAE-enabled code will work, except of course the SFINAE.
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
        sfinae1(A0 const& a0)
    {
        return 0;
    }
#endif // BOOST_NO_SFINAE

    template <typename T>
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

BOOST_PRAGMA_MESSAGE("Test should compile.");
#else
BOOST_PRAGMA_MESSAGE("Test not compiled.");
#include <boost/core/lightweight_test.hpp>
#endif // Compiler won't ICE.

int main()
{
#if defined LIBS_PARAMETER_TEST_WILL_NOT_ICE
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
        test::_tester = test::values(S("foo"), 1.f, 2)
      , test::_name = S("foo")
      , test::_value = 1.f
    );

    test::class_0 u;

    BOOST_TEST(2 == u.i);
    BOOST_TEST(1.f == u.f);

    test::class_1 x(
        test::values(S("foo"), 1.f, 2), S("foo"), test::_index = 2
    );

    x.f(test::values(S("foo"), 1.f, 2), S("foo"));
    x.f(
        test::_tester = test::values(S("foo"), 1.f, 2), test::_name = S("foo")
    );
    x.f2(test::values(S("foo"), 1.f, 2), S("foo"));
    x.f2(
        test::_tester = test::values(S("foo"), 1.f, 2), test::_name = S("foo")
    );

    test::class_1 const& x_const = x;

    x_const.f(test::values(S("foo"), 1.f, 2), S("foo"));
    x_const.f(
        test::_tester = test::values(S("foo"), 1.f, 2), test::_name = S("foo")
    );
    x_const.f2(test::values(S("foo"), 1.f, 2), S("foo"));
    x_const.f2(
        test::_tester = test::values(S("foo"), 1.f, 2), test::_name = S("foo")
    );
    test::class_1::f_static(test::values(S("foo"), 1.f, 2), S("foo"));
    test::class_1::f_static(
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
#endif // Compiler won't ICE.

    return boost::report_errors();
}

