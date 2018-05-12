#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "mem_abs.h"
#include "interpreter.h"
#include "interrupt.h"

//naglowki poszczegolnych opcodow 
void F_ADD1(void);
//...

//wzorce opcodow 
#define ID_ADD_R1_R2            0x01
//...


void doInstr(CodeType T){
    switch((T & 0xF000)>>12){   //wy³uskanie w³aœciwego kodu operacji
        case ID_ADD_R1_R2:       //wywolac instrukcje ADD R1,R2
            F_ADD1();
            break;
        //...
        default:
            printf("Wykryto nieznana instrukcje (PC=0x%08lx, T=0x%04lx)\r\n", getPC(), T);
            saveCPUState();
            exit(-1);
    }
}
