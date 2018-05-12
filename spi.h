#ifndef SPI_H_INCLUDED
#define SPI_H_INCLUDED

void spi_init(void);
void spi(void);

int divider(int time);
void shift_register(unsigned char bit);

#endif // SPI_H_INCLUDED
