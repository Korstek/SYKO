#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "mem_abs.h"

#define FILE_GPIO_IN               "file_gpio_in.txt"
#define length                      20

DataType DDRB_REGISTER;
CounterType gpio_in[length];
int div_count, transmission_counter; //okresla ile razy sygnal sie zmienil, czas od rozpoczecia transmisji

/********************************************
*funkcja: inicjalizacja gpio (rejestry,     *
*pliki i zmienne                            *
*argument: brak                             *
*wartosc zwracana: brak                     *
*********************************************/
void gpio_init()
{
    setMEMD(0x24,0x2C);
    setMEMD(0x25,0x04);
    
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
    transmission_counter=0;

    set_ss(0); //transmisja od samego poczatku
}

/********************************************
*funkcja: sluzy do kontroli gpio w czasie   *
*rzeczywistym                               *
*argument: brak                             *
*wartosc zwracana: brak                     *
*********************************************/
void gpio(void)
{
    signal();
}

/********************************************
*funkcja: plik z sygnalem zawiera tylko     *
*momenty jego zmiany, ale jako ze jest to   *
*sygnal 0/1 mozemy go w pelni odtworzyc     *
*(zakladamy, ze na poczotku jest 0)         *
*argument: brak                             *
*wartosc zwracana: brak                     *
*********************************************/
void signal(void)
{
    if(gpio_in[div_count]==transmission_counter)
    {
        div_count++;
        if(div_count%2==0)
            set_miso(0);
        else
            set_miso(1);
    }
    transmission_counter++;
}

/********************************************
*funkcja: odczyt wartosci pinu miso         *
*argument: brak                             *
*wartosc zwracana: stan pinu (1-high, 0-low)*
*********************************************/
int get_miso()
{
    return (getMEMD(0x25) & 0b00010000)>>4;
}

/********************************************
*funkcja: ustawianie wartosci pinu miso     *
*argument: nowy stan pinu (1-high, 0-low)   *
*wartosc zwracana: brak                     *
*********************************************/
void set_miso(int pin)
{
    if(pin==1)
        setMEMD(0x25,((getMEMD(0x25)&0xEF)|(0x01<<4)));
    else
        setMEMD(0x25,(getMEMD(0x25)&0xEF));
}

/********************************************
*funkcja: ustawianie wartosci pinu mosi     *
*argument: nowy stan pinu (1-high, 0-low)   *
*wartosc zwracana: brak                     *
*********************************************/
void set_mosi(int pin)
{
    if(pin==1)
        setMEMD(0x25,((getMEMD(0x25)&0xF7)|(0x01<<3)));
    else
        setMEMD(0x25,(getMEMD(0x25)&0xF7));
}

/********************************************
*funkcja: odczyt wartosci pinu sck          *
*argument: brak                             *
*wartosc zwracana: stan pinu (1-high, 0-low)*
*********************************************/
int get_sck(void)
{
    return (getMEMD(0x25) & 0b00100000)>>5;

}

/********************************************
*funkcja: ustawianie wartosci pinu sck      *
*(sygnalu synchronizujacego)                *
*argument: nowy stan pinu (1-high, 0-low)   *
*wartosc zwracana: brak                     *
*********************************************/
void set_sck(int pin)
{
    if(pin==1)
        setMEMD(0x25,((getMEMD(0x25)&0xDF)|(0x01<<5)));
    else
        setMEMD(0x25,(getMEMD(0x25)&0xDF));

}

/********************************************
*funkcja: odczyt wartosci pinu ss           *
*argument: brak                             *
*wartosc zwracana: stan pinu (1-high, 0-low)*
*********************************************/
int get_ss(void)
{
    return (getMEMD(0x25) & 0b00000100)>>2;
}

/********************************************
*funkcja: ustawianie wartosci pinu ss       *
*(slave select)                             *
*argument: nowy stan pinu (1-high, 0-low)   *
*wartosc zwracana: brak                     *
*********************************************/
void set_ss(int pin)
{
    if(pin==1)
        setMEMD(0x25,((getMEMD(0x25)&0xFB)|(0x01<<2)));
    else
        setMEMD(0x25,(getMEMD(0x25)&0xFB));
}

/********************************************
*funkcja: odczyt pliku z sygnalem wejsciowym*
*argument: nazwa pliku                      *
*wartosc zwracana: brak                     *
*********************************************/
void loadData(char *file)
{
    FILE *fp;
    CounterType value;
    int i = 0;

    if ((fp = fopen (file, "r")) == NULL)
    printf("GPIO_IN file not found (%s)!\n", file);

    while (!feof (fp) && fscanf (fp, "%lu", &value) && i++ < length ) //pobierane dane w unsigned long
    {
        gpio_in[i-1] = value;
    }
    fclose (fp);
}
