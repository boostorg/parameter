// Copyright Cromwell D. Enage 2013.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_AUX_PREPROCESSOR_CONVERT_BINARY_SEQ_HPP
#define BOOST_PARAMETER_AUX_PREPROCESSOR_CONVERT_BINARY_SEQ_HPP

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/facilities/empty.hpp>
#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor/repetition/for.hpp>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/preprocessor/seq/elem.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/seq/push_back.hpp>
#include <boost/preprocessor/seq/fold_left.hpp>
#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

#define BOOST_PARAMETER_AUX_PP_AUGMENT_BINARY_SEQ_INDEX_FOLD_OP(s, seq, idx) \
    BOOST_PP_SEQ_PUSH_BACK(seq, (idx, BOOST_PP_SEQ_SIZE(seq)))
/**/

#define BOOST_PARAMETER_AUX_PP_CONVERT_BINARY_SEQ_FOLD_OP(s, seq, elem) \
    ( \
        BOOST_PP_SEQ_PUSH_BACK( \
            BOOST_PP_SEQ_ELEM(0, seq) \
          , BOOST_PP_CAT( \
                BOOST_PP_SEQ_ELEM(1, seq) \
              , BOOST_PP_TUPLE_ELEM(2, 1, elem) \
            ) BOOST_PP_IIF( \
                BOOST_PP_TUPLE_ELEM(2, 0, elem) \
              , BOOST_PP_SEQ_ELEM(2, seq) \
              , BOOST_PP_SEQ_ELEM(3, seq) \
            ) BOOST_PP_CAT( \
                BOOST_PP_SEQ_ELEM(4, seq) \
              , BOOST_PP_TUPLE_ELEM(2, 1, elem) \
            ) \
        ) \
    )(BOOST_PP_SEQ_ELEM(1, seq))(BOOST_PP_SEQ_ELEM(2, seq)) \
    (BOOST_PP_SEQ_ELEM(3, seq))(BOOST_PP_SEQ_ELEM(4, seq))
/**/

// This macro converts the specified Boost.Preprocessor sequence of 1s and 0s
// into a formal function parameter list.
//
// Example:
//
// BOOST_PARAMETER_AUX_PP_CONVERT_BINARY_SEQ((1)(0)(1)(0), P, &, const&, p)
// expands to
// P0 & p0, P1 const& p1, P2 & p2, P3 const& p3
#define BOOST_PARAMETER_AUX_PP_CONVERT_BINARY_SEQ(seq, P, i1, i0, p) \
    BOOST_PP_SEQ_ENUM( \
        BOOST_PP_SEQ_ELEM( \
            0 \
          , BOOST_PP_SEQ_FOLD_LEFT( \
                BOOST_PARAMETER_AUX_PP_CONVERT_BINARY_SEQ_FOLD_OP \
              , (BOOST_PP_SEQ_NIL)(P)(i1)(i0)(p) \
              , BOOST_PP_SEQ_FOLD_LEFT( \
                    BOOST_PARAMETER_AUX_PP_AUGMENT_BINARY_SEQ_INDEX_FOLD_OP \
                  , BOOST_PP_EMPTY() \
                  , seq \
                ) \
            ) \
        ) \
    )
/**/

#endif // BOOST_PARAMETER_AUX_PREPROCESSOR_CONVERT_BINARY_SEQ_HPP

