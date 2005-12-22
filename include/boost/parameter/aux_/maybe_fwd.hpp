// Copyright Daniel Wallin 2005. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_MAYBE_FWD_051212_HPP
# define BOOST_PARAMETER_MAYBE_FWD_051212_HPP

# include <boost/detail/is_xxx.hpp>

namespace boost { namespace parameter { namespace aux {

template <class T>
struct maybe;

BOOST_DETAIL_IS_XXX_DEF(maybe,maybe,1)

template <class T>
struct get_reference
{
    typedef typename T::reference type;
};
    
}}} // namespace boost::parameter::aux

#endif // BOOST_PARAMETER_MAYBE_FWD_051212_HPP

