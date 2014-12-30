---
layout: post
title: "Time Warner Cable"
date: 2014-12-24 01:28:04
categories: Arduino Internet data analysis
---

Motivation
==========

I love the internet. Unfortunately, my ISP doesn't always deliver. I am a 
customer with Time Warner Cable's and I have, on occasion, found that latency is
super high or that I don't have a connection at all. These occurrences are not
frequent enough to call the Time Warner about but they are annoying. 
For the record, I would considered switching ISPs, but I don't have any other options. 
Time Warner is the only ISP where I live.

In order to empower myself a bit, I decided to take measurements of my
ping time for a month and determine how bad my situation is. Maybe my 
brushes with latency and timeouts were anomalies.


Methods
=======

Round trip ping times were collected by an Arduino with an ethernet 
shield. The IP address to which all UDP requests were sent was 8.8.8.8. This
IP address is one of the IP addresses for Google's DNS servers.

The Arduino was run from Tue, 25 Nov 2014 16:54:04 GMT until 
Sat, 20 Dec 2014 17:41:51 GMT. 

The software loaded onto the Arduino can be downloaded by clicking
`here <../media/2014-12-24-Time-Warner-Cable/PingLogger.ino>`_.


Results
=======




Discussion
==========






