// Copyright Daniel Wallin 2005. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/dec.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>

#define N BOOST_PP_ITERATION()

#define BOOST_PARAMETER_PY_ARG_TYPES(z, n, _) \
    typedef typename mpl::next< \
        BOOST_PP_CAT(iter,BOOST_PP_DEC(n)) \
    >::type BOOST_PP_CAT(iter,n); \
    \
    typedef typename mpl::deref<BOOST_PP_CAT(iter,n)>::type BOOST_PP_CAT(arg,n);

template <class M, class R, class Args>
struct invoker<N, M, R, Args>
{
    typedef typename mpl::begin<Args>::type iter0;
    typedef typename mpl::deref<iter0>::type arg0;

    BOOST_PP_REPEAT_FROM_TO(1, N, BOOST_PARAMETER_PY_ARG_TYPES, ~)

    static R execute(
        BOOST_PP_ENUM_BINARY_PARAMS(N, typename arg, ::type a)
    )
    {
        return M()(
            boost::type<R>()
          , BOOST_PP_ENUM_BINARY_PARAMS(N, keyword<typename arg, ::keyword>::get() = a)
        );
    }
};

#undef BOOST_PARAMETER_PY_ARG_TYPES
#undef N

