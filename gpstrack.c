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

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <json.h>
#include <pubnub.h>
#include <pubnub-sync.h>

#include "minmea.h"

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
    json_object *gps;  
    enum json_tokener_error error = json_tokener_success;

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

    struct pubnub_sync *sync = pubnub_sync_init();
    struct pubnub *p = pubnub_init(PUBKEY, SUBKEY, &pubnub_sync_callbacks, sync);

    /*parse only RMC sentences*/	
    while (fgets(line, sizeof(line), fp) != NULL) {
        if (minmea_sentence_id(line, false) == MINMEA_SENTENCE_RMC){

                if (minmea_parse_rmc(&frame, line)) {
                    minmea_gettime(&ts, &frame.date, &frame.time);
                    ptm = localtime(&ts.tv_sec);
                    strftime(time, sizeof (time), "%Y-%m-%d %H:%M:%S", ptm);

                    /*convert speed from knot to mps*/
                    speed = KNT2MPS*minmea_tofloat(&frame.speed);

                    asprintf(&gps_json_string, gps_data, 
                             minmea_tocoord(&frame.latitude), 
                             minmea_tocoord(&frame.longitude), 
                             speed, 
                             time);

                    gps = json_tokener_parse_verbose(gps_json_string, &error);

                    /*printing the json object*/
                    debug("gps json: %s\n",json_object_to_json_string(gps));

                    /*publish only if taget moving*/
                    if (speed > SPEED_MIN) {
                        pubnub_publish(p,CHANNEL,gps,TMOUT,NULL,NULL);
	                if (pubnub_sync_last_result(sync) != PNR_OK)
	                    printf("pubnub publish error!\n");
                   }
                }
        }
    }
    free(gps_json_string);
    pubnub_done(p);
    fclose(fp);
    return 0;
}
