#ifndef INCLUDE_NINETY47_DUKASCOPY_LZMA_H_
#define INCLUDE_NINETY47_DUKASCOPY_LZMA_H_

/*
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

#include <vector>
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


unsigned char *decompress(
        unsigned char *inBuffer, size_t inSize, int *status, size_t *outSize);


bool bufferIsLZMA(const unsigned char *buffer, size_t size);

}  // namespace lzma
}  // namespace n47

#endif  // INCLUDE_NINETY47_DUKASCOPY_LZMA_H_
