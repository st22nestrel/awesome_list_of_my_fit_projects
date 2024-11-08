#!/usr/bin/env python3
# autor: Timotej Ponek xponek00
#minitask 7
def deprecated(func):
    def wrapper(x, y):
        print(f'Call to deprecated function: {func.__name__}')
        return print(func(x,y))
    return wrapper

@deprecated
def some_old_function(x, y):
    return x + y

some_old_function(1,2)

# should write:
# Call to deprecated function: some_old_function
# 3
