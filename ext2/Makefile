#
# File:- Makefile 
# written by Manish Regmi (manish_regmi@hotmail.com)
#
# Makefile for easy compilation.:-)



OPTIONS = -Wall -O2
INCLS = -I. -I../ -I../platform/ 

CC = gcc
AS = as
LD = ld

	
OBJS = ext2io.o inode.o mount.o namei.o

OBJFILES = $(wildcard *.o)

all: ext2.o
.c.o:
	$(CC) -c $(OPTIONS) $(INCLS) $< -o $@

ext2.o: $(OBJS)
	$(LD) -r -o ext2.o $(OBJS)
clear:
	rm -f $(OBJFILES)
	rm -f *.c~ *~

