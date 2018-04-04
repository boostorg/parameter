// Copyright Daniel Wallin 2006.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_CAST_060902_HPP
#define BOOST_PARAMETER_CAST_060902_HPP

namespace boost { namespace parameter { namespace aux {

    struct use_default_tag
    {
        inline use_default_tag operator()() const
        {
            return *this;
        }
    };
}}} // namespace boost::parameter::aux

#include <boost/parameter/config.hpp>

#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))

#define BOOST_PARAMETER_FUNCTION_CAST(value, predicate) value

#else

namespace boost { namespace parameter { namespace aux {

    // Handles possible implicit casts.  Used by preprocessor.hpp to
    // normalize user input.
    //
    // cast<void*>::execute() is identity
    // cast<void*(X)>::execute() is identity
    // cast<void(X)>::execute() casts to X
    //
    // preprocessor.hpp uses this like this:
    //
    //     #define X(value, predicate)
    //         cast<void predicate>::execute(value)
    //
    //     X(something, *)
    //     X(something, *(predicate))
    //     X(something, (int))

    template <class T, class Args>
    struct cast;

    template <class Args>
    struct cast<void*,Args>
    {
        inline static boost::parameter::aux::use_default_tag
        execute(boost::parameter::aux::use_default_tag)
        {
            return boost::parameter::aux::use_default_tag();
        }

        inline static boost::parameter::aux::use_default_tag
        remove_const(boost::parameter::aux::use_default_tag)
        {
            return boost::parameter::aux::use_default_tag();
        }

        template <class U>
        inline static U& execute(U& value)
        {
            return value;
        }

        template <class U>
        inline static U& remove_const(U& x)
        {
            return x;
        }

#if defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
        template <class U>
        inline static U&& execute(U&& value)
        {
            return static_cast<U&&>(value);
        }

        template <class U>
        inline static U&& remove_const(U&& x)
        {
            return static_cast<U&&>(x);
        }
#endif
    };

#if BOOST_WORKAROUND(__SUNPRO_CC, BOOST_TESTED_AT(0x580))
    typedef void* voidstar;

    template <class T, class Args>
    struct cast<boost::parameter::aux::voidstar(T),Args>
      : boost::parameter::aux::cast<void*,Args>
    {
    };
#else
    template <class T, class Args>
    struct cast<void*(T),Args> : boost::parameter::aux::cast<void*,Args>
    {
    };
#endif
}}} // namespace boost::parameter::aux

#include <boost/mpl/placeholders.hpp>

namespace boost { namespace parameter { namespace aux {

    // This is a hack used in cast<> to turn the user supplied type,
    // which may or may not be a placeholder expression, into one,
    // so that it will be properly evaluated by mpl::apply.
    template <class T, class Dummy = boost::mpl::_1>
    struct as_placeholder_expr
    {
        typedef T type;
    };
}}} // namespace boost::parameter::aux

#include <boost/mpl/apply.hpp>

#if defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
#include <boost/type_traits/add_rvalue_reference.hpp>
#else
#include <boost/type_traits/add_lvalue_reference.hpp>
#include <boost/type_traits/remove_const.hpp>
#endif

namespace boost { namespace parameter { namespace aux {

    template <class T, class Args>
    struct cast<void(T),Args>
    {
        typedef typename boost::mpl::apply2<
            boost::parameter::aux::as_placeholder_expr<T>
          , Args
          , Args
        >::type type0;

#if defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
        typedef typename boost::add_rvalue_reference<type0>::type reference;
#else
        typedef typename boost::add_lvalue_reference<
            typename boost::remove_const<type0>::type 
        >::type reference;
#endif

        inline static boost::parameter::aux::use_default_tag
        execute(boost::parameter::aux::use_default_tag)
        {
            return boost::parameter::aux::use_default_tag();
        }

        inline static boost::parameter::aux::use_default_tag
        remove_const(boost::parameter::aux::use_default_tag)
        {
            return boost::parameter::aux::use_default_tag();
        }

        inline static type0 execute(type0 value)
        {
            return static_cast<type0>(value);
        }

#if defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
        template <class U>
        inline static reference remove_const(U&& x)
        {
            return static_cast<reference>(x);
        }
#else
        template <class U>
        inline static reference remove_const(U const& x)
        {
            return const_cast<reference>(x);
        }
#endif
    };
}}} // namespace boost::parameter::aux

#define BOOST_PARAMETER_FUNCTION_CAST(value, predicate, args)                \
    boost::parameter::aux::cast<void predicate, args>::remove_const(         \
        boost::parameter::aux::cast<void predicate, args>::execute(value)    \
    )
/**/

#endif // Borland workarounds needed.

#endif // BOOST_PARAMETER_CAST_060902_HPP

