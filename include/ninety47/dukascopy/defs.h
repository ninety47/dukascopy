#ifndef _ninety47_dukascopy_defs_header_included_
#define _ninety47_dukascopy_defs_header_included_


#include "ninety47/dukascopy/loki.hpp"

#ifndef _N47_ISIN
#define _N47_ISIN(x, a, b) a <= x && x <= b
#endif

#define N47_E_OK 0

namespace n47 {

const int ROW_SIZE = ::n47::loki::SizeOf< TYPELIST_5(unsigned int, int, int, int, int) >::value;

} // namespace n47

#endif // _ninety47_dukascopy_defs_header_included_
