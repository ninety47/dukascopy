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


#include <ninety47/dukascopy.h>
#include <cstdio>


int main(void) {
    n47::bytesTo<unsigned int, n47::LittleEndian> bytesTo_ui_little;
    n47::bytesTo<unsigned int, n47::BigEndian> bytesTo_ui_big;

    unsigned char le[4] = { 0x00, 0x00, 0x00, 0x01 };
    unsigned char be[4] = { 0x01, 0x00, 0x00, 0x00 };
    unsigned int target = 16777216;
    unsigned int target_error = 1;
    unsigned int result;

    printf("target: %d\n",  target);
    result = bytesTo_ui_little(le);
    printf("bytesTo_ui_little(le) => %d\n", result);
    if (result != target) return 2;

    result = bytesTo_ui_little(be);
    printf("bytesTo_ui_little(be) => %d\n", result);
    if (result != target_error) return 2;

    result = bytesTo_ui_big(be);
    printf("bytesTo_ui_big(be) => %d\n", result);
    if (result != target) return 2;

    result = bytesTo_ui_big(le);
    printf("bytesTo_ui_big(le) => %d\n", result);
    if (result != target_error) return 2;

    return 0;
}
