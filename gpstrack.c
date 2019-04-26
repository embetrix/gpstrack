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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>


#include "minmea.h"
#include "gpstrack.h"



int main(int argc, char **argv)
{
    FILE *fp;
    char line[MINMEA_MAX_LENGTH];
    char time[40];

    struct minmea_sentence_rmc frame;
    struct timespec ts;
    struct tm* ptm;
    float speed = 0.0;
    char *gps_json_string = NULL;

    static const char* const gps_data = STRINGIFY(
    [
     {
	"latlng": [
	 %f,
	 %f
	],
	"speed": %f,
	"time": "%s"
     }
    ]
    );

    if (argc > 1)
        fp = fopen(argv[1], "r");
    else {
        printf("Usage: %s /dev/ttyACM0\n",argv[0]);
        return(-1);
    }

    if(!fp) {
        printf("Error opening %s\n",argv[1]);
        return(-1);
    }

    /*parse only RMC sentences*/	
    while (fgets(line, sizeof(line), fp) != NULL) {
        if (minmea_sentence_id(line, false) == MINMEA_SENTENCE_RMC) {

                if (minmea_parse_rmc(&frame, line)) {
                    minmea_gettime(&ts, &frame.date, &frame.time);
                    ptm = localtime(&ts.tv_sec);
                    strftime(time, sizeof (time), "%Y-%m-%d %H:%M:%S", ptm);

                    /*convert speed from knot to mps*/
                    speed = KNT2MPS*minmea_tofloat(&frame.speed);
                    
                    /*format json string*/
                    asprintf(&gps_json_string, gps_data, 
                             minmea_tocoord(&frame.latitude), 
                             minmea_tocoord(&frame.longitude), 
                             speed, 
                             time);

                    /*printing the json object*/
                    debug("json: %s\n",gps_json_string);
                }
        }
    }
    free(gps_json_string);
    fclose(fp);
    return 0;
}
