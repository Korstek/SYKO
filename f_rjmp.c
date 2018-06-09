#include <stdio.h>
#include "types.h"
#include "mem_abs.h"

void F_RJMP(void){
    if((getOpcode() & 0x0800)==0) {
        printf("0x%04X[0x%04X]: RJMP PC+%d+1\n", getPC(), getOpcode(), (getOpcode() & 0x0FFF));
        setPC(getPC() + (getOpcode() & 0x0FFF)+1);
    }
    else
    {
        printf("0x%04X[0x%04X]: RJMP PC%d+1\n", getPC(), getOpcode(), -(0x1000-(getOpcode() & 0x0FFF)));
        setPC(getPC()-(0x1000-(getOpcode() & 0x0FFF)) + 1);
    }
    addCounter(1);
}
