
#include <boost/parameter/config.hpp>

#if !defined(BOOST_GCC) || BOOST_WORKAROUND(BOOST_GCC, < 40800) || \
    defined(BOOST_PARAMETER_HAS_PERFECT_FORWARDING)
#define LIBS_PARAMETER_TEST_WILL_NOT_ICE
#endif

#include <boost/core/lightweight_test.hpp>
#include <boost/config/pragma_message.hpp>

#if defined LIBS_PARAMETER_TEST_WILL_NOT_ICE

#include <boost/parameter.hpp>

BOOST_PARAMETER_NAME((_graph, graphs) graph)
BOOST_PARAMETER_NAME((_visitor, graphs) visitor)
BOOST_PARAMETER_NAME((_root_vertex, graphs) in(root_vertex))
BOOST_PARAMETER_NAME((_index_map, graphs) in(index_map))
BOOST_PARAMETER_NAME((_color_map, graphs) in_out(color_map))

#include <boost/graph/graph_traits.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>

#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
#include <boost/type_traits/is_convertible.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_same.hpp>
#else
#include <type_traits>
#endif

struct vertex_descriptor_predicate
{
    template <typename T, typename Args>
    struct apply
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
      : boost::is_convertible<
#else
      : boost::mpl::if_<
            std::is_convertible<
#endif
                T
              , typename boost::graph_traits<
                    typename boost::parameter::value_type<
                        Args
                      , graphs::graph
                    >::type
                >::vertex_descriptor
            >
#if !defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
          , boost::mpl::true_
          , boost::mpl::false_
        >
#endif
    {
    };
};

#if !defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
#include <boost/mpl/eval_if.hpp>
#endif

struct graph_predicate
{
    template <typename T, typename Args>
    struct apply
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
      : boost::mpl::if_<
            boost::is_convertible<
#else
      : boost::mpl::eval_if<
            std::is_convertible<
#endif
                typename boost::graph_traits<T>::traversal_category
              , boost::incidence_graph_tag
            >
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
          , boost::is_convertible<
#else
          , boost::mpl::if_<
                std::is_convertible<
#endif
                    typename boost::graph_traits<T>::traversal_category
                  , boost::vertex_list_graph_tag
#if !defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
                >
              , boost::mpl::true_
              , boost::mpl::false_
#endif
            >
          , boost::mpl::false_
        >
    {
    };
};

#include <boost/property_map/property_map.hpp>

#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
#include <boost/type_traits/is_same.hpp>
#endif

struct color_map_predicate
{
    template <typename T, typename Args>
    struct apply
      : boost::is_same<
            typename boost::property_traits<T>::key_type
          , typename boost::graph_traits<
                typename boost::parameter::value_type<
                    Args
                  , graphs::graph
                >::type
            >::vertex_descriptor
        >
    {
    };
};

#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
#include <boost/type_traits/is_integral.hpp>
#endif

struct index_map_predicate
{
    template <typename T, typename Args>
    struct apply
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
      : boost::mpl::if_<
            boost::is_integral<
#else
      : boost::mpl::eval_if<
            std::is_integral<
#endif
                typename boost::property_traits<T>::value_type
            >
#if defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
          , boost::is_same<
#else
          , boost::mpl::if_<
                std::is_same<
#endif
                    typename boost::property_traits<T>::key_type
                  , typename boost::graph_traits<
                        typename boost::parameter::value_type<
                            Args
                          , graphs::graph
                        >::type
                    >::vertex_descriptor
#if !defined(BOOST_PARAMETER_USES_BOOST_VICE_CXX11_TYPE_TRAITS)
                >
              , boost::mpl::true_
              , boost::mpl::false_
#endif
            >
          , boost::mpl::false_
        >
    {
    };
};

#include <boost/graph/properties.hpp>
#include <vector>

template <typename Size, typename IndexMap>
boost::iterator_property_map<
    std::vector<boost::default_color_type>::iterator
  , IndexMap
  , boost::default_color_type
  , boost::default_color_type&
>&
    default_color_map(Size num_vertices, IndexMap const& index_map)
{
    static std::vector<boost::default_color_type> colors(num_vertices);
    static boost::iterator_property_map<
        std::vector<boost::default_color_type>::iterator
      , IndexMap
      , boost::default_color_type
      , boost::default_color_type&
    > m(colors.begin(), index_map);
    return m;
}

#include <boost/graph/depth_first_search.hpp>

BOOST_PARAMETER_FUNCTION((void), depth_first_search, graphs,
    (required
        (graph, *(graph_predicate))
    )
    (optional
        (visitor
          , *  // not easily checkable
          , boost::dfs_visitor<>()
        )
        (root_vertex
          , *(vertex_descriptor_predicate)
          , *vertices(graph).first
        )
        (index_map
          , *(index_map_predicate)
          , get(boost::vertex_index, graph)
        )
        (color_map
          , *(color_map_predicate)
          , default_color_map(num_vertices(graph), index_map)
        )
    )
)
{
}

#include <boost/graph/adjacency_list.hpp>
#include <utility>

BOOST_PRAGMA_MESSAGE("Test should compile.");
#else
BOOST_PRAGMA_MESSAGE("Test not compiled.");
#endif // Compiler won't ICE.

int main()
{
#if defined LIBS_PARAMETER_TEST_WILL_NOT_ICE
    typedef boost::adjacency_list<
        boost::vecS
      , boost::vecS
      , boost::directedS
    > G;
    enum {u, v, w, x, y, z, N};
    typedef std::pair<std::size_t,std::size_t> E;
    E edges[] = {
        E(u, v), E(u, x), E(x, v), E(y, x),
        E(v, y), E(w, y), E(w, z), E(z, z)
    };
    G g(edges, edges + sizeof(edges) / sizeof(E), N);

    ::depth_first_search(g);
    ::depth_first_search(g, _root_vertex = static_cast<std::size_t>(x));
#endif
    return boost::report_errors();
}

