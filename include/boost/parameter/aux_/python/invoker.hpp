// Copyright Daniel Wallin 2005.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_INVOKER_051210_HPP
#define BOOST_PARAMETER_INVOKER_051210_HPP

#include <boost/mpl/begin.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/size.hpp>
#include <boost/parameter/keyword.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>

namespace boost { namespace parameter { namespace python { namespace aux {

    template <long Arity, typename M, typename R, typename Args>
    struct invoker;

    template <typename M, typename R>
    struct make_invoker
    {
        template <typename Args>
        struct apply
        {
            typedef ::boost::parameter::python::aux::invoker<
                ::boost::mpl::size<Args>::value,M,R,Args
            > type;
        };
    };

    template <long Arity, typename M, typename R, typename T, typename Args>
    struct member_invoker;

    template <typename M, typename R, typename T>
    struct make_member_invoker
    {
        template <typename Args>
        struct apply
        {
            typedef ::boost::parameter::python::aux::member_invoker<
                ::boost::mpl::size<Args>::value,M,R,T,Args
            > type;
        };
    };

    template <long Arity, typename T, typename R, typename Args>
    struct call_invoker;

    template <typename T, typename R>
    struct make_call_invoker
    {
        template <typename Args>
        struct apply
        {
            typedef ::boost::parameter::python::aux::call_invoker<
                ::boost::mpl::size<Args>::value,T,R,Args
            > type;
        };
    };

    template <long Arity, typename T, typename Args>
    struct init_invoker;

    template <typename T>
    struct make_init_invoker
    {
        template <typename Args>
        struct apply
        {
            typedef ::boost::parameter::python::aux::init_invoker<
                ::boost::mpl::size<Args>::value,T,Args
            > type;
        };
    };

    template <typename M, typename R, typename Args>
    struct invoker<0,M,R,Args>
    {
        static R execute()
        {
            return M()(::boost::type<R>());
        }
    };

    template <typename M, typename R, typename T, typename Args>
    struct member_invoker<0,M,R,T,Args>
    {
        static R execute(T& self)
        {
            return M()(::boost::type<R>(), self);
        }
    };

    template <typename T, typename R, typename Args>
    struct call_invoker<0,T,R,Args>
    {
        static R execute(T& self)
        {
            return self();
        }
    };

    template <typename T, typename Args>
    struct init_invoker<0,T,Args>
    {
        static T* execute(T& self)
        {
            return new T;
        }
    };

#define BOOST_PP_ITERATION_PARAMS_1 \
    (4, ( \
        1, BOOST_PARAMETER_MAX_ARITY, \
        <boost/parameter/aux_/python/invoker_iterate.hpp>, 1 \
    ))
#include BOOST_PP_ITERATE()

#define BOOST_PP_ITERATION_PARAMS_1 \
    (4, ( \
        1, BOOST_PARAMETER_MAX_ARITY, \
        <boost/parameter/aux_/python/invoker_iterate.hpp>, 2 \
    ))
#include BOOST_PP_ITERATE()

#define BOOST_PP_ITERATION_PARAMS_1 \
    (4, ( \
        1, BOOST_PARAMETER_MAX_ARITY, \
        <boost/parameter/aux_/python/invoker_iterate.hpp>, 3 \
    ))
#include BOOST_PP_ITERATE()

#define BOOST_PP_ITERATION_PARAMS_1 \
    (4, ( \
        1, BOOST_PARAMETER_MAX_ARITY, \
        <boost/parameter/aux_/python/invoker_iterate.hpp>, 4 \
    ))
#include BOOST_PP_ITERATE()

}}}} // namespace boost::parameter::python::aux

#endif // BOOST_PARAMETER_INVOKER_051210_HPP

