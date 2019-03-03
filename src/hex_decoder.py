#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Jan 20 18:51:41 2019

@author: quentin
"""
import sys

# Translator: ASM instructions to hex


OP_CODES = {
    0:'stop', 1:'add', 2:'sub', 3:'mult', 4:'div', 5:'and', 6:'or',
    7:'xor', 8:'shl', 9:'shr', 10:'slt', 11:'sle', 12:'seq', 13:'load',
    14:'store', 15:'jmp', 16:'braz', 17:'branz', 18:'scall'
}

# Load instructions in hex from file and store them in a list
def load_ASM(fileName):
    
    # Place lines from file in array and remove trailing whitespaces
    lines = [line.rstrip('\n') for line in open(fileName)]
    return lines
    
# receives a list of hex instructions and converts them into a list of numbers
def decode_hex_instructions(hexInstructions):
    
    numInstructions = []
    
    for hexInstr in hexInstructions:
        hexInstr = int(hexInstr,16)
        instrCode = (hexInstr & 0xF8000000) >> 27
        numInstr = [instrCode]
        
        if (instrCode>0)&(instrCode<15):      #All Binary instructions
            regAlpha = (hexInstr & 0x7C00000)   >> 22; #First register
            numInstr.append(regAlpha)
            imm      = (hexInstr & 0x200000)   >> 21;  #Bit: value (1) or a register (0)
            numInstr.append(imm)
            o        = (hexInstr & 0x1FFFE0)   >> 5;   #Value or register
            numInstr.append(o)
            regBeta  = (hexInstr & 0x1F);              #Second register
            numInstr.append(regBeta)

        
        elif ((instrCode)==15):                 #jmp instruction
            imm      = (hexInstr & 0x4000000)  >> 26; #Bit: value (1) or a register (0)
            numInstr.append(imm)
            o        = (hexInstr & 0x3FFFFE0)  >> 5;  #Value or register
            numInstr.append(o)
            regBeta  = (hexInstr & 0x1F);             #Second register
            numInstr.append(regBeta)
        
        elif (instrCode==16)|(instrCode==17): #braz/branz instructions
            regAlpha = (hexInstr & 0x7C00000)   >> 22; #First register
            numInstr.append(regAlpha)
            a        = (hexInstr & 0x3FFFFF);          #Adress
            numInstr.append(a)
        
        elif (instrCode==18):                  #scall instruction
            n = (hexInstr & 0xDFFFFFF);              #n
            numInstr.append(n)
        
        numInstructions.append(numInstr)
    
    return numInstructions

# Receive a list of lists of numbers and convert them into asm instructions
def compute_asm_instructions(numInstructions):
    asmInstructions = []
    for numInstr in numInstructions:
        #print(numInstr)
        instrCode = numInstr[0]        
        asmInstr = str(OP_CODES[instrCode])
        
        #-------------BINARY-------------
        if (instrCode>0)&(instrCode<15):
            asmInstr += ' r' + str(numInstr[1])
            if numInstr[2] == 0:
                asmInstr += ' r' + str(numInstr[3])
            else:
                asmInstr += ' #' + str(numInstr[3])
            asmInstr += ' r' + str(numInstr[4])
            
        #--------------JMP--------------
        elif ((instrCode)==15):
            if numInstr[1] == 0:
                asmInstr += ' r' + str(numInstr[2])
            else:
                asmInstr += ' #' + str(numInstr[2])
            asmIntr += ' r' + str(numInstr[3])
            
        #-----------BRAZ/BRANZ-----------
        elif (instrCode==16)|(instrCode==17):
            asmInstr += ' r' + str(numInstr[1]) + ' #' + str(numInstr[2])
           
        #-------------SCALL--------------
        elif (instrCode==18):
            asmInstr += ' #' + str(numInstr[2])
          
        #print(asmInstr)          
        asmInstructions.append(asmInstr)
        
    return asmInstructions
    
# receives a list of hex instructions and a file name and writes instructions into file
def output_asm_instructions(asmInstructions, fileName):
    
    outputFile = open(fileName, 'w')
    for instr in asmInstructions:
        outputFile.write(instr + '\n')

def main(inputFileName):
    # Obtaining the file name
    fileName = (((inputFileName.split('/'))[-1]).split('.'))[0]
    # Generating the file name of the result
    outputFileName = './decode_test_results/' + fileName + '_result.asm'
    
    hexInstructions = load_ASM(inputFileName)
    numInstructions = decode_hex_instructions(hexInstructions)
    asmInstructions = compute_asm_instructions(numInstructions)
    output_asm_instructions(hexInstructions, outputFileName)
    
    return outputFileName
    
if __name__ == '__main__':
    argList = sys.argv
    inputFileName = argList[1]
    print('HEX file decoded under: ' + main(inputFileName))
