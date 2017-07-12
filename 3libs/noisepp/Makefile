TARGET_LIB = libNoisepp.a
OBJS = NoiseBuilders.o \
NoiseColourValue.o \
NoiseEndianUtils.o \
NoiseGradientRenderer.o \
NoiseImage.o \
NoiseInStream.o \
NoiseJobQueue.o \
NoiseModules.o \
NoiseOutStream.o \
NoiseReader.o \
NoiseSystem.o \
NoiseWriter.o

PSPSDK = $(shell psp-config --pspsdk-path)
PSPPATH = $(shell psp-config --psp-prefix)

INCDIR = $(PSPPATH)/include
CFLAGS = -O2 -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

include $(PSPSDK)/lib/build.mak
