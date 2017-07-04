# Description

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

* install dependencies on host Ubuntu:
  $ sudo apt-get install libjson-c-dev libcurl4-openssl-dev libevent-dev

# Build
* build and install first PubNub C SDK:

        $ git clone https://github.com/embexus/pubnub-c-sdk.git
        $ cd pubnub-c-sdk && make && make install

# Usage

# Licensing

This software is licensed under GPL Version 2.0+
