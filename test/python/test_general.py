r'''>>> import general
    >>> general.f(1)
    1.0
    >>> general.f(x=2)
    2.0
    >>> general.f(1,y=2)
    0.5
    >>> general.f(x=4,z=2)
    4.0
    >>> general.f(y=2,x=4)
    2.0
    >>> general.f(z=0,y=2,x=4)
    2.0
'''

def run(args = None):
    if args is not None:
        import sys
        sys.argv = args
    import doctest, test_general
    return doctest.testmod(test_general)

if __name__ == '__main__':
    import sys
    sys.exit(run()[0])
