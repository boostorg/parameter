// Copyright Daniel Wallin 2005. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_MAYBE_051212_HPP
# define BOOST_PARAMETER_MAYBE_051212_HPP

# include <boost/type_traits/is_reference.hpp>
# include <boost/type_traits/add_reference.hpp>
# include <boost/type_traits/add_const.hpp>
# include <boost/optional.hpp>
# include <boost/mpl/if.hpp>

namespace boost { namespace parameter { namespace aux {

struct empty_maybe_tag {};

template <class T>
struct maybe
{
    typedef typename mpl::if_<
        is_reference<T>
      , T
      , typename add_reference<typename add_const<T>::type>::type
    >::type reference;

    maybe()
    {}

    explicit maybe(reference x)
      : value(x)
    {}

    typedef reference(maybe<T>::*safe_bool)() const;
    
    operator safe_bool() const
    {
        return value ? &maybe<T>::get : 0 ;
    }

    reference get() const
    {
        return value.get();
    }

    boost::optional<T> value;
};

}}} // namespace boost::parameter::aux

#endif // BOOST_PARAMETER_MAYBE_051212_HPP

