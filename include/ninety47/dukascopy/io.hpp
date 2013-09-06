#ifndef INCLUDE_NINETY47_DUKASCOPY_IO_HPP_
#define INCLUDE_NINETY47_DUKASCOPY_IO_HPP_
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
