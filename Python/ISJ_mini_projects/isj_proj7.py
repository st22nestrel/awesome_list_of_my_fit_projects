#!/usr/bin/env python3
# autor: Timotej Ponek xponek00

import collections

my_counter = collections.Counter()

def log_and_count(counts, key = None):
    def decorate(func):
        def wrapper(*args, **kwars):
            if key is None:
                counts[func.__name__] += 1
            else:
                counts[key] += 1
            func(*args, **kwars)
            print(f'called {func.__name__} with {args} and {kwars}')
            #return func(*args, **kwars)
        return wrapper
    return decorate


@log_and_count(key='basic functions', counts=my_counter)
def f1(a, b=2):
    return a ** b

@log_and_count(key='basic functions', counts=my_counter)
def f2(a, b=3):
    return a ** 2 + b


@log_and_count(counts=my_counter)
def f3(a, b=5):
    return a ** 3 - b

f1(2)
f2(2, b=4)
f1(a=2, b=4)
f2(4)
f2(5)
f3(5)
f3(5,4)

print(my_counter)
