// Copyright Daniel Wallin 2005. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <boost/mpl/for_each.hpp>

#include "stats/accumulators.hpp"
#include "stats/statistics/stats.hpp"
#include "stats/statistics/count.hpp"
#include "stats/statistics/min.hpp"
#include "stats/statistics/mean.hpp"
#include "stats/statistics/sum.hpp"
#include "stats/statistics/weighted_mean.hpp"
#include "stats/statistics/moment.hpp"
#include "stats/statistics/order.hpp"
#include "stats/statistics/order_variate.hpp"
#include "stats/statistics/with_error.hpp"
#include "stats/statistics/variates/weights.hpp"
#include "stats/statistics/variates/covariate.hpp"

using namespace boost;
using namespace boost::accumulators;

typedef accumulator_set<
    double
  , stats<tag::order_variate<double, tag::covariate1> > 
> accumulator_type;

template<typename Range>
void output_range(Range const &range)
{
    typedef typename boost::remove_reference<
        typename boost::iterator_reference<
            typename boost::range_result_iterator<Range>::type
        >::type
    >::type output_type;
    std::copy(range.begin(), range.end(), std::ostream_iterator<output_type>(std::cout, " "));
    std::cout.put('\n');
}

void output(accumulator_type const& acc)
{
    output_range(order_variate<double, tag::covariate1>(acc));
}

#include <boost/python.hpp>
#include <boost/parameter/python/general.hpp>

namespace boost { namespace accumulators { namespace tag 
{
  char const* keyword_name(sample*)
  {
      return "sample";
  }

  char const* keyword_name(covariate1*)
  {
      return "covariate1";
  }
}}} // namespace boost::accumulators::tag

namespace boost { namespace accumulators { namespace detail 
{
  char const* keyword_name(cache_size_tag*)
  {
      return "cache_size";
  }
}}} // namespace boost::accumulators::detail
  
BOOST_PYTHON_MODULE(accumulator_set)
{ 
    using namespace boost::python;
    namespace py = boost::parameter::python;

    class_<accumulator_type>("accumulator_set")
        .def(
            py::init<
                mpl::vector1<
                    accumulators::detail::cache_size_tag
                >
              , mpl::vector1<unsigned>
            >()
        )
        .def(
            py::call<
                mpl::vector2<
                    tag::sample
                  , tag::covariate1
                >
              , mpl::vector3<void, double, double>
            >()
        );

    def("output", output);
}

