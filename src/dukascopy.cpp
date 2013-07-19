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



namespace n47 {



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



} // namespace n47

