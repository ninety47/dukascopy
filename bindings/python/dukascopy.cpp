

#include <ninety47/dukascopy.h>
#include <boost/python.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <datetime.h>

using namespace boost::python;

namespace pt = boost::posix_time;

BOOST_PYTHON_MODULE(dukascopy) {

    def("load_bin", load_bin);
    def("load_bi5", load_bi5);

    class_<n47::tick>("tick", init<pt::ptime, pt::time_duration, float, float, float, float>)
        .def_readwrite("ep

}

