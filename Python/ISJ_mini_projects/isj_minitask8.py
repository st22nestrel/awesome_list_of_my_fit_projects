# minitask 8
# f is a general iterator, for example a file
f = open('students.txt')
it = f
header = next(f)
for line in it:
    line = line.rstrip()
    print(line)