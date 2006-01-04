r'''>>> import parameter
    >>> parameter.add(1,2)
    3
    >>> parameter.add(x=1,y=2)
    3
    >>> parameter.add(1,2,z=3)
    6
    >>> parameter.add(x=1,y=2,z=3,u=4)
    10
'''

def run(args = None):
    if args is not None:
        import sys
        sys.argv = args
    import doctest, test_simple
    return doctest.testmod(test_simple)

if __name__ == '__main__':
    import sys
    sys.exit(run()[0])
