objects = commands.o device.o chamber_device.o hcwin_device.o log_device.o telnet_device.o death_valley.o
cc = gcc -g
cflags = -Wall -I.. -I/usr/local/include -Wno-variadic-macros -Wno-missing-braces -Wno-pointer-sign -c
lflags = -Wall
liblink = -L/usr/lib64 -L/usr/lib -L/usr/local/lib -lpthread -lmiranda_ground
exec = death_valley.bin

all: $(objects)
	$(cc) $(lflags) $(objects) -o $(exec) $(liblink)

commands.o: commands.c commands.h
	$(cc) $(cflags) commands.c

device.o: device.c device.h chamber_device.h hcwin_device.h log_device.h telnet_device.h
	$(cc) $(cflags) device.c

chamber_device.o: chamber_device.c chamber_device.h
	$(cc) $(cflags) chamber_device.c

hcwin_device.o: hcwin_device.c hcwin_device.h
	$(cc) $(cflags) hcwin_device.c

log_device.o: log_device.c log_device.h chamber_device.h hcwin_device.h
	$(cc) $(cflags) log_device.c

telnet_device.o: telnet_device.c telnet_device.h
	$(cc) $(cflags) telnet_device.c

death_valley.o: death_valley.c death_valley.h commands.h device.h
	$(cc) $(cflags) death_valley.c

clean:
	rm -f *.o
	rm -f $(exec)
