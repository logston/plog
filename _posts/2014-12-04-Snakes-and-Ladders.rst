---
layout: post
title: "Snakes and Ladders"
date: 2015-09-07 21:45:00
categories: Games Go golang goroutines
---

Motivation
==========

I was visiting my friends Urbi and Derek over last winter and we decided to
pull out a game. Urbi pulled out a 1000-piece puzzle of Baton Rouge, 
a slightly loved deck of cards, I think Life (the game of, that is), 
and Snakes and Ladders.

.. image:: /media/2014-12-04_Snakes_and_Ladders/snakes_and_ladders.JPG

Derek immediately groaned at the idea of Snakes and Ladders. I, having not seen
this game (or its equivalent Shoots and Ladders) since grade school, was too
nostalgic to say no to the game that Urbi was pushing.

After a solid look at the board and the die though, I realized where Derek 
was coming from. This was a non-game game. There was no strategy to this game,
only the physical labor of rolling a die. Still swept up by a bit of memories,
I rolled on with Urbi. I have to say, it felt good to hit those ladders and it
pinched to slide down those snakes. In a game of absolute chance, where I
couldn't control anything but when I stopped exerting enough force to keep a
die in my hand, I was fully invested. After the 17th role however, I got a bit
bored. Me and Derek started to talk about the rules of the game I realized
that playing with anyone in a given sitting is like playing your self in two
sequential sittings; since this is a game of complete chance, who plays has
no effect on the out come of the game for that player. Once I realized that
humans were not necessary for this game, I decided to program it.


Methods
=======

I chose Go to build out this game because:

1. I wanted to try Go. My other option was Python but I knew Python. 
   Always be learning and growing.

2. I wanted to `Monte Carlo <https://en.wikipedia.org/wiki/Monte_Carlo_method>`_
   this game to see what the spread [#]_ is like and I figured I could get a lot of
   data fast if I had a lot of workers playing the game in parallel; I heard Go
   was good for concurrency.

The code I wrote:

.. code::
    :class: ipynb-cell

    package main

    import (
      "fmt"
      "math/rand"
      "sync"
    )

    const NUMBER_OF_GAMES int = 1000000000

    var wg sync.WaitGroup
    var jumps = map[int]int{
        100: 0,    99: 0,    98: 0,    97: 0,    96: 81,   95: 0,    94: 11,   93: 0,    92: 0,    91: 70,
         81: 0,    82: 0,    83: 43,   84: 0,    85: 0,    86: 93,   87: 0,    88: 0,    89: 56,   90: 0,
         80: 0,    79: 0,    78: 0,    77: 0,    76: 0,    75: 0,    74: 0,    73: 0,    72: 90,   71: 0,
         61: 0,    62: 0,    63: 82,   64: 0,    65: 0,    66: 0,    67: 0,    68: 36,   69: 0,    70: 0,
         60: 21,   59: 0,    58: 0,    57: 0,    56: 0,    55: 0,    54: 0,    53: 0,    52: 0,    51: 0,
         41: 0,    42: 77,   43: 0,    44: 15,   45: 0,    46: 0,    47: 0,    48: 0,    49: 0,    50: 0,
         40: 62,   39: 0,    38: 0,    37: 0,    36: 0,    35: 0,    34: 0,    33: 9,    32: 0,    31: 85,
         21: 0,    22: 0,    23: 5,    24: 0,    25: 0,    26: 0,    27: 0,    28: 0,    29: 0,    30: 0,
         20: 55,   19: 0,    18: 0,    17: 0,    16: 0,    15: 0,    14: 0,    13: 0,    12: 0,    11: 0,
          1: 0,     2: 0,     3: 0,     4: 35,    5: 0,     6: 0,     7: 0,     8: 47,    9: 0,    10: 0,
    }

    func playgame() {
      var pos, moves int = 1, 0
      for pos < 100 {
        pos += rand.Intn(5) + 1
        if jumps[pos] > 0 {
          pos = jumps[pos]
        }
        moves += 1
      }
      defer wg.Done()
      fmt.Println(moves)
    }

    func main() {
      wg.Add(NUMBER_OF_GAMES)
      for i := 0; i < NUMBER_OF_GAMES; i++ {
        go playgame()
      }
      wg.Wait()
    }

The program simply plays the game and counts how many die rolls it took to reach
one hundred. If the program noticed that a die roll landed the computerized game
player on a snake or ladder, appropriate actions were taken to reset the position
of the game player.

Now, the meticulous reader will notice that my original script (this one above)
was designed to play one billion games. My laptop did not agree.

.. image:: /media/2014-12-04_Snakes_and_Ladders/one-billion-60gb.png

After letting the program consume a little over 60 GB, my laptop killed
the process. First lesson about Go, Go does not give my laptop the ability
to create and run 1 billion goroutines. It's important to learn you limits
while you're young.

After fiddling with the number of goroutines, I settled (got the program to run)
with 16.25 million goroutines.


Analysis and Results
====================

Importing my 16.25 million data points into an IPython notebook, 
I was able to generate the following histogram.

.. code:: python
    :class: ipynb-cell

    import matplotlib.pyplot as plt
    import numpy as np
    import pandas as pd

    pd.set_option('display.mpl_style', 'default')  # give plots a more pleasing visual style

    %matplotlib inline

    array = np.genfromtxt('game_data.txt')

    _, axes = plt.subplots(figsize=(16,10))
    axes.set_xlabel('Die rolls')
    axes.set_ylabel('Number of times game was completed in X number of die rolls')
    plt.hist(array, bins=100)

.. image:: /media/2014-12-04_Snakes_and_Ladders/die-roll-hist.png

Further number crunching yields the following numbers:

====  =====
basic stats
-----------
stat  value
====  =====
min   6
max   704
mean  53
mode  14
====  =====


Discussion
==========

Some poor virtual soul had to role the die 704 times before they got to 100.
That's one heck of a snake-ladder loop. Of course if this player was flesh and
blood, they would have stopped when someone else won. And on average (using mode
as the average since this histogram is so skewed toward the right), players tend
to finish in about 14 rolls and one was able to finish in just 6 rolls!

Conclusion
==========

Given that goroutines require some state, you can not run one billion of them
on a MacBook. If you are playing snakes and ladders and you've just passed your
14th turn (and your not finished) you're on the loosing side of the histogram.

I don't have plans to update my Go code to figure out what loops players are or
are not ending up in but I'm very interested in hearing if anyone else discovers
these pieces of info.

Lastly, I'm very open to hearing critiques about my Go code. I'm still learning
and when I'm learning I like to hear opinions. Feel free.


Footnotes
---------

.. [#] Spread = Distribution of number of roles required to complete game.

