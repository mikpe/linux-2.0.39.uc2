# $Id: Makefile,v 1.1.1.1 1999-11-22 03:47:41 christ Exp $
# Makefile for Sparc library files..
#

CFLAGS := $(CFLAGS) -ansi

OBJS  = mul.o rem.o sdiv.o udiv.o umul.o urem.o ashrdi3.o memcpy.o memset.o \
        strlen.o

lib.a: $(OBJS)
	$(AR) rcs lib.a $(OBJS)
	sync

memcpy.o: memcpy.S
	$(CC) -ansi -c -o memcpy.o memcpy.S

strlen.o: strlen.S
	$(CC) -ansi -c -o strlen.o strlen.S

mul.o: mul.S
	$(CC) -c -o mul.o mul.S

rem.o: rem.S
	$(CC) -DST_DIV0=0x2 -c -o rem.o rem.S

sdiv.o: sdiv.S
	$(CC) -DST_DIV0=0x2 -c -o sdiv.o sdiv.S

udiv.o: udiv.S
	$(CC) -DST_DIV0=0x2 -c -o udiv.o udiv.S

umul.o: umul.S
	$(CC) -c -o umul.o umul.S

urem.o: urem.S
	$(CC) -DST_DIV0=0x2 -c -o urem.o urem.S

ashrdi3.o: ashrdi3.S
	$(CC) -c -o ashrdi3.o ashrdi3.S

dep:

include $(TOPDIR)/Rules.make
