
#include <boost/parameter.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/noncopyable.hpp>
#include <boost/type_traits/is_class.hpp>
#include <memory>
#include <boost/config.hpp>

using namespace boost::parameter;

namespace boost { namespace python {

    BOOST_PARAMETER_TEMPLATE_KEYWORD(class_type)
    BOOST_PARAMETER_TEMPLATE_KEYWORD(base_list)
    BOOST_PARAMETER_TEMPLATE_KEYWORD(held_type)
    BOOST_PARAMETER_TEMPLATE_KEYWORD(copyable)

    template <typename B = int>
    struct bases
    {
    };
}}

namespace boost { namespace python {

    typedef boost::parameter::parameters<
        boost::parameter::required<
            tag::class_type
          , boost::is_class<boost::mpl::_>
        >
      , boost::parameter::optional<
            tag::base_list
          , boost::mpl::is_sequence<boost::mpl::_>
        >
      , boost::parameter::optional<tag::held_type>
      , boost::parameter::optional<tag::copyable>
    > class_signature;
}}

namespace boost { namespace python {

    template <
        typename A0
      , typename A1 = boost::parameter::void_
      , typename A2 = boost::parameter::void_
      , typename A3 = boost::parameter::void_
    >
    struct class_
    {
        // Create ArgumentPack
        typedef typename class_signature::BOOST_NESTED_TEMPLATE bind<
            A0, A1, A2, A3
        >::type args;

        // Extract first logical parameter.
        typedef typename boost::parameter::value_type<
            args, tag::class_type
        >::type class_type;

        typedef typename boost::parameter::value_type<
            args, tag::base_list, boost::python::bases<>
        >::type base_list;

        typedef typename boost::parameter::value_type<
            args, tag::held_type, class_type
        >::type held_type;

        typedef typename boost::parameter::value_type<
            args, tag::copyable, void
        >::type copyable;
    };
}}

struct B
{
};

struct D
{
};

typedef boost::python::class_<
    boost::python::class_type<B>
  , boost::python::copyable<boost::noncopyable>
> c1;

typedef boost::python::class_<
    D
  , boost::python::held_type<
#if defined(BOOST_NO_CXX11_SMART_PTR)
        std::auto_ptr<D>
#else
        std::unique_ptr<D>
#endif
    >
  , boost::python::base_list<boost::python::bases<B> >
> c2;

#include <boost/mpl/aux_/test.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>

MPL_TEST_CASE()
{
    BOOST_MPL_ASSERT((boost::is_same<c1::class_type, B>));
    BOOST_MPL_ASSERT((
        boost::is_same<c1::base_list, boost::python::bases<> >
    ));
    BOOST_MPL_ASSERT((boost::is_same<c1::held_type, B>));
    BOOST_MPL_ASSERT((boost::is_same<c1::copyable, boost::noncopyable>));

    BOOST_MPL_ASSERT((boost::is_same<c2::class_type, D>));
    BOOST_MPL_ASSERT((
        boost::is_same<c2::base_list, boost::python::bases<B> >
    ));

#if defined(BOOST_NO_CXX11_SMART_PTR)
    BOOST_MPL_ASSERT((
        boost::is_same<c2::held_type, std::auto_ptr<D> >
    ));
#else
    BOOST_MPL_ASSERT((
        boost::is_same<c2::held_type, std::unique_ptr<D> >
    ));
#endif

    BOOST_MPL_ASSERT((boost::is_same<c2::copyable, void>));
}

