/*
 * (C) Copyright 2017
 * Embexus Embedded Systems Solutions, ayoub.zaki@embexus.com
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */


#define DEBUG 1
#define debug(args ...) if (DEBUG) fprintf(stderr, args)
#define STRINGIFY(...) #__VA_ARGS__

 #define PUBKEY "pub-c-xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
 #define SUBKEY "sub-c-xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"

#define CHANNEL "gps-location"
#define TMOUT 10

#define SPEED_MIN 0.1

/* Knot to meter/s conversion*/
#define KNT2MPS 0.51444444444


