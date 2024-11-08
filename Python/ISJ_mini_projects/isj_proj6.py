#!/usr/bin/env python3
# autor: Timotej Ponek xponek00

class Polynomial:
    """Class that represents polynomial
    """
    def __init__(self, *args, **kwargs):
        """Example of arguments given to construct
        polynom x + x^2

        Args:
            args: "0, 1, 1" or "[0, 1, 1]"
            kwargs: "x1 = 1, x2 = 1" for
        """
        if len(kwargs) == 0:
            if isinstance(args[0], list):
                self.polynom = args[0]
            else:
                self.polynom = []
                for x in args:
                    self.polynom.append(x)
        else:
            self.polynom = []
            for x, val in kwargs.items():
                index = int(x[1:]) # get numerical index of x
                self.polynom.extend([0] * (1 + index - len(self.polynom))) # extend array if needed -> index basically does not exist
                self.polynom[index] = val # insert value to already existing index

    
    def __repr__(self):
        text = str()
        polynom_len = len(self.polynom) - 1
        if polynom_len < 0:
            return "0"

        for index, value in reversed(list(enumerate((self.polynom)))):
            if value == 0:
                continue
            text += f'{str(value) if value < 0 else " + "+str(value)}' + (
                'x^' + str(index) if index > 1
                else 'x' if index == 1
                else ''
            )
        
        if text == '':
            return "0"

        text = text.replace('-', ' - ').replace(' 1x', ' x')
        return text[1:] if text[1] == ' - ' else text[3:]


    def __eq__(self, other: object) -> bool:
        max_len = max(len(self.polynom), len(other.polynom))
        self_len = len(self.polynom)
        other_len = len(other.polynom)
        for i in range(max_len):
            val_self = self.polynom[i] if i < self_len else 0
            val_other = other.polynom[i] if i < other_len else 0
            if val_self != val_other:
                return False
        return True


    def __add__(self, other: object):
        add_result = []
        self_len = len(self.polynom)
        other_len = len(other.polynom)
        for i in range(max(self_len, other_len)):
            val1 = self.polynom[i] if i < self_len else 0
            val2 = other.polynom[i] if i < other_len else 0
            add_result.append(val1 + val2)
        return Polynomial(add_result)


    def __mul__(self, other: object):
        result = [0] * (len(self.polynom) + len(other.polynom) - 1)
        for x in range(len(self.polynom)):
            for y in range(len(other.polynom)):
                result[x+y] += self.polynom[x] * other.polynom[y]
        return Polynomial(result)


    def __pow__(self, power: int):
        if power < 0:
            return
        if power == 0:
            return Polynomial(1)
        if power == 1:
            return self
        result = self
        for i in range(2, power + 1):
            result = result * self  
        return result


    def derivative(self):
        """Derivates polynomial on which is called
        """
        res = [0] * len(self.polynom)
        for index, value in enumerate(self.polynom):
            res[index] = index * value
        res.pop(0)
        return Polynomial(res)


    def at_value(self, x, y = None):
        """Gives value for polynomial.
        When both 'x' and 'y' are present, gives 
        result of substraction of polynomial at value 'y'
        from polynomial at value 'x'.

        Args:
            x (int)
            y (int)
            
        """
        result = 0
        if not y:
            for i in range(len(self.polynom)):
                result += self.polynom[i] * x ** i
        else:
            for i in range(len(self.polynom)):
                result += self.polynom[i] * y ** i - self.polynom[i] * x ** i
        return result
