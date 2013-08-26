
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


