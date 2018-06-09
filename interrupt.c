#include <stdio.h>
#include "gpio.h"
#include "types.h"
#include "mem_abs.h"
#include "interpreter.h"

void initInterrupt(void){
    //....
}

long int_generate=-1;
void set_intterrupt(long int_gen){
    int_generate=int_gen;
}

void checkInterrupt(long counter){
    if(getCounter()==int_generate){
		printf("Start INT!!!(T=0x%08lx)\r\n", int_generate);
	}

	if(((getMEMD(0x4C) & 0b10000000)>>7)==1) //SPIE0 - sprawdzamy, czy przerwania sa odblokowane
	{
        if(((getMEMD(0x4D) & 0b10000000)>>7)==1) //SPIF0 - sprawdzamy, czy SPI wystawilo flage
        {
            CodeType T;
            printf("Start INT for SPI!!!\n");
            setPC(0x0001);
            T=getOpcode();
            doInstr(T);
            T=getOpcode();
            doInstr(T);
            setMEMD(0x4D,(getMEMD(0x4D)&0x7F));
            set_ss(0);
            T=getOpcode();
            doInstr(T);
            printf("End of INT for SPI!!!\n");
        }
	}
}
