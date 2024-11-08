##
# file: dataStructs.py
# autor: Timotej Ponek xponek00
# IPP second assignment

"""Module implements:
    Binary tree for frame data representation
    Stack
    Function to convert string to given dataType
"""
from enum import IntEnum

class dType(IntEnum):
    """Enum representing data types
    """
    INT = 0
    FLOAT = 1
    STRING = 2
    BOOL = 3
    NIL = 4
    VAR = 5

class Node:
    """Class that represents binary tree
    """
    def __init__(self, name: None, dataType = None, value = None):
        """
        Args:
            name (string): name of variable
            dataType (enum): data type of value
            value (string)
        """
        self.lPtr = None
        self.rPtr = None
        self.name = name
        self.dataType = dataType
        self.value = value

    def insert(self, name: None, dataType = None, value = None):
        """Insert new element to binary tree

        Args:
            name (string): name of variable
            dataType (enum): data type of value
            value (string)
        
        Returns:
            int: 0 n success, error code on failure
        """
        search = self
        while search is not None and search.name is not None:
            if(name < search.name):
                if(search.lPtr is None):
                    toInsert = Node(name, dataType)
                    search.lPtr = toInsert
                    return 0
                search = search.lPtr
            elif(name > search.name):
                if(search.rPtr is None):
                    toInsert = Node(name, dataType)
                    search.rPtr = toInsert
                    return 0
                search = search.rPtr
            else:
                return 52 # redefinition of variable
        search.name = name
        search.dataType = dataType
        search.value = value
        return 0

    def actualize(self, name: None, dataType = None, value = None):
        """Actualize existing element in binary tree

        Args:
            name (string): name of variable
            dataType (enum): data type of value
            value (string)
        
        Returns:
            int: 0 n success, error code on failure
        """
        search = self
        while search is not None and search.name is not None:
            if(name == search.name):
                search.dataType = dataType
                search.value = value
                return 0
            if(name < search.name):
                search = search.lPtr
            elif(name > search.name):
                search = search.rPtr
        return 54 # asking for non-existing variable (frame exists)

    def search(self, name: str):
        """Search for element in binary tree

        Args:
            name (string): name of variable
            
        Returns:
            dataType (enum): data type of value, None on failure
            value: corresponding with dataType, if dataType is None then it is used as error code
        """
        toSearch = self
        while toSearch is not None and toSearch.name is not None:
            if(name == toSearch.name):
                return toSearch.dataType, toSearch.value
            elif(name < toSearch.name):
                toSearch = toSearch.lPtr
            elif(name > toSearch.name):
                toSearch = toSearch.rPtr
        return None, 54

def toType(dataType, value):
    """Convert string value to corresponding dataType value

    Args:
        dataType (enum): expected data type of value
        value (string)
        
    Returns:
        None on failure, otherwise converted value
    """
    if(dataType == dType.INT):
        if(value == ""):
            return 0
        else:
            return int(value)
    elif(dataType == dType.FLOAT):
        return float.fromhex(value)
    elif(dataType == dType.STRING):
        return value if value else ""
    elif(dataType == dataType.BOOL):
        if(value == "true"):
            return True
        else:
            return False
    else:
        return None
    
class Stack:
    """Class that represents stack operations
    """
    def __init__(self):
        self.dataType = list()
        self.value = list()
   
    def push(self, dataType, value, cast = False):
        """ Pushes value and dataType to stack

        Args:
            dataType (enum): data type of value
            value
            cast (bool): set this on false if you are insering converted value (value from variable)
                         otherwise true (value from XML)

        Returns:
            int: 0 (always succeeds)
        """
        self.dataType.append(dataType)
        if(cast):
            self.value.append(toType(dataType, value))
        else:
            self.value.append(value)
        return 0

    def topAndPop(self):
        """ Pops from stack

        Returns:
            dataType (enum): data type of value, on failure None
            value: value, on failure None
        """
        index = len(self.dataType)
        if(index != 0):
            return self.dataType.pop(index-1), self.value.pop(index-1)
        else:
            return None, None

    def isEmpty(self):
        """ Check if stack is empty
        
        Returns:
            bool
        """
        return len(self.dataType) == 0
