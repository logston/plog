Cardboard Air Conditioner Insulator
###################################

:date: 2016-01-05 06:28:04
:categories: Arduino A/C data analysis

.. image:: ../legacy/2016-01-05-Cardboard-AC-Insulator/IMG_0018-1.jpg
    :width: 800px

Motivation
==========

I love air conditioning. My father owned his own sheet metal & HVAC
construction company and I've always thought refrigeration was quite cool.
I also *need* an A/C while living in NYC in the summer. I can not sleep
without an A/C going most nights between May and October. But I really don't
like taking the A/C out of the window and storing it over the winter.

Enter the cardboard box. I cut several pieces of cardboard to size so that
the end product would fit snuggly in my window frame and cover my A/C. I
wanted to see if it would sufficiently block the cold from coming in.
Here's my research.


Methods
=======

I connected an Arduino to three temperature sensors (DS18B20) via phone wire.
I placed one sensor outside of my home, one between the AC and the
cardboard insulator, and one in front of the cardboard insulator.

.. image:: ../legacy/2016-01-05-Cardboard-AC-Insulator/IMG_0015-1.jpg
    :width: 800px

.. image:: ../legacy/2016-01-05-Cardboard-AC-Insulator/IMG_0016-1.jpg
    :width: 800px

.. image:: ../legacy/2016-01-05-Cardboard-AC-Insulator/IMG_0020-1.jpg
    :width: 800px

The Arduino was run from approximately Nov 1st, 2015 to
approximately Jan 1st, 2016.

The software loaded onto the Arduino can be downloaded from this
`link <../legacy/2016-01-05-Cardboard-AC-Insulator/gather.cpp>`_.


Analysis and Results
====================

The script that was used to analyze the data pulled from the Arduino can be 
downloaded `here <../legacy/2016-01-05-Cardboard-AC-Insulator/visualize.py>`_. 
This python script produced the image and two tables worth of data below.

The data can be downloaded 
`too <../legacy/2016-01-05-Cardboard-AC-Insulator/TEMPS.TXT>`_.

.. image:: ../legacy/2016-01-05-Cardboard-AC-Insulator/results.png
    :width: 800px


+------------------+--------+---------+
| Sensor ID        | Mean   | Std Dev |
+------------------+--------+---------+
| 28CA1F5707000079 |  12.43 | 4.88    |
+------------------+--------+---------+
| 285F3AFE040000D7 | 14.19  | 4.80    |
+------------------+--------+---------+
| 28307DFD04000000 | 20.72  | 1.23    |
+------------------+--------+---------+

+---------------+-------------+--------------------+--------------------+
| Comparison    | t-Statistic | Two-Tailed p-Value | Average Difference |
+---------------+-------------+--------------------+--------------------+
| --79 v. --D7  | 262.15      | 0.0                | 1.77               |
+---------------+-------------+--------------------+--------------------+
| --D7 v. --00  | 1337.87     | 0.0                | 6.53               |
+---------------+-------------+--------------------+--------------------+


Welch's t-test was used to compute the t-statistic.


Discussion
==========

Welp... it's clear to me that this cardboard box does keep the
apartment warmer than if I was to leave A/C in the window and forgo
the insulator. The there is a clear difference in means between the sensor
between the A/C and the cardboard insulator (285F3AFE040000D7) and the
sensor in the apartment (28307DFD04000000). **This box keeps my place warmer
by six degrees Celsius on average.**


