// Copyright Daniel Wallin 2005. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_GENERAL_051210_HPP
# define BOOST_PARAMETER_GENERAL_051210_HPP

# include <boost/mpl/vector.hpp>
# include <boost/mpl/fold.hpp>
# include <boost/mpl/prior.hpp>
# include <boost/mpl/shift_right.hpp>
# include <boost/mpl/shift_left.hpp>
# include <boost/mpl/bitand.hpp>
# include <boost/mpl/pair.hpp>
# include <boost/mpl/size.hpp>
# include <boost/mpl/push_back.hpp>
# include <boost/mpl/or.hpp>
# include <boost/mpl/count_if.hpp>
# include <boost/mpl/transform.hpp>
# include <boost/mpl/front.hpp>
# include <boost/mpl/iterator_range.hpp>
# include <boost/mpl/next.hpp>
# include <boost/mpl/begin_end.hpp>
# include <boost/mpl/not.hpp>
# include <boost/type.hpp>
# include <boost/parameter/python/aux_/invoker.hpp>
# include <boost/python/def.hpp>

namespace boost { namespace parameter { namespace python {

namespace aux
{

  template <class K>
  struct is_optional
    : mpl::not_<typename K::second>
  {};

  template <class K, class Required, class T>
  struct arg_spec
  {
      typedef K keyword;
      typedef Required required;
      typedef T type;
  };
  
  template <class K, class T>
  struct make_arg_spec
  {
      typedef arg_spec<typename K::first, typename K::second, T> type;
  };

  template <class Spec, class State>
  struct combinations_op
  {
      typedef typename State::second bits;
      typedef typename State::first result0;

      typedef typename mpl::if_<
          mpl::or_<
              typename Spec::required
            , mpl::bitand_<bits, mpl::long_<1> >
          >
        , typename mpl::push_back<result0, Spec>::type
        , result0
      >::type result;

      typedef typename mpl::if_<
          typename Spec::required
        , bits
        , typename mpl::shift_right<bits, mpl::long_<1> >::type
      >::type next_bits;

      typedef mpl::pair<
          result
        , next_bits
      > type;
  };

  // Used as start value in the recursive arg() composition below.
  struct no_keywords
  {
      template <class T>
      T const& operator,(T const& x) const
      {
          return x;
      }
  };
  
  template <class F, class Iter, class End, class Keywords>
  void def_combination_aux(char const* name, F f, Iter, End, Keywords const& keywords)
  {
      typedef typename mpl::deref<Iter>::type spec;
      typedef typename spec::keyword kw;

      def_combination_aux(
          name, f, typename mpl::next<Iter>::type(), End()
        , (
              keywords, boost::python::arg(kw::keyword())
          )
      );
  }

  template <class F, class End, class Keywords>
  void def_combination_aux(char const* name, F f, End, End, Keywords const& keywords)
  {
      boost::python::def(name, f, keywords);
  } 

  template <class F, class End>
  void def_combination_aux(char const* name, F f, End, End, no_keywords const&)
  {
      boost::python::def(name, f);
  }
  
  template <class R, class Specs, class Bits, class M>
  void def_combination(char const* name, boost::type<R>, Specs*, Bits, M*)
  {
      typedef typename mpl::fold<
          Specs
        , mpl::pair<mpl::vector0<>, Bits>
        , combinations_op<mpl::_2, mpl::_1>
      >::type combination0;

      typedef typename combination0::first combination;
      typedef typename mpl::size<combination>::type arity;

      def_combination_aux(
          name
        , &invoker<arity::value, M, R, combination>::execute
        , typename mpl::begin<combination>::type()
        , typename mpl::end<combination>::type()
        , no_keywords()
      );
  }

  template <class R, class Specs, class Bits, class End, class M>
  void def_combinations(char const* name, boost::type<R>, Specs*, Bits, End, M*)
  {
      def_combination(name, boost::type<R>(), (Specs*)0, Bits(), (M*)0);

      def_combinations(
          name
        , boost::type<R>()
        , (Specs*)0
        , mpl::long_<Bits::value - 1>()
        , End()
        , (M*)0
      );
  }

  template <class Specs, class R, class End, class M>
  void def_combinations(char const* name, boost::type<R>, Specs*, End, End, M*)
  {}

} // namespace aux

template <class M, class Signature>
void def(char const* name, Signature)
{
    typedef mpl::iterator_range<
        typename mpl::next<
            typename mpl::begin<Signature>::type
        >::type
      , typename mpl::end<Signature>::type
    > arg_types;

    typedef typename mpl::count_if<
        typename M::keywords
      , aux::is_optional<mpl::_1>
    >::type optional_arity;

    typedef typename mpl::transform<
        typename M::keywords
      , arg_types
      , aux::make_arg_spec<mpl::_1, mpl::_2>
    >::type arg_specs;

    typedef typename mpl::front<Signature>::type result_type;

    aux::def_combinations(
        name
      , boost::type<result_type>()
      , (arg_specs*)0
      , typename mpl::shift_left<mpl::long_<1>, optional_arity>::type()
      , mpl::long_<0>()
      , (M*)0
    );
}

}}} // namespace boost::parameter::python

#endif // BOOST_PARAMETER_GENERAL_051210_HPP

