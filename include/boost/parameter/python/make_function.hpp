// Copyright Daniel Wallin 2005. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_MAKE_FUNCTION_051130_HPP
# define BOOST_PARAMETER_MAKE_FUNCTION_051130_HPP

#include <boost/mpl/next.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/begin_end.hpp>

#include <boost/python/args.hpp>
#include <boost/python/make_function.hpp>
#include <boost/python/default_call_policies.hpp>

namespace boost { namespace parameter { namespace python {

namespace detail
{

  // Used as start value in the recursive arg() composition below.
  struct no_keywords
  {
      template <class T>
      T const& operator,(T const& x) const
      {
          return x;
      }
  };

  template <class T, class Iter, class End, class Keywords, class K>
  boost::python::object make_function_aux0(
      T* p, Iter, End e, Keywords const& keywords, mpl::pair<mpl::true_,K>)
  {
      return make_function_aux(
          p, typename mpl::next<Iter>::type(), e
        , (
              keywords, boost::python::arg(K::keyword())
          )
      );
  }

  template <class T, class Iter, class End, class Keywords, class K>
  boost::python::object make_function_aux0(
      T* p, Iter, End e, Keywords const& keywords, mpl::pair<mpl::false_,K>)
  {
      return make_function_aux(
          p, typename mpl::next<Iter>::type(), e
        , (
              keywords, boost::python::arg(K::keyword()) = T::default_(K())
          )
      );
  }

  template <class T, class Iter, class End, class Keywords>
  boost::python::object make_function_aux(
      T* p, Iter i, End e, Keywords const& keywords)
  {
      return make_function_aux0(
          p, i, e, keywords, typename mpl::deref<Iter>::type()
      );
  }

  template <class T, class End, class Keywords>
  boost::python::object make_function_aux(
      T*, End, End, Keywords const& keywords)
  {
      return boost::python::make_function(
          T()
        , boost::python::default_call_policies()
        , keywords
        , typename T::signature()
      );
  }

} // namespace detail

template <class T>
boost::python::object make_function()
{
    typedef typename T::keywords keywords;

    return detail::make_function_aux(
        (T*)0
      , typename mpl::begin<keywords>::type()
      , typename mpl::end<keywords>::type()
      , detail::no_keywords()
    );
}

}}} // namespace boost::parameter::python

#endif // BOOST_PARAMETER_MAKE_FUNCTION_051130_HPP

