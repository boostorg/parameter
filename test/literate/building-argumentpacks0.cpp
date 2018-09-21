
#include <boost/parameter.hpp>
#include <iostream>

BOOST_PARAMETER_NAME(index)

template <typename ArgumentPack>
int print_index(ArgumentPack const& args)
{
    std::cout << "index = " << args[_index] << std::endl;
    return 0;
}

BOOST_PARAMETER_NAME(name)

template <typename ArgumentPack>
int print_name_and_index(ArgumentPack const& args)
{
    std::cout << "name = " << args[_name] << "; ";
    return print_index(args);
}

#include <boost/core/lightweight_test.hpp>
#include <boost/mpl/placeholders.hpp>

#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
#include <boost/type_traits/is_convertible.hpp>
#else
#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <type_traits>
#endif

int main()
{
    int x = print_index(_index = 3);  // prints "index = 3"
    int y = print_name_and_index((_index = 3, _name = "jones"));
    boost::parameter::parameters<
        boost::parameter::required<
            tag::name
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
          , boost::is_convertible<boost::mpl::_,char const*>
#else
          , boost::mpl::if_<
                std::is_convertible<boost::mpl::_,char const*>
              , boost::mpl::true_
              , boost::mpl::false_
            >
#endif
        >
      , boost::parameter::optional<
            tag::index
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
          , boost::is_convertible<boost::mpl::_,int>
#else
          , boost::mpl::if_<
                std::is_convertible<boost::mpl::_,int>
              , boost::mpl::true_
              , boost::mpl::false_
            >
#endif
        >
    > spec;
    char const sam[] = "sam";
    int twelve = 12;
    int z0 = print_name_and_index(spec(sam, twelve));
    int z1 = print_name_and_index(spec(_index=12, _name="sam"));
    BOOST_TEST(!x);
    BOOST_TEST(!y);
    BOOST_TEST(!z0);
    BOOST_TEST(!z1);
    return boost::report_errors();
}

