// Copyright Daniel Wallin, David Abrahams 2005.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef UNWRAP_CV_REFERENCE_050328_HPP
#define UNWRAP_CV_REFERENCE_050328_HPP

namespace boost {

    template <class T>
    class reference_wrapper;
} // namespace boost

#include <boost/parameter/aux_/yesno.hpp>

namespace boost { namespace parameter { namespace aux {

    //
    // reference_wrapper support -- if perfect forwarding is unsupported,
    // then when passing arguments positionally by non-const reference,
    // we ask users of named parameter interfaces to use ref(x) to wrap them.
    //

    template <class U>
    boost::parameter::aux::yes_tag
    is_cv_reference_wrapper_check(
        boost::reference_wrapper<U> const volatile*
    );

    boost::parameter::aux::no_tag is_cv_reference_wrapper_check(...);
}}} // namespace boost::parameter::aux

#include <boost/config.hpp>

#if !defined(BOOST_NO_CXX11_HDR_FUNCTIONAL)
#include <functional>

namespace boost { namespace parameter { namespace aux {

    // Support for std::ref(x) -- Cromwell D. Enage
    template <class U>
    boost::parameter::aux::yes_tag
    is_cv_reference_wrapper_check(std::reference_wrapper<U> const volatile*);
}}} // namespace boost::parameter::aux
#endif

#include <boost/mpl/bool.hpp>
#include <boost/tti/detail/dnullptr.hpp>
#include <boost/config/workaround.hpp>

#if !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
#include <boost/mpl/eval_if.hpp>
#endif

#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
#include <boost/type_traits/remove_reference.hpp>
#else
#include <type_traits>
#endif

namespace boost { namespace parameter { namespace aux {

    // This metafunction returns mpl::true_ if T is of type
    // reference_wrapper<U> cv.
    template <class T>
    struct is_cv_reference_wrapper
    {
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
        BOOST_STATIC_CONSTANT(
            bool, value = (
                sizeof(
                    boost::parameter::aux::is_cv_reference_wrapper_check(
                        static_cast<
                            typename boost::remove_reference<T>::type*
                        >(BOOST_TTI_DETAIL_NULLPTR)
                    )
                ) == sizeof(boost::parameter::aux::yes_tag)
            )
        );
#else
        BOOST_STATIC_CONSTANT(
            bool, value = (
                sizeof(
                    boost::parameter::aux::is_cv_reference_wrapper_check(
                        static_cast<
                            typename std::remove_reference<T>::type*
                        >(BOOST_TTI_DETAIL_NULLPTR)
                    )
                ) == sizeof(boost::parameter::aux::yes_tag)
            )
        );
#endif // BOOST_NO_CXX11_HDR_TYPE_TRAITS

        typedef boost::mpl::bool_<
#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
            is_cv_reference_wrapper::
#endif 
        value> type;
    };

#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
    template <
        class T
      , class = typename boost::parameter::aux::is_cv_reference_wrapper<
            T
        >::type
    >
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
    struct unwrap_cv_reference : boost::remove_reference<T>
#else
    struct unwrap_cv_reference : std::remove_reference<T>
#endif
    {
    };

    template <class T>
    struct unwrap_cv_reference<T const,boost::mpl::false_>
    {
        typedef T const type;
    };

    template <class T>
    struct unwrap_cv_reference<T,boost::mpl::true_> : T
    {
    };
#else // !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
    // Needed for unwrap_cv_reference below. T might be const, so
    // eval_if<> might fail because of deriving from T const on EDG.
    template <class T>
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
    struct get_type : boost::remove_reference<T>::type
#else
    struct get_type : std::remove_reference<T>::type
#endif
    {
    };

    // Produces the unwrapped type to hold a reference to in
    // tagged_argument<>.  Can't use boost::unwrap_reference<> here
    // because it doesn't handle the case where T = reference_wrapper<U> cv.
    template <class T>
    struct unwrap_cv_reference
      : boost::mpl::eval_if<
            boost::parameter::aux::is_cv_reference_wrapper<T>
          , boost::parameter::aux::get_type<T>
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
          , boost::remove_reference<T>
#else
          , std::remove_reference<T>
#endif
        >
    {
    };
#endif // Borland workarounds needed.
}}} // namespace boost::parameter::aux

#endif // UNWRAP_CV_REFERENCE_050328_HPP

