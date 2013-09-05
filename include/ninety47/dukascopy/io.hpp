#ifndef INCLUDE_NINETY47_DUKASCOPY_IO_HPP_
#define INCLUDE_NINETY47_DUKASCOPY_IO_HPP_
/**
 * Copyright 2013 Michael O'Keeffe.
 */

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem.hpp>
#include <fstream>

namespace n47 {
namespace io {

namespace fs = boost::filesystem;

template <typename T>
T *loadToBuffer(const char *filename, size_t *size) {
    T *buffer = 0;

    fs::path file(filename);
    if ( fs::exists(file) && fs::is_regular(file) ) {
        *size = fs::file_size(file);
        buffer = new T[*size];
        std::ifstream fin(filename, std::ifstream::binary);
        if (fin) {
            fin.read(reinterpret_cast<char*>(buffer), *size);
        }
        fin.close();
    } else {
        *size = 0;
    }
    return buffer;
}


}  // namespace io
}  // namespace n47

#endif  // INCLUDE_NINETY47_DUKASCOPY_IO_HPP_
