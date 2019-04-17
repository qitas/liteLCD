CFLAGS=-c -Wall -O2
CC=arm-linux-gnueabihf-gcc
AR=arm-linux-gnueabihf-ar

objs = main.o spi.o fd.o gpio.o pwm.o lcd.o font.o 
#dis.o

all: show


show: $(objs)
	$(CC) $(objs) -o show

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm  -rf *.o  show

