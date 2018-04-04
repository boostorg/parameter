
#include <boost/parameter.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/mpl/and.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_same.hpp>
#include <vector>
#include <utility>

BOOST_PARAMETER_NAME((_graph, graphs) graph)
BOOST_PARAMETER_NAME((_visitor, graphs) visitor)
BOOST_PARAMETER_NAME((_root_vertex, graphs) root_vertex)
BOOST_PARAMETER_NAME((_index_map, graphs) index_map)
BOOST_PARAMETER_NAME((_color_map, graphs) color_map)

struct graph_predicate
{
    template <class T, class Args>
    struct apply
      : boost::mpl::and_<
            boost::is_convertible<
                typename boost::graph_traits<T>::traversal_category
              , boost::incidence_graph_tag
            >
          , boost::is_convertible<
                typename boost::graph_traits<T>::traversal_category
              , boost::vertex_list_graph_tag
            >
        >
    {
    };
};

struct vertex_descriptor_predicate
{
    template <class T, class Args>
    struct apply
      : boost::is_convertible<
            T
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

struct index_map_predicate
{
    template <class T, class Args>
    struct apply
      : boost::mpl::and_<
            boost::is_integral<
                typename boost::property_traits<T>::value_type
            >
          , boost::is_same<
                typename boost::property_traits<T>::key_type
              , typename boost::graph_traits<
                    typename boost::parameter::value_type<
                        Args
                      , graphs::graph
                    >::type
                >::vertex_descriptor
            >
        >
    {
    };
};

struct color_map_predicate
{
    template <class T, class Args>
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

template <class Size, class IndexMap>
boost::iterator_property_map<
    std::vector<boost::default_color_type>::iterator
  , IndexMap
  , boost::default_color_type
  , boost::default_color_type&
>
default_color_map(Size num_vertices, IndexMap const& index_map)
{
    std::vector<boost::default_color_type> colors(num_vertices);
    return boost::iterator_property_map<
        std::vector<boost::default_color_type>::iterator
      , IndexMap
      , boost::default_color_type
      , boost::default_color_type&
    >(colors.begin(), index_map);
}

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

int main()
{
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

    return 0;
}

