#include <stdio.h>
#include "types.h"
#include "mem_abs.h"

void F_RJMP(void){
    printf("0x%04X[0x%04X]: RJMP PC+0x%04X+1\n", getPC(), getOpcode(), getOpcode() & 0x0FFF);

    if((getOpcode() & 0x0800)==0) {
        setPC(getPC() + (getOpcode() & 0x0FFF) + 1);
        printf("PC: 0x%04X\n", getPC());
    }
    else
        setPC(getPC()-(0x1000-(getOpcode() & 0x0FFF)) + 1);
        printf("PC:0x%04X\n", getPC());
    addCounter(1);
}
