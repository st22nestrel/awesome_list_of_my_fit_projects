##
# file: parsePy.py
# autor: Timotej Ponek xponek00
# IPP second assignment

"""Module implements:
    parseInstructions: function that parses instructions in XML sorted in list
    labelStack: dict of all valid labels found in instructions
    
    other functions are helpers, reuse them with caution
"""

import xml.etree.ElementTree as ET
import re
from sys import stderr
from html import unescape

labelStack = dict()
i = 0

def addDuplicate(listed, item):
    """Checks if instruction with given order atribute wasn't already parsed

    Args:
        listed (list): all parsed order attributes 
        item (int): order atribbute

    Returns:
        bool
    """
    if item in listed:
        return False
    else:
        listed.append(item)
        return True

def parseInstructions(instructions):
    """Parses XML instruction of IPPcode21

    Args:
        instructions (list)

    Returns:
        int: 0 on success, error code on failure
    """
    global i
    allOrders = list()
    instrSize = len(instructions)
    while i < instrSize:
        order = int(instructions[i].attrib['order'])
        if(order > 0  and addDuplicate(allOrders, order) and instructions[i].tag == 'instruction'):
            try:
                instructions[i].attrib['opcode'] = instructions[i].attrib['opcode'].upper()
                chooseFnc = opCode.get(instructions[i].attrib['opcode'])
            except:
                exit(32)
            # sort arguments and remove unsorted
            children = instructions[i].getchildren()
            ducks = sorted(children, key=lambda child: (child.tag))
            children = [instructions[i].remove(child) for child in children]
            instructions[i].extend(ducks)
            if(chooseFnc and chooseFnc(instructions[i]) == 0):
                i += 1
                continue
            else:
                return 32 # wrong instruction
        else:
            # we do not allow negative order values
            return 32
    return 0

def ReturnArgs(instr, nmb):
    """Returns list of args-tags from all elements in instruction tag

    Args:
        instr (list): list of arguments
        nmb (int): expected number of arguments in list

    Returns:
        int: 0 on success, error code on failure
    """
    counter = 1
    toReturn = []
    argsLength = len(instr)
    if(argsLength > nmb or argsLength < nmb):
        print("Error unexpected number of aguments", file=stderr)
        exit(32)

    while counter <= nmb:
        for child in instr:
            if(child.tag == ("arg"+str(counter))):
                toReturn.append(child)
            if(len(child.getchildren())):
                exit(32)
        if(len(toReturn) < counter):
            print("Error missing argument", file=stderr)
            exit(32)
        else:
            counter += 1
    return toReturn        

"""Functions bellow check if the given string val is valid coresponding type

Args:
    instr(Element): 

Returns:
    int: 1 on failure, 0 on success
"""
def VarSymb(instr):
    child = instr.getchildren()
    Args = ReturnArgs(child, 2)
    if(CheckVar(Args[0])):
        if(CheckSymb(Args[1])):
            return 0
    return 1

def Var(instr):
    child = instr.getchildren()
    Args = ReturnArgs(child, 1)
    if(CheckVar(Args[0])):
        return 0
    return 1

def Symb(instr):
    child = instr.getchildren()
    Args = ReturnArgs(child, 1)
    if(CheckSymb(Args[0])):
        return 0
    return 1

def Label(instr):
    global i
    child = instr.getchildren()
    Args = ReturnArgs(child, 1)
    if(CheckLabel(Args[0])):
        if(instr.attrib['opcode'] == "LABEL"):
            if(labelStack.get(Args[0].text) == None):
                labelStack[Args[0].text] = i
            else:
                print("Duplicate label", file=stderr)
                exit(52)
        return 0
    return 1

def Var2Symb(instr):
    child = instr.getchildren()
    Args = ReturnArgs(child, 3)
    if(CheckVar(Args[0])):
        if(CheckSymb(Args[1])):
            if(CheckSymb(Args[2])):
                return 0
    return 1

def VarType(instr):
    child = instr.getchildren()
    Args = ReturnArgs(child, 2)
    if(CheckVar(Args[0])):
        if(CheckType(Args[1])):
            return 0
    return 1

def Label2Symb(instr):
    child = instr.getchildren()
    Args = ReturnArgs(child, 3)
    if(CheckLabel(Args[0])):
        if(CheckSymb(Args[1])):
            if(CheckSymb(Args[2])):
                return 0
    return 1

def opInstr(instr):
    return 0


#&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&#

def CheckSymb(val):
    """Check if given string value is valid symbol

    Args:
        val (string)

    Returns:
        bool
    """
    toValidate = types.get(val.attrib["type"])
    if(toValidate and toValidate(val)):
        return True
    else:
        return False

#############################################################################

"""Functions bellow check if the given string val is valid coresponding data type

Args:
    val (string)

Returns:
    bool
"""
def validInt(val):
    if(val.text == None or re.match(r"^[\+|\-]?[0-9]*$", val.text)):
        return True
    else:
        return False

def validFloat(val):
    # we check for valid conversion to float when interpreting, no way to make valid regex
    return True

def validString(val):
    if(val.text == None):
        return True
    elif (re.match(r"^([^\x00-\x20\x23\x5C]*(\x5C(?:[0-1][0-9]{2}|2[0-4][0-9]|25[0-5]|\d{0,2}$|$))?)*$", val.text, re.U)):
        val.text = unescape(val.text)
        val.text = re.sub(r'\x5C\d{0,2}$', "", val.text) #strip not complete escape sequences
        val.text = re.sub(r'\x5C\d{3}', lambda res: chr(int(res.group().strip("\x5C"))), val.text) #replace escape sequences with ascii
        return True
    else:
        return False

def validBool(val):
    if(val.text and re.match(r"^true|false$", val.text,)):
        return True
    else:
        return False

def validNil(val):
    if(val.text and re.match(r"^(nil)?$", val.text,)):
        return True
    else:
        return False

############################################################################

"""Functions bellow check if the given string val is valid coresponding type

Args:
    val (string)

Returns:
    bool
"""
def CheckVar(val):
    if( val.text and re.match(r"^[LTG]F@[a-zA-Z_\-$&%*!?][a-zA-Z0-9_\-$&%*!?]*$", val.text, re.I)
        and val.attrib["type"] == "var"):
        return True
    else:
        return False

def CheckLabel(val):
    if( val.text and re.match(r"^[a-zA-Z_\-$&%*!?][a-zA-Z0-9_\-$&%*!?]*$", val.text, re.I)
        and val.attrib["type"] == "label"):
        return True
    else:
        return False

def CheckType(val):
    if( val.text and re.match(r"^(int|float|bool|string)$", val.text, re.I)
        and val.attrib["type"] == "type"):
        return True
    else:
        return False

#dictionary for opcodes
opCode = {"MOVE": VarSymb, "NOT": VarSymb, "INT2CHAR": VarSymb, "STRLEN": VarSymb, "TYPE" : VarSymb, "INT2FLOAT": VarSymb, "FLOAT2INT": VarSymb,
            "DEFVAR": Var, "POPS": Var,
            "RETURN": opInstr, "CREATEFRAME": opInstr, "PUSHFRAME": opInstr, "POPFRAME": opInstr, "BREAK" : opInstr,
            "CLEARS" : opInstr, "ADDS" : opInstr, "SUBS" : opInstr, "MULS" : opInstr, "IDIVS" : opInstr, "DIVS" : opInstr,
            "LTS" : opInstr, "GTS" : opInstr, "EQS" : opInstr, "ANDS" : opInstr, "ORS" : opInstr, "NOTS" : opInstr,
            "INT2CHARS" : opInstr, "STRI2INTS" : opInstr,
            "PUSHS": Symb, "WRITE": Symb, "EXIT": Symb, "DPRINT" : Symb,
            "ADD": Var2Symb, "SUB": Var2Symb, "MUL": Var2Symb, "IDIV": Var2Symb, "LT": Var2Symb, "GT": Var2Symb, "EQ": Var2Symb, "AND": Var2Symb,
            "OR": Var2Symb, "STRI2INT": Var2Symb, "CONCAT": Var2Symb, "GETCHAR": Var2Symb, "SETCHAR" : Var2Symb, "DIV": Var2Symb,
            "READ": VarType,
            "CALL": Label, "JUMP": Label, "LABEL" : Label, "JUMPIFEQS" : Label, "JUMPIFNEQS" : Label,
            "JUMPIFEQ": Label2Symb, "JUMPIFNEQ" : Label2Symb
            }
#dictionary for dataTypes
types = {"int": validInt, "float": validFloat, "string": validString, "bool": validBool, "nil": validNil, "var":CheckVar}