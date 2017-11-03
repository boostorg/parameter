// Copyright Daniel Wallin, David Abrahams 2005.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_TAGGED_ARGUMENT_FWD_HPP
#define BOOST_PARAMETER_TAGGED_ARGUMENT_FWD_HPP

#include <boost/parameter/config.hpp>

namespace boost { namespace parameter { namespace aux {

template <class Keyword, class Arg>
class tagged_argument;

#if defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
template <class Keyword, class Arg>
class tagged_argument_rref;
#endif
}}} // namespace boost::parameter::aux

#endif // BOOST_PARAMETER_TAGGED_ARGUMENT_FWD_HPP

