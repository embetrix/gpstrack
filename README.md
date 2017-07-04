# gpstrack

[gpstrack](http://github.com/embexus/gpstrack) is a geo-localization application written in C intended for Embedded-Linux platforms 
extended with a GPS module or dongle.

gpstrack uses [minmnea](https://github.com/cloudyourcar/minmea) library for parsing GPS/NMEA sentences. 
GPS information (position, speed, time) is parsed, formated as JSON data and streamed in real-time to [PubNub](https://www.pubnub.com) cloud.

![](https://embexus.com/gps/gpstrack.gif)

## Dependencies

 * [PubNub C SDK](https://github.com/embexus/pubnub-c-sdk)
 * libjson-c
 * libcurl
 * libcrypto
 * libevent

install dependencies on Ubuntu host:

        $ sudo apt-get install libjson-c-dev libcurl4-openssl-dev libevent-dev

## PubNub/MapBox accounts

To be able to upload data on [PubNub](https://www.pubnub.com) cloud you need to have an account, 
create an application and add a channel to be used to stream the GPS data feed.

[Mapbox](https://www.mapbox.com/) account is also required for Map Live visualization.


## Build

build and install first [PubNub C SDK](https://github.com/embexus/pubnub-c-sdk):

        $ git clone https://github.com/embexus/pubnub-c-sdk.git
        $ cd pubnub-c-sdk 
        $ make 
        $ sudo make install

clone [gpstrack](http://github.com/embexus/gpstrack) repository:

        $ git clone https://github.com/embexus/gpstrack.git

change [gpstrack.h](https://github.com/embexus/gpstrack/blob/master/gpstrack.h) to input PubNub Channel, Publish Key and Subscribe Key information.

build [gpstrack](http://github.com/embexus/gpstrack) :

        $ make

## Usage

GPS Module or Dongle need to be connected to your device over Serial or USB to get GPS data:

        $ ./gpstrack /dev/ttyACM0

If you don't have a GPS device you can simulate it by inputing the [test.nmea](https://github.com/embexus/gpstrack/blob/master/test.nmea.gz) data file

        $ gzip -dc test.nmea.gz > test.nmea
        $ ./gpstrack test.nmea

## Map live view

[EON real-time map dashboard](https://www.pubnub.com/developers/eon/) is used to visualize the position on the map in realtime.
To use change [gpstrack.html](https://github.com/embexus/gpstrack/blob/master/gpstrack.html) to input:
   
 * PubNub subcription key
 * PubNub Channel
 * MapBox Id

Finally you can then open the html file locally with a browser or upload it to a webserver.

## Licensing

This software is licensed under GPL Version 2.0+
