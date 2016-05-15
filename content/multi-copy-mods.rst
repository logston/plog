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

.. code:: python

    from multiprocessing import Pool
    import os
    import sys
    import statistics

    def f(_x):
        pid = os.getpid()
        print(pid, id(sys.modules['statistics']))
        sys.modules['statistics'] = 12345678902345678
        print(pid, id(sys.modules['statistics']))

    if __name__ == '__main__':
        with Pool(5) as p:
            p.map(f, [1, 2, 3])

.. parsed-literal::

    ➜  ~ python multi.py
    75254 4324259864
    75254 4322978864
    75255 4324259864
    75255 4322978864
    75256 4324259864
    75256 4322978864

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


Looks like copy on write. https://en.wikipedia.org/wiki/Copy-on-write
