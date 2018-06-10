#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "mem_abs.h"
#include "interpreter.h"
#include "interrupt.h"

//naglowki poszczegolnych opcodow
void F_RJMP(void);
void F_IN(void);
void F_RET(void);
void F_CBI(void);
void F_SBI(void);
//...

//wzorce opcodow
#define ID_IN                   0x0B	//OUT: 1011 1xxx xxxx xxxx, IN: 1011 0xxx xxxx xxxx
#define ID_RJMP                 0x0C
#define ID_RET_OR_CBI_OR_SBI    0x09    //RET: 1001 0101 0000 1000, CBI: 1001 1000 xxxx xxxx, SBI: 1001 1010 xxxx xxxx
//...


void doInstr(CodeType T){
    switch((T & 0xF000)>>12){   //wy�uskanie w�a�ciwego kodu operacji
        case ID_RJMP:           //wywolac instrukcje RJMP
            F_RJMP();
            break;
        case ID_RET_OR_CBI_OR_SBI:
            if((T & 0x0FFF)==0x0508)
                F_RET();
            else
                if((T & 0x0F00) >> 8 == 0x0A)
                    F_SBI();
                else
                    F_CBI();
            break;
        case ID_IN:        //wywolac instrukcje OUT lub IN
            F_IN();
            break;

        //...
        default:
            printf("Wykryto nieznana instrukcje (PC=0x%08lx, T=0x%04lX)\r\n", getPC(), T);
            saveCPUState();
            exit(-1);
    }
}
