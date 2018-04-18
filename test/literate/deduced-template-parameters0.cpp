
#include <boost/parameter.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/noncopyable.hpp>
#include <memory>
#include <boost/config.hpp>

namespace boost { namespace python {

    BOOST_PARAMETER_TEMPLATE_KEYWORD(class_type)
    BOOST_PARAMETER_TEMPLATE_KEYWORD(base_list)
    BOOST_PARAMETER_TEMPLATE_KEYWORD(held_type)
    BOOST_PARAMETER_TEMPLATE_KEYWORD(copyable)
}}

namespace boost { namespace python {
    namespace detail {

        struct bases_base
        {
        };
    }

    template <class A0 = void, class A1 = void, class A2 = void>
    struct bases : detail::bases_base
    {
    };
}}

#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/eval_if.hpp>

#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
#include <boost/type_traits.hpp>
#else
#include <type_traits>
#endif

namespace boost { namespace python {

    typedef boost::parameter::parameters<
        boost::parameter::required<
            tag::class_type
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
#if defined(BOOST_TT_HAS_CONFORMING_IS_CLASS_IMPLEMENTATION) && \
    (1 == BOOST_TT_HAS_CONFORMING_IS_CLASS_IMPLEMENTATION)
          , boost::is_class<boost::mpl::_>
#else
          , boost::mpl::if_<
                boost::is_scalar<boost::mpl::_>
              , boost::mpl::false_
              , boost::mpl::true_
            >
#endif
#else // !defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
          , boost::mpl::if_<
                std::is_class<boost::mpl::_>
              , boost::mpl::true_
              , boost::mpl::false_
            >
#endif // BOOST_NO_CXX11_HDR_TYPE_TRAITS
        >
      , boost::parameter::optional<
            boost::parameter::deduced<tag::base_list>
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
          , boost::is_base_of<detail::bases_base,boost::mpl::_>
#else
          , boost::mpl::if_<
                std::is_base_of<detail::bases_base,boost::mpl::_>
              , boost::mpl::true_
              , boost::mpl::false_
            >
#endif
        >
      , boost::parameter::optional<
            boost::parameter::deduced<tag::held_type>
          , boost::mpl::eval_if<
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
                boost::is_base_of<detail::bases_base,boost::mpl::_>
#else
                std::is_base_of<detail::bases_base,boost::mpl::_>
#endif
              , boost::mpl::false_
              , boost::mpl::if_<
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
                    boost::is_same<noncopyable,boost::mpl::_>
#else
                    std::is_same<noncopyable,boost::mpl::_>
#endif
                  , boost::mpl::false_
                  , boost::mpl::true_
                >
            >
        >
      , boost::parameter::optional<
            boost::parameter::deduced<tag::copyable>
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
          , boost::is_same<noncopyable,boost::mpl::_>
#else
          , boost::mpl::if_<
                std::is_same<noncopyable,boost::mpl::_>
              , boost::mpl::true_
              , boost::mpl::false_
            >
#endif
        >
    > class_signature;

    template <
        class A0
      , class A1 = boost::parameter::void_
      , class A2 = boost::parameter::void_
      , class A3 = boost::parameter::void_
    >
    struct class_
    {
        // Create ArgumentPack
        typedef typename boost::python::class_signature::BOOST_NESTED_TEMPLATE
        bind<A0,A1,A2,A3>::type args;

        // Extract first logical parameter.
        typedef typename boost::parameter::value_type<
            args,boost::python::tag::class_type
        >::type class_type;

        typedef typename boost::parameter::value_type<
            args,boost::python::tag::base_list,boost::python::bases<>
        >::type base_list;

        typedef typename boost::parameter::value_type<
            args,boost::python::tag::held_type,class_type
        >::type held_type;

        typedef typename boost::parameter::value_type<
            args,boost::python::tag::copyable,void
        >::type copyable;
    };
}}

struct B
{
};

struct D
{
};

typedef boost::python::class_<B,boost::noncopyable> c1;

#if defined(BOOST_NO_CXX11_SMART_PTR)
typedef boost::python::class_<D,std::auto_ptr<D>,boost::python::bases<B> > c2;
#else
typedef boost::python::class_<
    D,std::unique_ptr<D>,boost::python::bases<B>
> c2;
#endif

#include <boost/mpl/assert.hpp>
#include <boost/mpl/aux_/test.hpp>

MPL_TEST_CASE()
{
#if defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
    BOOST_MPL_ASSERT((boost::is_same<c1::class_type,B>));
    BOOST_MPL_ASSERT((boost::is_same<c1::base_list,boost::python::bases<> >));
    BOOST_MPL_ASSERT((boost::is_same<c1::held_type,B>));
    BOOST_MPL_ASSERT((boost::is_same<c1::copyable,boost::noncopyable>));
    BOOST_MPL_ASSERT((boost::is_same<c2::class_type,D>));
    BOOST_MPL_ASSERT((boost::is_same<c2::base_list,boost::python::bases<B> >));
#if defined(BOOST_NO_CXX11_SMART_PTR)
    BOOST_MPL_ASSERT((
        boost::is_same<c2::held_type,std::auto_ptr<D> >
    ));
#else
    BOOST_MPL_ASSERT((
        boost::is_same<c2::held_type,std::unique_ptr<D> >
    ));
#endif
    BOOST_MPL_ASSERT((boost::is_same<c2::copyable,void>));
#else // !defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
    BOOST_MPL_ASSERT((
        typename boost::mpl::if_<
            std::is_same<c1::class_type,B>
          , boost::mpl::true_
          , boost::mpl::false_
        >::type
    ));
    BOOST_MPL_ASSERT((
        typename boost::mpl::if_<
            std::is_same<c1::base_list,boost::python::bases<> >
          , boost::mpl::true_
          , boost::mpl::false_
        >::type
    ));
    BOOST_MPL_ASSERT((
        typename boost::mpl::if_<
            std::is_same<c1::held_type,B>
          , boost::mpl::true_
          , boost::mpl::false_
        >::type
    ));
    BOOST_MPL_ASSERT((
        typename boost::mpl::if_<
            std::is_same<c1::copyable,boost::noncopyable>
          , boost::mpl::true_
          , boost::mpl::false_
        >::type
    ));
    BOOST_MPL_ASSERT((
        typename boost::mpl::if_<
            std::is_same<c2::class_type,D>
          , boost::mpl::true_
          , boost::mpl::false_
        >::type
    ));
    BOOST_MPL_ASSERT((
        typename boost::mpl::if_<
            std::is_same<c2::base_list,boost::python::bases<B> >
          , boost::mpl::true_
          , boost::mpl::false_
        >::type
    ));
#if defined(BOOST_NO_CXX11_SMART_PTR)
    BOOST_MPL_ASSERT((
        typename boost::mpl::if_<
            std::is_same<c2::held_type,std::auto_ptr<D> >
          , boost::mpl::true_
          , boost::mpl::false_
        >::type
    ));
#else
    BOOST_MPL_ASSERT((
        typename boost::mpl::if_<
            std::is_same<c2::held_type,std::unique_ptr<D> >
          , boost::mpl::true_
          , boost::mpl::false_
        >::type
    ));
#endif // BOOST_NO_CXX11_SMART_PTR
    BOOST_MPL_ASSERT((
        typename boost::mpl::if_<
            std::is_same<c2::copyable,void>
          , boost::mpl::true_
          , boost::mpl::false_
        >::type
    ));
#endif // BOOST_NO_CXX11_HDR_TYPE_TRAITS
}

