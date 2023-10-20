#  ____     ___ |    / _____ _____
# |  __    |    |___/    |     |
# |___| ___|    |    \ __|__   |     gsKit Open Source Project.
# ----------------------------------------------------------------------
# Copyright 2004 - Chris "Neovanglist" Gilbert <Neovanglist@LainOS.org>
# Licenced under Academic Free License version 2.0
# Review gsKit README & LICENSE files for further details.
#
# examples/textures/Makefile - Makefile for "textures" example.
#

EE_BIN  = ngdn.elf
EE_OBJS = main.o textureManager.o soundManager.o input.o game.o ./external/cJSON.o
EE_LIBS_EXTRA += -laudsrv -lc -lgs -lpad -lpthread 


all: $(EE_BIN) audsrv.irx

audsrv.irx:
	cp $(PS2SDK)/iop/irx/audsrv.irx $@

clean:
	rm -f $(EE_BIN) $(EE_OBJS) *.adp audsrv.irx

include ${GSKITSRC}/Makefile.pref
include ../Makefile.global
include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal
