#ifndef __INTERRUPTS_H__
#define __INTERRUPTS_H__

void initInterrupt(void);
void set_intterrupt(long int_gen);
void checkInterrupt(long counter);

#endif //__INTERRUPTS_H__
