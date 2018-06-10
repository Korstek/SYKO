#include <stdio.h>
#include "types.h"
#include "mem_abs.h"

void F_SBI(void){
    DataType A = (getOpcode() & 0x0F8) >> 3;
    DataType b = (getOpcode() & 0x07);

    setRegister(A+REGISTERS_COUNT,getRegister(A+REGISTERS_COUNT) | (1<<b));

    printf("0x%04X[0x%04X]: SBI R%d, 1<<%d\n", getPC(), getOpcode(), A, b);

    setPC(getPC()+1);
    addCounter(1);

}
