// Copyright Daniel Wallin, David Abrahams 2005.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_TAGGED_ARGUMENT_050328_HPP
#define BOOST_PARAMETER_TAGGED_ARGUMENT_050328_HPP

namespace boost { namespace parameter { namespace aux {

    struct error_const_lvalue_bound_to_out_parameter;
    struct error_lvalue_bound_to_consume_parameter;
    struct error_rvalue_bound_to_out_parameter;
}}} // namespace boost::parameter::aux

#include <boost/parameter/keyword_fwd.hpp>
#include <boost/parameter/config.hpp>
#include <boost/mpl/eval_if.hpp>

#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_const.hpp>
#else
#include <type_traits>
#endif

namespace boost { namespace parameter { namespace aux {

    template <class Keyword, class Arg>
    struct tagged_argument_type
      : boost::mpl::eval_if<
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
            boost::is_same<
#else
            std::is_same<
#endif
                typename Keyword::qualifier
              , boost::parameter::out_reference
            >
          , boost::parameter::aux::error_const_lvalue_bound_to_out_parameter
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
          , boost::remove_const<Arg>
#else
          , std::remove_const<Arg>
#endif
        >
    {
    };
}}} // namespace boost::parameter::aux

#include <boost/parameter/aux_/tagged_argument_fwd.hpp>
#include <boost/parameter/aux_/is_tagged_argument.hpp>
#include <boost/parameter/aux_/default.hpp>
#include <boost/parameter/aux_/unwrap_cv_reference.hpp>
#include <boost/parameter/aux_/void.hpp>
#include <boost/parameter/aux_/arg_list.hpp>
#include <boost/parameter/aux_/result_of0.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/apply_wrap.hpp>

#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS) || defined(BOOST_MSVC)
#include <boost/type_traits/is_const.hpp>
#endif

#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
#include <boost/type_traits/is_function.hpp>
#include <boost/type_traits/remove_reference.hpp>
#endif

#if defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)
#include <boost/move/utility_core.hpp>
#include <boost/core/enable_if.hpp>
#endif

#if defined(BOOST_NO_CXX11_HDR_FUNCTIONAL)
#include <boost/function.hpp>
#else
#include <functional>
#endif

namespace boost { namespace parameter { namespace aux {

    // Holds an lvalue reference to an argument of type Arg associated with
    // keyword Keyword
    template <class Keyword, class Arg>
    class tagged_argument : boost::parameter::aux::tagged_argument_base
    {
#if defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)
        typedef typename boost::mpl::eval_if<
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
            boost::is_same<
#else
            std::is_same<
#endif
                typename Keyword::qualifier
              , boost::parameter::consume_reference
            >
          , boost::parameter::aux::error_lvalue_bound_to_consume_parameter
          , boost::mpl::eval_if<
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS) || defined(BOOST_MSVC)
                // MSVC 11.0 on AppVeyor reports error C2039:
                // '_Is_const': is not a member of 'std::_Ptr_traits<_Ty>'
                boost::is_const<Arg>
#else
                std::is_const<Arg>
#endif
              , boost::parameter::aux::tagged_argument_type<Keyword,Arg>
              , boost::mpl::identity<Arg>
            >
        >::type arg_type;
#else // !defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
        typedef typename boost::remove_const<Arg>::type arg_type;
#else
        typedef typename std::remove_const<Arg>::type arg_type;
#endif
#endif // BOOST_PARAMETER_HAS_PERFECT_FORWARDING

     public:
        typedef Keyword key_type;

        // Wrap plain (non-UDT) function objects in either
        // a boost::function or a std::function. -- Cromwell D. Enage
        typedef typename boost::mpl::if_<
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
            boost::is_function<arg_type>
#else
            std::is_function<arg_type>
#endif
#if defined(BOOST_NO_CXX11_HDR_FUNCTIONAL)
          , boost::function<arg_type>
#else
          , std::function<arg_type>
#endif
          , Arg
        >::type value_type;

        // If Arg is void_, then this type will evaluate to void_&.  If the
        // supplied argument is a plain function, then this type will evaluate
        // to a reference-to-const function wrapper type.  If the supplied
        // argument is an lvalue, then Arg will be deduced to the lvalue
        // reference. -- Cromwell D. Enage
        typedef typename boost::mpl::if_<
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
            boost::is_function<arg_type>
#else
            std::is_function<arg_type>
#endif
          , value_type const&
          , Arg&
        >::type reference;

     private:
        // Store plain functions by value, everything else by reference.
        // -- Cromwell D. Enage
        typename boost::mpl::if_<
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
            boost::is_function<arg_type>
#else
            std::is_function<arg_type>
#endif
          , value_type
          , reference
        >::type value;

     public:
        template <typename A>
        inline explicit tagged_argument(A& x) : value(x)
        {
        }

        inline tagged_argument(tagged_argument const& copy)
          : value(copy.value)
        {
        }

        // A metafunction class that, given a keyword and a default type,
        // returns the appropriate result type for a keyword lookup given
        // that default.
        struct binding
        {
            template <class KW, class Default, class Reference>
            struct apply
            {
                typedef typename boost::mpl::eval_if<
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
                    boost::is_same<KW,key_type>
#else
                    std::is_same<KW,key_type>
#endif
                  , boost::mpl::if_<Reference,reference,value_type>
                  , boost::mpl::identity<Default>
                >::type type;
            };
        };

        // Comma operator to compose argument list without using parameters<>.
        // Useful for argument lists with undetermined length.
        template <class Keyword2, class Arg2>
        inline boost::parameter::aux::arg_list<
            tagged_argument<Keyword,Arg>
          , boost::parameter::aux::arg_list<tagged_argument<Keyword2,Arg2> > 
        >
        operator,(tagged_argument<Keyword2,Arg2> const& x) const
        {
            return boost::parameter::aux::arg_list<
                tagged_argument<Keyword,Arg>
              , boost::parameter::aux::arg_list<tagged_argument<Keyword2,Arg2> > 
            >(
                *this
              , boost::parameter::aux::arg_list<
                    tagged_argument<Keyword2,Arg2>
                >(x, boost::parameter::aux::empty_arg_list())
            );
        }

#if defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)
        template <class Keyword2, class Arg2>
        inline boost::parameter::aux::arg_list<
            tagged_argument<Keyword,Arg>
          , boost::parameter::aux::arg_list<
                boost::parameter::aux::tagged_argument_rref<Keyword2,Arg2>
            > 
        >
        operator,(
            boost::parameter::aux::tagged_argument_rref<
                Keyword2
              , Arg2
            > const& x
        ) const
        {
            return boost::parameter::aux::arg_list<
                tagged_argument<Keyword,Arg>
              , boost::parameter::aux::arg_list<
                    boost::parameter::aux::tagged_argument_rref<Keyword2,Arg2>
                > 
            >(
                *this
              , boost::parameter::aux::arg_list<
                    boost::parameter::aux::tagged_argument_rref<Keyword2,Arg2>
                >(x, boost::parameter::aux::empty_arg_list())
            );
        }
#endif // BOOST_PARAMETER_HAS_PERFECT_FORWARDING

        // Accessor interface.
        inline reference get_value() const
        {
            return this->value;
        }

        inline reference
        operator[](boost::parameter::keyword<Keyword> const&) const
        {
            return this->get_value();
        }

#if defined(BOOST_NO_FUNCTION_TEMPLATE_ORDERING) || \
    BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
        template <class KW, class Default>
        inline Default&
        get_with_default(
            boost::parameter::aux::default_<KW,Default> const& x
          , int
        ) const
        {
            return x.value;
        }

#if defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)
        template <class KW, class Default>
        inline Default&&
        get_with_default(
            boost::parameter::aux::default_r_<KW,Default> const& x
          , int
        ) const
        {
            return boost::forward<Default>(x.value);
        }
#endif // BOOST_PARAMETER_HAS_PERFECT_FORWARDING

        template <class Default>
        inline reference
        get_with_default(
            boost::parameter::aux::default_<key_type,Default> const&
          , long
        ) const
        {
            return this->get_value();
        }

        template <class KW, class Default>
        inline typename boost::mpl::apply_wrap3<
            binding
          , KW
          , Default&
          , boost::mpl::true_
        >::type
        operator[](boost::parameter::aux::default_<KW,Default> const& x) const
        {
            return this->get_with_default(x, 0L);
        }

        template <class KW, class F>
        inline typename boost::parameter::aux::result_of0<F>::type 
        get_with_lazy_default(
            boost::parameter::aux::lazy_default<KW,F> const& x
          , int
        ) const
        {
            return x.compute_default();
        }

        template <class F>
        inline reference
        get_with_lazy_default(
            boost::parameter::aux::lazy_default<key_type,F> const&
          , long
        ) const
        {
            return this->get_value();
        }

        template <class KW, class F>
        inline typename boost::mpl::apply_wrap3<
            binding
          , KW
          , typename boost::parameter::aux::result_of0<F>::type
          , boost::mpl::true_
        >::type
        operator[](boost::parameter::aux::lazy_default<KW,F> const& x) const
        {
            return this->get_with_lazy_default(x, 0L);
        }
#else
//#if !defined(BOOST_NO_FUNCTION_TEMPLATE_ORDERING) && \
//    !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
        template <class Default>
        inline reference
        operator[](
            boost::parameter::aux::default_<key_type,Default> const&
        ) const
        {
            return this->get_value();
        }

        template <class F>
        inline reference
        operator[](
            boost::parameter::aux::lazy_default<key_type,F> const&
        ) const
        {
            return this->get_value();
        }

        template <class KW, class Default>
        inline Default&
        operator[](boost::parameter::aux::default_<KW,Default> const& x) const
        {
            return x.value;
        }

#if defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)
        template <class KW, class Default>
        inline Default&&
        operator[](boost::parameter::aux::default_r_<KW,Default> const& x) const
        {
            return boost::forward<Default>(x.value);
        }
#endif

        template <class KW, class F>
        inline typename boost::parameter::aux::result_of0<F>::type
        operator[](boost::parameter::aux::lazy_default<KW,F> const& x) const
        {
            return x.compute_default();
        }

        template <class ParameterRequirements>
        static typename ParameterRequirements::has_default
        satisfies(ParameterRequirements*);

        template <class HasDefault, class Predicate>
        static typename boost::mpl::apply_wrap1<Predicate,value_type>::type
        satisfies(
            boost::parameter::aux::parameter_requirements<
                key_type
              , Predicate
              , HasDefault
            >*
        );
#endif // Function template ordering, Borland workarounds needed.

#if BOOST_WORKAROUND(BOOST_MSVC, BOOST_TESTED_AT(1310))
        // warning suppression
     private:
        void operator=(tagged_argument const&);

     public:    
#endif
        // MPL sequence support
        // Convenience for users
        typedef tagged_argument type;
        // For the benefit of iterators
        typedef boost::parameter::aux::empty_arg_list tail_type;
        // For dispatching to sequence intrinsics
        typedef boost::parameter::aux::arg_list_tag tag;
    };

#if defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)
    // Holds an rvalue reference to an argument of type Arg associated with
    // keyword Keyword
    template <class Keyword, class Arg>
    struct tagged_argument_rref : boost::parameter::aux::tagged_argument_base
    {
        typedef typename boost::mpl::eval_if<
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
            boost::is_same<
#else
            std::is_same<
#endif
                typename Keyword::qualifier
              , boost::parameter::out_reference
            >
          , boost::parameter::aux::error_rvalue_bound_to_out_parameter
          , boost::mpl::identity<Keyword>
        >::type key_type;
        typedef Arg value_type;
        typedef Arg&& reference;

     private:
        reference value;

     public:
        inline explicit tagged_argument_rref(reference x)
          : value(boost::forward<Arg>(x))
        {
        }

        inline tagged_argument_rref(tagged_argument_rref const& copy)
          : value(boost::forward<Arg>(copy.value))
        {
        }

        // A metafunction class that, given a keyword and a default type,
        // returns the appropriate result type for a keyword lookup given
        // that default.
        struct binding
        {
            template <class KW, class Default, class Reference>
            struct apply
            {
                typedef typename boost::mpl::eval_if<
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
                    boost::is_same<KW,key_type>
#else
                    std::is_same<KW,key_type>
#endif
                  , boost::mpl::if_<Reference,reference,value_type>
                  , boost::mpl::identity<Default>
                >::type type;
            };
        };

        // Comma operator to compose argument list without using parameters<>.
        // Useful for argument lists with undetermined length.
        template <class Keyword2, class Arg2>
        inline boost::parameter::aux::arg_list<
            boost::parameter::aux::tagged_argument_rref<Keyword,Arg>
          , boost::parameter::aux::arg_list<
                boost::parameter::aux::tagged_argument<Keyword2,Arg2>
            > 
        >
        operator,(
            boost::parameter::aux::tagged_argument<Keyword2,Arg2> const& x
        ) const
        {
            return boost::parameter::aux::arg_list<
                tagged_argument_rref<Keyword,Arg>
              , boost::parameter::aux::arg_list<
                    boost::parameter::aux::tagged_argument<Keyword2,Arg2>
                > 
            >(
                *this
              , boost::parameter::aux::arg_list<
                    boost::parameter::aux::tagged_argument<Keyword2,Arg2>
                >(x, boost::parameter::aux::empty_arg_list())
            );
        }

        template <class Keyword2, class Arg2>
        inline boost::parameter::aux::arg_list<
            tagged_argument_rref<Keyword,Arg>
          , boost::parameter::aux::arg_list<
                tagged_argument_rref<Keyword2,Arg2>
            > 
        >
        operator,(tagged_argument_rref<Keyword2,Arg2> const& x) const
        {
            return boost::parameter::aux::arg_list<
                tagged_argument_rref<Keyword,Arg>
              , boost::parameter::aux::arg_list<
                    tagged_argument_rref<Keyword2,Arg2>
                > 
            >(
                *this
              , boost::parameter::aux::arg_list<
                    tagged_argument_rref<Keyword2,Arg2>
                >(x, boost::parameter::aux::empty_arg_list())
            );
        }

        // Accessor interface.
        inline reference get_value() const
        {
            return boost::forward<Arg>(this->value);
        }

        inline reference
        operator[](boost::parameter::keyword<Keyword> const&) const
        {
            return this->get_value();
        }

#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
        template <class KW, class Default>
        inline Default&&
        get_with_default(
            boost::parameter::aux::default_r_<KW,Default> const& x
          , int
        ) const
        {
            return boost::forward<Default>(x.value);
        }

        template <class Default>
        inline reference
        get_with_default(
            boost::parameter::aux::default_r_<key_type,Default> const&
          , long
        ) const
        {
            return this->get_value();
        }

        template <class KW, class Default>
        inline typename boost::mpl::apply_wrap3<
            binding
          , KW
          , Default&&
          , boost::mpl::true_
        >::type
        operator[](
            boost::parameter::aux::default_r_<KW,Default> const& x
        ) const
        {
            return this->get_with_default(x, 0L);
        }

        template <class KW, class F>
        inline typename boost::parameter::aux::result_of0<F>::type 
        get_with_lazy_default(
            boost::parameter::aux::lazy_default<KW,F> const& x
          , int
        ) const
        {
            return x.compute_default();
        }

        template <class F>
        inline reference
        get_with_lazy_default(
            boost::parameter::aux::lazy_default<key_type,F> const&
          , long
        ) const
        {
            return this->get_value();
        }

        template <class KW, class F>
        inline typename boost::mpl::apply_wrap3<
            binding
          , KW
          , typename boost::parameter::aux::result_of0<F>::type
          , boost::mpl::true_
        >::type
        operator[](boost::parameter::aux::lazy_default<KW,F> const& x) const
        {
            return this->get_with_lazy_default(x, 0L);
        }
#else // !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
        template <class Default>
        inline reference
        operator[](
            boost::parameter::aux::default_r_<key_type,Default> const&
        ) const
        {
            return this->get_value();
        }

        template <class F>
        inline reference
        operator[](
            boost::parameter::aux::lazy_default<key_type,F> const&
        ) const
        {
            return this->get_value();
        }

        template <class KW, class Default>
        inline Default&&
        operator[](
            boost::parameter::aux::default_r_<KW,Default> const& x
        ) const
        {
            return boost::forward<Default>(x.value);
        }

        template <class KW, class F>
        inline typename result_of0<F>::type
        operator[](boost::parameter::aux::lazy_default<KW,F> const& x) const
        {
            return x.compute_default();
        }

        template <class ParameterRequirements>
        static typename ParameterRequirements::has_default
        satisfies(ParameterRequirements*);

        template <class HasDefault, class Predicate>
        static typename boost::mpl::apply_wrap1<Predicate,value_type>::type
        satisfies(
            boost::parameter::aux::parameter_requirements<
                key_type
              , Predicate
              , HasDefault
            >*
        );
#endif // Borland workarounds needed.

#if BOOST_WORKAROUND(BOOST_MSVC, BOOST_TESTED_AT(1310))
        // warning suppression
     private:
        void operator=(tagged_argument_rref const&);

     public:    
#endif
        // MPL sequence support
        // Convenience for users
        typedef tagged_argument_rref type;
        // For the benefit of iterators
        typedef boost::parameter::aux::empty_arg_list tail_type;
        // For dispatching to sequence intrinsics
        typedef boost::parameter::aux::arg_list_tag tag;
    };
#endif // BOOST_PARAMETER_HAS_PERFECT_FORWARDING
}}} // namespace boost::parameter::aux

#endif // BOOST_PARAMETER_TAGGED_ARGUMENT_050328_HPP

