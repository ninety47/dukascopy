
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


unsigned char *decompress(unsigned char *inBuffer, size_t inSize, int &status, size_t &outSize) {
    unsigned char *outBuffer = 0;
    elzma_file_format format = ELZMA_lzma;

    elzma_decompress_handle handle;
    handle = elzma_decompress_alloc();

    if (handle == 0) {
        status = -1;
    } else {    // decompression...
        datastream ds(inBuffer, inSize);
        status = elzma_decompress_run(
            handle,
            inputCallback, (void *) &ds,
            outputCallback, (void *) &ds,
            format
        );
        if (status == ELZMA_E_OK) {
            outSize = ds.outData.size();
            outBuffer = new unsigned char[ ds.outData.size() ];
            std::copy(ds.outData.begin(), ds.outData.end(), outBuffer);
        }
        elzma_decompress_free(&handle);
    }

    return outBuffer;
}


bool bufferIsLZMA(unsigned char properties)
/*
   LZMA detection based upon properties bytes in the header. Extracted
   from the LZMA file format documentation copied into this /docs and from
   this URL:

   http://svn.python.org/projects/external/xz-5.0.3/doc/lzma-file-format.txt
   [Downloaded on the 29 August 2013]

   The Properties field contains three properties. An abbreviation
   is given in parentheses, followed by the value range of the
   property. The field consists of

   1) the number of literal context bits (lc, [0, 8]);
   2) the number of literal position bits (lp, [0, 4]); and
   3) the number of position bits (pb, [0, 4]).

   The properties are encoded using the following formula:

   Properties = (pb * 5 + lp) * 9 + lc

   The following C code illustrates a straightforward way to
   decode the Properties field:

   uint8_t lc, lp, pb;
   uint8_t prop = get_lzma_properties();
   if (prop > (4 * 5 + 4) * 9 + 8)
       return LZMA_PROPERTIES_ERROR;

   pb = prop / (9 * 5);
   prop -= pb * 9 * 5;
   lp = prop / 9;
   lc = prop - lp * 9;

   So I've just replicated the above code with some tweaks
   and extra checks.
*/
{
    static const unsigned char LZMA_PROPERTIES_MAX = 224; // (4 * 5 + 4) * 9 + 8
    unsigned char lc, lp, pb;

    if (properties > LZMA_PROPERTIES_MAX)
        return false;

    pb = properties / (9 * 5);
    properties -= pb * 9 * 5;
    lp = properties / 9;
    lc = properties - lp * 9;

    return _N47_ISIN(pb, 0, 8) && _N47_ISIN(lp, 0, 4) && _N47_ISIN(lc, 0, 4);
}

} // namespace n47::lzma
} // namespace n47


