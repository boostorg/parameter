// Copyright Cromwell D. Enage 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_AUGMENT_PREDICATE_HPP
#define BOOST_PARAMETER_AUGMENT_PREDICATE_HPP

namespace boost { namespace parameter { namespace aux {

    struct lambda_tag;
}}} // namespace boost::parameter::aux

#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/remove_reference.hpp>

namespace boost { namespace parameter { namespace aux {

    template <class T>
    struct augment_predicate_is_const
      : boost::is_const<typename boost::remove_reference<T>::type>
    {
    };
}}} // namespace boost::parameter::aux

#include <boost/parameter/keyword_fwd.hpp>
#include <boost/type_traits/is_lvalue_reference.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/eval_if.hpp>

namespace boost { namespace parameter { namespace aux {

    template <class T, class Tag>
    struct augment_predicate_check_out_ref
      : boost::mpl::eval_if<
            boost::is_same<
                typename Tag::qualifier
              , boost::parameter::out_reference
            >
          , boost::mpl::eval_if<
                boost::is_lvalue_reference<T>
              , boost::mpl::eval_if<
                    boost::parameter::aux::augment_predicate_is_const<T>
                  , boost::mpl::false_
                  , boost::mpl::true_
                >
              , boost::mpl::false_
            >
          , boost::mpl::true_
        >::type
    {
    };

    template <class T, class Tag>
    struct augment_predicate_check_consume_ref
      : boost::mpl::eval_if<
            boost::is_same<
                typename Tag::qualifier
              , boost::parameter::consume_reference
            >
          , boost::mpl::eval_if<
                boost::is_lvalue_reference<T>
              , boost::mpl::false_
              , boost::mpl::true_
            >
          , boost::mpl::true_
        >::type
    {
    };
}}} // namespace boost::parameter::aux

#include <boost/parameter/config.hpp>
#include <boost/mpl/apply_wrap.hpp>
#include <boost/mpl/lambda.hpp>

namespace boost { namespace parameter { namespace aux {

    template <class Predicate, class Tag>
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
                typename boost::mpl::eval_if<
                    boost::parameter::aux
                    ::augment_predicate_check_out_ref<T,Tag>
                  , boost::parameter::aux
                    ::augment_predicate_check_consume_ref<T,Tag>
                  , boost::mpl::true_
                >::type
              , boost::mpl::apply_wrap2<actual_predicate,T,Args>
              , boost::mpl::false_
            >::type type;
        };
    };
}}} // namespace boost::parameter::aux

#endif // BOOST_PARAMETER_AUGMENT_PREDICATE_HPP

