# (C) Copyright 2017
#
# Ayoub Zaki, Embexus Embedded Systems Solutions, ayoub.zaki@embexus.com
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#

CFLAGS  = -Wall -Wextra -Werror -std=c99
CFLAGS += -D_POSIX_C_SOURCE=199309L -D_BSD_SOURCE -D_DEFAULT_SOURCE -D_DARWIN_C_SOURCE
CFLAGS += -I/usr/local/include -I/usr/include/json-c

LIBS    = -lpubnub -ljson-c -lcurl -lcrypto -levent 
LDFLAGS = -L/usr/local/lib

all:  gpstrack

clean:
	$(RM) gpstrack *.o

gpstrack: gpstrack.o minmea.o
	$(CC) $(LDFLAGS) $< $(LIBS) -o $@

minmea.o: minmea.c minmea.h
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: all clean
