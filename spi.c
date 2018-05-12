#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "mem_abs.h"

DataType SPI_STATUS_REGISTER;
DataType SPI_CONTROL_REGISTER;

int SPIE0;
int SPE0;
int DORD0;
int MSTR0;
int CPOL0;
int CPHA0;
int SPR0[2];

int SPIF0;
int WCOL0;
int SPI2X0;

int period;

void spi_init() //Pierwsze załadowanie wszystkich rejestrów
{
    // /* rozwiązanie awaryjne - należy usunąć przed oddaniem
    setMEMD(0x4C,0xDC);
    setMEMD(0x4D,0x00);
    // */

    SPI_CONTROL_REGISTER=getMEMD(0x4C);
    SPI_STATUS_REGISTER=getMEMD(0x4D);

    SPIE0=(SPI_CONTROL_REGISTER & 0b10000000)>>7;
    SPE0=(SPI_CONTROL_REGISTER & 0b01000000)>>6;
    DORD0=(SPI_CONTROL_REGISTER & 0b00100000)>>5;
    MSTR0=(SPI_CONTROL_REGISTER & 0b00010000)>>4;
    CPOL0=(SPI_CONTROL_REGISTER & 0b00001000)>>3;
    CPHA0=(SPI_CONTROL_REGISTER & 0b00000100)>>2;
    SPR0[1]=(SPI_CONTROL_REGISTER & 0b00000010)>>1;
    SPR0[0]=SPI_CONTROL_REGISTER & 0b0000001;

    SPIF0=(SPI_STATUS_REGISTER & 0b10000000)>>7;
    WCOL0=(SPI_STATUS_REGISTER & 0b01000000)>>6;
    SPI2X0=SPI_STATUS_REGISTER & 0b0000001;

    /* Tym można zprawdzić poprawność odczytu rejestrów:
    printf("SPIE0=%d\n",SPIE0);
    printf("SPE0=%d\n",SPE0);
    printf("DORD0=%d\n",DORD0);
    printf("MSTR0=%d\n",MSTR0);
    printf("CPOL0=%d\n",CPOL0);
    printf("CPHA0=%d\n",CPHA0);
    printf("SPR0[1]=%d\n",SPR0[1]);
    printf("SPR0[0]=%d\n",SPR0[0]);

    printf("SPIF0=%d\n",SPIF0);
    printf("WCOL0=%d\n",WCOL0);
    printf("SPI2X0=%d\n",SPI2X0);
    */

    if(SPI2X0==1)
        if(SPR0[1]==1)
            if(SPR0[0]==1)
                period=64;
            else
                period=32;
        else
            if(SPR0[0]==1)
                period=8;
            else
                period=2;
    else
        if(SPR0[1]==1)
            if(SPR0[0]==1)
                period=128;
            else
                period=64;
        else
            if(SPR0[0]==1)
                period=16;
            else
                period=4;

    if(SPE0==1)
    {
        if(MSTR0==1)
        {
            if(CPOL0==1 && CPHA0==1)
            {
                printf("SPI dziala poprawnie\n");
            }
            else
                printf("SPI jest w trybie nieobslugiwanym (CPOL0 lub CPHA0 jest rozne od 1)\n");
        }
        else
            printf("SPI w trybie slave - tryb nieobslugiwany\n");
    }
    else
        printf("SPI jest wylaczone\n");

}

void spi(void)
{
    if(divider(getCounter())==1)
    {
        shift_register(1); //wstawić wartość odebranego bitu
        printf("shift_register=0x%02X]\n", getMEMD(0x4E));
    }
}

int divider(int time)
{
    if(time%period==0)
        return 1;
    else
        return 0;
}


void shift_register(unsigned char bit)
{
    if(DORD0==1)
        setMEMD(0x4E,(getMEMD(0x4E)>>1) | (0xf0 & bit));
    else
        setMEMD(0x4E,(getMEMD(0x4E)<<1) | (0x01 & bit));
}
