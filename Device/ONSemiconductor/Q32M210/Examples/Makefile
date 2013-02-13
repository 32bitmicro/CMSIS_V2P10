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
# - Top level Makefile that builds all of the sample projects and libraries
#   beneath it.
#-----------------------------------------------------------------------------
# $Revision: 1.3 $
# $Date: 2012/04/26 18:50:01 $
#-----------------------------------------------------------------------------

SUBDIRS = syslib/GCC eepromlib/GCC mathlib/GCC sample_code/blinky/GCC sample_code/analog/GCC sample_code/opmode/GCC sample_code/eeprom/GCC sample_code/flash/GCC sample_code/i2c_master/GCC sample_code/i2c_slave/GCC sample_code/pcm/GCC sample_code/spi/GCC sample_code/uart/GCC sample_code/usb/GCC sample_code/rpm_sample/GCC

# Support both Windows and MinGW/Linux by checking whether 
ifdef SystemRoot

all:
	for %%d in (${SUBDIRS}) do ${MAKE} -C %%d

clean:
	for %%d in (${SUBDIRS}) do ${MAKE} -C %%d clean

else

all:
	for dir in $(SUBDIRS) ; do ( cd $$dir ; $(MAKE) all ) ; done

clean:
	for dir in $(SUBDIRS) ; do ( cd $$dir ; $(MAKE) clean ) ; done

endif
