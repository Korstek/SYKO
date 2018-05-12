#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "mem_abs.h"

#define FILE_GPIO_IN               "file_gpio_in.txt"
#define length                      10

DataType DDRB_REGISTER;
CounterType gpio_in[length];
int div_count; //okresla ile razy sygnal sie zmienil

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

    loadData(FILE_GPIO_IN);
    div_count=0;
}

void gpio(void)
{
    signal();
}

void signal(void)
{
    if(gpio_in[div_count]==getCounter())
    {
        div_count++;
        if(div_count%2==0)
            set_miso(0);
        else
            set_miso(1);
    }
}

int get_miso()
{
    return (getMEMD(0x25) & 0b00010000)>>4;
}

void set_miso(int pin)
{
    if(pin==1)
        setMEMD(0x25,((getMEMD(0x25)&0xEF)|(0x01<<4)));
    else
        setMEMD(0x25,(getMEMD(0x25)&0xEF));
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

void loadData(char *file)
{
    FILE *fp;
    CounterType value;
    int i = 0;

    if ((fp = fopen (file, "r")) == NULL)
    printf("GPIO_IN file not found (%s)!\n", file);

    while (!feof (fp) && fscanf (fp, "%lu", &value) && i++ < length )
    {
        gpio_in[i-1] = value;
    }
    fclose (fp);
}
