// Copyright Cromwell D. Enage 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_AUGMENT_PREDICATE_HPP
#define BOOST_PARAMETER_AUGMENT_PREDICATE_HPP

#include <boost/parameter/keyword_fwd.hpp>
#include <boost/parameter/config.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/type_traits/is_lvalue_reference.hpp>
#include <boost/type_traits/is_scalar.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace parameter { namespace aux {

    template <typename V, typename R, typename Tag>
    struct augment_predicate_check_consume_ref
      : ::boost::mpl::eval_if<
            ::boost::is_scalar<V>
          , ::boost::mpl::true_
          , ::boost::mpl::eval_if<
                ::boost::is_same<
                    typename Tag::qualifier
                  , ::boost::parameter::consume_reference
                >
              , ::boost::mpl::if_<
                    ::boost::is_lvalue_reference<R>
                  , ::boost::mpl::false_
                  , ::boost::mpl::true_
                >
              , boost::mpl::true_
            >
        >::type
    {
    };
}}} // namespace boost::parameter::aux

#include <boost/type_traits/is_const.hpp>

namespace boost { namespace parameter { namespace aux {

    template <typename V, typename R, typename Tag>
    struct augment_predicate_check_out_ref
      : ::boost::mpl::eval_if<
            ::boost::is_same<
                typename Tag::qualifier
              , ::boost::parameter::out_reference
            >
          , ::boost::mpl::eval_if<
                ::boost::is_lvalue_reference<R>
              , ::boost::mpl::if_<
                    ::boost::is_const<V>
                  , ::boost::mpl::false_
                  , ::boost::mpl::true_
                >
              , ::boost::mpl::false_
            >
          , ::boost::mpl::true_
        >::type
    {
    };
}}} // namespace boost::parameter::aux

#include <boost/parameter/aux_/lambda_tag.hpp>
#include <boost/mpl/apply_wrap.hpp>
#include <boost/mpl/lambda.hpp>

namespace boost { namespace parameter { namespace aux {

    template <typename Predicate, typename R, typename Tag>
    struct augment_predicate
    {
        typedef typename ::boost::mpl::lambda<
            Predicate
          , ::boost::parameter::aux::lambda_tag
        >::type actual_predicate;

        template <typename T, typename Args>
        struct apply
        {
            typedef typename ::boost::mpl::eval_if<
                typename ::boost::mpl::if_<
                    ::boost::parameter::aux
                    ::augment_predicate_check_consume_ref<T,R,Tag>
                  , ::boost::parameter::aux
                    ::augment_predicate_check_out_ref<T,R,Tag>
                  , ::boost::mpl::false_
                >::type
              , ::boost::mpl::apply_wrap2<actual_predicate,T,Args>
              , ::boost::mpl::false_
            >::type type;
        };
    };
}}} // namespace boost::parameter::aux

#endif  // include guard

