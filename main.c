#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "mem_abs.h"
#include "interpreter.h"
#include "interrupt.h"
#include "spi.h"
#include "gpio.h"

#define FILE_COUNTER            "file_counter.bin"
#define FILE_PC                 "file_pc.bin"
#define FILE_DATA               "file_data.bin"
#define FILE_CODE               "file_code.bin"
#define FILE_OUT                "file_gpio_out.txt"

void saveCPUState(void){
    saveMEMD(FILE_DATA);        //Zapisz zawartoœæ pamiêci danych do pliku
    savePC(FILE_PC);            //Zapisz wartoœc PC
    saveCounter(FILE_COUNTER);  //Zapisz liczbe wykonanych cykli
}
int main(int argc, char *argv[]) {
    long counter=0, max_counter=0, i=0;      //liczba wykonanych instrukcji i zadana liczba instrukcji do wykonania
    long int_gen=-1;
    CodeType T;                         //zmienna pomocnicza - ma przechowywaæ opcode instrukcji

    loadMEMC(FILE_CODE);                //£adowanie pamiêci kodu z pliku
    loadMEMD(FILE_DATA);                //£adowanie pamiêci danych z pliku (w tym rejestrówm)
    loadPC(FILE_PC);                    //£adowanie wartoœci PC
    loadCounter(FILE_COUNTER);          //£adowanie licznika cykli

    dumpMEMConfiguration();

    if(argc>1){	                        //pierwszy parametr wywolania okresla liczbe instrukcji do wykonania
        max_counter=strtoul(argv[1], NULL, 10);
        max_counter+=getCounter();
    }
    if(max_counter==0){
        max_counter=getCounter()+1;     //nie podanie argumentu wywolania lub b³edne jego podanie - ustala wykonanie jednego cyklu
    }
    if(argc>2){                         //drugi parametr wywolania okresla liczbe instrukcji po ktorych ma byc wygenerowane przerwanie
        int_gen=strtoul(argv[2], NULL, 10);
    }
    if(int_gen==0){
        int_gen=-1;                     //nie podanie argumentu wywolania lub b³edne jego podanie - ustala wykonanie jednej instrukcji
    }
    if(int_gen>0)
        set_intterrupt(int_gen);        //zapamietaj kiedy wywolac przerwanie

    int spi_state[3][max_counter];

    gpio_init();
    spi_init();

    for(;;){
        gpio();
        spi();

        spi_state[0][getCounter()]=get_sck();  //zbieranie interesujacych nas parametrow do jednej tabeli
        spi_state[1][getCounter()]=get_shift_register();
        spi_state[2][getCounter()]=get_ss();

        T=getOpcode();                  //T=opcode operacji (w³¹cznie z arg. wbudowanym)
        doInstr(T);                     //wykonaj instrukcje
        checkInterrupt(getCounter());   //sprawdŸ czy trzeba wygenerowac przerwanie

        if(getCounter()>=max_counter){  //czy wykonano zadan¹ liczbê cykli
            saveCPUState();

            FILE *fp;
            int i = 0;

            if ((fp = fopen (FILE_OUT, "w")) == NULL)
                printf("OUT file not found!\n");
            fprintf(fp, "Counter\tSCK\tShift Register\tSS\n");
            for(i=0;i<max_counter;i++)
                fprintf(fp, "%d\t%d\t0x%02x\t\t%d\n",i,spi_state[0][i],spi_state[1][i],spi_state[2][i]);
            fclose (fp);

            return 0;
        }
    }

    printf("Bledne wykonanie emulacji (PC=0x%08lx, T=0x%08lx)\r\n", getPC(), T);
    saveCPUState();                     //!!! - Tu niepowinnismy siê nigdy znaleŸæ
    return -2;
}
