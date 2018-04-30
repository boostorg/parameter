
#include <boost/parameter/config.hpp>

#if !defined(BOOST_GCC) || BOOST_WORKAROUND(BOOST_GCC, < 60000) || \
    defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)
#define LIBS_PARAMETER_TEST_WILL_NOT_ICE
#endif

#include <boost/config/pragma_message.hpp>

#if defined LIBS_PARAMETER_TEST_WILL_NOT_ICE

#include <boost/parameter.hpp>

namespace boost {

    int vertex_index = 0;

    template <typename T = int>
    struct dfs_visitor
    {
    };
}

BOOST_PARAMETER_NAME(graph)
BOOST_PARAMETER_NAME(visitor)
BOOST_PARAMETER_NAME(root_vertex)
BOOST_PARAMETER_NAME(index_map)
BOOST_PARAMETER_NAME(in_out(color_map))

BOOST_PARAMETER_FUNCTION((void), f, tag,
    (required (graph, *))
    (optional
        (visitor,     *, boost::dfs_visitor<>())
        (root_vertex, *, *vertices(graph).first)
        (index_map,   *, get(boost::vertex_index,graph))
        (color_map,   *,
            default_color_map(num_vertices(graph), index_map)
        )
    )
)
{
}

BOOST_PRAGMA_MESSAGE("Test should compile.");
#else
BOOST_PRAGMA_MESSAGE("Test not compiled.");
#endif // Compiler won't ICE.

