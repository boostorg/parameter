// Copyright Daniel Wallin 2005. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_INVOKER_051210_HPP
# define BOOST_PARAMETER_INVOKER_051210_HPP

# include <boost/mpl/begin.hpp>
# include <boost/mpl/next.hpp>
# include <boost/mpl/deref.hpp>
# include <boost/parameter/keyword.hpp>
# include <boost/preprocessor/iteration/iterate.hpp>

namespace boost { namespace parameter { namespace python { namespace aux {

template <long Arity, class M, class R, class Args>
struct invoker;

template <class M, class R, class Args>
struct invoker<0, M, R, Args>
{
    static R execute()
    {
        return M()(boost::type<R>());
    }
};

# define BOOST_PP_ITERATION_PARAMS_1 (3, \
    (1, BOOST_PARAMETER_MAX_ARITY, <boost/parameter/python/aux_/invoker_iterate.hpp>))
# include BOOST_PP_ITERATE()

}}}} // namespace boost::parameter::python::aux

#endif // BOOST_PARAMETER_INVOKER_051210_HPP

