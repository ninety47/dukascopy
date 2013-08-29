
#include <easylzma/compress.h>
#include <easylzma/decompress.h>

#include <vector>
#include <algorithm>
#include <cassert>

#include "ninety47/dukascopy/lzma.h"


namespace n47 {
namespace lzma {

size_t outputCallback(void *ctx, const void *buf, size_t size) {
    std::vector<unsigned char>::size_type old_size;
    datastream *ds = (datastream*) ctx;
    assert(ds != 0);
    unsigned char *buffer = const_cast<unsigned char*>(
            static_cast<const unsigned char*>(buf));
    size_t index;
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
            outBuffer = new unsigned char[ ds.outData.size() ];
            std::copy(ds.outData.begin(), ds.outData.end(), outBuffer);
        }
        elzma_decompress_free(&handle);
    }

    return outBuffer;
}


} // namespace n47::lzma
} // namespace n47


