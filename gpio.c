#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "mem_abs.h"

DataType DDRB_REGISTER;

void gpio_init()
{
    // /* rozwiązanie awaryjne - należy usunąć przed oddaniem
    setMEMD(0x24,0x2C);
    setMEMD(0x25,0x00);
    // */
    DDRB_REGISTER=getMEMD(0x24);

    int DDRB5=(DDRB_REGISTER & 0b00100000)>>5;
    int DDRB4=(DDRB_REGISTER & 0b00010000)>>4;
    int DDRB3=(DDRB_REGISTER & 0b00001000)>>3;
    int DDRB2=(DDRB_REGISTER & 0b00000100)>>2;

    if(DDRB2==1 && DDRB3==1 && DDRB4==0 && DDRB5==1)
        printf("GPIO dziala poprawnie\n");
    else
        printf("GPIO nie jest poprawnie skonfigurowane\n");

}


int get_miso()
{
    return (getMEMD(0x25) & 0b00010000)>>4;
}

void set_mosi(int pin)
{
    if(pin==1)
        setMEMD(0x25,((getMEMD(0x25)&0xF7)|(0x01<<3)));
    else
        setMEMD(0x25,(getMEMD(0x25)&0xF7));
}

void set_sck(int pin)
{
    if(pin==1)
        setMEMD(0x25,((getMEMD(0x25)&0xDF)|(0x01<<5)));
    else
        setMEMD(0x25,(getMEMD(0x25)&0xDF));

}

void set_ss(int pin)
{
    if(pin==1)
        setMEMD(0x25,((getMEMD(0x25)&0xFB)|(0x01<<2)));
    else
        setMEMD(0x25,(getMEMD(0x25)&0xFB));
}
