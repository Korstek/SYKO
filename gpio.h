#ifndef GPIO_H_INCLUDED
#define GPIO_H_INCLUDED

int get_miso(void);

void gpio_init(void);

void set_mosi(int pin);
void set_sck(int pin);
void set_ss(int pin);

#endif // GPIO_H_INCLUDED
