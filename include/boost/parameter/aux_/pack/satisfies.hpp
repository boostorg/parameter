// Copyright David Abrahams, Daniel Wallin 2003.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_AUX_PACK_SATISFIES_HPP
#define BOOST_PARAMETER_AUX_PACK_SATISFIES_HPP

#include <boost/mpl/bool.hpp>
#include <boost/config.hpp>
#include <boost/config/workaround.hpp>

#if BOOST_WORKAROUND(BOOST_MSVC, == 1310)
#include <boost/parameter/aux_/augment_predicate.hpp>
#include <boost/parameter/aux_/void.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/apply_wrap.hpp>
#include <boost/type_traits/is_same.hpp>
#else
#include <boost/parameter/aux_/yesno.hpp>
#include <boost/tti/detail/dnullptr.hpp>
#endif

namespace boost { namespace parameter { namespace aux {

    // Returns mpl::true_ iff the given ParameterRequirements are satisfied by
    // ArgList.
    template <typename ArgList, typename ParameterRequirements>
    struct satisfies
    {
#if BOOST_WORKAROUND(BOOST_MSVC, == 1310)
        // VC7.1 can't handle the sizeof() implementation below,
        // so we use this instead.
        typedef typename ::boost::mpl::apply_wrap3<
            typename ArgList::binding
          , typename ParameterRequirements::keyword
          , ::boost::parameter::void_
          , ::boost::mpl::false_
        >::type bound;

        typedef typename ::boost::mpl::eval_if<
            ::boost::is_same<bound,::boost::parameter::void_>
          , typename ParameterRequirements::has_default
          , ::boost::mpl::apply_wrap2<
                ::boost::parameter::aux::augment_predicate<
                    typename ParameterRequirements::predicate
                  , typename ArgList::reference
                  , typename ArgList::key_type
                >
              , bound
              , ArgList
            >
        >::type type;
#else   // !BOOST_WORKAROUND(BOOST_MSVC, == 1310)
        BOOST_STATIC_CONSTANT(
            bool, value = (
                sizeof(
                    ::boost::parameter::aux::to_yesno(
                        ArgList::satisfies(
                            static_cast<ParameterRequirements*>(
                                BOOST_TTI_DETAIL_NULLPTR
                            )
                          , static_cast<ArgList*>(BOOST_TTI_DETAIL_NULLPTR)
                        )
                    )
                ) == sizeof(::boost::parameter::aux::yes_tag)
            )
        );

        typedef ::boost::mpl::bool_<satisfies::value> type;
#endif  // MSVC 7.1 workarounds needed.
    };
}}} // namespace boost::parameter::aux

#include <boost/parameter/aux_/pack/as_parameter_requirements.hpp>

namespace boost { namespace parameter { namespace aux {
    // Returns mpl::true_ if the requirements of the given ParameterSpec
    // are satisfied by ArgList.
    template <typename ArgList, typename ParameterSpec>
    struct satisfies_requirements_of
      : ::boost::parameter::aux::satisfies<
            ArgList
          , typename ::boost::parameter::aux
            ::as_parameter_requirements<ParameterSpec>::type
        >
    {
    };
}}} // namespace boost::parameter::aux

#endif  // include guard

