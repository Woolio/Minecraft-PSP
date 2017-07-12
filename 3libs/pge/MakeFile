TARGET_LIB = lpge.a
OBJS = \
	pge.o \
	pgeAdHoc.o \
	pgeControls.o \
	pgeDir.o \
	pgeEmulator.o \
	pgeFile.o \
	pgeFont.o \
	pgeGfx.o \
	pgeMath.o \
	pgeMp3.o \
	pgeNet.o \
	pgeObj.o \
	pgeParticle.o \
	pgeSystem.o \
	pgeTexture.o \
	pgeUtils.o \
	pgeVram.o \
	pgeWav.o \
	pgeZip.o

INCDIR = $(PSPPATH)/include
ASFLAGS = $(CFLAGS)

LIBDIR =
LDFLAGS =
LIBS=

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak

install:
	cp $(TARGET_LIB) $(PSPSDK)/lib
	cp pspmath.h $(PSPSDK)/include