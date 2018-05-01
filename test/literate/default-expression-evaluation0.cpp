
#include <boost/parameter/config.hpp>

#if !defined(LIBS_PARAMETER_TEST_WILL_NOT_ICE)
#if !defined(BOOST_GCC) || (defined(__MINGW32__) && (1 == __MINGW32__)) || \
    BOOST_WORKAROUND(BOOST_GCC, < 40800) || \
    defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)
#define LIBS_PARAMETER_TEST_WILL_NOT_ICE
#endif
#endif

#include <boost/core/lightweight_test.hpp>
#include <boost/config/pragma_message.hpp>

#if defined(LIBS_PARAMETER_TEST_WILL_NOT_ICE)

#include <boost/parameter.hpp>
#include <iostream>

BOOST_PARAMETER_NAME(graph)
BOOST_PARAMETER_NAME(visitor)
BOOST_PARAMETER_NAME(root_vertex)
BOOST_PARAMETER_NAME(index_map)
BOOST_PARAMETER_NAME(color_map)

#include <boost/graph/depth_first_search.hpp> // for dfs_visitor

BOOST_PARAMETER_FUNCTION((void), depth_first_search, tag,
    (required
        (graph, *)
        (visitor, *)
        (root_vertex, *)
        (index_map, *)
        (color_map, *)
    )
)
{
    std::cout << "graph=" << graph << std::endl;
    std::cout << "visitor=" << visitor << std::endl;
    std::cout << "root_vertex=" << root_vertex << std::endl;
    std::cout << "index_map=" << index_map << std::endl;
    std::cout << "color_map=" << color_map << std::endl;
}

BOOST_PRAGMA_MESSAGE("Test should compile.");
#else
BOOST_PRAGMA_MESSAGE("Test not compiled.");
#endif // Compiler won't ICE.

int main()
{
#if defined(LIBS_PARAMETER_TEST_WILL_NOT_ICE)
    depth_first_search(1, 2, 3, 4, 5);
    depth_first_search(
        "1", '2', _color_map = '5'
      , _index_map = "4", _root_vertex = "3"
    );
#endif
    return boost::report_errors();
}

