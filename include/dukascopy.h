
#ifndef _ninety47_libbi5_header_109823as0920fs8_
#define _ninety47_libbi5_header_109823as0920fs8_

#include <vector>
#include <ctime>

namespace n47 {

struct tick;

typedef std::vector<tick*> tick_data;

typedef std::vector<tick*>::iterator tick_data_iterator;

struct tick {

    tick(time_t epoch_, unsigned int tts, float a, float b, int av, int bv)
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
    int askv, bidv;
};


tick* tickFromBuffer(char *buffer, time_t epoch, float digits, size_t offset=0);


tick_data* read_bin(char *buffer, size_t buffer_size, time_t epoch, float point_value);


tick_data* read_bi5(char *lzma_filename, time_t epoch, float point_value, size_t buffer_size=-1);

} // namespace n47

#endif
