#include <stdio.h>
#include "types.h"

#include "mem_abs.h"

void F_OUT(void){
    DataType Rr=(getOpcode() & 0x01F0)>>4;                             //identyfikacja numeru rejestru
    DataType A=((getOpcode() & 0x0600)>>5) | (getOpcode() & 0x000F); //identyfikacja miejsca w I/O

    printf("0x%04X[0x%04X]: OUT I/O(0x%x), R%d\n", getPC(), getOpcode(), A, Rr);

    setRegister(A, getRegister(Rr));

    setPC(getPC()+1);                                                  //zwiększenie licznika rozkazów

    addCounter(1);
}