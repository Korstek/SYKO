#ifndef GPIO_H_INCLUDED
#define GPIO_H_INCLUDED

int get_miso(void);
void set_miso(int pin);

void gpio_init(void);

void gpio(void);

void set_mosi(int pin);

int get_sck(void);
void set_sck(int pin);

int get_ss(void);
void set_ss(int pin);

void loadData(char *file);

void signal(void);

#endif // GPIO_H_INCLUDED
