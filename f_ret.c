#include <stdio.h>
#include "types.h"
#include "mem_abs.h"

void F_RET(void){
    DataType A = getMEMD(getSP()); // pobranie starszej czesci PC z stosu
    incSP(); //zmniejszenie rozmiaru stosu
    AddressType newPC = (A << 8); //przypisanie starszej czesci slowa

    A = getMEMD(getSP()); // pobranie mlodszej czesci PC z stosu
    incSP(); //zmniejszenie rozmiaru stosu
    newPC = newPC | A; //przypisanie mlodszej czesci slowa

    printf("0x%04X[0x%04X]: RET 0x%04X\n", getPC(), getOpcode(), newPC);

    setPC(newPC);
    addCounter(1);
}
