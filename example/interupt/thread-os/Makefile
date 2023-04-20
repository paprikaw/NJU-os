NAME := kernel
SRCS := $(shell find . -name "*.c")
export AM_HOME=/workspaces/NJU_OS/os-workbench/abstract-machine
export ARCH=x86_64-qemu
include $(AM_HOME)/Makefile

debug:
	qemu-system-x86_64 -s -S build/kernel-x86_64-qemu &
	gdb -x gdb.init
