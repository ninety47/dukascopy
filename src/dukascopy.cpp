/**
 *
 */


#include <easylzma/compress.h>
#include <easylzma/decompress.h>
#include <ninety47/dukascopy.h>
#include <ninety47/defs.h>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <cstdlib>
#include <algorithm>
#include <vector>

#include <cstdio>

namespace n47 {

namespace pt = boost::posix_time;

namespace { // private

struct datastream {
    std::vector<unsigned char> inData;
    //std::vector<unsigned char>::iterator inLoc;
    size_t inLoc;
    size_t inLen;
    std::vector<unsigned char> outData;
    size_t outCalls;
    size_t inCalls;

    datastream(unsigned char *raw, size_t n) {
        inData.assign(raw, raw+n);
        //inLoc = inData.begin();
        inLoc = 0;
        inLen = inData.size();
        outCalls = 0;
        inCalls = 0;
    }
};


size_t outputCallback(void *ctx, const void *buf, size_t size) {
    std::vector<unsigned char>::size_type old_size;
    datastream *ds = (datastream*) ctx;
    assert(ds != 0);
    unsigned char *buffer = const_cast<unsigned char*>(
            static_cast<const unsigned char*>(buf));
    size_t index;
    ds->outCalls++;
    if (size > 0) {
        ds->outData.reserve( ds->outData.size() + size );
        for (index = 0; index < size; index++) {
            ds->outData.push_back( *(buffer + index));
        }
    }

    return size;
}


int inputCallback(void *ctx, void *buf, size_t *size) {
    size_t rd = 0;
    size_t index = 0;
    datastream *ds = (datastream*) ctx;
    assert(ds != 0);
    unsigned char *buffer = static_cast<unsigned char*>(buf);
    rd = ds->inLen < *size ? ds->inLen : *size;

    if ( rd > 0 ) {
        for (index = 0; index < rd; index++) {
            *(buffer + index) = ds->inData[ds->inLoc];
            ds->inLoc++;
        }
        ds->inLen -= rd;
    }

    *size = rd;

    return 0;
}


unsigned char *decompress(unsigned char *inBuffer, size_t inSize, int *status, size_t *outSize) {
    unsigned char *outBuffer = 0;
    elzma_file_format format = ELZMA_lzma;

    elzma_decompress_handle handle;
    handle = elzma_decompress_alloc();

    if (handle == 0) {
        *status = -1;
    } else {    // decompression...
        datastream ds(inBuffer, inSize);
        *status = elzma_decompress_run(
            handle,
            inputCallback, (void *) &ds,
            outputCallback, (void *) &ds,
            format
        );
        if (*status == ELZMA_E_OK) {
            *outSize = ds.outData.size();
            //outBuffer = &(ds.outData);
            outBuffer = new unsigned char[ ds.outData.size() ];
            std::copy(ds.outData.begin(), ds.outData.end(), outBuffer);
        }
        elzma_decompress_free(&handle);
    }

    return outBuffer;
}

} // private


tick *tickFromBuffer(unsigned char *buffer, pt::ptime epoch, float digits, size_t offset){
    bytesTo<unsigned int, n47::BigEndian> bytesTo_unsigned;
    bytesTo<float, n47::BigEndian> bytesTo_float;

    unsigned int ts = bytesTo_unsigned( buffer + offset );
    pt::time_duration ms = pt::millisec(ts);
    unsigned int ofs = offset + sizeof(unsigned int);
    float ask = bytesTo_unsigned(buffer + ofs) * digits;
    ofs += sizeof(int);
    float bid = bytesTo_unsigned(buffer + ofs) * digits;
    ofs += sizeof(int);
    float askv = bytesTo_float(buffer + ofs);
    ofs += sizeof(int);
    float bidv = bytesTo_float(buffer + ofs);

    return new tick(epoch, ms, ask, bid, askv, bidv);
}


tick_data* read_bin(unsigned char *buffer, size_t buffer_size, pt::ptime epoch, float point_value) {
    std::vector<tick*> *data = new std::vector<tick*>();
    std::vector<tick*>::iterator iter;

    std::size_t offset = 0;

    while ( offset < buffer_size ) {
        data->push_back( tickFromBuffer(buffer, epoch, point_value, offset) );
        offset += ROW_SIZE;
    }

    return data;
}


tick_data* read_bi5(unsigned char *lzma_buffer, size_t lzma_buffer_size, pt::ptime epoch, float point_value, size_t *buffer_size) {
    //unsigned char *buffer = 0;
    size_t outSize = 0;

    // decompress
    int status;
    unsigned char *buffer = decompress(lzma_buffer, lzma_buffer_size, &status, &outSize);

    //printf("ELZMA_E_OK = %d. Returned with status = %d buffer = %d\n", ELZMA_E_OK, status, buffer);

    if (status != ELZMA_E_OK) {
        *buffer_size = 0;
        return 0;
    } else {
        *buffer_size = outSize;
        // convert to tick data (with read_bin).
        return read_bin(buffer, outSize, epoch, point_value);
    }
}


} // namespace n47

