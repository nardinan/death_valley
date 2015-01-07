objects = console.o commands.o device.o main.o
cc = gcc -g
cflags = -Wall -I.. -Wno-variadic-macros -Wno-missing-braces -Wno-gnu -Wno-pointer-sign -c -pedantic
lflags = -Wall
liblink = -L../miranda -L/usr/lib64 -L/usr/lib -lpthread -lmiranda_ground
exec = death_valley.bin

all: $(objects)
	$(cc) $(lflags) $(objects) -o $(exec) $(liblink)

console.o: console.c console.h
	$(cc) $(cflags) console.c

commands.o: commands.c commands.h console.h
	$(cc) $(cflags) commands.c

device.o: device.c device.h commands.h
	$(cc) $(cflags) device.c

main.o: main.c device.h
	$(cc) $(cflags) main.c

clean:
	rm -f *.o
	rm -f $(exec)
