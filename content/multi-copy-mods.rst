Does multiprocessing copy `sys.modules`?
########################################

:date: 2016-05-15 20:00:00
:categories: python multiprocessing sys.modules

I wanted to know if the `multiprocessing` library in Python copies the 
`sys.modules` `dict`.

.. code:: python

    from multiprocessing import Pool
    import os
    import sys

    def f(_x):
        pid = os.getpid()
        print(pid, id(sys))

    if __name__ == '__main__':
        with Pool(5) as p:
            p.map(f, [1, 2, 3])

.. parsed-literal::

    ➜  ~ python multi.py
    73924 4458196488
    73925 4458196488
    73926 4458196488

Looks like the location of the `sys` package is at the same location in memory
for each process. I wonder if we have to grab an actual module in `sys.modules`
to see a change in location ...

.. code:: python

    from multiprocessing import Pool
    import os
    import sys

    def f(_x):
        pid = os.getpid()
        print(pid, id(sys.modules['os']))

    if __name__ == '__main__':
        with Pool(5) as p:
            p.map(f, [1, 2, 3])

.. parsed-literal::

    ➜  ~ python multi.py
    74407 4366832248
    74408 4366832248
    74409 4366832248

Nope. No change. I wonder if there is some magic going on an all python
processes of a certain flavor share memory for the core packages in
`sys.modules`? Let's try importing something that is not automatically
populated in `sys.modules` on startup ...

.. code:: python

    from multiprocessing import Pool
    import os
    import sys
    import statistics

    def f(_x):
        pid = os.getpid()
        print(pid, id(sys.modules['statistics']))

    if __name__ == '__main__':
        with Pool(5) as p:
            p.map(f, [1, 2, 3])

.. parsed-literal::

    ➜  ~ python multi.py
    74550 4465539176
    74551 4465539176
    74552 4465539176

Nope. `sys.modules['statistics']` shares the same location in memory too.
Is that because we imported it in the first python process? If we were
to import `statistics` in a "pooled" python process, would we get a different
location in memory?

.. code:: python

    from multiprocessing import Pool
    import os
    import sys

    def f(_x):
        import statistics
        pid = os.getpid()
        print(pid, id(sys.modules['statistics']))

    if __name__ == '__main__':
        with Pool(5) as p:
            p.map(f, [1, 2, 3])

.. parsed-literal::

    ➜  ~ python multi.py
    74625 4526424936
    74624 4526424776
    74626 4526425096

Yes! Look at that! So a pool of Python processes share their "parent's" memory
until something needs to be added to that process's memory. Then, I assume that
the children ask for more personalized memory to contain new imports.
Now I wonder, can I force a change in the location of memory of a package
that one one of the pooled processes inherited?

.. code:: python

    from multiprocessing import Pool
    import os
    import sys
    import statistics

    def f(_x):
        pid = os.getpid()
        sys.modules['statistics'] = 12345678902345678
        print(pid, id(sys.modules['statistics']))

    if __name__ == '__main__':
        with Pool(5) as p:
            p.map(f, [1, 2, 3])

.. parsed-literal::

    ➜  ~ python multi.py
    75027 4388645936
    75028 4388645936
    75029 4388645936

Well, that didn't do it. But maybe it did. Maybe the
`sys.modules['statistics']` was changed for each pooled process equally?
Maybe if we used a different number for each process, we'd get different
locations in memory for `sys.modules['statistics']`. Let's see ...

.. code:: python

    from multiprocessing import Pool
    import os
    import sys
    import statistics

    def f(x):
        pid = os.getpid()
        n = 1234 ** x
        sys.modules['statistics'] = n
        print(pid, id(sys.modules['statistics']))

    if __name__ == '__main__':
        with Pool(5) as p:
            p.map(f, [1, 2, 3])

.. parsed-literal::

    ➜  ~ python multi.py
    75507 4388120016
    75508 4388120144
    75509 4388120080

Yup. Looks like copy on write. https://en.wikipedia.org/wiki/Copy-on-write

