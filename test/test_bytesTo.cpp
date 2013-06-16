
#include "dukascopy.h"
#include <cstdio>


int main(void) {

    n47::bytesTo<float, n47::LittleEndian> bytesTo_float_little;
    n47::bytesTo<float, n47::BigEndian> bytesTo_float_big;

    char lit[4] = { 0x66, 0x66, 0x86, 0x3F };
    char big[4] = { 0x3F, 0x86, 0x66, 0x66 };
    unsigned int target = 1065772646;

    printf("target: %d\n",  target);

    float result = bytesTo_float_little(big);
    printf("result: %3.3f\n", result);
    result = bytesTo_float_big(big);
    printf("result: %3.3f\n", result);

    float value = 0.0;
    int index;

    for (index = 0; index < sizeof(float); index++) {
        ((char*) &value)[index] = big[4 - 1 - index];
    }
    printf("result: %3.3f\n", value);
    return 0;
}


