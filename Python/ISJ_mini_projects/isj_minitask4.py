# minitask 4
mcase = {'a': 10, 'b': 34, 'A': 7, 'Z': 3}
wanted = {'a': 17, 'b': 34, 'z': 3}

out = {}

for key, value in mcase.items():
    # if we have key in dict, add to value, else insert new key with value
    if out.get(key.lower()):
        out[key.lower()] += mcase[key]
    else:
        out[key.lower()] = mcase[key]
print(out)