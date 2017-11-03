// Copyright Cromwell D. Enage 2013.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_AUX_PREPROCESSOR_NOT_EQUAL_BINARY_SEQ_HPP
#define BOOST_PARAMETER_AUX_PREPROCESSOR_NOT_EQUAL_BINARY_SEQ_HPP

#include <boost/preprocessor/comparison/not_equal.hpp>
#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/seq/fold_left.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

// This macro maintains a tuple (r, seq), where r is the intermediate result
// and seq is the other sequence whose head will be compared to e.
#define BOOST_PARAMETER_AUX_PP_NOT_EQUAL_BINARY_SEQ_OP(s, result_tuple, e) \
    ( \
        BOOST_PP_IIF( \
            BOOST_PP_TUPLE_ELEM(2, 0, result_tuple) \
          , 1 \
          , BOOST_PP_NOT_EQUAL( \
                BOOST_PP_SEQ_HEAD(BOOST_PP_TUPLE_ELEM(2, 1, result_tuple)) \
              , e \
            ) \
        ) \
      , BOOST_PP_SEQ_TAIL(BOOST_PP_TUPLE_ELEM(2, 1, result_tuple)) \
    )
/**/

#define BOOST_PARAMETER_AUX_PP_NOT_EQUAL_BINARY_SEQ_IMPL_1(lhs, rhs) 1
/**/

// This macro expands to 0 if every element in lhseq is equal to the
// corresponding element in rhseq.  This macro expands to 1 otherwise.
#define BOOST_PARAMETER_AUX_PP_NOT_EQUAL_BINARY_SEQ_IMPL_0(lhseq, rhseq) \
    BOOST_PP_TUPLE_ELEM( \
        2 \
      , 0 \
      , BOOST_PP_SEQ_FOLD_LEFT( \
            BOOST_PARAMETER_AUX_PP_NOT_EQUAL_BINARY_SEQ_OP \
          , (0, lhseq) \
          , rhseq \
        ) \
    )
/**/

// This macro expands to 1 if lhseq and rhseq have different sizes.
// Only if lhseq and rhseq have the same size will their elements be compared.
#define BOOST_PARAMETER_AUX_PP_NOT_EQUAL_BINARY_SEQ(lhseq, rhseq) \
    BOOST_PP_IIF( \
        BOOST_PP_NOT_EQUAL( \
            BOOST_PP_SEQ_SIZE(lhseq) \
          , BOOST_PP_SEQ_SIZE(rhseq) \
        ) \
      , BOOST_PARAMETER_AUX_PP_NOT_EQUAL_BINARY_SEQ_IMPL_1 \
      , BOOST_PARAMETER_AUX_PP_NOT_EQUAL_BINARY_SEQ_IMPL_0 \
    )(lhseq, rhseq)
/**/

#endif // BOOST_PARAMETER_AUX_PREPROCESSOR_NOT_EQUAL_BINARY_SEQ_HPP

