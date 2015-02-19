objects = console.o commands.o device.o chamber_device.o hcwin_device.o log_device.o telnet_device.o death_valley.o
cc = gcc -g
cflags = -Wall -I.. -Wno-variadic-macros -Wno-missing-braces -Wno-pointer-sign -c -pedantic
lflags = -Wall
liblink = -L../miranda -L/usr/lib64 -L/usr/lib -lpthread -lmiranda_ground
exec = death_valley.bin

all: $(objects)
	$(cc) $(lflags) $(objects) -o $(exec) $(liblink)

console.o: console.c console.h
	$(cc) $(cflags) console.c

commands.o: commands.c commands.h console.h
	$(cc) $(cflags) commands.c

device.o: device.c device.h console.h chamber_device.h hcwin_device.h log_device.h telnet_device.h
	$(cc) $(cflags) device.c

chamber_device.o: chamber_device.c chamber_device.h console.h
	$(cc) $(cflags) chamber_device.c

hcwin_device.o: hcwin_device.c hcwin_device.h console.h
	$(cc) $(cflags) hcwin_device.c

log_device.o: log_device.c log_device.h console.h chamber_device.h hcwin_device.h
	$(cc) $(cflags) log_device.c

telnet_device.o: telnet_device.c telnet_device.h console.h
	$(cc) $(cflags) telnet_device.c

death_valley.o: death_valley.c death_valley.h commands.h device.h 
	$(cc) $(cflags) death_valley.c

clean:
	rm -f *.o
	rm -f $(exec)
