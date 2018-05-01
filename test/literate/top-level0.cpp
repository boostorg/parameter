
#include <boost/parameter/config.hpp>

#if !defined(LIBS_PARAMETER_TEST_WILL_NOT_ICE)
/*
#if !defined(BOOST_GCC) || ( \
        defined(__MINGW32__) && (1 == __MINGW32__) \
    ) || BOOST_WORKAROUND(BOOST_GCC, < 40800) || \
    defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING) || ( \
        BOOST_WORKAROUND(BOOST_GCC, >= 60000) && \
        BOOST_WORKAROUND(BOOST_GCC, < 70000) \
    )
*/
#define LIBS_PARAMETER_TEST_WILL_NOT_ICE
//#endif
#endif

#include <boost/core/lightweight_test.hpp>
#include <boost/config/pragma_message.hpp>

#if defined(LIBS_PARAMETER_TEST_WILL_NOT_ICE)

#include <boost/parameter.hpp>

namespace test {

    BOOST_PARAMETER_NAME(title)
    BOOST_PARAMETER_NAME(width)
    BOOST_PARAMETER_NAME(titlebar)

    BOOST_PARAMETER_FUNCTION((int), new_window, tag,
        (required (title,*)(width,*)(titlebar,*))
    )
    {
        return 0;
    }

    BOOST_PARAMETER_TEMPLATE_KEYWORD(deleter)
    BOOST_PARAMETER_TEMPLATE_KEYWORD(copy_policy)

    template <typename T>
    struct Deallocate
    {
    };

    struct DeepCopy
    {
    };

    namespace parameter = boost::parameter;

    struct Foo
    {
    };

    template <typename T, typename A0, typename A1>
    struct smart_ptr
    {
        smart_ptr(Foo*)
        {
        }
    };
}

using namespace test;

#else
BOOST_PRAGMA_MESSAGE("Test not compiled.");
#endif // Compiler won't ICE.

int main()
{
#if defined(LIBS_PARAMETER_TEST_WILL_NOT_ICE)
    int x = test::new_window("alert", test::_width=10, test::_titlebar=false);
    Foo* foo = new Foo();
    test::smart_ptr<
        Foo
      , test::deleter<test::Deallocate<Foo> >
      , test::copy_policy<test::DeepCopy>
    > p(foo);
    delete foo;
#endif
    return boost::report_errors();
}

