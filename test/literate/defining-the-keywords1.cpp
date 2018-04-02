
#include <boost/parameter/keyword.hpp>

namespace graphs {

    namespace tag { struct graph; } // keyword tag type

    namespace // unnamed
    {
        // A reference to the keyword object
        boost::parameter::keyword<tag::graph> const& _graph
            = boost::parameter::keyword<tag::graph>::instance;
    }
}

