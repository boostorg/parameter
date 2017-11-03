// Copyright Cromwell D. Enage 2013.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_AUX_PREPROCESSOR_INC_BINARY_SEQ_HPP
#define BOOST_PARAMETER_AUX_PREPROCESSOR_INC_BINARY_SEQ_HPP

#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/preprocessor/seq/push_back.hpp>
#include <boost/preprocessor/seq/fold_left.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

// This macro maintains a tuple (seq, carry), where seq is the intermediate
// result and carry is a flag that will unset upon finding an element == 0.
#define BOOST_PARAMETER_AUX_PP_INC_BINARY_SEQ_OP(s, result_tuple, element) \
    ( \
        BOOST_PP_SEQ_PUSH_BACK( \
            BOOST_PP_TUPLE_ELEM(2, 0, result_tuple) \
          , BOOST_PP_IIF( \
                BOOST_PP_TUPLE_ELEM(2, 1, result_tuple) \
              , BOOST_PP_IIF(element, 0, 1) \
              , element \
            ) \
        ) \
      , BOOST_PP_IIF(BOOST_PP_TUPLE_ELEM(2, 1, result_tuple), element, 0) \
    )
/**/

// This macro takes in the tuple (seq, carry), with carry indicating whether
// or not seq originally contained all 1s.  If so, then seq now contains all
// 0s, and this macro pushes an extra 0 before expanding to the new sequence.
// Otherwise, this macro expands to seq as is.
#define BOOST_PARAMETER_AUX_PP_INC_BINARY_SEQ_IMPL(seq_and_carry) \
    BOOST_PP_IIF( \
        BOOST_PP_TUPLE_ELEM(2, 1, seq_and_carry) \
      , BOOST_PP_SEQ_PUSH_BACK(BOOST_PP_TUPLE_ELEM(2, 0, seq_and_carry), 0) \
      , BOOST_PP_TUPLE_ELEM(2, 0, seq_and_carry) \
    )
/**/

// This macro treats the specified sequence of 1s and 0s like a binary number
// in reverse and expands to a sequence representing the next value up.
// However, if the input sequence contains all 1s, then the output sequence
// will contain one more element but all 0s.
//
// Examples:
// seq = (1)(0)(1)(0) --> return (0)(1)(1)(0)
// seq = (1)(1)(1)(0) --> return (0)(0)(0)(1)
// seq = (1)(1)(1)(1) --> return (0)(0)(0)(0)(0)
#define BOOST_PARAMETER_AUX_PP_INC_BINARY_SEQ(seq) \
    BOOST_PARAMETER_AUX_PP_INC_BINARY_SEQ_IMPL( \
        BOOST_PP_SEQ_FOLD_LEFT( \
            BOOST_PARAMETER_AUX_PP_INC_BINARY_SEQ_OP \
          , (BOOST_PP_SEQ_NIL, 1) \
          , seq \
        ) \
    )
/**/

#endif // BOOST_PARAMETER_AUX_PREPROCESSOR_INC_BINARY_SEQ_HPP

