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
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/tti/detail/dnullptr.hpp>
#include <boost/config.hpp>

#if !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
#include <boost/mpl/eval_if.hpp>
#endif

#if !defined BOOST_NO_CXX11_HDR_FUNCTIONAL
#include <functional>
#endif

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

#if !defined BOOST_NO_CXX11_HDR_FUNCTIONAL
    // Support for std::ref(x) -- Cromwell D. Enage
    template <class U>
    boost::parameter::aux::yes_tag
    is_cv_reference_wrapper_check(std::reference_wrapper<U> const volatile*);
#endif

    boost::parameter::aux::no_tag is_cv_reference_wrapper_check(...);

    // This metafunction returns mpl::true_ if T is of type
    // reference_wrapper<U> cv.
    template <class T>
    struct is_cv_reference_wrapper
    {
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
    struct unwrap_cv_reference : boost::remove_reference<T>
    {
    };

    template <class T>
    struct unwrap_cv_reference<T const, boost::mpl::false_>
    {
        typedef T const type;
    };

    template <class T>
    struct unwrap_cv_reference<T, boost::mpl::true_> : T
    {
    };
#else // !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
    // Needed for unwrap_cv_reference below. T might be const, so
    // eval_if<> might fail because of deriving from T const on EDG.
    template <class T>
    struct get_type : remove_reference<T>::type
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
          , boost::remove_reference<T>
        >
    {
    };
#endif // Borland workarounds needed.
}}} // namespace boost::parameter::aux

#endif // UNWRAP_CV_REFERENCE_050328_HPP

