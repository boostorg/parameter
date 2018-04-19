// Copyright Daniel Wallin, David Abrahams 2005.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef KEYWORD_050328_HPP
#define KEYWORD_050328_HPP

#include <boost/parameter/keyword_fwd.hpp>
#include <boost/parameter/aux_/tag.hpp>
#include <boost/parameter/aux_/default.hpp>
#include <boost/parameter/config.hpp>

#if !defined(BOOST_NO_SFINAE)
#include <boost/mpl/if.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/core/enable_if.hpp>

#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_scalar.hpp>
#else
#include <type_traits>
#endif
#endif // BOOST_NO_SFINAE

#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS) || defined(BOOST_MSVC)
#include <boost/type_traits/is_const.hpp>
#endif

#if defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)
#include <boost/move/utility_core.hpp>
#endif

namespace boost { namespace parameter {

    // Instances of unique specializations of keyword<...> serve to
    // associate arguments with parameter names.  For example:
    //
    //     struct rate_;             // parameter names
    //     struct skew_;
    //
    //     namespace
    //     {
    //         keyword<rate_> rate;  // keywords
    //         keyword<skew_> skew;
    //     }
    //
    //     ...
    //
    //     f(rate = 1, skew = 2.4);
    template <class Tag>
    struct keyword
    {
        template <class T>
#if defined(BOOST_NO_SFINAE)
        inline typename aux::tag<Tag,T&>::type
#else
        inline typename boost::lazy_enable_if<
            typename boost::mpl::eval_if<
                typename boost::mpl::if_<
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
                    boost::is_same<
#else
                    std::is_same<
#endif
                        typename Tag::qualifier
                      , boost::parameter::out_reference
                    >
                  , boost::mpl::true_
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
                  , boost::is_same<
#else
                  , std::is_same<
#endif
                        typename Tag::qualifier
                      , boost::parameter::forward_reference
                    >
                >::type
              , boost::mpl::if_<
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS) || defined(BOOST_MSVC)
                    // MSVC 11.0 on AppVeyor reports error C2039:
                    // '_Is_const': is not a member of 'std::_Ptr_traits<_Ty>'
                    boost::is_const<T>
#else
                    std::is_const<T>
#endif
                  , boost::mpl::false_
                  , boost::mpl::true_
                >
              , boost::mpl::false_
            >::type
          , aux::tag<Tag,T&>
        >::type BOOST_CONSTEXPR
#endif // BOOST_NO_SFINAE
        operator=(T& x) const
        {
            typedef typename aux::tag<Tag,T&>::type result;
            return result(x);
        }

        template <class Default>
#if defined(BOOST_NO_SFINAE)
        inline aux::default_<Tag,Default>
#else
        inline typename boost::enable_if<
            typename boost::mpl::eval_if<
                typename boost::mpl::if_<
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
                    boost::is_same<
#else
                    std::is_same<
#endif
                        typename Tag::qualifier
                      , boost::parameter::out_reference
                    >
                  , boost::mpl::true_
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
                  , boost::is_same<
#else
                  , std::is_same<
#endif
                        typename Tag::qualifier
                      , boost::parameter::forward_reference
                    >
                >::type
              , boost::mpl::if_<
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS) || defined(BOOST_MSVC)
                    // MSVC 11.0 on AppVeyor reports error C2039:
                    // '_Is_const': is not a member of 'std::_Ptr_traits<_Ty>'
                    boost::is_const<Default>
#else
                    std::is_const<Default>
#endif
                  , boost::mpl::false_
                  , boost::mpl::true_
                >
              , boost::mpl::false_
            >::type
          , aux::default_<Tag,Default>
        >::type
#endif // BOOST_NO_SFINAE
        operator|(Default& d) const
        {
            return aux::default_<Tag,Default>(d);
        }

        template <class Default>
        inline aux::lazy_default<Tag,Default> operator||(Default& d) const
        {
            return aux::lazy_default<Tag,Default>(d);
        }

        template <class T>
#if defined(BOOST_NO_SFINAE)
        inline typename aux::tag<Tag,T const&>::type
#else
        inline typename boost::lazy_enable_if<
            typename boost::mpl::eval_if<
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
                boost::is_same<
#else
                std::is_same<
#endif
                    typename Tag::qualifier
                  , boost::parameter::in_reference
                >
              , boost::mpl::true_
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
              , boost::is_same<
#else
              , boost::mpl::if_<
                    std::is_same<
#endif
                        typename Tag::qualifier
                      , boost::parameter::forward_reference
#if !defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
                    >
                  , boost::mpl::true_
                  , boost::mpl::false_
#endif
                >
            >::type
          , aux::tag<Tag,T const&>
        >::type BOOST_CONSTEXPR
#endif // BOOST_NO_SFINAE
        operator=(T const& x) const
        {
            typedef typename aux::tag<Tag,T const&>::type result;
            return result(x);
        }

        template <class Default>
#if defined(BOOST_NO_SFINAE)
        inline aux::default_<Tag,Default const>
#else
        inline typename boost::enable_if<
            typename boost::mpl::eval_if<
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
                boost::is_same<
#else
                std::is_same<
#endif
                    typename Tag::qualifier
                  , boost::parameter::in_reference
                >
              , boost::mpl::true_
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
              , boost::is_same<
#else
              , boost::mpl::if_<
                    std::is_same<
#endif
                        typename Tag::qualifier
                      , boost::parameter::forward_reference
#if !defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
                    >
                  , boost::mpl::true_
                  , boost::mpl::false_
#endif
                >
            >::type
          , aux::default_<Tag,Default const>
        >::type
#endif // BOOST_NO_SFINAE
        operator|(Default const& d) const
        {
            return aux::default_<Tag,Default const>(d);
        }

        template <class Default>
        inline aux::lazy_default<Tag,Default>
        operator||(Default const& d) const
        {
            return aux::lazy_default<Tag,Default>(d);
        }

#if defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)
        template <class T>
        inline typename boost::lazy_enable_if<
            typename boost::mpl::eval_if<
                typename boost::mpl::if_<
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
                    boost::is_same<
#else
                    std::is_same<
#endif
                        typename Tag::qualifier
                      , boost::parameter::consume_reference
                    >
                  , boost::mpl::true_
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
                  , boost::is_same<
#else
                  , std::is_same<
#endif
                        typename Tag::qualifier
                      , boost::parameter::forward_reference
                    >
                >::type
              , boost::mpl::true_
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
              , boost::mpl::if_<
                    boost::is_same<
#else
              , boost::mpl::eval_if<
                    std::is_same<
#endif
                        typename Tag::qualifier
                      , boost::parameter::in_reference
                    >
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
                  , boost::is_scalar<T>
#else
                  , boost::mpl::if_<
                        std::is_scalar<T>
                      , boost::mpl::true_
                      , boost::mpl::false_
                    >
#endif
                  , boost::mpl::false_
                >
            >::type
          , aux::tag<Tag,T>
        >::type BOOST_CONSTEXPR
        operator=(T&& x) const
        {
            typedef typename aux::tag<Tag,T>::type result;
            return result(boost::forward<T>(x));
        }

        template <class T>
        inline typename boost::lazy_enable_if<
            typename boost::mpl::eval_if<
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
                boost::is_same<
#else
                std::is_same<
#endif
                    typename Tag::qualifier
                  , boost::parameter::in_reference
                >
              , boost::mpl::true_
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
              , boost::is_same<
#else
              , boost::mpl::if_<
                    std::is_same<
#endif
                        typename Tag::qualifier
                      , boost::parameter::forward_reference
#if !defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
                    >
                  , boost::mpl::true_
                  , boost::mpl::false_
#endif
                >
            >::type
          , aux::tag<Tag,T const>
        >::type BOOST_CONSTEXPR
        operator=(T const&& x) const
        {
            typedef typename aux::tag<Tag,T const>::type result;
            return result(boost::forward<T const>(x));
        }

        template <class Default>
        inline typename boost::enable_if<
            typename boost::mpl::eval_if<
                typename boost::mpl::if_<
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
                    boost::is_same<
#else
                    std::is_same<
#endif
                        typename Tag::qualifier
                      , boost::parameter::consume_reference
                    >
                  , boost::mpl::true_
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
                  , boost::is_same<
#else
                  , std::is_same<
#endif
                        typename Tag::qualifier
                      , boost::parameter::forward_reference
                    >
                >::type
              , boost::mpl::true_
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
              , boost::mpl::if_<
                    boost::is_same<
#else
              , boost::mpl::eval_if<
                    std::is_same<
#endif
                        typename Tag::qualifier
                      , boost::parameter::in_reference
                    >
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
                  , boost::is_scalar<Default>
#else
                  , boost::mpl::if_<
                        std::is_scalar<Default>
                      , boost::mpl::true_
                      , boost::mpl::false_
                    >
#endif
                  , boost::mpl::false_
                >
            >::type
          , aux::default_r_<Tag,Default>
        >::type
        operator|(Default&& d) const
        {
            return aux::default_r_<Tag,Default>(boost::forward<Default>(d));
        }

        template <class Default>
        inline typename boost::enable_if<
            typename boost::mpl::eval_if<
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
                boost::is_same<
#else
                std::is_same<
#endif
                    typename Tag::qualifier
                  , boost::parameter::in_reference
                >
              , boost::mpl::true_
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
              , boost::is_same<
#else
              , boost::mpl::if_<
                    std::is_same<
#endif
                        typename Tag::qualifier
                      , boost::parameter::forward_reference
#if !defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
                    >
                  , boost::mpl::true_
                  , boost::mpl::false_
#endif
                >
            >::type
          , aux::default_r_<Tag,Default const>
        >::type
        operator|(Default const&& d) const
        {
            return aux::default_r_<Tag,Default const>(
                boost::forward<Default const>(d)
            );
        }
#endif // BOOST_PARAMETER_HAS_PERFECT_FORWARDING

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
#define BOOST_PARAMETER_KEYWORD(tag_namespace, name)                         \
    namespace tag_namespace                                                  \
    {                                                                        \
        struct name                                                          \
        {                                                                    \
            static char const* keyword_name()                                \
            {                                                                \
                return #name;                                                \
            }                                                                \
        };                                                                   \
    }                                                                        \
    namespace                                                                \
    {                                                                        \
        ::boost::parameter::keyword<tag_namespace::name> const& name         \
          = ::boost::parameter::keyword<tag_namespace::name>::instance;      \
    }
/**/

#endif // KEYWORD_050328_HPP

