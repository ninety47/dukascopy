#ifndef _ninety47_duakasopy_lzma_header_included_
#define _ninety47_duakasopy_lzma_header_included_

#include "ninety47/dukascopy/defs.h"

namespace n47 {
namespace lzma {

struct datastream {
    std::vector<unsigned char> inData;
    size_t inLoc;
    size_t inLen;
    std::vector<unsigned char> outData;

    datastream(unsigned char *raw, size_t n) {
        inData.assign(raw, raw+n);
        inLoc = 0;
        inLen = inData.size();
    }
};


size_t outputCallback(void *ctx, const void *buf, size_t size);


int inputCallback(void *ctx, void *buf, size_t *size);


unsigned char *decompress(unsigned char *inBuffer, size_t inSize, int &status, size_t &outSize);


bool bufferIsLZMA(const unsigned char *buffer, size_t size);

} // namespace n47::lzma
} // namespace n47

#endif // _ninety47_duakasopy_lzma_header_included_
