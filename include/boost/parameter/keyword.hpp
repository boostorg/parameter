// Copyright Daniel Wallin, David Abrahams 2005.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef KEYWORD_050328_HPP
#define KEYWORD_050328_HPP

#include <boost/parameter/aux_/tag.hpp>
#include <boost/parameter/aux_/default.hpp>
#include <boost/parameter/config.hpp>

#if defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
#include <boost/move/utility_core.hpp>
#endif

namespace boost { namespace parameter {

// Instances of unique specializations of keyword<...> serve to
// associate arguments with parameter names.  For example:
//
//    struct rate_;             // parameter names
//    struct skew_;
//
//    namespace
//    {
//        keyword<rate_> rate;  // keywords
//        keyword<skew_> skew;
//    }
//
//    ...
//
//    f(rate = 1, skew = 2.4);
template <class Tag>
struct keyword
{
    template <class T>
    inline typename aux::tag<Tag, T&>::type BOOST_CONSTEXPR
    operator=(T& x) const
    {
        typedef typename aux::tag<Tag, T&>::type result;
        return result(x);
    }

    template <class Default>
    inline aux::default_<Tag, Default> operator|(Default& d) const
    {
        return aux::default_<Tag, Default>(d);
    }

    template <class Default>
    inline aux::lazy_default<Tag, Default> operator||(Default& d) const
    {
        return aux::lazy_default<Tag, Default>(d);
    }

    template <class T>
    inline typename aux::tag<Tag, T const&>::type BOOST_CONSTEXPR
    operator=(T const& x) const
    {
        typedef typename aux::tag<Tag, T const&>::type result;
        return result(x);
    }

    template <class Default>
    inline aux::default_<Tag, Default const> operator|(Default const& d) const
    {
        return aux::default_<Tag, Default const>(d);
    }

    template <class Default>
    inline aux::lazy_default<Tag, Default> operator||(Default const& d) const
    {
        return aux::lazy_default<Tag, Default>(d);
    }

#if defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
    template <class T>
    inline typename aux::tag<Tag, T>::type BOOST_CONSTEXPR
    operator=(T&& x) const
    {
        typedef typename aux::tag<Tag, T>::type result;
        return result(boost::forward<T>(x));
    }
#endif

 public: // Insurance against ODR violations
    // Users will need to define their keywords in header files.  To
    // prevent ODR violations, it's important that the keyword used in
    // every instantiation of a function template is the same object.
    // We provide a reference to a common instance of each keyword
    // object and prevent construction by users.
    static keyword<Tag> const instance;

    // This interface is deprecated.
    static keyword<Tag>& get()
    {
        return const_cast<keyword<Tag>&>(instance);
    }
};

template <class Tag>
keyword<Tag> const keyword<Tag>::instance = {};

}} // namespace boost::parameter

// Reduces boilerplate required to declare and initialize keywords without
// violating ODR.  Declares a keyword tag type with the given name in
// namespace tag_namespace, and declares and initializes a reference in an
// anonymous namespace to a singleton instance of that type.
#define BOOST_PARAMETER_KEYWORD(tag_namespace,name)                     \
    namespace tag_namespace                                             \
    {                                                                   \
        struct name                                                     \
        {                                                               \
            static char const* keyword_name()                           \
            {                                                           \
                return #name;                                           \
            }                                                           \
        };                                                              \
    }                                                                   \
    namespace                                                           \
    {                                                                   \
        ::boost::parameter::keyword<tag_namespace::name> const& name    \
          = ::boost::parameter::keyword<tag_namespace::name>::instance; \
    }

#endif // KEYWORD_050328_HPP

