#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "types.h"

//Linux compliance
#ifndef O_BINARY
    #define O_BINARY 0
#endif

#define GEN_REGISTERS_IN_MEMD_OFFSET    0x0000
#define IO_REGISTERS_IN_MEMD_OFFSET     GEN_REGISTERS_IN_MEMD_OFFSET+REGISTERS_COUNT
#define EIO_REGISTERS_IN_MEMD_OFFSET    IO_REGISTERS_IN_MEMD_OFFSET+IO_REGISTERS_COUNT

CodeType    MEMC[MAX_ADDRESS+1];    //obszar pamiêci kodu
DataType    MEMD[MAX_ADDRESS+1];    //obszar pamiêci danych
DataType    *GEN_REG=(DataType*)(&(MEMD[GEN_REGISTERS_IN_MEMD_OFFSET]));    //deklaracja miejsca przechowywania rejestrów ogolnego przeznaczenia
DataType    *IO_REG=(DataType*)(&(MEMD[IO_REGISTERS_IN_MEMD_OFFSET]));      //deklaracja miejsca przechowywania rejestrów IO
DataType    *EIO_REG=(DataType*)(&(MEMD[EIO_REGISTERS_IN_MEMD_OFFSET]));    //deklaracja miejsca przechowywania rejestrów EIO

AddressType   PC;                   //licznik rozkazów
DataType    FLAGS;                  //flagi procesora
CounterType counter;                //licznik wykonanych cykli
AddressType SP;                     //Stack Pointer


void dumpMEMConfiguration(void){
    printf("REGISTERS_COUNT:     0x%08x\n", REGISTERS_COUNT);
    printf("MAX_REGISTER:        0x%08x\n", MAX_REGISTER);
    printf("IO_REGISTERS_COUNT:  0x%08x\n", IO_REGISTERS_COUNT);
    printf("MAX_IO_REGISTERS:    0x%08x\n", MAX_IO_REGISTERS);
    printf("EIO_REGISTERS_COUNT: 0x%08x\n", EIO_REGISTERS_COUNT);
    printf("MAX_EIO_REGISTERS:   0x%08x\n", MAX_EIO_REGISTERS);
    printf("GEN_REGISTERS_IN_MEMD_OFFSET: 0x%08x\n", GEN_REGISTERS_IN_MEMD_OFFSET);
    printf("IO_REGISTERS_IN_MEMD_OFFSET:  0x%08x\n", IO_REGISTERS_IN_MEMD_OFFSET);
    printf("EIO_REGISTERS_IN_MEMD_OFFSET: 0x%08x\n", EIO_REGISTERS_IN_MEMD_OFFSET);
}
void loadPC(char *file){          //£adowanie nowej wartoœci PC
    int file_ptr;
    AddressType tPC;
    file_ptr=open(file, O_RDWR | O_BINARY, 0);
    if(file_ptr<0){
        printf("PC file not found (%s)!\n", file);
        exit(-2);
    }
    lseek(file_ptr, 0, SEEK_SET);
    printf("Read PC file (%s) in %dbytes ", file, read(file_ptr, &tPC, sizeof(AddressType)));
    PC=(tPC & 0xFF00)>>8 | (tPC & 0x00FF)<<8;    //Endianess correction
    PC=0;                           //Niech to na razie zaczyna się od zera
    printf("[PC=0x%04X]\n", PC);
    close(file_ptr);
}
void loadCounter(char *file){          //£adowanie ostatnio zapisanego licznika cykli
    int file_ptr;
    file_ptr=open(file, O_RDWR | O_BINARY, 0);
    if(file_ptr<0){
        printf("Counter file not found (%s)!\n", file);
        exit(-2);
    }
    lseek(file_ptr, 0, SEEK_SET);
    printf("Read counter file (%s) in %dbytes ", file, read(file_ptr, &counter, sizeof(CounterType)));
    counter=0;                      //Niech to na razie zaczyna się od zera
    printf("[counter=0x%08X]\n", counter);
    close(file_ptr);
}
void loadMEMC(char *file){          //£adowanie pamiêci kodu z pliku
    int file_ptr;
    file_ptr=open(file, O_RDWR | O_BINARY, 0);
    if(file_ptr<0){
        printf("MEMC file not found (%s)!\n", file);
        exit(-3);
    }
    lseek(file_ptr, 0, SEEK_SET);
    printf("Read MEMC (%s) file in %dbytes\n", file, read(file_ptr, (void*)MEMC, MAX_ADDRESS+1));
    close(file_ptr);
}
void loadMEMD(char *file){        //£adowanie pamiêci danych z pliku
    int file_ptr;
    file_ptr=open(file, O_RDWR | O_BINARY, 0);
    if(file_ptr<0){
        printf("MEMD file not found (%s)!\n", file);
        exit(-4);
    }
    lseek(file_ptr, 0, SEEK_SET);
    printf("Read MEMD (%s) file in %dbytes\n", file, read(file_ptr, (void*)MEMD, MAX_ADDRESS+1));
    close(file_ptr);
}
void savePC(char *file){        //Zapisz wartoœc PC
    int file_ptr;
    AddressType tPC=PC;
    file_ptr=open(file, O_RDWR | O_BINARY, 0);
    if(file_ptr<0){
        printf("PC cannot open to write (%s)!\n", file);
        exit(-5);
    }
    lseek(file_ptr, 0, SEEK_SET);
    tPC=(PC & 0xFF00)>>8 | (PC & 0x00FF)<<8;		//Endianess correction
    printf("Write PC (%s) file in %dbytes [PC=0x%04X]\n", file, write(file_ptr, &tPC, sizeof(AddressType)), PC);
    close(file_ptr);
}
void saveCounter(char *file){        //Zapisz wartoœc licznika cykli
    int file_ptr;
    file_ptr=open(file, O_RDWR | O_BINARY, 0);
    if(file_ptr<0){
        printf("Cannot open to write (%s)!\n", file);
        exit(-5);
    }
    lseek(file_ptr, 0, SEEK_SET);
    printf("Write counter (%s) file in %dbytes [counter=0x%08X]\n", file, write(file_ptr, &counter, sizeof(CounterType)), counter);
    close(file_ptr);
}
void saveMEMD(char *file){        //zapisz zawartoœæ pamiêci danych do pliku
    int file_ptr;
    file_ptr=open(file, O_RDWR | O_BINARY, 0);
    if(file_ptr<0){
        printf("MEMD cannot open to write (%s)!\n", file);
        exit(-6);
    }
    lseek(file_ptr, 0, SEEK_SET);
    printf("Saved MEMD file (%s) in %dbytes\n", file, write(file_ptr, (void*)MEMD, MAX_ADDRESS));
    close(file_ptr);
}
CodeType getMEMC(AddressType p){
    //CodeType t=(MEMC[p] & 0xFF00)>>8 | (MEMC[p] & 0x00FF)<<8; tymczasowo usuniete - z jakiegoś powodu nastepuje podwujna zamiana
    return MEMC[p];
}
DataType getMEMD(AddressType p){
    return MEMD[p];
}
void setMEMD(AddressType p, DataType d){
    MEMD[p]=d;
}
void setPC(AddressType v){
    PC=v;
}
AddressType getPC(void){
    return PC;
}
CodeType getOpcode(void){
    return getMEMC(PC);
}
CounterType getCounter(void){
    return counter;
}
void addCounter(CounterType n){
    counter+=n;
}
DataType getRegister(int n){
    return GEN_REG[n];
}
void setRegister(int n, DataType v){
    GEN_REG[n]=v;
}
void setFlagsRegister(int b){
    FLAGS = FLAGS | (1<<b);
}
void resetFlagsRegister(int b){
    FLAGS = FLAGS & (~(1<<b));
}
int getFlagsRegister(int b){
    return FLAGS & (1<<b);
}

void setSP(AddressType p){
    AddressType SP = p;
    setMEMD(SPL, (DataType)(SP & 0x00FF));
    setMEMD(SPH, (DataType)(SP >> 8)); // StackPointer w dwoch 8 bitowych wartosciach w Data Memory
    printf("SP: 0x%04X\n", SP);
}

AddressType getSP(void){
    AddressType SP = (getMEMD(SPH) << 8) | getMEMD(SPL); //scalenie StackPointera z dwoch 8 bitowych wartosci w Data Memory
    return SP;
}

void incSP(void){
    AddressType SP = getMEMD(SPL) | (getMEMD(SPH) << 8);
    SP++;
    setMEMD( SPL, (DataType)(SP & 0xFF) );
    setMEMD( SPH, (DataType)(SP >> 8) );
}
void decSP(void){
    AddressType SP = getMEMD(SPL) | (getMEMD(SPH) << 8);
    SP--;
    setMEMD( SPL, (DataType)(SP & 0xFF) );
    setMEMD( SPH, (DataType)(SP >> 8) );
}

