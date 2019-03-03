#ifndef ISS_H
#define ISS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <getopt.h>
#include <string.h>

#define NUM_REGS 32
#define SIZE_PROG 100
#define SIZE_MEM 100

/* UNIX terminal colors */
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

int fetch();

/* decode */
void decode_op( int instr );
void decode_jmp( int instr );
void decode_braz( int instr );
void decode_scall( int instr );
void decode( int instr );

/* evaluate */
void eval();

/* GUI */
void showRegs();

/* run the simulator */
void run();

/* file handling */
void getProgram(FILE *fp);
void getData(FILE *fp);

#endif //ISS_H

