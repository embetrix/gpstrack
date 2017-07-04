# gpstrack

gpstrack is a geo-localization application written in C intended for Embedded-Linux plaftforms such as (Beaglebone, RPI, RPI3, ...) extended with a GPS module or dongle.

It uses minmea library for parsing GPS NMEA sentences. GPS data (position, speed, time) is parsed, formated as JSON and streamed in real-time to PubNub cloud.

This software is licensed under GPL Version 2.0+
