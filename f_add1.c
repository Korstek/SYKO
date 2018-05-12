#include <stdio.h>
#include "types.h"
#include "mem_abs.h"

void F_ADD1(void){
    DataType R1=getOpcode() & 0x000F;                        //identyfikacja numeru rejestru arg. 1
    DataType R2=(getOpcode() & 0x00F0)>>4;                  //identyfikacja numeru rejestru arg. 2

    printf("0x%04X[0x%04X]: ADD R%d, R%d\n", getPC(), getOpcode(), R1, R2);

    if((getRegister(R1) + getRegister(R2)) > MAX_DATA_VAL)
        setFlagsRegister(FLAG_C);                           //ustawienie flagi przeniesienia
    else
        resetFlagsRegister(FLAG_C);                         //wykasowanie flagi przeniesienia
        
    setRegister(R1, (getRegister(R1) + getRegister(R2)) % MAX_DATA_VAL);    //w³aœciwe obliczenie

    setPC(getPC()+1);                                       //zwiêkszenie licznika rozkazów

    addCounter(1);
}
