/**
 * Copyright 2013 Michael O'Keeffe.
 */

#include "ninety47/dukascopy.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <algorithm>
#include <vector>
#include "ninety47/dukascopy/defs.h"
#include "ninety47/dukascopy/io.hpp"
#include "ninety47/dukascopy/lzma.h"



namespace n47 {

namespace pt = boost::posix_time;


tick *tickFromBuffer(
        unsigned char *buffer, pt::ptime epoch, float digits, size_t offset) {
    bytesTo<unsigned int, n47::BigEndian> bytesTo_unsigned;
    bytesTo<float, n47::BigEndian> bytesTo_float;

    unsigned int ts = bytesTo_unsigned(buffer + offset);
    pt::time_duration ms = pt::millisec(ts);
    unsigned int ofs = offset + sizeof(ts);
    float ask = bytesTo_unsigned(buffer + ofs) * digits;
    ofs += sizeof(ts);
    float bid = bytesTo_unsigned(buffer + ofs) * digits;
    ofs += sizeof(ts);
    float askv = bytesTo_float(buffer + ofs);
    ofs += sizeof(ts);
    float bidv = bytesTo_float(buffer + ofs);

    return new tick(epoch, ms, ask, bid, askv, bidv);
}


tick_data* read_bin(
        unsigned char *buffer, size_t buffer_size, pt::ptime epoch, float point_value) {
    std::vector<tick*> *data = new std::vector<tick*>();
    std::vector<tick*>::iterator iter;

    std::size_t offset = 0;

    while ( offset < buffer_size ) {
        data->push_back(tickFromBuffer(buffer, epoch, point_value, offset));
        offset += ROW_SIZE;
    }

    return data;
}


tick_data* read_bi5(
        unsigned char *lzma_buffer, size_t lzma_buffer_size, pt::ptime epoch,
        float point_value, size_t *bytes_read) {
    tick_data *result = 0;

    // decompress
    int status;
    unsigned char *buffer = n47::lzma::decompress(lzma_buffer,
            lzma_buffer_size, &status, bytes_read);

    if (status != N47_E_OK) {
        bytes_read = 0;
    } else {
        // convert to tick data (with read_bin).
        result = read_bin(buffer, *bytes_read, epoch, point_value);
        delete [] buffer;
    }

    return result;
}


tick_data* read(
        const char *filename, pt::ptime epoch, float point_value, size_t *bytes_read) {
    tick_data *result = 0;
    size_t buffer_size = 0;
    unsigned char *buffer = n47::io::loadToBuffer<unsigned char>(filename, &buffer_size);

    if ( buffer != 0 ) {
        if ( n47::lzma::bufferIsLZMA(buffer, buffer_size) ) {
            result = read_bi5(buffer, buffer_size, epoch, point_value, bytes_read);
            // Reading in as bi5 failed lets double check its not binary
            // data in the buffer.
            if (result == 0) {
                result = read_bin(buffer, buffer_size, epoch, point_value);
            }
        } else {
            result = read_bin(buffer, buffer_size, epoch, point_value);
            *bytes_read = buffer_size;
        }
        delete [] buffer;

        if (result != 0 && result->size() != (*bytes_read / n47::ROW_SIZE)) {
            delete result;
            result = 0;
        }
    }
    return result;
}

}  // namespace n47
