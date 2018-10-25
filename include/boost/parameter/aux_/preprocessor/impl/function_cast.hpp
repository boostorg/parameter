// Copyright Daniel Wallin 2006.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_AUX_PREPROCESSOR_IMPL_FUNCTION_CAST_HPP
#define BOOST_PARAMETER_AUX_PREPROCESSOR_IMPL_FUNCTION_CAST_HPP

#include <boost/config.hpp>
#include <boost/config/workaround.hpp>

#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))

#define BOOST_PARAMETER_FUNCTION_CAST(value, predicate) value

#else   // !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))

namespace boost { namespace parameter { namespace aux {

    // Handles possible implicit casts.  Used by preprocessor.hpp
    // to normalize user input.
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
    template <typename VoidExpr, typename Args>
    struct cast;
}}} // namespace boost::parameter::aux

#include <boost/parameter/aux_/use_default_tag.hpp>

namespace boost { namespace parameter { namespace aux {

    template <typename Args>
    struct cast<void*, Args>
    {
        static ::boost::parameter::aux::use_default_tag
            execute(::boost::parameter::aux::use_default_tag)
        {
            return ::boost::parameter::aux::use_default_tag();
        }

        static ::boost::parameter::aux::use_default_tag
            remove_const(::boost::parameter::aux::use_default_tag)
        {
            return ::boost::parameter::aux::use_default_tag();
        }

        template <typename U>
        static U& execute(U& value)
        {
            return value;
        }

        template <typename U>
        static U& remove_const(U& x)
        {
            return x;
        }
    };
}}} // namespace boost::parameter::aux

#if BOOST_WORKAROUND(__SUNPRO_CC, BOOST_TESTED_AT(0x580))
#include <boost/parameter/aux_/void.hpp>
#endif

namespace boost { namespace parameter { namespace aux {

    template <typename Predicate, typename Args>
#if BOOST_WORKAROUND(__SUNPRO_CC, BOOST_TESTED_AT(0x580))
    struct cast< ::boost::parameter::aux::voidstar(Predicate),Args>
#else
    struct cast<void*(Predicate),Args>
#endif
      : ::boost::parameter::aux::cast<void*,Args>
    {
    };
}}} // namespace boost::parameter::aux

#include <boost/mpl/placeholders.hpp>

namespace boost { namespace parameter { namespace aux {

    // This is a hack used in cast<> to turn the user supplied type,
    // which may or may not be a placeholder expression, into one,
    // so that it will be properly evaluated by mpl::apply.
    template <typename T, typename Dummy = ::boost::mpl::_1>
    struct as_placeholder_expr
    {
        typedef T type;
    };
}}} // namespace boost::parameter::aux

#include <boost/mpl/apply.hpp>
#include <boost/type_traits/add_lvalue_reference.hpp>
#include <boost/type_traits/remove_const.hpp>

namespace boost { namespace parameter { namespace aux {

    template <typename T, typename Args>
    struct cast<void(T), Args>
    {
        typedef typename ::boost::mpl::apply2<
            ::boost::parameter::aux::as_placeholder_expr<T>
          , Args
          , Args
        >::type type0;

        typedef typename ::boost::add_lvalue_reference<
            typename ::boost::remove_const<type0>::type 
        >::type reference;

        static ::boost::parameter::aux::use_default_tag
            execute(::boost::parameter::aux::use_default_tag)
        {
            return ::boost::parameter::aux::use_default_tag();
        }

        static ::boost::parameter::aux::use_default_tag
            remove_const(::boost::parameter::aux::use_default_tag)
        {
            return ::boost::parameter::aux::use_default_tag();
        }

        static type0 execute(type0 value)
        {
            return value;
        }

        template <typename U>
        static reference remove_const(U const& x)
        {
            return const_cast<reference>(x);
        }
    };
}}} // namespace boost::parameter::aux

#define BOOST_PARAMETER_FUNCTION_CAST(value, predicate, args) \
    ::boost::parameter::aux::cast<void predicate, args>::remove_const( \
        ::boost::parameter::aux::cast<void predicate, args>::execute(value) \
    )

#endif  // Borland workarounds needed.
#endif  // include guard

