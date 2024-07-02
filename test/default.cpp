#include <boost/parameter/parameters.hpp>
#include <boost/parameter/name.hpp>

#if __cplusplus >= 201103L
  #include <type_traits>
#else // C++03
  #include <boost/mpl/bool.hpp>
  #include <boost/mpl/not.hpp>
  #include <boost/mpl/assert.hpp>
  #include <boost/type_traits/is_same.hpp>
#endif


BOOST_PARAMETER_TEMPLATE_KEYWORD(type_foo_param)
BOOST_PARAMETER_TEMPLATE_KEYWORD(type_bar_param)

class A {};
class B {};
class C {};


#if __cplusplus >= 201103L
    template<class... Args>
    class DefaultParams 
    {
    public:
        typedef typename boost::parameter::parameters<
            boost::parameter::optional<tag::type_foo_param>
        >::bind<Args...>::type args;
  
        typedef typename boost::parameter::value_type<
            args, tag::type_foo_param,  B
        >::type foo_t;
    };
#else // C++03
    template<class Arg0, class Arg1>
    class DefaultParams 
    {
    public:
        typedef typename boost::parameter::parameters<
            boost::parameter::optional<tag::type_foo_param>
        >::bind<Arg0, Arg1>::type args;
  
        typedef typename boost::parameter::value_type<
            args, tag::type_foo_param,  B
        >::type foo_t;
    };
#endif 

void test()
{
    typedef DefaultParams<
        type_bar_param<C>,
        type_foo_param<A>
    > Params;

#if __cplusplus >= 201103L
    static_assert(std::is_same<Params::foo_t, A>::value, "Expected A type");
    static_assert(!std::is_same<Params::foo_t, B>::value, "Failing back to default value is not allowed here");
#else // C++03
    BOOST_MPL_ASSERT(( boost::is_same<Params::foo_t, A> ));
    BOOST_MPL_ASSERT(( boost::mpl::not_< boost::is_same<Params::foo_t, B> > ));
#endif
}
