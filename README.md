# Gpstrack

[gpstrack](http://github.com/embexus/gpstrack) is a geo-localization application written in C intended for Embedded-Linux plaftforms 
such as (Beaglebone, RPI, RPI3, etc...) extended with a GPS module or dongle.

gpstrack uses [minmnea](https://github.com/cloudyourcar/minmea) library for parsing GPS NMEA sentences. 
GPS data (position, speed, time) is parsed, formated as JSON and streamed in real-time to [PubNub](https://www.pubnub.com) cloud.

# Dependencies

 [PubNub C SDK](https://github.com/embexus/pubnub-c-sdk)
 libjson-c
 libcurl
 libcrypto
 libevent

* install dependencies on Ubuntu host:

        $ sudo apt-get install libjson-c-dev libcurl4-openssl-dev libevent-dev

# PubNub/MapBox accounts

To be able to upload data on [PubNub](https://www.pubnub.com) cloud you need to have an account, 
create an application and add a channel to be used to stream the GPS data feed.

[Mapbox](https://www.mapbox.com/) account is also needed for Map Live visualization.


# Build

* build and install first [PubNub C SDK](https://github.com/embexus/pubnub-c-sdk):

        $ git clone https://github.com/embexus/pubnub-c-sdk.git
        $ cd pubnub-c-sdk 
        $ make 
        $ sudo make install

* clone [gpstrack.h](http://github.com/embexus/gpstrack/gpstrack.h) repository:

        $ git clone https://github.com/embexus/gpstrack.git

* change [gpstrack.h](http://github.com/embexus/gpstrack/gpstrack.h) to input PubNub Channel, Publish Key and Subscribe Key information.

* build [gpstrack](http://github.com/embexus/gpstrack) :

        $ make

# Usage

GPS Module or Dongle is needed to 

# Licensing

This software is licensed under GPL Version 2.0+
