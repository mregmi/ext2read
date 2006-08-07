
# Copyright (C) 2004, 2005	Manish Regmi (regmi dot manish at gmail dot com)
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
 
INCL = -I. -I./platform/ -I./ext2/
OBJS = ./ext2/ext2.o ./platform/platform.o partition.o print.o
OPTIONS = -Wall -O2

LD = ld
CC = gcc
COPY = cp
STRIP = strip
OBJDUMP = objdump

all:
	make -C platform
	make -C ext2
	make ml

ml:ext2read
.c.o:
	$(CC) -c $(OPTIONS) $(INCL) $< -o $@

ext2read: $(OBJS)	
	$(LD) -o ext2read $(OBJS)
	
clear:
	make -C platform clear
	make -C ext2 clear
	rm -f *.o *.c~ *~ ext2read
	
