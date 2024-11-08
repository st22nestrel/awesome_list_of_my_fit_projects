#!/usr/bin/env python3
# autor: Timotej Ponek xponek00

def first_with_given_key(iterable, key = lambda x: x):
    to_return = set()
    for item in iterable:
        if key(item) not in to_return:
            to_return.add(key(item))
            yield item

if __name__ == '__main__':
    print(tuple(first_with_given_key([[1],[2,3],[4],[5,6,7],[8,9]], key = len))) #vypíše ([1], [2, 3], [5, 6, 7])
