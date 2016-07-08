##### Makefile for BOOKSHELF #####
## If building on rpi, ensure export HOSTTYPE is included in your .bashrc ##
######


CC=g++
CFLAGS=-Wall -std=c++0x -pthread

#DFLAGS=-D BOOKSHELF_BUILD_RASPI -I/.wiring_pi/include -L/.wiring_pi/lib  -Wl,--start-group /.wiring_pi/lib/libwiringPi.so -Wl,--end-group -l
#DFLAGS=-D BOOKSHELF_BUILD_RASPI -lwiringpi


ifeq ($(shell echo $(HOSTTYPE)), arm)
  DFLAGS=-D BOOKSHELF_BUILD_RASPI -lwiringpi
endif

ARMCC=arm-linux-gnueabihf-g++
ARMDFLAGS=-D BOOKSHELF_BUILD_RASPI -I./wiring_pi/include -L./wiring_pi/lib_arm  -Wl,--start-group ./wiring_pi/lib_arm/libWiringPi.so -Wl,--end-group

GAMEINCLUDE = 	./include
GAMESOURCE = 	./src/*.cpp ./src/*.c

#PINLIBINC = 	./pinlib/include
#PINLIBSRC = 	./pinlib/src/*.cpp


#READLINESRC  =		./readline/*.c
#LIB = readline


#pinlib.o : $(PINLIBINC) $(PINLIBSRC)
#	$(CC) $(CFLAGS) -I $(PINLIBINC) $(PINLIBSRC)


bookshelf :
	$(CC) $(CFLAGS) -D _DEBUG -D _LINUX $(DFLAGS) -iquote $(GAMEINCLUDE) $(GAMESOURCE) -o bookshelf

arm :
	$(ARMCC) $(CFLAGS) -D _DEBUG -D _LINUX $(ARMDFLAGS) -iquote $(GAMEINCLUDE) $(GAMESOURCE) -o bookshelf_arm

arm_cc:
	make arm && scp bookshelf_arm pi@192.168.0.14:/home/pi/gs_arm && rsync -avz public_html pi@192.168.0.14:/home/pi/gs_arm && ssh 192.168.0.14 -lpi -t 'sudo /bin/bash -c "cd /home/pi/gs_arm && /home/pi/gs_arm/bookshelf_arm"'


clean:
	$(RM) bookshelf
clean_arm:
	$(RM) bookshelf_arm

