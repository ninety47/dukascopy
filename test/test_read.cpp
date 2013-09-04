#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE DukascopyTestRead

#include <iostream>
#include <iomanip>
#include <limits>
#include <cmath>
#include <boost/test/unit_test.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

#include <ninety47/dukascopy.h>
#include <ninety47/dukascopy/defs.h>
#include <ninety47/dukascopy/io.hpp>
#include <ninety47/dukascopy/lzma.h>
#include <ninety47/numeric/float.hpp>

#define STRINGIZE(x) #x
#define STRINGIZE_VALUE_OF(x) STRINGIZE(x)

#ifndef TEST_DATA_PREFIX
#define TEST_DATA_PREFIX ..
#endif

#define LZMA_DATA "/test/data/compressed/GBPJPY/2012/11/03/01h_ticks.bi5"
#define BIN_DATA "/test/data/raw/GBPJPY/2012/11/03/01h_ticks.bin"
#define EPOCH_YEAR 2012
#define EPOCH_MONTH (11 + 1)
#define EPOCH_DAY 3
#define EPOCH_HOUR 1

namespace num = n47::numeric;

namespace pt = boost::posix_time;
namespace gr = boost::gregorian;

size_t num_ticks;

// Test data time-stamp
pt::ptime epoch;

// Expceted ticks
n47::tick *expected;

// Raw binary data.
unsigned char *bin_data;
size_t bin_data_size;

// Compressed version of the raw binary data.
unsigned char *lzma_data;
size_t lzma_data_size;


struct ReadTestConfig {

    ReadTestConfig() {
        num_ticks = 8;

        // To do: embed the binary and LZMA data in the test case.

        // Load the binary data
        std::stringstream ss;
        ss << STRINGIZE_VALUE_OF(TEST_DATA_PREFIX) << BIN_DATA;
        std::string filename = ss.str();
        bin_data = n47::io::loadToBuffer<unsigned char>(filename.c_str(), bin_data_size);

        // Load the LZMA data
        ss.str("");
        ss << STRINGIZE_VALUE_OF(TEST_DATA_PREFIX) << LZMA_DATA;
        filename = ss.str();
        lzma_data = n47::io::loadToBuffer<unsigned char>(filename.c_str(), lzma_data_size);

        // Create "num_ticks" ticks to comapre against.
        expected = new n47::tick[num_ticks];

        size_t index;
        epoch = pt::ptime(
                    gr::date(EPOCH_YEAR, EPOCH_MONTH, EPOCH_DAY),
                    pt::hours(EPOCH_HOUR));
        unsigned int dt[] = {
            3581, 5142, 5202, 5321, 5441, 5503, 5631, 5751
        };

        float pv[][4] = {
            // bid, bidv, adk, askv
            {131.945f, 1.5f, 131.966f, 1.5f},
            {131.943f, 1.5f, 131.964f, 1.5f},
            {131.943f, 1.5f, 131.964f, 2.25f},
            {131.944f, 1.5f, 131.964f, 1.5f},
            {131.944f, 1.5f, 131.964f, 1.5f},
            {131.944f, 1.5f, 131.960f, 1.5f},
            {131.943f, 1.5f, 131.962f, 1.5f},
            {131.943f, 1.5f, 131.965f, 1.5f}
        };

        for (index = 0; index < num_ticks; index++) {
            expected[index].epoch = epoch;
            expected[index].td = pt::millisec(dt[index]);
            expected[index].bid = pv[index][0];
            expected[index].bidv = pv[index][1];
            expected[index].ask = pv[index][2];
            expected[index].askv = pv[index][3];
        }
    }

    ~ReadTestConfig() {
        delete[] expected;
        delete[] bin_data;
        delete[] lzma_data;
    }

};


inline float almostEqual(float a, float b) {
    return num::almostEqual(a, b, 0.0005f);
}

bool ticksEqual(n47::tick &a, n47::tick &b) {
    return a.epoch == b.epoch && a.td == b.td &&
           almostEqual(a.ask, b.ask) && almostEqual(a.bid, b.bid) &&
           almostEqual(a.askv, b.askv) && almostEqual(a.bidv, b.bidv);
}


BOOST_GLOBAL_FIXTURE( ReadTestConfig );

BOOST_AUTO_TEST_CASE( test_tickFromBuffer ) {
    size_t offset = 0;
    size_t index;
    n47::tick *tick;

    for (index = 0; index < num_ticks; index++) {
        tick = n47::tickFromBuffer(bin_data, epoch, PV_YEN_PAIR, offset);
        BOOST_CHECK_MESSAGE( ticksEqual(*tick, expected[index]),
                index << ". result: " << tick->str() << " expected: " << expected[index].str());
        offset += n47::ROW_SIZE;
        delete tick;
    }

}


BOOST_AUTO_TEST_CASE( test_read_bin ) {
    size_t index;
    BOOST_REQUIRE_GT(bin_data_size, num_ticks * n47::ROW_SIZE);
    n47::tick_data *ticks = n47::read_bin(bin_data, bin_data_size, epoch, PV_YEN_PAIR);
    n47::tick_data *null_ticks = 0;
    BOOST_REQUIRE_NE(ticks, null_ticks);
    BOOST_REQUIRE_NE(ticks->size(), 0);

    for (index = 0; index < num_ticks; index++) {
        BOOST_CHECK_MESSAGE( ticksEqual(*(ticks->at(index)), expected[index]),
                index << ". result: " << ticks->at(index)->str() << " expected: " << expected[index].str());
    }

    delete ticks;
}


BOOST_AUTO_TEST_CASE( test_read_bi5 ) {
    size_t index;
    size_t bytes_read;
    BOOST_REQUIRE_GT(lzma_data_size, num_ticks * n47::ROW_SIZE);
    n47::tick_data *ticks = n47::read_bi5(lzma_data, lzma_data_size, epoch, PV_YEN_PAIR, bytes_read);
    n47::tick_data *null_ticks = 0;
    BOOST_REQUIRE_NE(ticks, null_ticks);
    BOOST_REQUIRE_NE(ticks->size(), 0);

    for (index = 0; index < num_ticks; index++) {
        BOOST_CHECK_MESSAGE( ticksEqual(*(ticks->at(index)), expected[index]),
                index << ". result: " << ticks->at(index)->str() << " expected: " << expected[index].str());
    }

    delete ticks;
}


BOOST_AUTO_TEST_CASE( test_lzma_bufferIsLZMA ) {

    BOOST_CHECK_MESSAGE(
        !n47::lzma::bufferIsLZMA(bin_data, bin_data_size),
        "bin_data should not be identified as LZMA compressed data!"
    );
    BOOST_CHECK_MESSAGE(
        n47::lzma::bufferIsLZMA(lzma_data, lzma_data_size),
        "lzma_data should be identified as LZMA compressed data!"
    );
}


BOOST_AUTO_TEST_CASE( test_read_with_bin ) {
    size_t index;
    size_t bytes_read;
    std::stringstream ss;
    ss << STRINGIZE_VALUE_OF(TEST_DATA_PREFIX) << BIN_DATA;
    std::string filename = ss.str();
    n47::tick_data *ticks = n47::read(filename.c_str(), epoch, PV_YEN_PAIR, bytes_read);
    n47::tick_data *null_ticks = 0;
    BOOST_REQUIRE_NE(ticks, null_ticks);
    BOOST_REQUIRE_GT(ticks->size(), 0);

    for (index = 0; index < num_ticks; index++) {
        BOOST_CHECK_MESSAGE( ticksEqual(*(ticks->at(index)), expected[index]),
                index << ". result: " << ticks->at(index)->str() << " expected: " << expected[index].str());
    }

    delete ticks;
}


BOOST_AUTO_TEST_CASE( test_read_with_bi5 ) {
    size_t index;
    size_t bytes_read;
    std::stringstream ss;
    ss << STRINGIZE_VALUE_OF(TEST_DATA_PREFIX) << LZMA_DATA;
    std::string filename = ss.str();
    n47::tick_data *ticks = n47::read(filename.c_str(), epoch, PV_YEN_PAIR, bytes_read);
    n47::tick_data *null_ticks = 0;
    BOOST_REQUIRE_NE(ticks, null_ticks);
    BOOST_REQUIRE_GT(ticks->size(), 0);

    for (index = 0; index < num_ticks; index++) {
        BOOST_CHECK_MESSAGE( ticksEqual(*(ticks->at(index)), expected[index]),
                index << ". result: " << ticks->at(index)->str() << " expected: " << expected[index].str());
    }

    delete ticks;
}

