
#include <boost/parameter/config.hpp>

#if !defined(BOOST_GCC) || BOOST_WORKAROUND(BOOST_GCC, < 40800) || \
    defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING) || ( \
        BOOST_WORKAROUND(BOOST_GCC, >= 40900) && \
        BOOST_WORKAROUND(BOOST_GCC, < 70000) \
    )
#define LIBS_PARAMETER_TEST_WILL_NOT_ICE
#endif

#include <boost/config/pragma_message.hpp>

#if defined LIBS_PARAMETER_TEST_WILL_NOT_ICE

#include <boost/parameter.hpp>

namespace boost { namespace python {

    namespace tag {

        struct class_type;  // keyword tag type
    }

    template <typename T>
    struct class_type
      : parameter::template_keyword<tag::class_type,T>
    {
    };
}}

BOOST_PRAGMA_MESSAGE("Test should compile.");
#else
BOOST_PRAGMA_MESSAGE("Test not compiled.");
#endif // Compiler won't ICE.

