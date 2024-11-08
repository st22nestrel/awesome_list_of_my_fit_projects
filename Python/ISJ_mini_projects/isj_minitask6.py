# minitask 6
class Point:
    def __init__(self, *args) -> None:
        if len(args) == 0:
            self.x1 = 0
            self.x2 = 0
        else:
            self.x1 = args[0]
            self.x2 = args[1]

    def __repr__(self) -> str:
        return f'Point: [{self.x1}, {self.x2}]'
    
    def __sub__(self, other: object):
        x1 = self.x1 - other.x1
        x2 = self.x2 - other.x2
        return Point(x1, x2)

p0 = Point()
p1 = Point(3, 4)
print(p0-p1)
p2 = Point(1, 2)
result = p1-p2
print(result)