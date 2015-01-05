---
layout: post
title: "Time Warner Cable"
date: 2014-12-24 01:28:04
categories: Arduino Internet data analysis
---

Motivation
==========

I love the internet. Unfortunately, my ISP doesn't always deliver. I am a 
customer with Time Warner Cable and I have, on occasion, found that latency is
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


Analysis and Results
====================

.. code:: python

    import numpy as np
    import pandas as pd
    import datetime
.. code:: python

    df_raw = pd.io.parsers.read_csv('data.csv')
    df_raw.columns=['timestamp', 'ping']
.. code:: python

    def isnotint(val):
        try:
            int(val)
            return False
        except:
            return True
.. code:: python

    # Probably a better way to do this. Would love to know.
    timeout_indices = df_raw['ping'].map(isnotint)  
.. code:: python

    df_clean = df_raw.copy(deep=True)
    df_clean.loc[timeout_indices, 'ping'] = 0
    df_clean['timestamp'] = df_clean['timestamp'].map(datetime.datetime.fromtimestamp)
    df_clean['ping']= df_clean['ping'].map(int) # Probably a better way to do this.
    df_clean.dtypes

.. parsed-literal::

    timestamp    datetime64[ns]
    ping                  int64
    dtype: object

.. code:: python

    df_timeouts = df_raw.copy(deep=True)
    df_timeouts.loc[np.invert(timeout_indices), 'ping'] = 0
    # Make timeout pings equal size and 25% longer than the longest ping time recorded.
    df_timeouts.loc[timeout_indices, 'ping'] = df_clean['ping'].max() * 1.25
    df_timeouts['timestamp'] = df_timeouts['timestamp'].map(datetime.datetime.fromtimestamp)
.. code:: python

    # enable plotting in the current notebook with the inline backend
    %matplotlib inline
    
    import matplotlib as mpl
    import matplotlib.pyplot as plt
    
    pd.set_option('display.mpl_style', 'default')  # give plots a more pleasing visual style
    
    mpl.rcParams['figure.figsize'] = (15, 10) # Set default figure size
.. code:: python

    fig, ax = plt.subplots(1, 1)
    ax.set_xlabel('Date/Time')
    ax.set_ylabel('Round Trip Ping (ms)')
    df_timeouts.plot(x='timestamp', y='ping', ax=ax, legend=False, color='Pink')
    df_clean.plot(x='timestamp', y='ping', ax=ax, legend=False, color='DarkGreen')



.. parsed-literal::

    <matplotlib.axes._subplots.AxesSubplot at 0x111cb2588>




.. image:: /media/2014-12-24-Time-Warner-Cable/ping-test_7_1.png


Wow! That's a lot of pink! Were the timeouts that evenly dispersed? I
know there were quite a number of them. Approximately 12% of packets
were dropped (ie. timed out). Lets thin out the data a bit to see the
distribution of timeouts.

Also to note, there seems to be some periodicity to the increases in
ping durations.

Plausible causes:

-  increased loads on the network (ie. everyone comes home and surfs)
-  error in my aurduino sketch
-  Giant Rat chewing on telecom cable

.. code:: python

    fig, ax = plt.subplots(1, 1)
    ax.set_xlabel('Date/Time')
    ax.set_ylabel('Round Trip Ping (ms)')
    df_timeouts[::100].plot(x='timestamp', y='ping', ax=ax, legend=False, color='Pink')
    df_clean[::100].plot(x='timestamp', y='ping', ax=ax, legend=False, color='DarkGreen')

.. image:: /media/2014-12-24-Time-Warner-Cable/ping-test_9_1.png


.. code:: python

    number_of_timeouts = timeout_indices.sum()
    
    timeout_percentage = number_of_timeouts / len(df_raw)
    
    print('{0} out of {1} requests timed out. '
          '{2:.2%} of all requests timed out.'.format(number_of_timeouts,
                                                      len(df_raw),
                                                      timeout_percentage))

.. parsed-literal::

    99525 out of 809771 requests timed out. 12.29% of all requests timed out.


.. code:: python

    pings = df_clean.ping[~timeout_indices]

.. code:: python

    fig, ax = plt.subplots(1, 1)
    ax.set_xlabel('Ping (ms)')
    ax.set_ylabel('Number of requests for given ping duration')
    pings.hist(bins=1000, ax=ax)



.. parsed-literal::

    <matplotlib.axes._subplots.AxesSubplot at 0x12ba0aef0>




.. image:: /media/2014-12-24-Time-Warner-Cable/ping-test_12_1.png


.. code:: python

    print('{:.2%} of pings were over 100 ms.'.format(len(pings[pings > 100]) / len(pings)))

.. parsed-literal::

    1.27% of pings were over 100 ms.


.. code:: python

    print('The average ping was {:.0f} ms.'.format(pings.mean()))

.. parsed-literal::

    The average ping was 35 ms.


.. code:: python

    ok_pings = pings[pings <= 100]
    print('From the set of pings that were less than 100 ms, '
          'the average ping was {:.0f} ms.'.format(ok_pings.mean()))

.. parsed-literal::

    From the set of pings that were less than 100 ms, the average ping was 32 ms.


.. code:: python

    fig, ax = plt.subplots(1, 1)
    ax.set_xlabel('Ping (ms)')
    ax.set_ylabel('Number of requests for given ping duration')
    ok_pings.hist(bins=100, ax=ax)



.. parsed-literal::

    <matplotlib.axes._subplots.AxesSubplot at 0x117cd96a0>




.. image:: /media/2014-12-24-Time-Warner-Cable/ping-test_16_1.png


Interesting to note, there seems to be ping durations that are not
represented by any successful requests. For example, there were no
successful pings with a duration of 20 milliseconds. That just seems
wrong. I feel this data is inaccurate. Not sure how much I can trust
this data. I will rerun the experiment.

Discussion
==========
