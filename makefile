syko.exe: 
	gcc -c mem_abs.c -o mem_abs.o
	gcc -c f_add1.c -o f_add1.o
	gcc -c f_in.c -o f_in.o
	gcc -c f_out.c -o f_out.o
	gcc -c f_ret.c -o f_ret.o
	gcc -c f_rjmp.c -o f_rjmp.o
	gcc -c f_sbi.c -o f_sbi.o
	gcc -c f_sbrs.c -o f_sbrs.o
	gcc -c interrupt.c -o interrupt.o
	gcc -c interpreter.c -o interpreter.o
	gcc -c spi.c -o spi.o
	gcc -c gpio.c -o gpio.o
	gcc -c main.c -o main.o
	gcc mem_abs.o f_add1.o f_in.o f_out.o f_rjmp.o f_ret.o f_sbi.o f_sbrs.o interrupt.o interpreter.o main.o spi.o gpio.o -o syko.exe

clean:
	rm -f mem_abs.o
	rm -f f_add1.o
	rm -f f_in.o
	rm -f f_out.o
	rm -f f_rjmp.o
	rm -f f_ret.o
	rm -f f_sbi.o
	rm -f f_sbrs.o
	rm -f interrupt.o
	rm -f interpreter.o
	rm -f main.o
	rm -f spi.o
	rm -f gpio.o
	rm -f syko.exe
