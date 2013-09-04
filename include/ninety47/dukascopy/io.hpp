#ifndef _ninety47_dukascopy_io_header_included_
#define _ninety47_dukascopy_io_header_included_

#include <fstream>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem.hpp>


namespace n47 {

namespace io {

namespace fs = boost::filesystem;

template <typename T>
T *loadToBuffer(const char *filename, size_t &size) {
    T *buffer = 0;

    fs::path file(filename);
    if ( fs::exists(file) && fs::is_regular(file) ) {
        size = fs::file_size(file);
        buffer = new T[size];
        std::ifstream fin(filename, std::ifstream::binary);
        if (fin) {
            fin.read(reinterpret_cast<char*>(buffer), size);
        }
        fin.close();
    } else {
        size = 0;
    }
    return buffer;
}


} // namespace io
} // namespace n47

#endif
