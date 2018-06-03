#include <stdio.h>
#include "types.h"
#include "mem_abs.h"

void F_SBRS(void){
    CodeType T = getMEMC(getPC()); // aktualny stan licznika programu
    DataType Rr = (T & 0x01F0) >> 4;
    DataType b = (T & 0x0007);

    if ((getRegister(Rr) >> b) & 0x01) // jezeli bit nr b rejestru Rr jest ustawiony na 1
        setPC(getPC()+2); // zwiekszenie licznika programu o 2
    else
        setPC(getPC()+1); // zwiekszenie licznika programu o 1

    addCounter(1);

}