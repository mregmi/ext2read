#
# File:- Makefile 
# written by Manish Regmi (manish_regmi@hotmail.com)
#
# Makefile for easy compilation.:-)



OPTIONS = -Wall -O2
INCLS = -I. -I../ 

CC = gcc
AS = as
LD = ld

	
OBJS = platform_win32.o 

OBJFILES = $(wildcard *.o)

all: platform.o
.c.o:
	$(CC) -c $(OPTIONS) $(INCLS) $< -o $@

platform.o: $(OBJS)
	$(LD) -r -o platform.o $(OBJS)
clear:
	rm -f $(OBJFILES)
	rm -f *.c~ *~

