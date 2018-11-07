// Copyright Daniel Wallin 2006.
// Copyright Cromwell D. Enage 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_AUX_PREPROCESSOR_IMPL_FUNCTION_CAST_HPP
#define BOOST_PARAMETER_AUX_PREPROCESSOR_IMPL_FUNCTION_CAST_HPP

#include <boost/parameter/config.hpp>

#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))

#define BOOST_PARAMETER_FUNCTION_CAST_T(value_t, predicate, args) value_t

#include <boost/mpl/bool.hpp>

#define BOOST_PARAMETER_FUNCTION_CAST_B(value_t, predicate, args) \
    ::boost::mpl::true_

#include <utility>

namespace boost { namespace parameter { namespace aux {

    template <typename T, typename B>
    inline T&& forward(T&& t)
    {
        return ::std::forward<T>(t);
    }
}}} // namespace boost::parameter::aux

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

    template <typename T, typename B>
    inline ::boost::parameter::aux::use_default_tag
        forward(::boost::parameter::aux::use_default_tag)
    {
        return ::boost::parameter::aux::use_default_tag();
    }
}}} // namespace boost::parameter::aux

#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>

namespace boost { namespace parameter { namespace aux {

    template <typename Args>
    struct cast<void*,Args>
    {
        template <typename T, typename B>
        struct apply
        {
            typedef typename ::boost::mpl
            ::if_<B,T,::boost::mpl::true_>::type type;
        };
    };
}}} // namespace boost::parameter::aux

#include <boost/parameter/aux_/void.hpp>

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
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/remove_reference.hpp>

namespace boost { namespace parameter { namespace aux {

    template <typename Target, typename Source, typename Args>
    struct is_target_same_as_source
      : ::boost::mpl::if_<
            ::boost::is_same<
                typename ::boost::remove_const<
                    typename ::boost::remove_reference<
                        typename ::boost::mpl::apply2<
                            ::boost::parameter::aux
                            ::as_placeholder_expr<Target>
                          , Source
                          , Args
                        >::type
                    >::type
                >::type
              , typename ::boost::remove_const<Source>::type
            >
          , ::boost::mpl::true_
          , ::boost::mpl::false_
        >::type
    {
    };
}}} // namespace boost::parameter::aux

#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/is_const.hpp>

namespace boost { namespace parameter { namespace aux {

    // Covers the case where is_convertible<Source,Target> but not
    // is_same<Source,Target>.  Use cases are covered
    // by test/normalize_argument_types.cpp
    template <typename Source, typename Target>
    class cast_convert
    {
        typedef ::boost::parameter::aux::cast_convert<Source,Target> _self;

     public:
        typedef typename boost::mpl::eval_if<
            ::boost::is_const<Source>
          , ::boost::add_const<Target>
          , ::boost::remove_const<Target>
        >::type type;

     private:
        inline static typename _self::type
            _copy(typename ::boost::remove_const<Target>::type value)
        {
            return value;
        }

     public:
        inline static typename _self::type evaluate(Source&& source)
        {
            return _self::_copy(source);
        }
    };

    template <typename Target, typename Source, typename Args>
    struct cast_impl
      : ::boost::remove_reference<
            typename ::boost::mpl::apply2<
                ::boost::parameter::aux::as_placeholder_expr<Target>
              , Source
              , Args
            >::type
        >
    {
    };
}}} // namespace boost::parameter::aux

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>

namespace boost { namespace parameter { namespace aux {

    template <typename Target, typename Args>
    struct cast<void(Target),Args>
    {
        template <typename T, typename B>
        struct apply
        {
            typedef typename ::boost::mpl::eval_if<
                B
              , ::boost::mpl::eval_if<
                    ::boost::parameter::aux
                    ::is_target_same_as_source<Target,T,Args>
                  , ::boost::mpl::identity<T>
                  , ::boost::parameter::aux::cast_impl<
                        Target
                      , T
                      , Args
                    >
                >
              , ::boost::parameter::aux
                ::is_target_same_as_source<Target,T,Args>
            >::type type;
        };
    };
}}} // namespace boost::parameter::aux

#include <boost/mpl/apply_wrap.hpp>
#include <boost/parameter/value_type.hpp>

// Expands to the target type of the argument as indicated by the predicate.
#define BOOST_PARAMETER_FUNCTION_CAST_T(tag, predicate, args)                \
    typename ::boost::mpl::apply_wrap2<                                      \
        ::boost::parameter::aux::cast<void predicate, args>                  \
      , typename ::boost::parameter::value_type<                             \
            args                                                             \
          , tag                                                              \
          , ::boost::parameter::aux::use_default_tag                         \
        >::type                                                              \
      , ::boost::mpl::true_                                                  \
    >::type
/**/

// Expands to boost::mpl::true_ if and only if the argument's source and
// target types are the same.
#define BOOST_PARAMETER_FUNCTION_CAST_B(tag, predicate, args)                \
    typename ::boost::mpl::apply_wrap2<                                      \
        ::boost::parameter::aux::cast<void predicate, args>                  \
      , typename ::boost::parameter::value_type<                             \
            args                                                             \
          , tag                                                              \
          , ::boost::parameter::aux::use_default_tag                         \
        >::type                                                              \
      , ::boost::mpl::false_                                                 \
    >::type
/**/

#include <boost/core/enable_if.hpp>
#include <utility>

namespace boost { namespace parameter { namespace aux {

    // If the source and target types are not the same,
    // then perform an implicit conversion.
    template <typename Target, typename B, typename Source>
    inline typename ::boost::lazy_disable_if<
        B
      , ::boost::parameter::aux::cast_convert<Source,Target>
    >::type
        forward(Source&& source)
    {
        return ::boost::parameter::aux::cast_convert<Source,Target>
        ::evaluate(::std::forward<Source>(source));
    }

    // If the source and target types are the same,
    // then simply forward the argument.
    // However, treat rvalue references to scalars as const lvalue references.
    template <typename T, typename B>
    inline typename ::boost::enable_if<B,T const&>::type forward(T const& t)
    {
        return t;
    }

    template <typename T, typename B>
    inline typename ::boost::enable_if<
        typename ::boost::mpl::eval_if<
            B
          , ::boost::mpl::if_<
                ::boost::is_const<T>
              , ::boost::mpl::false_
              , ::boost::mpl::true_
            >
          , ::boost::mpl::false_
        >::type
      , T&
    >::type
        forward(T& t)
    {
        return t;
    }
}}} // namespace boost::parameter::aux

#include <boost/type_traits/is_scalar.hpp>

namespace boost { namespace parameter { namespace aux {

    template <typename T, typename B>
    inline typename ::boost::enable_if<
        typename ::boost::mpl::eval_if<
            B
          , ::boost::mpl::if_<
                ::boost::is_scalar<T>
              , ::boost::mpl::false_
              , ::boost::mpl::true_
            >
          , ::boost::mpl::false_
        >::type
      , T const&&
    >::type
        forward(T const&& t)
    {
        return static_cast<T const&&>(t);
    }

    template <typename T, typename B>
    inline typename ::boost::enable_if<
        typename ::boost::mpl::eval_if<
            B
          , ::boost::mpl::if_<
                ::boost::is_scalar<T>
              , ::boost::mpl::false_
              , ::boost::mpl::true_
            >
          , ::boost::mpl::false_
        >::type
      , T&&
    >::type
        forward(T&& t)
    {
        return ::std::forward<T>(t);
    }
}}} // namespace boost::parameter::aux

#endif  // Borland workarounds needed.
#endif  // include guard

