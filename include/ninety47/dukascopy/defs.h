#ifndef INCLUDE_NINETY47_DUKASCOPY_DEFS_H_
#define INCLUDE_NINETY47_DUKASCOPY_DEFS_H_
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

#include <cstdlib>
#include "ninety47/dukascopy/loki.hpp"

#ifndef _N47_ISIN
#define _N47_ISIN(x, a, b) a <= x && x <= b
#endif

#define N47_E_OK 0

namespace n47 {

const int ROW_SIZE = ::n47::loki::SizeOf<
    TYPELIST_5(unsigned int, int, int, int, int) >::value;

}  // namespace n47

#endif  // INCLUDE_NINETY47_DUKASCOPY_DEFS_H_
