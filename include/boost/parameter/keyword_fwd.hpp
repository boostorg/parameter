// Copyright Cromwell D. Enage 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_KEYWORD_FWD_HPP
#define BOOST_PARAMETER_KEYWORD_FWD_HPP

#include <boost/config.hpp>

namespace boost { namespace parameter {

#if defined BOOST_MSVC
    struct in;
    struct out;
    typedef boost::parameter::out in_out;
    struct forward;
    struct consume;
    typedef boost::parameter::consume move_from;

    typedef boost::parameter::in in_reference;
    typedef boost::parameter::out out_reference;
    typedef boost::parameter::in_out in_out_reference;
    typedef boost::parameter::forward forward_reference;
    typedef boost::parameter::consume consume_reference;
    typedef boost::parameter::move_from move_from_reference;
#else
    struct in_reference;
    struct out_reference;
    typedef boost::parameter::out_reference in_out_reference;
    struct forward_reference;
    struct consume_reference;
    typedef boost::parameter::consume_reference move_from_reference;
#endif // BOOST_MSVC

    template <class Tag>
    struct keyword;
}} // namespace boost::parameter

#endif // BOOST_PARAMETER_KEYWORD_FWD_HPP

