syko.exe: 
	gcc -c mem_abs.c -o mem_abs.o
	gcc -c f_add1.c -o f_add1.o
	gcc -c interrupt.c -o interrupt.o
	gcc -c interpreter.c -o interpreter.o
	gcc -c spi.c -o spi.o
	gcc -c gpio.c -o gpio.o
	gcc -c main.c -o main.o
	gcc mem_abs.o f_add1.o interrupt.o interpreter.o main.o spi.o gpio.o -o syko.exe

clean:
	rm -f mem_abs.o
	rm -f f_add1.o
	rm -f interrupt.o
	rm -f interpreter.o
	rm -f main.o
	rm -f spi.o
	rm -f gpio.o
	rm -f syko.exe
