/**
Copyright 2013 Michael O'Keeffe (a.k.a. ninety47).

This file is part of ninety47 Dukascopy toolbox.

The "ninety47 Dukascopy toolbox" is free software: you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation, either version 3 of the License,
or any later version.

"ninety47 Dukascopy toolbox" is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
Public License for more details.

You should have received a copy of the GNU General Public License along with
"ninety47 Dukascopy toolbox".  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ninety47/dukascopy/lzma.h"
#include <easylzma/compress.h>
#include <easylzma/decompress.h>
#include <cassert>
#include <algorithm>
#include "ninety47/dukascopy.h"
#include "ninety47/dukascopy/defs.h"

namespace n47 {
namespace lzma {

size_t outputCallback(void *ctx, const void *buf, size_t size) {
    datastream *ds = static_cast<datastream*>(ctx);
    assert(ds != 0);
    unsigned char *buffer = const_cast<unsigned char*>(
            static_cast<const unsigned char*>(buf));
    size_t index;
    if (size > 0) {
        ds->outData.reserve(ds->outData.size() + size);
        for (index = 0; index < size; index++) {
            ds->outData.push_back(*(buffer + index));
        }
    }

    return size;
}


int inputCallback(void *ctx, void *buf, size_t *size) {
    size_t rd = 0;
    size_t index = 0;
    datastream *ds = static_cast<datastream*>(ctx);
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


unsigned char *decompress(
        unsigned char *inBuffer, size_t inSize, int *status, size_t *outSize) {
    unsigned char *outBuffer = 0;
    elzma_file_format format = ELZMA_lzma;

    elzma_decompress_handle handle;
    handle = elzma_decompress_alloc();

    if (handle == 0) {
        *status = -1;
    } else {
        // decompression...
        datastream ds(inBuffer, inSize);
        *status = elzma_decompress_run(
            handle,
            inputCallback, static_cast<void*>(&ds),
            outputCallback, static_cast <void*>(&ds),
            format);

        if (*status == ELZMA_E_OK) {
            *outSize = ds.outData.size();
            outBuffer = new unsigned char[ ds.outData.size() ];
            std::copy(ds.outData.begin(), ds.outData.end(), outBuffer);
        }
        elzma_decompress_free(&handle);
    }

    return outBuffer;
}


bool bufferIsLZMA(const unsigned char *buffer, size_t size) {
/*
   LZMA detection based upon properties byte in the header and the
   uncompressed size. The LZMA file format documentation can be
   found in /docs and from this URL:

   http://svn.python.org/projects/external/xz-5.0.3/doc/lzma-file-format.txt
   [Downloaded on the 29 August 2013]

   The Properties field contains three properties. An abbreviation
   is given in parentheses, followed by the value range of the
   property. The field consists of

   1) the number of literal context bits (lc, [0, 8]);
   2) the number of literal position bits (lp, [0, 4]); and
   3) the number of position bits (pb, [0, 4]).

   The properties are encoded using the following formula:

   properties = (pb * 5 + lp) * 9 + lc

   The uncompressed size is a 64-bit little endian stored unsigned integer.
   I've applied the logic that, if the uncompressed size:

   1) is 0xFFFFFFFFFFFFFFFF (decompressed size is unknown) or
   2) is in [(buffer size) - (header size), 10 * (buffer size) - (header size)]
      where 10 is a guess at the upper limit of LZMA compression performance
      for the Dukascopy binary files.
*/
    // (4 * 5 + 4) * 9 + 8 = 224
    static const unsigned char LZMA_PROPERTIES_MAX = 224;
    n47::bytesTo<uint64_t, n47::LittleEndian> bytesTo_uint64_t;
    unsigned char properties, lc, lp, pb;
    uint64_t uncompressed_size;
    bool is_lzma = false;

    // Check the properties fields..
    properties = *buffer;
    if (properties <= LZMA_PROPERTIES_MAX) {
        pb = properties / (9 * 5);
        properties -= pb * 9 * 5;
        lp = properties / 9;
        lc = properties - lp * 9;
        is_lzma =  _N47_ISIN(pb, 0, 8) && _N47_ISIN(lp, 0, 4) && _N47_ISIN(lc, 0, 4);
    }

    // Check the uncompressed size
    if ( is_lzma ) {
        uncompressed_size = bytesTo_uint64_t(buffer + 5);
        is_lzma &= (uncompressed_size == 0xFFFFFFFFFFFFFFFF) ||
                   (_N47_ISIN(uncompressed_size, static_cast<uint64_t>(size -  13),
                              10 * static_cast<uint64_t>(size -  13)));
    }

    return is_lzma;
}

}  // namespace lzma
}  // namespace n47
