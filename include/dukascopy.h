
#ifndef _ninety47_libbi5_header_109823as0920fs8_
#define _ninety47_libbi5_header_109823as0920fs8_

#include <vector>
#include <ctime>

#include "../include/loki.hpp"

namespace n47 {

#define PV_YEN_PAIR 0.001
#define PV_DOLLAR_PAIR 0.00001


const int ROW_SIZE = ::n47::loki::SizeOf< TYPELIST_5(unsigned int, int, int, int, int) >::value;

struct tick;

typedef std::vector<tick*> tick_data;

typedef std::vector<tick*>::iterator tick_data_iterator;

struct tick {

    tick(time_t epoch_, unsigned int tts, float a, float b, float av, float bv)
    : epoch(epoch_), ts(tts), ask(a), bid(b), askv(av), bidv(bv)
    {}

    tick(const tick& rhs) {
        epoch = rhs.epoch;
        ts = rhs.ts;
        ask =  rhs.ask;
        bid = rhs.bid;
        askv = rhs.askv;
        bidv = rhs.bidv;
    }

    time_t epoch;
    unsigned int ts;
    float ask, bid;
    float askv, bidv;
};


struct BigEndian {};
struct LittleEndian {};
template <typename T, class endian>
struct bytesTo {
    T operator()(char *buffer);
};

template <typename T>
struct bytesTo<T, BigEndian>{
    T operator()(char *buffer) {
        T value;
        size_t index;
        for (index = sizeof(T); index > 0; index--) {
            ((char*) &value)[ sizeof(T) - index ]  =  *(buffer + index - 1);
        }
        return value;
    }
};

template <typename U>
struct bytesTo<U, LittleEndian>{
    U operator()(char *buffer) {
        U value;
        size_t index;
        for (index = 0; index < sizeof(U); index++) {
            ((char*) &value)[ index ] = *(buffer + index);
        }
        return value;
    }
};


tick* tickFromBuffer(char *buffer, time_t epoch, float digits, size_t offset=0);


tick_data* read_bin(char *buffer, size_t buffer_size, time_t epoch, float point_value);


tick_data* read_bi5(char *lzma_filename, time_t epoch, float point_value);

} // namespace n47

#endif
