#include "iss.h"
#include "shared.h"

unsigned regs[ NUM_REGS ];

/* Full assembled program */ 
unsigned program [ SIZE_PROG ];

/* Full cache */
int memory[ SIZE_MEM ];

/* program counter */
int pc = 0;

/* benchmark counter */
int bench_count = 0;

/* fetch the next instruction from the program */
int fetch()
{
  return program[pc++]; //returns the program[pc] THEN increments pc
}

/* instruction fields */
int instrNum = 0;
int regAlpha = 0;
int imm      = 0;
int o        = 0;
int regInter = 0;
int regBeta  = 0;
int n        = 0;
int a        = 0;
unsigned scallRes = 0;

/* the VM runs until this flag becomes 0 */
int running = 1;

//---------------------------------------DECODE------------------------------------

/* Decode a classic operation instruction */
void decode_op( int instr )
{
  //Instruction id is the first step of decode()
  regAlpha = (instr & 0x7C00000)   >> 22; //First register
  imm      = (instr & 0x200000)   >> 21;  //Immediate value (1) or a register (0)
  o        = (instr & 0x1FFFE0)   >> 5;   //Immediate value in the case of imm = (1)
  regInter = (instr & 0x3E0)      >> 5;   //Register in the case of imm = (0)
  regBeta  = (instr & 0x1F);              //Second register
}

/* Decode the jump instruction */
void decode_jmp( int instr )
{
  //Instruction id is the first step of decode()
  imm      = (instr & 0x4000000)  >> 26; //Immediate value (1) or a register (0)
  o        = (instr & 0x3FFFFE0)  >> 5;  //Immediate value in the case of imm = (1)
  regInter = (instr & 0x3E0)      >> 5;	 //Register in the case of imm = (0)
  regBeta  = (instr & 0x1F);             //Second register
}

/* Decode the braz/branz instruction */
void decode_braz( int instr )
{
  //Instruction id is the first step of decode()
  regAlpha = (instr & 0x7C00000)   >> 22; //First register
  a        = (instr & 0x3FFFFF);
}

/* Decode the scall instruction */
void decode_scall( int instr )
{
  //Instruction id is the first step of decode()
  n = (instr & 0xDFFFFFF);
}

void decode( int instr )
{
	instrNum = (instr & 0xF8000000) >> 27;
	switch(instrNum)
	{
		case 15: decode_jmp(instr); 
		         break;
		case 16:
		case 17: decode_braz(instr);
			     break;
		case 18: decode_scall(instr);
				 break;
		default: decode_op(instr); // The instructions from 1->14 pass through default
		         break;
	}
}

//---------------------------------------DECODE------------------------------------


//-------------------------------------EVALUATION----------------------------------
/* evaluate the last decoded instruction */
void eval()
{
  switch(instrNum)
  {
    case 0:
      	/* stop */
      	printf( "stop\n" );
      	running = 0;
		bench_count ++;
      	break;
	  
    case 1:
      /* add */
		if(imm==1) {//o is an immediate value
			if(step)printf( "add r%d #%d" RED " r%d\n" RESET, regAlpha, o, regBeta );
      		regs[ regBeta ] = regs[ regAlpha ] + o;
	 	}
	  	else if(imm==0){//o is a register
			if(step)printf( "add r%d r%d" RED " r%d\n" RESET, regAlpha, regInter, regBeta );
			regs[ regBeta ] = regs[ regAlpha ] + regs[ regInter ];
	 	}
		bench_count ++;
      	break;
	 	 
    case 2:
    	/* sub */
	  	if(imm==1) {//o is an immediate value
			if(step)printf( "sub r%d #%d" RED " r%d\n" RESET, regAlpha, o, regBeta );
      		regs[ regBeta ] = regs[ regAlpha ] - o;
		}
	    else if(imm==0){//o is a register
		    if(step)printf( "sub r%d r%d" RED " r%d\n" RESET, regAlpha, regInter, regBeta );
	  	    regs[ regBeta ] = regs[ regAlpha ] - regs[ regInter ];
	    }
		bench_count ++;
        break;
	  
    case  3: 
	    /* mult */ 
  	    if(imm==1) {//o is an immediate value
		    if(step)printf( "mult r%d #%d" RED " r%d\n" RESET, regAlpha, o, regBeta );
       	    regs[ regBeta ] = regs[ regAlpha ] * o;
 	    }
 	    else if(imm==0){//o is a register
		    if(step)printf( "mult r%d r%d" RED " r%d\n" RESET, regAlpha, regInter, regBeta );
  	     	regs[ regBeta ] = regs[ regAlpha ] * regs[ regInter ];
  	    }
		bench_count ++;
		bench_count ++;
	    break;
	  
	case  4: 
		/* div */ 
   	 	if(imm==1) {//o is an immediate value
	  	 	if(step)printf( "div r%d #%d" RED " r%d\n" RESET, regAlpha, o, regBeta );
   	   	    regs[ regBeta ] = regs[ regAlpha ] / o;
   	 	}
  	    else if(imm==0){//o is a register
	        if(step)printf( "div r%d r%d" RED " r%d\n" RESET, regAlpha, regInter, regBeta );
     	    regs[ regBeta ] = regs[ regAlpha ] / regs[ regInter ];
        }
		bench_count ++;
		bench_count ++;
        break;
		
	case  5: 
		/* and */ 
 		if(imm==1) {//o is an immediate value
  	 		if(step)printf( "and r%d #%d" RED" r%d\n" RESET, regAlpha, o, regBeta );
   	   	 	regs[ regBeta ] = regs[ regAlpha ] & o;
 		}
   	 	else if(imm==0){//o is a register
       	 	if(step)printf( "and r%d r%d" RED " r%d\n" RESET, regAlpha, regInter, regBeta );
 	   	 	regs[ regBeta ] = regs[ regAlpha ] & regs[ regInter ];
   	 	}
		bench_count ++;
		break;
	
	case  6: 
		/* or */ 
		if(imm==1) {//o is an immediate value
 			if(step)printf( "or r%d #%d" RED " r%d\n" RESET, regAlpha, o, regBeta );
   	 		regs[ regBeta ] = regs[ regAlpha ] | o;
		}
 		else if(imm==0){//o is a register
   	 	   if(step)printf( "or r%d r%d" RED" r%d\n" RESET, regAlpha, regInter, regBeta );
   		   regs[ regBeta ] = regs[ regAlpha ] | regs[ regInter ];
 	    }
		bench_count ++;
		break;
		
	case  7: 
		/* xor */ 
		if(imm==1) {//o is an immediate value
			if(step)printf( "xor r%d #%d" RED " r%d\n" RESET, regAlpha, o, regBeta );
			regs[ regBeta ] = regs[ regAlpha ] ^ o;
		}
		else if(imm==0){//o is a register
			if(step)printf( "xor r%d r%d" RED " r%d\n" RESET, regAlpha, regInter, regBeta );
			regs[ regBeta ] = regs[ regAlpha ] ^ regs[ regInter ];
		}
		bench_count ++;
		break;
	
	case  8: 
		/* shl */ 
		if(imm==1) {//o is an immediate value
			if(step)printf( "shl r%d #%d" RED " r%d\n" RESET, regAlpha, o, regBeta );
			regs[ regBeta ] = regs[ regAlpha ] << o;
		}
		else if(imm==0){//o is a register
			if(step)printf( "shl r%d r%d r%d\n", regAlpha, regInter, regBeta );
			regs[ regBeta ] = regs[ regAlpha ] << regs[ regInter ];
		}
		bench_count ++;
		break;
		
	case  9: 
		/* shr */ 
		if(imm==1) {//o is an immediate value
			if(step)printf( "shr r%d #%d" RED " r%d\n" RESET, regAlpha, o, regBeta );
			regs[ regBeta ] = regs[ regAlpha ] >> o;
		}
		else if(imm==0){//o is a register
			if(step)printf( "shr r%d r%d" RED " r%d\n" RESET, regAlpha, regInter, regBeta );
			regs[ regBeta ] = regs[ regAlpha ] >> regs[ regInter ];
		}
		bench_count ++;
		break;
		
	case 10: 
		/* slt */
		if(imm==1) {//o is an immediate value
			if(step)printf( "slt r%d #%d" RED " r%d\n" RESET, regAlpha, o, regBeta );
			if(regs[ regAlpha ]< (unsigned int) o) regs[ regBeta ] = 1; //regBeta receives 1 if regAlpha<o
			else regs[ regBeta ] = 0;
		}
		else if(imm==0){//o is a register
			if(step)printf( "slt r%d r%d" RED " r%d\n" RESET, regAlpha, regInter, regBeta );
			if(regs[ regAlpha ]<regs[ regInter ]) regs[ regBeta ] = 1; //regBeta receives 1 if regAlpha<regInter
			else regs[ regBeta ] = 0;
		} 
		bench_count ++;
		break;
	
	case 11: 
		/* sle */ 
		if(imm==1) {//o is an immediate value
			if(step)printf( "sle r%d #%d" RED " r%d\n" RESET, regAlpha, o, regBeta );
			if(regs[ regAlpha ]<= (unsigned int) o) regs[ regBeta ] = 1; //regBeta receives 1 if regAlpha<o
			else regs[ regBeta ] = 0;
		}
		else if(imm==0){//o is a register
			if(step)printf( "sle r%d r%d" RED " r%d\n" RESET, regAlpha, regInter, regBeta );
			if(regs[ regAlpha ]<=regs[ regInter ]) regs[ regBeta ] = 1; //regBeta receives 1 if regAlpha<regInter
			else regs[ regBeta ] = 0;
		} 
		bench_count ++;
		break;
		
	case 12: 
		/* seq */ 
		if(imm==1) {//o is an immediate value
			if(step)printf( "seq r%d #%d" RED " r%d\n" RESET, regAlpha, o, regBeta );
			if(regs[ regAlpha ]== (unsigned int) o) regs[ regBeta ] = 1; //regBeta receives 1 if regAlpha<o
			else regs[ regBeta ] = 0;
		}
		else if(imm==0){//o is a register
			if(step)printf( "seq r%d r%d" RED " r%d\n" RESET, regAlpha, regInter, regBeta );
			if(regs[ regAlpha ]==regs[ regInter ]) regs[ regBeta ] = 1; //regBeta receives 1 if regAlpha<regInter
			else regs[ regBeta ] = 0;
		}
		bench_count ++;
		break;
		
	case 13: 
		/* load */ 
		if(imm==1) {//o is an immediate value
			if(step)printf( "load r%d #%d" RED " r%d\n" RESET, regAlpha, o, regBeta );
			regs[ regBeta ] = memory[ regs[ regAlpha ] + o ];
		}
		else if(imm==0){//o is a register
			if(step)printf( "load r%d r%d" RED " r%d\n" RESET, regAlpha, regInter, regBeta );
			regs[ regBeta ] = memory[ regs[ regAlpha ] + regs[ regInter ] ];
		}
		bench_count ++;
		bench_count ++;
		break;
		
	case 14: 
		/* store */ 
		if(imm==1) {//o is an immediate value
			if(step)printf( "store " RED "r%d" RESET " #%d r%d\n", regAlpha, o, regBeta );
			memory[ regs[ regAlpha ] + o ] = regs[ regBeta ];
		}
		else if(imm==0){//o is a register
			if(step)printf( "store r%d r%d r%d\n", regAlpha, regInter, regBeta );
			memory[ regs[ regAlpha ] + regs[ regInter ] ] = regs[ regBeta ];
		}
		bench_count ++;
		bench_count ++;
		break;
		
	case 15: 
		/* jmp */ 
		if(imm==1) {//o is an immediate value
			if(step)printf( "jmp " GRN "#%d" RESET " " RED "r%d\n" RESET, o, regBeta );
			regs[ regBeta ] = pc+1; 	//Store the instruction next to jump in the regBeta
			pc = o; 					//Jump to the instruction located at the address o
		}
		else if(imm==0){//o is a register
			if(step)printf( "jmp " GRN "r%d" RESET " " RED "r%d\n" RESET, regInter, regBeta );
			regs[ regBeta ] = pc+1; 	//Store the instruction next to jump in the regBeta
			pc = regs[ regInter ]; 		//Jump to the instruction located at the address o
		}
		bench_count ++;
		break;
		
	case 16: 
		/* braz */ 
		if(step)printf( "braz r%d " GRN "%d\n" RESET, regAlpha, a );
		if (regs[ regAlpha ]==0) pc=a;
		bench_count ++;
		break;
		
	case 17: 
		/* branz */ 
		if(step)printf( "branz r%d " GRN "%d\n" RESET, regAlpha, a );
		if (regs[ regAlpha ]!=0) pc=a;
		bench_count ++;
		break;
		
	case 18: 
		/* scall */ 
		if(step)printf( "scall %d \n", n );
		if (n==0){ //Store the user's given value in the register 1
			scanf("%04X ",&scallRes);
			regs[ 1 ] = scallRes;	
		} 
		if (n==1){ //Displays the value in register 1
			printf("Register 1 contains: %d\n",regs[ 1 ]);	
		} 
		bench_count ++;
		break;
				
  }
  regs [ 0 ] = 0;
}

//-------------------------------------EVALUATION----------------------------------

//-----------------------------------FILE HANDLING---------------------------------

void getProgram(FILE *fp){
	size_t line_size = 20;
	char *buff = malloc(line_size);
	unsigned valInter;
	int i=0;
	while(fgets(buff,line_size,fp)!=NULL){
		valInter = (unsigned) strtoul(buff,NULL,16);
		program[i] = valInter;
		i++;
	}
	free(buff);
}

void getData(FILE *fp){
	size_t line_size = 20;
	char *buff = malloc(line_size);
	unsigned valInter;
	int i=0;
	while(fgets(buff,line_size,fp)!=NULL){
		valInter = (int) strtol(buff, (char **)NULL, 10);
		memory[i] = valInter;
		i++;
	}
	free(buff);
}

//-----------------------------------FILE HANDLING---------------------------------

//----------------------------------------GUI--------------------------------------


/* display all registers as 4-digit hexadecimal words */
void showRegs()
{
  int i;
  printf( "regs =" );
  for( i=0; i<NUM_REGS; i++ ){
	if (i%8==0) printf("\n");
	if ((instrNum>=1)&&(instrNum<=14)&&(i==regBeta))printf(RED "%04X " RESET, regs[ i ] );
	/*else if ((instrNum==15)&&(i==o))printf(GRN "%04X " RESET, regs[ i ] ); */
	else if ((instrNum==15)&&(i==regBeta))printf(RED "%04X " RESET, regs[ i ] );
	/*else if ((instrNum==16)&&(i==a))printf(GRN "%04X " RESET, regs[ i ] );
	else if ((instrNum==17)&&(i==a))printf(GRN "%04X " RESET, regs[ i ] ); */
	else{printf( "%04X ", regs[ i ] );};
  }
  printf( "\n\n" );
  sleep(1);
}

//----------------------------------------GUI--------------------------------------

//----------------------------------------RUN--------------------------------------

void run()
{
  clock_t run_time;
  while( running )
  {
	if(step)showRegs();
    int instr = fetch();
    decode( instr );
    eval();
	
  }
  run_time = clock();
  showRegs();
  printf("Mini_MIPS processed %d instructions in %f seconds",bench_count, (double) run_time/CLOCKS_PER_SEC);
}

//----------------------------------------RUN--------------------------------------


