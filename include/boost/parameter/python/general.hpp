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
# include <boost/python/def.hpp>
# include <boost/parameter/python/aux_/invoker.hpp>
# include <boost/parameter/aux_/maybe.hpp>

namespace boost { namespace parameter { namespace python {

PyObject* init_sentinel()
{
    static PyTypeObject sentinel_type  = {
        PyObject_HEAD_INIT(NULL)
        0,                                /* ob_size        */
        "Boost.Parameter.Unspecified",    /* tp_name        */
        PyType_Type.tp_basicsize,         /* tp_basicsize   */
        0,                                /* tp_itemsize    */
        0,                                /* tp_dealloc     */
        0,                                /* tp_print       */
        0,                                /* tp_getattr     */
        0,                                /* tp_setattr     */
        0,                                /* tp_compare     */
        0,                                /* tp_repr        */
        0,                                /* tp_as_number   */
        0,                                /* tp_as_sequence */
        0,                                /* tp_as_mapping  */
        0,                                /* tp_hash        */
        0,                                /* tp_call        */
        0,                                /* tp_str         */
        0,                                /* tp_getattro    */
        0,                                /* tp_setattro    */
        0,                                /* tp_as_buffer   */
        Py_TPFLAGS_DEFAULT,               /* tp_flags       */
        0,                                /* tp_doc         */
    };

    assert(PyType_Ready(&sentinel_type) == 0);
    return boost::python::upcast<PyObject>(&sentinel_type);
}

inline boost::python::handle<>& sentinel_value()
{
    static boost::python::handle<> x;
    return x;
}

struct empty_tag_to_python
{
    empty_tag_to_python()
    {
        boost::python::to_python_converter<
            boost::parameter::aux::empty_maybe_tag, empty_tag_to_python
        >();

        sentinel_value() = boost::python::handle<>(init_sentinel());
    }

    static PyObject* convert(boost::parameter::aux::empty_maybe_tag)
    {
        return boost::python::xincref(sentinel_value().get());
    }
};

}}} // namespace boost::parameter::python

namespace boost { namespace python 
{

  // Converts a Python value to a maybe<T>
  template <class T>
  struct arg_from_python<parameter::aux::maybe<T> >
    : arg_from_python<T>
  {
      arg_from_python(PyObject* p)
        : arg_from_python<T>(p)
        , empty(false)
      {
          if (parameter::python::sentinel_value().get() == p)
          {
              empty = true;
          }
      }

      bool convertible() const
      {
          if (empty) return true;
          
          return arg_from_python<T>::convertible();
      }

      boost::parameter::aux::maybe<T> operator()()
      {
          if (empty)
          {
              return boost::parameter::aux::maybe<T>();
          }
          else
          {
              return boost::parameter::aux::maybe<T>(
                  arg_from_python<T>::operator()()
              );
          }
      }

      bool empty;
  };

}} // namespace boost::python

namespace boost { namespace parameter { namespace python {

namespace aux
{

  template <class K>
  struct is_optional
    : mpl::not_<
          mpl::or_<typename K::required, typename K::optimized_default>
      >
  {};

  template <class K, class Required, class Optimized, class T>
  struct arg_spec
  {
      typedef K keyword;
      typedef Required required;
      typedef T type;
      typedef Optimized optimized_default;
  };
  
  template <class K, class T>
  struct make_arg_spec
  {
      typedef arg_spec<
          typename K::first, typename K::second, typename K::third, T
      > type;
  };

  template <class Spec, class State>
  struct combinations_op
  {
      typedef typename State::second bits;
      typedef typename State::first result0;

      typedef typename mpl::if_<
          mpl::or_<
              typename Spec::required
            , typename Spec::optimized_default
            , mpl::bitand_<bits, mpl::long_<1> >
          >
        , typename mpl::push_back<result0, Spec>::type
        , result0
      >::type result;

      typedef typename mpl::if_<
          mpl::or_<
              typename Spec::required
            , typename Spec::optimized_default
          >
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

  template <class K>
  char const* keyword_name(K*)
  {
      return K::keyword();
  }
  
  template <class Def, class F, class Iter, class End, class Keywords>
  void def_combination_aux0(
      Def def, F f, Iter, End, Keywords const& keywords, mpl::false_)
  {
      typedef typename mpl::deref<Iter>::type spec;
      typedef typename spec::keyword kw;

      def_combination_aux(
          def, f, typename mpl::next<Iter>::type(), End()
        , (
              keywords, boost::python::arg(keyword_name((kw*)0))
          )
      );
  }

  template <class Def, class F, class Iter, class End, class Keywords>
  void def_combination_aux0(
      Def def, F f, Iter, End, Keywords const& keywords, mpl::true_)
  {
      typedef typename mpl::deref<Iter>::type spec;
      typedef typename spec::keyword kw;

      def_combination_aux(
          def, f, typename mpl::next<Iter>::type(), End()
        , (
              keywords, boost::python::arg(kw::keyword()) 
                = boost::parameter::aux::empty_maybe_tag()
          )
      );
  }

  inline void initialize_converter()
  {
      static empty_tag_to_python cv;
  }

  template <class Def, class F, class Iter, class End, class Keywords>
  void def_combination_aux(
      Def def, F f, Iter, End, Keywords const& keywords)
  {
      typedef typename mpl::deref<Iter>::type spec;

      initialize_converter();

      typedef typename mpl::and_<
          typename spec::optimized_default
        , mpl::not_<typename spec::required>
      >::type optimized_default;
      
      def_combination_aux0(
          def, f, Iter(), End(), keywords, optimized_default()
      );
  }

  template <class Def, class F, class End, class Keywords>
  void def_combination_aux(
      Def def, F f, End, End, Keywords const& keywords)
  {
      def(f, keywords);
  } 

  template <class Def, class F, class End>
  void def_combination_aux(
      Def def, F f, End, End, no_keywords const&)
  {
      def(f);
  }

  template <
      class Def, class Specs, class Bits, class Invoker
  >
  void def_combination(
      Def def, Specs*, Bits, Invoker*)
  {
      typedef typename mpl::fold<
          Specs
        , mpl::pair<mpl::vector0<>, Bits>
        , combinations_op<mpl::_2, mpl::_1>
      >::type combination0;

      typedef typename combination0::first combination;

      typedef typename mpl::apply_wrap1<
          Invoker, combination
      >::type invoker;

      def_combination_aux(
          def
        , &invoker::execute
        , typename mpl::begin<combination>::type()
        , typename mpl::end<combination>::type()
        , no_keywords()
      );
  }

  template <
      class Def, class Specs, class Bits, class End, class Invoker
  >
  void def_combinations(
      Def def, Specs*, Bits, End, Invoker*)
  {
      def_combination(def, (Specs*)0, Bits(), (Invoker*)0);

      def_combinations(
          def
        , (Specs*)0
        , mpl::long_<Bits::value + 1>()
        , End()
        , (Invoker*)0
      );
  }

  template <
      class Def, class Specs, class End, class Invoker
  >
  void def_combinations(
      Def, Specs*, End, End, Invoker*)
  {}

  template <class Class>
  struct def_class
  {
      def_class(Class& cl, char const* name)
        : cl(cl)
        , name(name)
      {}

      template <class F>
      void operator()(F f) const
      {
          cl.def(name, f);
      }

      template <class F, class Keywords>
      void operator()(F f, Keywords const& keywords) const
      {
          cl.def(name, f, keywords);
      }

      Class& cl;
      char const* name;
  };

  template <class Class>
  struct def_init
  {
      def_init(Class& cl)
        : cl(cl)
      {}

      template <class F>
      void operator()(F f) const
      {
          cl.def(
              "__init__"
            , boost::python::make_constructor(f)
          );
      }

      template <class F, class Keywords>
      void operator()(F f, Keywords const& keywords) const
      {
          cl.def(
              "__init__"
            , boost::python::make_constructor(
                  f, boost::python::default_call_policies(), keywords
              )
          );
      }

      Class& cl;
  };

  struct def_function
  {
      def_function(char const* name)
        : name(name)
      {}
      
      template <class F>
      void operator()(F f) const
      {
          boost::python::def(name, f);
      }

      template <class F, class Keywords>
      void operator()(F f, Keywords const& keywords) const
      {
          boost::python::def(name, f, keywords);
      }

      char const* name;
  };  
  
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

    typedef typename mpl::transform<
        typename M::keywords
      , arg_types
      , aux::make_arg_spec<mpl::_1, mpl::_2>
    >::type arg_specs;

    typedef typename mpl::count_if<
        arg_specs
      , aux::is_optional<mpl::_1>
    >::type optional_arity;
    
    typedef typename mpl::front<Signature>::type result_type;
    typedef typename mpl::shift_left<mpl::long_<1>, optional_arity>::type upper;

    aux::def_combinations(
        aux::def_function(name)
      , (arg_specs*)0
      , mpl::long_<0>()
      , mpl::long_<upper::value>()
      , (aux::make_invoker<M, result_type>*)0
    );
}

template <class M, class Class, class Signature>
void def(Class& cl, char const* name, Signature)
{
    typedef mpl::iterator_range<
        typename mpl::next<
            typename mpl::begin<Signature>::type
        >::type
      , typename mpl::end<Signature>::type
    > arg_types;

    typedef typename mpl::transform<
        typename M::keywords
      , arg_types
      , aux::make_arg_spec<mpl::_1, mpl::_2>
    >::type arg_specs;

    typedef typename mpl::count_if<
        arg_specs
      , aux::is_optional<mpl::_1>
    >::type optional_arity;
    
    typedef typename mpl::front<Signature>::type result_type;
    typedef typename mpl::shift_left<mpl::long_<1>, optional_arity>::type upper;

    aux::def_combinations(
        aux::def_class<Class>(cl, name)
      , (arg_specs*)0
      , mpl::long_<0>()
      , mpl::long_<upper::value>()
      , (aux::make_invoker<M, result_type>*)0
    );
}

namespace aux
{

  template <class K>
  struct keyword
  {
      typedef K type;
  };

  template <class K>
  struct keyword<K*>
    : keyword<K>
  {};

  template <class K>
  struct required
  {
      typedef mpl::true_ type;
  };

  template <class K>
  struct required<K*>
  {
      typedef mpl::false_ type;
  };

  template <class K>
  struct optimized
  {
      typedef mpl::true_ type;
  };

  template <class K>
  struct optimized<K**>
  {
      typedef mpl::false_ type;
  };
  
  template <class K, class T>
  struct make_kw_spec
  {
      typedef arg_spec<
          typename keyword<K>::type
        , typename required<K>::type
        , typename optimized<K>::type
        , T
      > type;    
  };

} // namespace aux

template <class Keywords, class Signature>
struct init 
  : boost::python::def_visitor<init<Keywords, Signature> >
{
    template <class Class>
    void visit(Class& cl) const
    {
        typedef typename mpl::transform<
            Keywords
          , Signature
          , aux::make_kw_spec<mpl::_1, mpl::_2>
        >::type arg_specs;

        typedef typename mpl::count_if<
            arg_specs
          , aux::is_optional<mpl::_1>
        >::type optional_arity;

        typedef typename mpl::shift_left<mpl::long_<1>, optional_arity>::type upper;

        aux::def_combinations(
            aux::def_init<Class>(cl)
          , (arg_specs*)0
          , mpl::long_<0>()
          , mpl::long_<upper::value>()
          , (aux::make_init_invoker<typename Class::wrapped_type>*)0
        );
    }
};

template <class Keywords, class Signature>
struct call 
  : boost::python::def_visitor<call<Keywords, Signature> >
{
    template <class Class>
    void visit(Class& cl) const
    {
        typedef mpl::iterator_range<
            typename mpl::next<
                typename mpl::begin<Signature>::type
            >::type
          , typename mpl::end<Signature>::type
        > arg_types;

        typedef typename mpl::transform<
            Keywords
          , arg_types
          , aux::make_kw_spec<mpl::_1, mpl::_2>
        >::type arg_specs;

        typedef typename mpl::count_if<
            arg_specs
          , aux::is_optional<mpl::_1>
        >::type optional_arity;

        typedef typename mpl::front<Signature>::type result_type;
        typedef typename mpl::shift_left<mpl::long_<1>, optional_arity>::type upper;

        aux::def_combinations(
            aux::def_class<Class>(cl, "__call__")
          , (arg_specs*)0
          , mpl::long_<0>()
          , mpl::long_<upper::value>()
          , (aux::make_call_invoker<typename Class::wrapped_type, result_type>*)0
        );
    }
};

}}} // namespace boost::parameter::python

#endif // BOOST_PARAMETER_GENERAL_051210_HPP

