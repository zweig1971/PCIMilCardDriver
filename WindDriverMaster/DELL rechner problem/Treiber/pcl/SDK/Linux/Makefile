#
#  Linux Kernel Driver for PCL PCI Devices
#
#  Makefile
#  (c) 2005-2007 Alpermann + Velte EE GmbH
#                  V 3.36
#  

EXTRA_CFLAGS += -DLINUX
EXTRA_AFLAGS +=
EXTRA_LDFLAGS +=
EXTRA_ARFLAGS +=

DRIVEROBJS = avpcldrv.o

LIBSOURCES = funcs.c avpcl.c pci_lib_linux.c
LIBNAME    = avpcl32
# 3.36
LIBMAJOR   = 3		
# 3.36
LIBMINOR   = 36

ifneq ($(KERNELRELEASE),)
obj-m   := avpcl32.o
avpcl32-objs   += $(DRIVEROBJS)

else
KDIR    := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all: default library

default:
# make module
	$(MAKE) -C $(KDIR)  SUBDIRS=$(PWD) modules

library:
# make lib
	$(CC) -fPIC -g -Wall -I. -DLINUX -D'PATHTODEVICEFILES="/dev/"'\
	-shared -Wl,-soname,lib$(LIBNAME).so.$(LIBMAJOR) \
	-o lib$(LIBNAME).so.$(LIBMAJOR).$(LIBMINOR) $(LIBSOURCES)

module: default
# make the module and delete temp files
	rm -f avpcldrv.o .avpcldrv.ko.cmd avpcldrv.mod.* .avpcldrv.* *~

clean:
	rm -f $(DRIVEROBJS)
	rm -f .avpcldrv.* *~ .pci_lib_linux.o.cmd
	rm -f avpcl32.o avpcl32.ko .avpcl32.ko.cmd avpcl32.mod.* .avpcl32.*
	rm -rf .tmp_versions
	rm -f lib$(LIBNAME).so*

PclTest: PclTest.c
	$(CC) -DLINUX -o PclTest PclTest.c -I. -L. -lncurses -lavpcl32

endif

