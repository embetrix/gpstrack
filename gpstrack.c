/*
 * (C) Copyright 2017
 * Ayoub Zaki, Embexus Embedded Systems Solutions, ayoub.zaki@embexus.com
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

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "minmea.h"

int main(int argc, char **argv)
{
    FILE *fp;
    char line[MINMEA_MAX_LENGTH];
    char time[40];
    struct minmea_sentence_rmc frame;
    struct timespec ts;
    struct tm* ptm;
    
    if (argc > 1)
        fp = fopen(argv[1], "r");

    if(!fp) {
        printf("Error opening %s\n",argv[1]);
        return(-1);
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        if (minmea_sentence_id(line, false) == MINMEA_SENTENCE_RMC){
                
                if (minmea_parse_rmc(&frame, line)) {
                    minmea_gettime(&ts, &frame.date, &frame.time);
                    ptm = localtime (&ts.tv_sec);
                    strftime (time, sizeof (time), "%Y-%m-%d %H:%M:%S", ptm);
                    printf( "Time: %s - Coordinates: (%f,%f) - Speed: %f\n", 
                            time,
                            minmea_tocoord(&frame.latitude),
                            minmea_tocoord(&frame.longitude),
                            minmea_tofloat(&frame.speed));
                }
        }
    }
    fclose(fp);
    return 0;
}
