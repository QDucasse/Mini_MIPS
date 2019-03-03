#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Jan 20 18:51:41 2019

@author: quentin
"""
import sys

# Translator: ASM instructions to hex


OP_CODES = {
    'stop': 0, 'add': 1, 'sub': 2, 'mult': 3, 'div': 4, 'and': 5, 'or': 6,
    'xor': 7, 'shl': 8, 'shr': 9, 'slt': 10, 'sle': 11, 'seq': 12, 'load': 13,
    'store': 14, 'jmp': 15, 'braz': 16, 'branz': 17, 'scall': 18
}

LABEL_CODES = {}

# Load instructions in assembly from file and store them in a list
def load_ASM(fileName):
    
    # Place lines from file in array and remove trailing whitespaces
    lines = [line.rstrip('\n') for line in open(fileName)]
    # Remove comments
    lines = [line.split(';')[0] for line in lines]
    lines = [line.strip('\t') for line in lines]
    lines = [line.strip() for line in lines]
    #Remove empty lines
    lines = [line for line in lines if line != '']
    return lines

# Receive a list of assembly instructions and remove the labels
def analyze_labels(asmInstructions):
    asmInstructions = [instruction.split() for instruction in asmInstructions]
    instr_adrs = 0
    for asmInstr in asmInstructions:
        if asmInstr[0][0] == 'L':
            LABEL_CODES[asmInstr[0][:-1]] =  instr_adrs #-1 for the ':'
            del asmInstr[0] #Remove the label from the text
        instr_adrs += 1
    return asmInstructions
    
    
# Receive a list of assembly instructions and convert them into lists of numbers
def analyze_instructions(asmInstructions):
    numInstructions = []
    # Convert every word into a number
    for asmInstr in asmInstructions:
        #print(asmInstr)
        numInstr = []
        # convert operation name to operation code
        instrCode = OP_CODES[asmInstr[0]]
        numInstr.append(instrCode)
        # convert registers and immediates to num values
        #-------------BINARY-------------
        if (instrCode>0)&(instrCode<15):
            
            # remove 1st character ('r')
            # 'r15' -> 15
            numInstr.append(int(asmInstr[1][1:]))
                        
            # 0 if value is a register
            # 1 if value is an immediate value or a Label
            if (asmInstr[2][0] == 'r') or (asmInstr[2][0] == 'R'):
                numInstr.append(0)
                # remove 1st character ('r' or '#')
                # 'r15' -> 15 | '#300' -> 300
                numInstr.append(int(asmInstr[2][1:]))
            else:
                numInstr.append(1)
                if asmInstr[2][0] == 'L':
                     #replace the label by its instruction adress
                     numInstr.append(LABEL_CODES[asmInstr[2]])
                else:
                     # remove 1st character '#'
                     # '#300' -> 300
                     numInstr.append(int(asmInstr[2][1:]))
            
            # remove 1st character ('r')
            # 'r15' -> 15
            numInstr.append(int(asmInstr[3][1:]))
        
        #--------------JMP--------------
        elif ((instrCode)==15):
            
            # 0 if value is a register
            # 1 if value is an immediate value or a label
            if (asmInstr[1][0] == 'r') or (asmInstr[1][0] == 'R'):
               numInstr.append(0)
               # remove 1st character ('r' or '#')
               # 'r15' -> 15 | '#300' -> 300
               numInstr.append(int(asmInstr[2][1:]))
            else:
               numInstr.append(1)
               if asmInstr[1][0] == 'L':
                    #replace the label by its instruction adress
                    numInstr.append(LABEL_CODES[asmInstr[1]])
               else:
                    # remove 1st character '#'
                    # '#300' -> 300
                    numInstr.append(int(asmInstr[1][1:]))
            
            # remove 1st character 'r' 
            # 'r15' -> 15 
            numInstr.append(int(asmInstr[2][1:]))
        
        #-----------BRAZ/BRANZ-----------
        elif (instrCode==16)|(instrCode==17):
            
            # remove 1st character ('r')
            # 'r15' -> 15
            numInstr.append(int(asmInstr[1][1:]))
            
            # address label or immediate value
            if asmInstr[2][0] == 'L':
                 #replace the label by its instruction adress
                 numInstr.append(LABEL_CODES[asmInstr[2]])
            else:
                 # remove 1st character '#'
                 # '#300' -> 300
                 numInstr.append(int(asmInstr[2][1:]))
        
        #-------------SCALL--------------
        elif (instrCode==18):
            # n
            numInstr.append(int(asmInstr[1][1:]))
        #print(numInstr)
        numInstructions.append(numInstr)
    
    return numInstructions


# receives a list of lists of numbers and converts them into hex instructions
def compute_hex_instructions(numInstructions):
    
    hexInstructions = []
    
    for numInstr in numInstructions:
        decInstr = 0
        instrCode = numInstr[0]
        decInstr += numInstr[0] << 27
        
        if (instrCode>0)&(instrCode<15):      #All Binary instructions
            decInstr += numInstr[1] << 22        #regAlpha
            decInstr += numInstr[2] << 21        #imm
            decInstr += numInstr[3] << 5         #o
            decInstr += numInstr[4]              #regBeta
        
        elif ((instrCode)==15):                 #jmp instruction
            decInstr += numInstr[1] << 26        #imm
            decInstr += numInstr[2] << 5         #o
            decInstr += numInstr[3]              #regBeta
        
        elif (instrCode==16)|(instrCode==17): #braz/branz instructions
            decInstr += numInstr[1] << 22        #regAlpha
            decInstr += numInstr[2]              #a
        
        elif (instrCode==18):                  #scall instruction
            decInstr += numInstr[1]              #n
        
        hexInstructions.append(hex(decInstr))
    
    return hexInstructions


# receives a list of hex instructions and a file name and writes instructions into file
def output_hex_instructions(hexInstructions, fileName):
    
    outputFile = open(fileName, 'w')
    for instr in hexInstructions:
        outputFile.write(instr + '\n')

def main(inputFileName):
    # Obtaining the file name
    fileName = (((inputFileName.split('/'))[-1]).split('.'))[0]
    # Generating the file name of the result
    outputFileName = './test_results/' + fileName + '_result.hex'
    
    asmInstructions = load_ASM(inputFileName)
    asmInstructionsNoLabels = analyze_labels(asmInstructions)
    #print(LABEL_CODES)
    numInstructions = analyze_instructions(asmInstructionsNoLabels)
    hexInstructions = compute_hex_instructions(numInstructions)
    
    output_hex_instructions(hexInstructions, outputFileName)
    return outputFileName
    
if __name__ == '__main__':
    argList = sys.argv
    inputFileName = argList[1]
    print('ASM file encoded under: ' + main(inputFileName))
