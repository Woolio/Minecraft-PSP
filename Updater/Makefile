TARGET = updater
OBJS = main.o

INCDIR = 
CFLAGS = -O2 -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR =
LIBS = -lpge -lpspgum_vfpu -lpspvfpu -lpspgu -lpspsdk -lpspaudio -lpspaudiocodec -lpsprtc -lpspwlan -lpng -lminizip  -lz -lpsppower -lm -lpspnet_adhoc -lpspnet_adhocctl -lpspnet_adhocmatching -lpsphttp -lpspusb -lpspusbstor -lpspopenpsid

BUILD_PRX = 1

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = MineCraft Updater
PSP_EBOOT_ICON = ICON0.png

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
