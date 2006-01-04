'''
>>> import accumulator_set
>>> acc = accumulator_set.accumulator_set(cache_size = 4)
>>> acc(2.1, covariate1 = 21)
>>> acc(1.1, covariate1 = 11)
>>> acc(2.1, covariate1 = 21)
>>> acc(1.1, covariate1 = 11)
>>> accumulator_set.covariate1(acc)
[21, 21, 11, 11]
>>> acc(21.1, covariate1 = 211)
>>> acc(11.1, covariate1 = 111)
>>> acc(21.1, covariate1 = 211)
>>> acc(11.1, covariate1 = 111)
>>> accumulator_set.covariate1(acc)
[211, 211, 111, 111]
>>> acc(42.1, covariate1 = 421)
>>> acc(41.1, covariate1 = 411)
>>> acc(42.1, covariate1 = 421)
>>> acc(41.1, covariate1 = 411)
>>> accumulator_set.covariate1(acc)
[421, 421, 411, 411]
>>> acc(32.1, covariate1 = 321)
>>> acc(31.1, covariate1 = 311)
>>> acc(32.1, covariate1 = 321)
>>> acc(31.1, covariate1 = 311)
>>> accumulator_set.covariate1(acc)
[421, 421, 411, 411]
'''

def run(args = None):
    if args is not None:
        import sys
        sys.argv = args
    import doctest, test_accumulator
    return doctest.testmod(test_accumulator)

if __name__ == '__main__':
    import sys
    sys.exit(run()[0])
