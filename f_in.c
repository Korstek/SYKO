#include <stdio.h>
#include "types.h"
#include "mem_abs.h"

void F_IN(void){
    DataType Rd=(getOpcode() & 0x01F0)>>4;                       //identyfikacja numeru rejestru arg. 1
    DataType A=((getOpcode() & 0x0600)>>5) | (getOpcode() & 0x000F);                  //identyfikacja numeru rejestru arg. 2

    printf("0x%04X[0x%04X]: IN R%d, R%d\n", getPC(), getOpcode(), A, Rd);

    setRegister(Rd, getRegister(A));

    setPC(getPC()+1);                                       //zwiększenie licznika rozkazów

    addCounter(1);
}