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

namespace  {

struct _lzma_exception {
    _lzma_exception(int errorno, const char* msg)
    : errorno(errorno), msg(msg)
    {}

    _lzma_exception(const _lzma_exception &rhs)
    : errorno(rhs.errorno), msg(rhs.msg)
    {}

    int errorno;
    const char *msg;
};


/**
* \brief    Initialise the LZMA input stream.
*
* This is a local helper function.
*
* \param    stream Pointer to an uninitiliased lzma_stream.
*
*/
void _lzma_init_decoder(lzma_stream *stream) {
    lzma_ret ret = lzma_stream_decoder(
            stream, UINT64_MAX, LZMA_CONCATENATED);

    switch (ret) {
    case LZMA_OK:
        // Do Nothing all is good with the world!
        break;
    case LZMA_MEM_ERROR:
        throw _lzma_exception(LZMA_MEM_ERROR, "Memory allocation failed");
        break;
    case LZMA_FORMAT_ERROR:
        throw _lzma_exception(LZMA_FORMAT_ERROR, "The input is not in the .xz format");
        break;
    case LZMA_OPTIONS_ERROR:
        throw _lzma_exception(LZMA_OPTIONS_ERROR, "Unsupported compression options");
        break;
    case LZMA_DATA_ERROR:
        throw _lzma_exception(LZMA_DATA_ERROR, "Compressed file is corrupt");
        break;
    case LZMA_BUF_ERROR:
        throw _lzma_exception(LZMA_BUF_ERROR, "Compressed file is truncated or otherwise corrupt");
        break;
    default:
        throw _lzma_exception(-1, "Unkown error. Possibly a big with liblzma!");
    }
}



void _lzma_decompress_to_buffer(lzma_stream *stream, FILE *fin, std::vector<uint8_t> *buffer) {
    lzma_ret ret;
    lzma_action action = LZMA_RUN;

    uint8_t inbuffer[BUFSIZ];
    uint8_t outbuffer[BUFSIZ];

    stream->next_in = NULL;
    stream->avail_in = 0;
    stream->next_out = outbuffer;
    stream->avail_out = sizeof(outbuffer);

    while (true) {
        if (stream->avail_in == 0 && !feof(fin)) {
            stream->next_in = inbuffer;
            stream->avail_in = fread(inbuffer, 1, sizeof(inbuffer), fin);

            if (ferror(fin)) {
                throw _lzma_exception(-1, "Error read from input file.");
            }

            if (feof(fin)) {
                action = LZMA_FINISH;
            }
            printf("\tbuffered some data\n");
        }
        ret = lzma_code(stream, action);

        if (stream->avail_out == 0 || ret == LZMA_STREAM_END) {
            size_t write_size = sizeof(outbuffer) - stream->avail_out;
            for (size_t i = 0; i < write_size; i++) {
                buffer->push_back( outbuffer[i] );
            }
            stream->next_out = outbuffer;
            stream->avail_out = sizeof(outbuffer);
        }

        printf("\tdecoded some data\n");
        if (ret == LZMA_STREAM_END) {
            printf("\tStream end.\n");
            break;
        }

        if (ret != LZMA_OK) {
            switch (ret) {
            case LZMA_MEM_ERROR:
                throw _lzma_exception(LZMA_MEM_ERROR, "Memory allocation failed");
                break;
            case LZMA_FORMAT_ERROR:
                throw _lzma_exception(LZMA_FORMAT_ERROR, "The input is not in the .xz format");
                break;
            case LZMA_OPTIONS_ERROR:
                throw _lzma_exception(LZMA_OPTIONS_ERROR, "Unsupported compression options");
                break;
            case LZMA_DATA_ERROR:
                throw _lzma_exception(LZMA_DATA_ERROR, "Compressed file is corrupt");
                break;
            case LZMA_BUF_ERROR:
                throw _lzma_exception(LZMA_BUF_ERROR, "Compressed file is truncated or otherwise corrupt");
                break;
            default:
                throw _lzma_exception(-1, "Unknown error, possibly a bug");
                break;
            }
        }
    } // while (true)
}

_lzma_exception last_error(0,0);


} // private


const char *last_error_msg() {
    return last_error.msg == 0 ? "None" : last_error.msg;
}


int last_error_num() {
    return last_error.errorno;
}


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



tick_data* read_bi5(FILE *lzma_fin, time_t epoch, float point_value) {
    lzma_stream stream = LZMA_STREAM_INIT;
    std::vector<uint8_t> *buffer = new std::vector<uint8_t>();

    if ( lzma_fin == 0 ) {
        last_error.errorno = -1;
        last_error.msg = "File pointer is null";
        return 0;
    }

    try {
        _lzma_init_decoder(&stream);
        _lzma_decompress_to_buffer(&stream, lzma_fin, buffer);
    } catch (_lzma_exception &ex) {
        last_error = ex;
        delete buffer;
        return 0;
    }

    uint8_t *cbuffer = new uint8_t[ buffer->size() ];
    std::copy(buffer->begin(), buffer->end(), cbuffer);
    tick_data *td = read_bin((char*) cbuffer, buffer->size(), epoch, point_value);

    delete buffer;
    delete [] cbuffer;
    return td;
}

} // namespace n47

