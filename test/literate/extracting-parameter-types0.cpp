
#include <boost/parameter.hpp>

BOOST_PARAMETER_NAME(name)
BOOST_PARAMETER_NAME(index)

template <typename T>
void noop(T&&)
{
}

template <typename Name, typename Index>
int deduce_arg_types_impl(Name&& name, Index&& index)
{
    noop(std::forward<Name>(name));
    noop(std::forward<Index>(index));
    return index;
}

template <typename ArgumentPack>
int deduce_arg_types(ArgumentPack const& args)
{
    return deduce_arg_types_impl(args[_name], args[_index|42]);
}

#include <boost/core/lightweight_test.hpp>

int main()
{
    int a1 = deduce_arg_types((_name = "foo"));
    int a2 = deduce_arg_types((_name = "foo", _index = 3));
    BOOST_TEST_EQ(a1, 42);
    BOOST_TEST_EQ(a2, 3);
    return boost::report_errors();
}

