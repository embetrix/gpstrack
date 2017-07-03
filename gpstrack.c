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
#include <json.h>
#include <pubnub.h>
#include <pubnub-sync.h>

#include "minmea.h"
#include "gpstrack.h"


void publish_gps_data(struct pubnub_sync *s, struct pubnub *p, json_object *data, float minspeed) {

    struct json_object* tmpobj;
    float speed = 0.0;

    /*extract speed key from json data*/
    if (json_object_object_get_ex(json_object_array_get_idx(data,0), "speed", &tmpobj)) {
        speed = json_object_get_double(tmpobj);
        /*publish only if taget moving*/
        if (speed > minspeed) {
            pubnub_publish(p,CHANNEL,data,TMOUT,NULL,NULL);
            if (pubnub_sync_last_result(s) != PNR_OK)
                printf("pubnub publish error!\n");
        }   
    }
}

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
    struct pubnub *pubnb = pubnub_init(PUBKEY, SUBKEY, &pubnub_sync_callbacks, sync);

    /*parse only RMC sentences*/	
    while (fgets(line, sizeof(line), fp) != NULL) {
        if (minmea_sentence_id(line, false) == MINMEA_SENTENCE_RMC){

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

                    gps = json_tokener_parse(gps_json_string);
                    /*printing the json object*/
                    debug("json: %s\n",json_object_to_json_string(gps));
                    publish_gps_data(sync, pubnb, gps, SPEED_MIN);
                }
        }
    }
    free(gps_json_string);
    pubnub_done(pubnb);
    fclose(fp);
    return 0;
}
