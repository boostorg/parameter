
#include <boost/parameter.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>

BOOST_PARAMETER_NAME((_graph, graphs) graph)
BOOST_PARAMETER_NAME((_visitor, graphs) visitor)
BOOST_PARAMETER_NAME((_root_vertex, graphs) root_vertex)
BOOST_PARAMETER_NAME((_index_map, graphs) index_map)
BOOST_PARAMETER_NAME((in_out(_color_map), graphs) color_map)

using boost::mpl::_;

// We first need to define a few metafunctions that we use in the
// predicates below.

template <class G>
struct traversal_category
{
    typedef typename boost::graph_traits<G>::traversal_category type;
};

template <class G>
struct vertex_descriptor
{
    typedef typename boost::graph_traits<G>::vertex_descriptor type;
};

template <class G>
struct value_type
{
    typedef typename boost::property_traits<G>::value_type type;
};

template <class G>
struct key_type
{
    typedef typename boost::property_traits<G>::key_type type;
};

template <class Size, class IndexMap>
boost::iterator_property_map<
    boost::default_color_type*
  , IndexMap
  , boost::default_color_type
  , boost::default_color_type&
>
default_color_map(Size num_vertices, IndexMap const& index_map)
{
    std::vector<boost::default_color_type> colors(num_vertices);
    return &colors[0];
}

template <class T>
struct graph_predicate
  : boost::mpl::and_<
        boost::is_convertible<
            typename traversal_category<T>::type
          , boost::incidence_graph_tag
        >
      , boost::is_convertible<
            typename traversal_category<T>::type
          , boost::vertex_list_graph_tag
        >
    >
{
};

template <class T, class G>
struct index_map_predicate
  : boost::mpl::and_<
        boost::is_integral<
            typename value_type<T>::type
        >
      , boost::is_same<
            typename key_type<T>::type
          , typename vertex_descriptor<G>::type
        >
    >
{
};

template <class T, class G>
struct color_map_predicate
  : boost::is_same<
        typename key_type<T>::type
      , typename vertex_descriptor<G>::type
    >
{
};

BOOST_PARAMETER_FUNCTION(
    (void), depth_first_search, graphs
  , (required
        (graph, *(graph_predicate<_>))
    )
    (optional
        (visitor, *, boost::dfs_visitor<>()) // not checkable
        (root_vertex
          , (typename vertex_descriptor<graphs::graph::_>::type)
          , *vertices(graph).first
        )
        (index_map
          , *(index_map_predicate<_,graphs::graph::_>)
          , get(boost::vertex_index, graph)
        )
        (color_map
          , *(color_map_predicate<_,graphs::graph::_>)
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
    typedef std::pair<int, int> E;
    E edges[] = {
        E(u, v), E(u, x), E(x, v), E(y, x),
        E(v, y), E(w, y), E(w, z), E(z, z)
    };
    G g(edges, edges + sizeof(edges) / sizeof(E), N);

    ::depth_first_search(g);
    ::depth_first_search(g, _root_vertex = static_cast<int>(x));

    return 0;
}

