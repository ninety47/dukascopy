#ifndef INCLUDE_NINETY47_DUKASCOPY_DEFS_H_
#define INCLUDE_NINETY47_DUKASCOPY_DEFS_H_

/**
 * Copyright 2013 Michael O'Keeffe.
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
