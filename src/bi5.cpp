/**
 *
 */


#include "../include/bi5.h"
#include "../include/lowkey.hpp"

#include <ctime>
#include <algorithm>
#include <vector>

namespace n47 {

size_t ROW_SIZE = 20;

template <typename T>
void byteSwap(T *p) {
    int i;
    for (i = 0; i < sizeof(T)/2; ++i) {
        std::swap( ((char *) p)[i], ((char *) p)[sizeof(T)-1-i] );
    }
}


template <typename T>
T bytesTo(char *buffer, bool little_endian) {
    T value = 0;
    size_t index;
    const size_t nbits = 8 * sizeof(char);


    if ( little_endian ) {
        for (index = sizeof(T); index > 0; index--) {
            value = (value << nbits) + *(buffer + index - 1);
        }
    } else {
        for (index = 0; index < sizeof(T); index++) {
            value = (value << nbits) + *(buffer + index);
        }
    }
    return value;
}


tick *tickFromBuffer(char *buffer, time_t epoch, float digits, size_t offset){
    bool little_endian = true;

    unsigned int ts = bytesTo<unsigned int>( buffer + offset, little_endian );
    unsigned int ofs = offset + sizeof(unsigned int);
    float ask = bytesTo<int>(buffer + ofs, little_endian) * digits;
    ofs += sizeof(int);
    float bid = bytesTo<int>(buffer + ofs, little_endian) * digits;
    ofs += sizeof(int);
    int askv = bytesTo<int>(buffer + ofs, little_endian);
    ofs += sizeof(int);
    int bidv = bytesTo<int>(buffer + ofs, little_endian);

    return new tick(epoch, ts, ask, bid, askv, bidv);
}


tick_data* readbi5(char *buffer, size_t buffer_size, time_t epoch, float point_value) {
    std::vector<tick*> *data = new std::vector<tick*>();
    std::vector<tick*>::iterator iter;

    int timestamp;
    float bid, ask;
    int bidvol, askvol;

    std::size_t offset = 0;

    while ( offset < buffer_size ) {
        data->push_back( tickFromBuffer(buffer, epoch, point_value, offset) );
        offset += ROW_SIZE;
    }

    return data;
}


} // namespace n47

