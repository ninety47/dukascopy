/**
 *
 */


#include <dukascopy.h>

#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <algorithm>
#include <vector>

#include <lzma.h>

namespace n47 {


#define N47_LITTLE_ENDIAN 0
#define N47_BIG_ENDIAN 1

tick *tickFromBuffer(char *buffer, time_t epoch, float digits, size_t offset){
    bytesTo<unsigned int, n47::BigEndian> bytesTo_unsigned;
    bytesTo<float, n47::BigEndian> bytesTo_float;

    unsigned int ts = bytesTo_unsigned( buffer + offset );
    unsigned int ofs = offset + sizeof(unsigned int);
    float ask = bytesTo_unsigned(buffer + ofs) * digits;
    ofs += sizeof(int);
    float bid = bytesTo_unsigned(buffer + ofs) * digits;
    ofs += sizeof(int);
    float askv = bytesTo_float(buffer + ofs);
    ofs += sizeof(int);
    float bidv = bytesTo_float(buffer + ofs);

    return new tick(epoch, ts, ask, bid, askv, bidv);
}


tick_data* read_bin(char *buffer, size_t buffer_size, time_t epoch, float point_value) {
    std::vector<tick*> *data = new std::vector<tick*>();
    std::vector<tick*>::iterator iter;

    std::size_t offset = 0;

    while ( offset < buffer_size ) {
        data->push_back( tickFromBuffer(buffer, epoch, point_value, offset) );
        offset += ROW_SIZE;
    }

    return data;
}




tick_data* read_bi5(char *lzma_filename, time_t epoch, float point_value) {
    lzma_stream strm = LZMA_STREAM_INIT;
    lzma_ret lzret;
    size_t n, index;

    std::vector<char> buffer;
    char *cbuffer;

    lzret = lzma_stream_decoder(&strm, UINT64_MAX, LZMA_CONCATENATED);

    if (lzret != LZMA_OK)
        return 0;

    FILE *fin = fopen(lzma_filename, "rb");

    if (fin == 0)
        return 0;


    lzma_action action = LZMA_RUN;
    uint8_t inbuf[BUFSIZ];
    uint8_t outbuf[BUFSIZ];

    strm.next_in = 0;
    strm.avail_in = 0;
    strm.next_out = outbuf;
    strm.avail_out = sizeof(outbuf);

    while (true) {
        if (strm.avail_in == 0 && !feof(fin)) {
            strm.next_in = inbuf;
            strm.avail_in = fread(inbuf, 1, sizeof(inbuf), fin);
            if (ferror(fin)) {
                return 0;
            }

            if (feof(fin)) {
                action = LZMA_FINISH;
            }
        }

        lzret = lzma_code(&strm, action);
        if (strm.avail_out == 0 || lzret == LZMA_STREAM_END) {
            n = sizeof(outbuf) - strm.avail_out;

            for (index = 0; index < n; index++) {
                buffer.push_back( outbuf[index] );
            }
            strm.next_out = outbuf;
            strm.avail_out = sizeof(outbuf);
        }
        if (lzret == LZMA_STREAM_END) {
            break;
        }
        if (lzret != LZMA_OK) {
            return 0;
        }
    }
    fclose(fin);

    cbuffer = new char[ buffer.size() ];
    std::copy(buffer.begin(), buffer.end(), cbuffer);

    tick_data *td = read_bin(cbuffer, buffer.size(), epoch, point_value);

    return td;
}

} // namespace n47

