// Copyright Daniel Wallin 2006.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//
// 2009.10.21 TDS remove depenency on boost::python::detail::referent_storage
//
#ifndef BOOST_PARAMETER_MAYBE_091021_HPP
#define BOOST_PARAMETER_MAYBE_091021_HPP

#include <boost/mpl/if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/optional.hpp>
#include <boost/aligned_storage.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/parameter/aux_/is_maybe.hpp>

namespace boost { namespace parameter { namespace aux {

template <class T>
struct referent_size;

template <class T>
struct referent_size<T&>
{
    BOOST_STATIC_CONSTANT(std::size_t, value = sizeof(T));
};

// A metafunction returning a POD type which can store U, where T == U&.
// If T is not a reference type, returns a POD which can store T.
template <class T>
struct referent_storage
  : boost::aligned_storage<referent_size<T>::value>
{
};

template <class T>
struct maybe : maybe_base
{
    typedef typename add_reference<
#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
        T const
#else
        typename add_const<T>::type
#endif
    >::type reference;

    typedef typename remove_cv<
        BOOST_DEDUCED_TYPENAME remove_reference<reference>::type
    >::type non_cv_value;

    inline explicit maybe(T value_) : value(value_), constructed(false)
    {
    }

    inline maybe() : value(), constructed(false)
    {
    }

    ~maybe()
    {
        if (constructed)
        {
            this->destroy();
        }
    }

    inline reference construct(reference value_) const
    {
        return value_;
    }

    template <class U>
    reference construct2(U const& value_) const
    {
        new (this->m_storage.address()) non_cv_value(value_);
        this->constructed = true;
        return *reinterpret_cast<non_cv_value*>(this->m_storage.address());
    }

    template <class U>
    inline reference construct(U const& value_) const
    {
        return this->construct2(value_);
    }

    void destroy()
    {
        reinterpret_cast<non_cv_value*>(
            this->m_storage.address()
        )->~non_cv_value();
    }

    typedef reference(maybe<T>::*safe_bool)() const;

    inline operator safe_bool() const
    {
        return this->value ? &maybe<T>::get : 0 ;
    }

    inline reference get() const
    {
        return this->value.get();
    }

 private:
    boost::optional<T> value;
    mutable bool constructed;
    mutable typename referent_storage<reference>::type m_storage;
};
}}} // namespace boost::parameter::aux

#endif // BOOST_PARAMETER_MAYBE_060211_HPP

