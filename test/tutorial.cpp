// Copyright David Abrahams 2005.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/parameter/name.hpp>

namespace graphs {

    BOOST_PARAMETER_NAME(graph)    // Note: no semicolon
    BOOST_PARAMETER_NAME(visitor)
    BOOST_PARAMETER_NAME(root_vertex)
    BOOST_PARAMETER_NAME(index_map)
    BOOST_PARAMETER_NAME(color_map)
}

#include <boost/detail/lightweight_test.hpp>
#include <cstring>

namespace graphs { namespace core {

    template <class ArgumentPack>
    void depth_first_search(ArgumentPack const& args)
    {
        BOOST_TEST(false == args[_color_map]);
        BOOST_TEST('G' == args[_graph]);
        BOOST_TEST(!strcmp("hello, world", args[_index_map]));
        BOOST_TEST(3.5 == args[_root_vertex]);
        BOOST_TEST(2 == args[_visitor]);
    }
}} // graphs::core

int main()
{
    using namespace graphs;

    core::depth_first_search((
        _graph = 'G', _visitor = 2, _root_vertex = 3.5
      , _index_map = "hello, world", _color_map = false
    ));
    return boost::report_errors();
}
