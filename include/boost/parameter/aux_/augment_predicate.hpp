// Copyright Cromwell D. Enage 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_AUGMENT_PREDICATE_HPP
#define BOOST_PARAMETER_AUGMENT_PREDICATE_HPP

namespace boost { namespace parameter { namespace aux {

    struct lambda_tag;
}}} // namespace boost::parameter::aux

#include <boost/parameter/keyword_fwd.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/config.hpp>

#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
#include <boost/type_traits/is_lvalue_reference.hpp>
#include <boost/type_traits/is_same.hpp>
#else
#include <type_traits>
#endif

namespace boost { namespace parameter { namespace aux {

    template <class R, class Tag>
    struct augment_predicate_check_consume_ref
      : boost::mpl::eval_if<
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
            boost::is_same<
#else
            std::is_same<
#endif
                typename Tag::qualifier
              , boost::parameter::consume_reference
            >
          , boost::mpl::if_<
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
                boost::is_lvalue_reference<R>
#else
                std::is_lvalue_reference<R>
#endif
              , boost::mpl::false_
              , boost::mpl::true_
            >
          , boost::mpl::true_
        >::type
    {
    };
}}} // namespace boost::parameter::aux

#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS) || defined(BOOST_MSVC)
#include <boost/type_traits/is_const.hpp>
#endif

namespace boost { namespace parameter { namespace aux {

    template <class V, class R, class Tag>
    struct augment_predicate_check_out_ref
      : boost::mpl::eval_if<
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
            boost::is_same<
#else
            std::is_same<
#endif
                typename Tag::qualifier
              , boost::parameter::out_reference
            >
          , boost::mpl::eval_if<
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
                boost::is_lvalue_reference<R>
#else
                std::is_lvalue_reference<R>
#endif
              , boost::mpl::if_<
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS) || defined(BOOST_MSVC)
                    // MSVC 11.0 on AppVeyor reports error C2039:
                    // '_Is_const': is not a member of 'std::_Ptr_traits<_Ty>'
                    boost::is_const<V>
#else
                    std::is_const<V>
#endif
                  , boost::mpl::false_
                  , boost::mpl::true_
                >
              , boost::mpl::false_
            >
          , boost::mpl::true_
        >::type
    {
    };
}}} // namespace boost::parameter::aux

#include <boost/mpl/apply_wrap.hpp>
#include <boost/mpl/lambda.hpp>

namespace boost { namespace parameter { namespace aux {

    template <class Predicate, class R, class Tag>
    struct augment_predicate
    {
        typedef typename boost::mpl::lambda<
            Predicate
          , boost::parameter::aux::lambda_tag
        >::type actual_predicate;

        template <class T, class Args>
        struct apply
        {
            typedef typename boost::mpl::eval_if<
                typename boost::mpl::if_<
                    boost::parameter::aux
                    ::augment_predicate_check_consume_ref<R,Tag>
                  , boost::parameter::aux
                    ::augment_predicate_check_out_ref<T,R,Tag>
                  , boost::mpl::false_
                >::type
              , boost::mpl::apply_wrap2<actual_predicate,T,Args>
              , boost::mpl::false_
            >::type type;
        };
    };
}}} // namespace boost::parameter::aux

#endif // BOOST_PARAMETER_AUGMENT_PREDICATE_HPP

