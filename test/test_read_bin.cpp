
#include <ninety47/dukascopy.h>
#include <ninety47/defs.h>

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>

#define STRINGIZE(x) #x
#define STRINGIZE_VALUE_OF(x) STRINGIZE(x)

#ifndef TEST_DATA_PREFIX
#define TEST_DATA_PREFIX ..
#endif

namespace fs = boost::filesystem;
namespace pt = boost::posix_time;
namespace gr = boost::gregorian;


int main(void) {
    char *buffer;
    size_t buffer_size;
    const char *test_data_prefix = STRINGIZE_VALUE_OF(TEST_DATA_PREFIX);
    const char *test_data_suffix = "/test/data/raw/GBPJPY/2012/11/03/01h_ticks.bin";
    int counter;

    char filename[ strlen(test_data_prefix) + strlen(test_data_suffix) + 1 ];
    snprintf(filename, sizeof(filename), "%s%s", test_data_prefix, test_data_suffix);

    fs::path p(filename);
    if ( fs::exists(p) && fs::is_regular(p)  ) {
        buffer_size = fs::file_size(p);
        buffer = new char[ buffer_size ];
    } else {
        std::cout << "Error: couldn't access the data file. |" << filename << "|" <<  std::endl;
        return 2;
    }

    std::ifstream fin;
    fin.open(filename, std::ifstream::binary);
    fin.read(buffer, buffer_size);

    if (fin) {
        std::cout << "Read " << buffer_size << " bytes." << std::endl;
    } else {
        std::cout << "Error: only read " << fin.gcount() << " bytes from the file." << std::endl;
    }
    fin.close();
    pt::ptime epoch(gr::date(2012, 12, 3), pt::hours(1));
    n47::tick_data *data = n47::read_bin(buffer, buffer_size, epoch, PV_YEN_PAIR);
    n47::tick_data_iterator iter;

    for (int i = 12; i < 16; i++)
        std::cout << std::setfill('0') << std::setw(2) << std::hex << (int) buffer[i] << " ";
    std::cout << std::dec << std::endl;

    std::cout << "time, ask, vol, bid, vol" << std::endl;
    counter = 0;
    for (iter = data->begin(); iter != data->end(); iter++) {
        std::cout << ((*iter)->epoch + (*iter)->td) << ", "
                  << (*iter)->bid << ", " << (*iter)->bidv << ", "
                  << (*iter)->ask << ", " << (*iter)->askv << std::endl;
        counter++;
    }
    std::cout << ".end." << std::endl << std::endl
              << "From " << buffer_size << " bytes we read " << counter << " records." << std::endl
              << buffer_size << " / " << n47::ROW_SIZE << " = " << (buffer_size / n47::ROW_SIZE) << std::endl;


    delete[] buffer;

}
