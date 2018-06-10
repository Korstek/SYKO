#include <stdio.h>
#include "types.h"
#include "mem_abs.h"

void F_CBI(void){
    DataType A = (getOpcode() & 0x0F8) >> 3;
    DataType b = (getOpcode() & 0x07);

    DataType result;
    result=0xFF<<(b+1) | 0xFF>>(8-b);
    setRegister(A+REGISTERS_COUNT,getRegister(A+REGISTERS_COUNT) & result);

    printf("0x%04X[0x%04X]: CBI R%d & 0x%02X\n", getPC(), getOpcode(), A, result);

    setPC(getPC()+1);
    addCounter(1);

}
