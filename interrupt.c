#include <stdio.h>

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
}
