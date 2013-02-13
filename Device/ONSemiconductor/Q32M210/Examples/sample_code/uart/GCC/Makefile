#-----------------------------------------------------------------------------
# Copyright (c) 2012 Semiconductor Components Industries, LLC
# (d/b/a ON Semiconductor). All Rights Reserved.
#
# This code is the property of ON Semiconductor and may not be redistributed
# in any form without prior written permission from ON Semiconductor. The
# terms of use and warranty for this code are covered by contractual
# agreements between ON Semiconductor and the licensee.
#-----------------------------------------------------------------------------
# Makefile
# - Sample code project Makefile
#-----------------------------------------------------------------------------
# $Revision: 1.3 $
# $Date: 2012/04/26 18:50:02 $
#-----------------------------------------------------------------------------

LEVEL = ../../..
SRCDIR = ..

TO_PC = 1

ifdef LOOPBACK
TO_PC = 0
endif

ifeq ($(TO_PC), 1)
APP = uart_to_pc
BUILDDIR = build_to_pc
else
APP = uart_loopback
BUILDDIR = build_loopback
endif

LIBS = syslib

include $(LEVEL)/Makefile.rules

ifeq ($(TO_PC), 1)
CFLAGS += -DUART_LOOPBACK=0 -DUART_TO_PC=1 -DBUILD_TYPE=UART_TO_PC
else
CFLAGS += -DUART_LOOPBACK=0 -DUART_TO_PC=1 -DBUILD_TYPE=UART_LOOPBACK
endif

