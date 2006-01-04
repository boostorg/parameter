// Copyright Daniel Wallin 2005. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <boost/python.hpp>
#include <boost/parameter/python/general.hpp>

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/count.hpp>
#include <boost/accumulators/statistics/min.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/sum.hpp>
#include <boost/accumulators/statistics/weighted_mean.hpp>
#include <boost/accumulators/statistics/moment.hpp>
#include <boost/accumulators/statistics/order.hpp>
#include <boost/accumulators/statistics/order_variate.hpp>
#include <boost/accumulators/statistics/with_error.hpp>
#include <boost/accumulators/framework/parameters/weights.hpp>
#include <boost/accumulators/statistics/variates/covariate.hpp>

using namespace boost;
using namespace boost::accumulators;

typedef accumulator_set<
    double
  , stats<tag::order_variate<int, tag::covariate1> >
> accumulator_type;

template<typename Range>
python::list listify_range(Range const &range)
{
    python::list l;
    typedef typename boost::range_result_iterator<Range>::type iterator;
    iterator begin = range.begin();
    iterator end = range.end();

    for(; begin != end; ++begin)
        l.append(*begin);

    return l;
}

python::list listify_covariate1(accumulator_type const& acc)
{
    return listify_range(order_variate<int, tag::covariate1>(acc));
}

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

    class_<accumulator_type>("accumulator_set", no_init)
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
              , mpl::vector3<void, double, int>
            >()
        );

    def("covariate1", listify_covariate1);
}

