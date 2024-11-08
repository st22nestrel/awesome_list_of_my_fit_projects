##
# file: interpret.py
# autor: Timotej Ponek xponek00
# IPP second assignment

import xml.etree.ElementTree as ET
from parsePy import parseInstructions, labelStack, stderr
import dataStructs
from dataStructs import dType, toType
import re
from sys import stdin

import argparse

# setup program arguments
parser = argparse.ArgumentParser(description='Interpret of IPPcode21', add_help=False)
parser.add_argument('--help', action="store_true",
                   help='Print this')
parser.add_argument('--source', nargs='?', metavar='file',
                   help='Input file with XML representation of code, stdin default')
parser.add_argument('--input', nargs='?', metavar='file',
                   help='File from where expected input will be loaded, stdin default')

types = {"int": dType.INT, "float": dType.FLOAT, "string": dType.STRING,
         "bool": dType.BOOL, "nil": dType.NIL, "var": dType.VAR}

xml = None
inFile = None

args, _ = parser.parse_known_args()

if(args.help):
    if(args.input or args.source):
        exit(10)
    else:
        parser.print_help()
        exit(0)

if(args.source == args.input):
    exit(10)
try:
    if(args.source == None or args.source == ""):
        xml = ET.parse(stdin)
    else:
        xml = ET.parse(args.source)
except:
    exit(31)


# bad XML format of input file
root = xml.getroot()
header = root.attrib.get('language')

if(header and header.casefold() == "ippcode21"):
    pass
else:
    exit(32)

header = None
try:
    # adopted from https://stackoverflow.com/a/47097775
    sortedInstructions = sorted(root, key=lambda child: (child.tag, int(child.attrib['order'])))
except:
    exit(32) # missing tag or order (also wrong order)

parseCode = parseInstructions(sortedInstructions)
if(parseCode != 0):
    exit(parseCode)

if(args.input == None or args.input == ""):
    pass
else:
    inFile = open(args.input, 'r')

##########################################################################################
"""Functions bellow insert new variable to corresponding frame

Args:
    name (string): variable name

Returns:
    int: 0 on success, error code on failure
"""
def insertGF(name):
    return globalSymbols.insert(name)

def insertLF(name):
    return localSymbols.insert(name) if localSymbols else 55 # non-existing frame

def insertTF(name):
    return tempSymbols.insert(name) if tempSymbols else 55 # non-existing frame

"""Functions bellow actualize value of existing variable in corresponding frame

Args:
    name (string): variable name
    typeD (enum): data type of value
    value (string)

Returns:
    int: 0 on success, error code on failure
"""
def actualizeGF(name, typeD, value):
    return globalSymbols.actualize(name, typeD, value)

def actualizeLF(name, typeD, value):
    return localSymbols.actualize(name, typeD, value) if localSymbols else (55)

def actualizeTF(name, typeD, value):
    return tempSymbols.actualize(name, typeD, value) if tempSymbols else (55)

"""Functions bellow get the variable value and data type from corresponding frame
on failure exit

Args:
    name (string): variable name

Returns:
    typeD (enum): data type of value
    value : value of data type corresponding with typeD
"""
def getGF(name):
    typeD, val = globalSymbols.search(name)
    if(typeD is None and val is not None):
        exit(val) # accessing non-existing variable (frame exists)
    return typeD, val

def getLF(name):
    typeD, val = localSymbols.search(name) if localSymbols else exit(55) # non-existing frame
    if(typeD is None and val is not None):
        exit(val) # accessing non-existing variable (frame exists)
    return typeD, val

def getTF(name):
    typeD, val = tempSymbols.search(name) if tempSymbols else exit(55) # non-existing frame
    if(typeD is None and val is not None):
        exit(val) # accessing non-existing variable (frame exists)
    return typeD, val

def getVarORSymbVals(dataType, val):
    """Gets real data and dataType of given val

    Args:
        dataType (enum)
        val (string): symbol or variable name

    Returns:
        dataType (enum): data type of value
        value : value of symbol or variable with data type corresponding with typeD
    """
    if(dataType == dType.VAR):
        return getSymb[val[0:2]](val[3:]) 
    else:
        return dataType, toType(dataType, val)

############################################################################################

"""Functions bellow executes IPPcode21 instructions 
and increments program counter (except jump instructions)

Returns:
    int: 0 on success, error code on failure
"""
def DEFVAR():
    global order, argNum
    order += 1
    arg1Frame = argNum[0].text[0:2]
    arg1Name = argNum[0].text[3:]
    # Insert to corresponding symbol table
    return insertSymb[arg1Frame](arg1Name)

def MOVE():
    global order, argNum
    order += 1
    arg1Frame = argNum[0].text[0:2]
    arg1Name = argNum[0].text[3:]
    arg2Text = argNum[1].text
    arg2Type = types[argNum[1].attrib['type']]

    arg2Type, arg2Val = getVarORSymbVals(arg2Type, arg2Text)

    if(arg2Type is None):
        return 56
    else:
        return actualizeSymb[arg1Frame](arg1Name, arg2Type, arg2Val)

def WRITE():
    global order, argNum
    arg1type = types[argNum[0].attrib['type']]
    arg1text = argNum[0].text
    order += 1
    # get from global symbol table
    varType, arg1text = getVarORSymbVals(arg1type, arg1text)

    if(varType == None):
        return 56 # missing value (in variable)
    else:
        if(varType == dType.FLOAT):
            arg1text = float.hex(arg1text)
        elif(varType == dType.BOOL):
            if(arg1text):
                arg1text = "true"
            else:
                arg1text = "false"
        elif(varType == dType.NIL):
            arg1text = ""

    print(arg1text, end='')
    return 0

def CREATEFRAME():
    global tempSymbols, order
    order += 1
    tempSymbols = dataStructs.Node(None, None)
    return 0

def PUSHFRAME():
    global tempSymbols, localSymbols, order
    order += 1
    frames.append(localSymbols)
    if(tempSymbols):
        localSymbols = tempSymbols
        tempSymbols = None
        return 0
    else:
        return 55 # non-existing frame

def POPFRAME():
    global tempSymbols, localSymbols, order
    order += 1
    tempSymbols = localSymbols
    frameSize = len(frames)
    localSymbols = frames.pop(frameSize-1) if frameSize else None
    return 0 if tempSymbols else 55 # non-existing frame

def CALL():
    global order
    order += 1
    callStack.append(order)
    arg1label = argNum[0].text
    
    order = labelStack.get(arg1label)
    if(order is None):
        return 52 # non-existent label
    else:
        return 0

def LABEL():
    global order
    order += 1
    return 0

def JUMP():
    global order
    arg1Text = argNum[0].text
    
    toJump = labelStack.get(arg1Text)
    if(toJump):
        order = toJump
        return 0
    else:
        return 52 # non-existent label

def JUMPIFEQ():
    global order
    arg1Text = argNum[0].text
    arg2Type = types[argNum[1].attrib['type']]
    arg2Text = argNum[1].text
    arg3Type = types[argNum[2].attrib['type']]
    arg3Text = argNum[2].text
    
    op1type, op1 = getVarORSymbVals(arg2Type, arg2Text)
    op2type, op2 = getVarORSymbVals(arg3Type, arg3Text)

    if(op1type is None or op2type is None):
        return 56 # missing value
    elif(op1type == op2type):
        if(op1 == op2):
            toJump = labelStack.get(arg1Text)
            if(toJump):
                order = toJump
                return 0
            else:
                return 52 # non-existent label
        else:
            order += 1
            return 0
    elif(op1type == dType.NIL or op2type == dType.NIL):
        order += 1
        return 0
    else:
        return 53 # wrong op types

def JUMPIFNEQ():
    global order
    arg1Text = argNum[0].text
    arg2Type = types[argNum[1].attrib['type']]
    arg2Text = argNum[1].text
    arg3Type = types[argNum[2].attrib['type']]
    arg3Text = argNum[2].text
    
    op1type, op1 = getVarORSymbVals(arg2Type, arg2Text)
    op2type, op2 = getVarORSymbVals(arg3Type, arg3Text)

    if(op1type is None or op2type is None):
        return 56 # missing value
    elif(op1type == op2type):
        if(op1 != op2):
            toJump = labelStack.get(arg1Text)
            if(toJump):
                order = toJump
                return 0
            else:
                return 52 # non-existent label
        else:
            order += 1
            return 0
    elif(op1type == dType.NIL or op2type == dType.NIL):
        toJump = labelStack.get(arg1Text)
        if(toJump):
            order = toJump
            return 0
        else:
            return 52 # non-existent label
    else:
        return 53 # wrong op types

def RETURN():
    global order
    callLen = len(callStack)
    if(callLen):
        order = callStack.pop( callLen - 1)
        return 0
    else:
        return 56 # CallStack empty

def PUSHS():
    global order
    order += 1
    arg1type = types[argNum[0].attrib['type']]
    if(arg1type == dType.VAR):
        varType, varVal = getVarORSymbVals(arg1type, argNum[0].text)
        if(varType is None):
            return 56 # missing value
        else:
            return dataStack.push(varType, varVal)
        pass
    else:
        arg1val = argNum[0].text
        return dataStack.push(arg1type, arg1val, True)

def POPS():
    global order
    order += 1
    arg1Text = argNum[0].text
    if(dataStack.isEmpty()):
        return 56 # missing value
    else:
        varType, varVal = dataStack.topAndPop()
        return actualizeSymb[arg1Text[0:2]](arg1Text[3:], varType, varVal)

def ADD():
    global order
    order += 1
    arg1Text = argNum[0].text
    arg2Type = types[argNum[1].attrib['type']]
    arg2Text = argNum[1].text
    arg3Type = types[argNum[2].attrib['type']]
    arg3Text = argNum[2].text
    
    op1type, op1 = getVarORSymbVals(arg2Type, arg2Text)
    op2type, op2 = getVarORSymbVals(arg3Type, arg3Text)

    if(op1type == op2type and (op1type == dType.INT or op1type == dType.FLOAT)):
        result = op1 + op2
        return actualizeSymb[arg1Text[0:2]](arg1Text[3:], op1type, result)
    elif(op1type is None or op2type is None):
        return 56 # missing value
    else:
        return 53 # wrong operand types

def SUB():
    global order
    order += 1
    arg1Text = argNum[0].text
    arg2Type = types[argNum[1].attrib['type']]
    arg2Text = argNum[1].text
    arg3Type = types[argNum[2].attrib['type']]
    arg3Text = argNum[2].text
    
    op1type, op1 = getVarORSymbVals(arg2Type, arg2Text)
    op2type, op2 = getVarORSymbVals(arg3Type, arg3Text)

    if(op1type == op2type and (op1type == dType.INT or op1type == dType.FLOAT)):
        result = op1 - op2
        return actualizeSymb[arg1Text[0:2]](arg1Text[3:], op1type, result)
    elif(op1type is None or op2type is None):
        return 56 # missing value
    else:
        return 53 # wrong operand types

def MUL():
    global order
    order += 1
    arg1Text = argNum[0].text
    arg2Type = types[argNum[1].attrib['type']]
    arg2Text = argNum[1].text
    arg3Type = types[argNum[2].attrib['type']]
    arg3Text = argNum[2].text
    
    op1type, op1 = getVarORSymbVals(arg2Type, arg2Text)
    op2type, op2 = getVarORSymbVals(arg3Type, arg3Text)

    if(op1type == op2type and (op1type == dType.INT or op1type == dType.FLOAT)):
        result = op1 * op2
        return actualizeSymb[arg1Text[0:2]](arg1Text[3:], op1type, result)
    elif(op1type is None or op2type is None):
        return 56 # missing value
    else:
        return 53 # wrong operand types

def IDIV():
    global order
    order += 1
    arg1Text = argNum[0].text
    arg2Type = types[argNum[1].attrib['type']]
    arg2Text = argNum[1].text
    arg3Type = types[argNum[2].attrib['type']]
    arg3Text = argNum[2].text
    
    op1type, op1 = getVarORSymbVals(arg2Type, arg2Text)
    op2type, op2 = getVarORSymbVals(arg3Type, arg3Text)

    if(op1type == op2type == dType.INT):
        if(op2 != 0):
            result = op1 // op2
            return actualizeSymb[arg1Text[0:2]](arg1Text[3:], dType.INT, result)
        else:
            return 57 # Division by zero
    elif(op1type is None or op2type is None):
        return 56 # missing value
    else:
        return 53 # wrong operand types

def DIV():
    global order
    order += 1
    arg1Text = argNum[0].text
    arg2Type = types[argNum[1].attrib['type']]
    arg2Text = argNum[1].text
    arg3Type = types[argNum[2].attrib['type']]
    arg3Text = argNum[2].text
    
    op1type, op1 = getVarORSymbVals(arg2Type, arg2Text)
    op2type, op2 = getVarORSymbVals(arg3Type, arg3Text)

    if(op1type == op2type == dType.FLOAT):
        if(op2 != 0):
            result = op1 // op2
            return actualizeSymb[arg1Text[0:2]](arg1Text[3:], dType.FLOAT, result)
        else:
            return 57 # Division by zero
    elif(op1type is None or op2type is None):
        return 56 # missing value
    else:
        return 53 # wrong operand types
    
def LT():
    global order
    order += 1
    arg1Text = argNum[0].text
    arg2Type = types[argNum[1].attrib['type']]
    arg2Text = argNum[1].text
    arg3Type = types[argNum[2].attrib['type']]
    arg3Text = argNum[2].text
    
    op1type, op1 = getVarORSymbVals(arg2Type, arg2Text)
    op2type, op2 = getVarORSymbVals(arg3Type, arg3Text)

    if(op1type is None or op2type is None):
        return 56 # missing value
    elif(op1type == op2type and op1type != dType.NIL):
        result = op1 < op2
        return actualizeSymb[arg1Text[0:2]](arg1Text[3:], dType.BOOL, result)
    else:
        return 53 # wrong operand types

def GT():
    global order
    order += 1
    arg1Text = argNum[0].text
    arg2Type = types[argNum[1].attrib['type']]
    arg2Text = argNum[1].text
    arg3Type = types[argNum[2].attrib['type']]
    arg3Text = argNum[2].text
    
    op1type, op1 = getVarORSymbVals(arg2Type, arg2Text)
    op2type, op2 = getVarORSymbVals(arg3Type, arg3Text)

    if(op1type is None or op2type is None):
        return 56 # missing value
    elif(op1type == op2type and op1type != dType.NIL):
        result = op1 > op2
        return actualizeSymb[arg1Text[0:2]](arg1Text[3:], dType.BOOL, result)
    else:
        return 53 # wrong operand types

def EQ():
    global order
    order += 1
    arg1Text = argNum[0].text
    arg2Type = types[argNum[1].attrib['type']]
    arg2Text = argNum[1].text
    arg3Type = types[argNum[2].attrib['type']]
    arg3Text = argNum[2].text
    
    op1type, op1 = getVarORSymbVals(arg2Type, arg2Text)
    op2type, op2 = getVarORSymbVals(arg3Type, arg3Text)


    if(op1type is None or op2type is None):
        return 56 # missing value
    elif(op1type == op2type):
        result = op1 == op2
        return actualizeSymb[arg1Text[0:2]](arg1Text[3:], dType.BOOL, result)
    elif(op1type == dType.NIL or op2type == dType.NIL):
        return actualizeSymb[arg1Text[0:2]](arg1Text[3:], dType.BOOL, False)
    else:
        return 53 # wrong operand types

def AND():
    global order
    order += 1
    arg1Text = argNum[0].text
    arg2Type = types[argNum[1].attrib['type']]
    arg2Text = argNum[1].text
    arg3Type = types[argNum[2].attrib['type']]
    arg3Text = argNum[2].text
    
    op1type, op1 = getVarORSymbVals(arg2Type, arg2Text)
    op2type, op2 = getVarORSymbVals(arg3Type, arg3Text)

    if(op1type is None or op2type is None):
        return 56 # missing value
    elif(op1type == op2type and op1type == dType.BOOL):
        result = op1 and op2
        return actualizeSymb[arg1Text[0:2]](arg1Text[3:], dType.BOOL, result)
    else:
        return 53 # wrong operand types

def OR():
    global order
    order += 1
    arg1Text = argNum[0].text
    arg2Type = types[argNum[1].attrib['type']]
    arg2Text = argNum[1].text
    arg3Type = types[argNum[2].attrib['type']]
    arg3Text = argNum[2].text
    
    op1type, op1 = getVarORSymbVals(arg2Type, arg2Text)
    op2type, op2 = getVarORSymbVals(arg3Type, arg3Text)

    if(op1type is None or op2type is None):
        return 56 # missing value
    elif(op1type == op2type and op1type == dType.BOOL):
        result = op1 or op2
        return actualizeSymb[arg1Text[0:2]](arg1Text[3:], dType.BOOL, result)
    else:
        return 53 # wrong operand types

def NOT():
    global order
    order += 1
    arg1Text = argNum[0].text
    arg2Type = types[argNum[1].attrib['type']]
    arg2Text = argNum[1].text
    
    op1type, op1 = getVarORSymbVals(arg2Type, arg2Text)

    if(op1type == dType.BOOL):
        result = not op1
        return actualizeSymb[arg1Text[0:2]](arg1Text[3:], dType.BOOL, result)
    elif(op1type is None):
        return 56 # missing value in variable
    else:
        return 53 # wrong operand types

def INT2CHAR():
    global order
    order += 1
    arg1Text = argNum[0].text
    arg2Type = types[argNum[1].attrib['type']]
    arg2Text = argNum[1].text
    
    op1type, op1 = getVarORSymbVals(arg2Type, arg2Text)

    if(op1type == dType.INT):
        if(-1 < op1 < 1_114_112):
            result = chr(op1)
            return actualizeSymb[arg1Text[0:2]](arg1Text[3:], dType.STRING, result)
        else:
            return 58 # invalid unicode symbol
    elif(op1type == None):
        return 56 # missing value in variable
    else:
        return 53 # wrong operand types

def STRI2INT():
    global order
    order += 1
    arg1Text = argNum[0].text
    arg2Type = types[argNum[1].attrib['type']]
    arg2Text = argNum[1].text
    arg3Type = types[argNum[2].attrib['type']]
    arg3Text = argNum[2].text
    
    op1type, op1 = getVarORSymbVals(arg2Type, arg2Text)
    op2type, op2 = getVarORSymbVals(arg3Type, arg3Text)

    if(op1type == dType.STRING and op2type == dType.INT):
        op1len = len(op1)
        if(op2 > -1 and op2 < op1len):
            result = ord(op1[op2])
            return actualizeSymb[arg1Text[0:2]](arg1Text[3:], dType.INT, result)
        else:
            return 58 # index outside string bounds
    elif(op1type == None or op2type == None):
        return 56 # missing value in variable
    else:
        return 53 # wrong operand types

def READ():
    global order
    order += 1
    arg1Text = argNum[0].text
    arg2Type = types[argNum[1].text]
    if(inFile):
        result = inFile.readline()
        if(result == ''):
            return actualizeSymb[arg1Text[0:2]](arg1Text[3:], dType.NIL, None)
        else:
            result = result.rstrip('\n')
    else:
        result = input()

    if(arg2Type == dType.INT):
        result = re.findall(r'^\s*[\+|\-]?\d+', result)
        if(result):
            return actualizeSymb[arg1Text[0:2]](arg1Text[3:], dType.INT, int(result[0]))
        else:
            return actualizeSymb[arg1Text[0:2]](arg1Text[3:], dType.NIL, None)
    elif(arg2Type == dType.FLOAT):
        try:
            result = float.fromhex(result)
            return actualizeSymb[arg1Text[0:2]](arg1Text[3:], dType.FLOAT, result)
        except ValueError:
            return actualizeSymb[arg1Text[0:2]](arg1Text[3:], dType.NIL, None)

    elif(arg2Type == dType.STRING):
        return actualizeSymb[arg1Text[0:2]](arg1Text[3:], dType.STRING, result)

    elif(arg2Type == dType.BOOL):
        if(result.lower() == "true"):
            return actualizeSymb[arg1Text[0:2]](arg1Text[3:], dType.BOOL, True)
        elif(result == ''):
            return actualizeSymb[arg1Text[0:2]](arg1Text[3:], dType.NIL, None)
        else:
            return actualizeSymb[arg1Text[0:2]](arg1Text[3:], dType.BOOL, False)
    
def CONCAT():
    global order
    order += 1
    arg1Text = argNum[0].text
    arg2Type = types[argNum[1].attrib['type']]
    arg2Text = argNum[1].text
    arg3Type = types[argNum[2].attrib['type']]
    arg3Text = argNum[2].text
    
    op1type, op1 = getVarORSymbVals(arg2Type, arg2Text)
    op2type, op2 = getVarORSymbVals(arg3Type, arg3Text)

    if(op1type == op2type == dType.STRING):
        result = op1 + op2
        return actualizeSymb[arg1Text[0:2]](arg1Text[3:], dType.STRING, result)
    elif(op1type is None or op2type is None):
        return 56 # missing value in variable
    else:
        return 53 # wrong operand types

def STRLEN():
    global order
    order += 1
    arg1Text = argNum[0].text
    arg2Type = types[argNum[1].attrib['type']]
    arg2Text = argNum[1].text
    
    op1type, op1 = getVarORSymbVals(arg2Type, arg2Text)

    if(op1type == dType.STRING):
        result = len(op1)
        return actualizeSymb[arg1Text[0:2]](arg1Text[3:], dType.INT, result)
    elif(op1type is None):
        return 56 # missing value in variable
    else:
        return 53 # wrong operand types

def GETCHAR():
    global order
    order += 1
    arg1Text = argNum[0].text
    arg2Type = types[argNum[1].attrib['type']]
    arg2Text = argNum[1].text
    arg3Type = types[argNum[2].attrib['type']]
    arg3Text = argNum[2].text
    
    op1type, op1 = getVarORSymbVals(arg2Type, arg2Text)
    op2type, op2 = getVarORSymbVals(arg3Type, arg3Text)

    if(op1type == dType.STRING and op2type == dType.INT):
        op1len = len(op1)
        if(op2 > -1 and op2 < op1len):
            result = op1[op2]
            return actualizeSymb[arg1Text[0:2]](arg1Text[3:], dType.STRING, result)
        else:
            return 58 # Index outside string bounds
    elif(op1type is None or op2type is None):
        return 56 # missing value in variable
    else:
        return 53 # wrong operand types

def SETCHAR():
    global order
    order += 1
    arg1Text = argNum[0].text
    arg1Type = types[argNum[0].attrib['type']]
    arg2Type = types[argNum[1].attrib['type']]
    arg2Text = argNum[1].text
    arg3Type = types[argNum[2].attrib['type']]
    arg3Text = argNum[2].text
    
    varType, var = getVarORSymbVals(arg1Type, arg1Text)
    op1type, op1 = getVarORSymbVals(arg2Type, arg2Text)
    op2type, op2 = getVarORSymbVals(arg3Type, arg3Text)

    if(varType == op2type == dType.STRING and op1type == dType.INT):
        varlen = len(var)
        if(op1 > -1 and op1 < varlen and len(op2) > 0):
            var = var[:op1] + op2[0] + var[op1 + 1:]
            return actualizeSymb[arg1Text[0:2]](arg1Text[3:], dType.STRING, var)
        else:
            return 58 # index outside string bounds
    elif(varType is None or op1type is None or op2type is None):
        return 56 # missing value in variable
    else:
        return 53 # wrong operand types

def INT2FLOAT():
    global order
    order += 1
    arg1Text = argNum[0].text
    arg2Type = types[argNum[1].attrib['type']]
    arg2Text = argNum[1].text
    
    op1type, op1 = getVarORSymbVals(arg2Type, arg2Text)

    if(op1type == dType.INT):
        return actualizeSymb[arg1Text[0:2]](arg1Text[3:], dType.FLOAT, float(op1))
    elif(op1type is None):
        return 56 # missing value in variable
    else:
        return 53 # wrong operand types

def FLOAT2INT():
    global order
    order += 1
    arg1Text = argNum[0].text
    arg2Type = types[argNum[1].attrib['type']]
    arg2Text = argNum[1].text
    
    op1type, op1 = getVarORSymbVals(arg2Type, arg2Text)

    if(op1type == dType.FLOAT):
        return actualizeSymb[arg1Text[0:2]](arg1Text[3:], dType.INT, int(op1))
    elif(op1type is None):
        return 56 # missing value in variable
    else:
        return 53 # wrong operand types

def CLEARS():
    global order, dataStack
    order += 1
    dataStack = dataStructs.Stack()
    return 0

def ADDS():
    global order, dataStack
    order += 1
    op2Type, op2 = dataStack.topAndPop()
    op1Type, op1 = dataStack.topAndPop()

    if(op1Type is not None):
        if(op1Type == op2Type and (op1Type == dType.INT or op1Type == dType.FLOAT)):
            result = op1 + op2
            return dataStack.push(op1Type, result)
        else:
            return 53 # wrong operand types
    else:
        return 56 # missing value in data stack

def SUBS():
    global order, dataStack
    order += 1
    op2Type, op2 = dataStack.topAndPop()
    op1Type, op1 = dataStack.topAndPop()

    if(op1Type is not None):
        if(op1Type == op2Type and (op1Type == dType.INT or op1Type == dType.FLOAT)):
            result = op1 - op2
            return dataStack.push(op1Type, result)
        else:
            return 53 # wrong operand types
    else:
        return 56 # missing value in data stack

def MULS():
    global order, dataStack
    order += 1
    op2Type, op2 = dataStack.topAndPop()
    op1Type, op1 = dataStack.topAndPop()

    if(op1Type is not None):
        if(op1Type == op2Type and (op1Type == dType.INT or op1Type == dType.FLOAT)):
            result = op1 * op2
            return dataStack.push(op1Type, result)
        else:
            return 53 # wrong operand types
    else:
        return 56 # missing value in data stack

def IDIVS():
    global order, dataStack
    order += 1
    op2Type, op2 = dataStack.topAndPop()
    op1Type, op1 = dataStack.topAndPop()

    if(op1Type is not None):
        if(op1Type == op2Type and op1Type == dType.INT):
            if(op2 != 0):
                result = op1 // op2
                return dataStack.push(op1Type, result)
            else:
                return 57 # Division by zero
        else:
            return 53 # wrong operand types
    else:
        return 56 # missing value in data stack

def DIVS():
    global order, dataStack
    order += 1
    op2Type, op2 = dataStack.topAndPop()
    op1Type, op1 = dataStack.topAndPop()

    if(op1Type is not None):
        if(op1Type == op2Type and op1Type == dType.FLOAT):
            if(op2 != 0):
                result = op1 // op2
                return dataStack.push(op1Type, result)
            else:
                return 57 # Division by zero
        else:
            return 53 # wrong operand types
    else:
        return 56 # missing value in data stack

def LTS():
    global order, dataStack
    order += 1
    op2Type, op2 = dataStack.topAndPop()
    op1Type, op1 = dataStack.topAndPop()

    if(op1Type is not None):
        if(op1Type == op2Type and op1Type != dType.NIL):
            result = op1 < op2
            return dataStack.push(dType.BOOL, result)
        else:
            return 53 # wrong operand types
    else:
        return 56 # missing value in data stack

def GTS():
    global order, dataStack
    order += 1
    op2Type, op2 = dataStack.topAndPop()
    op1Type, op1 = dataStack.topAndPop()

    if(op1Type is not None):
        if(op1Type == op2Type and op1Type != dType.NIL):
            result = op1 > op2
            return dataStack.push(dType.BOOL, result)
        else:
            return 53 # wrong operand types
    else:
        return 56 # missing value in data stack

def EQS():
    global order, dataStack
    order += 1
    op2Type, op2 = dataStack.topAndPop()
    op1Type, op1 = dataStack.topAndPop()

    if(op1Type is not None):
        if(op1Type == op2Type):
            result = op1 == op2
            return dataStack.push(dType.BOOL, result)
        elif(op1Type == dType.NIL or op2Type == dType.NIL):
            return dataStack.push(dType.BOOL, False)
        else:
            return 53 # wrong operand types
    else:
        return 56 # missing value in data stack

def ANDS():
    global order, dataStack
    order += 1
    op2Type, op2 = dataStack.topAndPop()
    op1Type, op1 = dataStack.topAndPop()

    if(op1Type is not None):
        if(op1Type == op2Type and op1Type == dType.BOOL):
            result = op1 and op2
            return dataStack.push(dType.BOOL, result)
        else:
            return 53 # wrong operand types
    else:
        return 56 # missing value in data stack

def ORS():
    global order, dataStack
    order += 1
    op2Type, op2 = dataStack.topAndPop()
    op1Type, op1 = dataStack.topAndPop()

    if(op1Type is not None):
        if(op1Type == op2Type and op1Type == dType.BOOL):
            result = op1 or op2
            return dataStack.push(dType.BOOL, result)
        else:
            return 53 # wrong operand types
    else:
        return 56 # missing value in data stack

def NOTS():
    global order, dataStack
    order += 1
    op1Type, op1 = dataStack.topAndPop()

    if(op1Type is not None):
        if(op1Type == dType.BOOL):
            result = not op1
            return dataStack.push(dType.BOOL, result)
        else:
            return 53 # wrong operand types
    else:
        return 56 # missing value in data stack

def INT2CHARS():
    global order, dataStack
    order += 1
    op1Type, op1 = dataStack.topAndPop()

    if(op1Type is not None):
        if(op1Type == dType.INT):
            if(-1 < op1 < 1_114_112):
                result = chr(op1)
                return dataStack.push(dType.STRING, result)
            else:
                return 58 # invalid unicode symbol
        else:
            return 53 # wrong operand types
    else:
        return 56 # missing value in data stack

def STRI2INTS():
    global order, dataStack
    order += 1
    op2Type, op2 = dataStack.topAndPop()
    op1Type, op1 = dataStack.topAndPop()

    if(op1Type is not None):
        if(op1Type == dType.STRING and op2Type == dType.INT):
            op1len = len(op1)
            if(op2 > -1 and op2 < op1len):
                result = ord(op1[op2])
                return dataStack.push(dType.INT, result)
            else:
                return 58 # index outside string bounds
        else:
            return 53 # wrong operand types
    else:
        return 56 # missing value in data stack
        
def INT2FLOATS():
    global order, dataStack
    order += 1
    op1type, op1 = dataStack.topAndPop()

    if(op1type is not None):
        if(op1type == dType.INT):
            return dataStack.push(dType.FLOAT, float(op1))
        else:
            return 53 # wrong operand types
    else:
        return 56 # missing value in data stack

def FLOAT2INTS():
    global order, dataStack
    order += 1
    op1type, op1 = dataStack.topAndPop()

    if(op1type is not None):
        if(op1type == dType.INT):
            return dataStack.push(dType.INT, int(op1))
        else:
            return 53 # wrong operand types
    else:
        return 56 # missing value in data stack

def JUMPIFEQS():
    global order, dataStack
    arg1Text = argNum[0].text
    op2Type, op2 = dataStack.topAndPop()
    op1Type, op1 = dataStack.topAndPop()

    if(op1Type is not None):
        if(op1Type == op2Type):
            if(op1 == op2):
                toJump = labelStack.get(arg1Text)
                if(toJump):
                    order = toJump
                    return 0
                else:
                    return 52 # non-existent label
            else:
                order += 1
                return 0
        elif(op1Type == dType.NIL or op2Type == dType.NIL):
            order += 1
            return 0
        else:
            return 53 # wrong operand types
    else:
        return 56 # missing value in data stack

def JUMPIFNEQS():
    global order, dataStack
    arg1Text = argNum[0].text
    op2Type, op2 = dataStack.topAndPop()
    op1Type, op1 = dataStack.topAndPop()

    if(op1Type is not None):
        if(op1Type == op2Type):
            if(op1 != op2):
                toJump = labelStack.get(arg1Text)
                if(toJump):
                    order = toJump
                    return 0
                else:
                    return 52 # non-existent label
            else:
                order += 1
                return 0
        elif(op1Type == dType.NIL or op2Type == dType.NIL):
            toJump = labelStack.get(arg1Text)
            if(toJump):
                order = toJump
                return 0
            else:
                return 52 # non-existent label
        else:
            return 53 # wrong operand types
    else:
        return 56 # missing value in data stack

def TYPE():
    global order
    order += 1
    arg1Text = argNum[0].text
    arg2Text = argNum[1].text
    arg2Type = types[argNum[1].attrib['type']]

    op1type, _ = getVarORSymbVals(arg2Type, arg2Text)

    if(op1type is None):
        return actualizeSymb[arg1Text[0:2]](arg1Text[3:], dType.STRING, "")
    else:
        result = op1type.name.lower()
        return actualizeSymb[arg1Text[0:2]](arg1Text[3:], dType.STRING, result)

def EXIT():
    global order
    order += 1
    arg1Text = argNum[0].text
    arg1Type = types[argNum[0].attrib['type']]

    op1type, op1 = getVarORSymbVals(arg1Type, arg1Text)
    if(op1type == dType.INT):
        exit(op1 if 0 <= op1 <50 else 57)
    elif(op1type is None):
        return 56 # missing value in data stack
    else:
        return 53 # wrong operand types

def DPRINT():
    global order
    order += 1
    arg1Text = argNum[0].text
    arg1Type = types[argNum[0].attrib['type']]

    op1type, op1 = getVarORSymbVals(arg1Type, arg1Text)

    if(arg1Type == "var" and op1type is not None):
        print(arg1Text, '=', op1, '(', op1type, ')', file=stderr)
        return 0
    if(op1type != None):
        print('Const@=', op1, '(', op1type, ')', file=stderr)
        return 0
    else:
        return 54 # accessing non-existing variable (frame exists)
    
def BREAK():
    global order
    order += 1

#Needed dictionaries
insertSymb = {"GF": insertGF, "LF": insertLF, "TF": insertTF}
actualizeSymb = {"GF": actualizeGF, "LF": actualizeLF, "TF": actualizeTF}
getSymb = {"GF": getGF, "LF": getLF, "TF": getTF}

#Global variables used during interpreting
order = 0
sizeOfInstr = len(sortedInstructions)
globalSymbols = dataStructs.Node(None, None)
frames = list()
callStack = list()
tempSymbols = None
localSymbols = None
dataStack = dataStructs.Stack()

#Loops through instructions
while(True and order < sizeOfInstr):
    instr = sortedInstructions[order].attrib['opcode']
    argNum = list(sortedInstructions[order])
    instrSuccess = locals()[instr]() # we can use locals, parsing ensured us that we will call only defined functions (representing opCodes)
    if (instrSuccess != 0):
        exit(instrSuccess)
        