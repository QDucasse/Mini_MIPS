#include "iss.h"
#include "shared.h"
#include <unistd.h>
#include <getopt.h>
#include <python2.7/Python.h>

/* step mode */
int step = 1;

struct globalArgs_t {
	FILE *hexFileToRun;			/* -r option */
	char *asmFileToEncode;		/* -e option */
	char *hexFileToDecode;		/* -d option */
	FILE *dataFileToLoad;		/* -l option */
	int step;				    /* -s option */
} globalArgs;

static const char *optString = "r:e:d:l:sh?";

static const struct option longOpts[] = {
	{ "run"   , required_argument, NULL, 'r' },
	{ "encode", required_argument, NULL, 'e' },
	{ "decode", required_argument, NULL, 'd' },
	{ "load"  , required_argument, NULL, 'l' },
	{ "step"  , no_argument      , NULL, 's' },
	{ "help"  , no_argument      , NULL, 'h' },
	{  NULL   , no_argument      , NULL,  0  }
};

void error(char *msg) {
	fprintf(stderr, "%s: %s\n", msg, strerror(errno));
	exit(1); 
} 

void display_usage( void )
{
	puts( "minimips - instruction simulator along with instruction encoding and decoding" );
	puts( "Usage: minimips [options]" );
	puts( "options format: label long_label arguments description");
	puts( "-r --run    file.hex ------- runs the hex file passed in argument through the simulator" );
	puts( "-e --encode file.asm ------- encodes an asm file and creates the corresponding hex file" );
	puts( "-d --decode file.hex ------- decodes a hex file and creates the corresponding asm file" );
	puts( "-l --load   file.data ------ loads the data in the file in the simulator memory" );
	puts( "-s --step   no_argument ---- triggers step by step execution" );
	puts( "-h --help   no_argument ---- displays help" );
	exit( EXIT_FAILURE );
}

int main( int argc, char *argv[] )
{
	int opt = 0;
	int longIndex = 0;
	
	/* Python program files */
	FILE *python_encoder;
	FILE *python_decoder;
	char *encoder_name = "hex_encoder.py";
	char *decoder_name = "hex_decoder.py";
	
	/* Initialize globalArgs before we get to work. */
	globalArgs.hexFileToRun = NULL;			/* -r option */
	globalArgs.asmFileToEncode = NULL;		/* -e option */
	globalArgs.hexFileToDecode = NULL;		/* -d option */
	globalArgs.dataFileToLoad = NULL;		/* -l option */
	globalArgs.step = 0;				    /* -s option */
	
	/* Process the arguments with getopt_long(), then 
	 * populate globalArgs. 
	 */
	opt = getopt_long( argc, argv, optString, longOpts, &longIndex );
	if(opt==-1)puts("Look at the README, -h or --help for information on the usage");
	
	while( opt != -1 ) {
		switch( opt ) {
			case 'r':
				if (strstr(optarg,".hex")==0){
					error("Wrong file input, run takes .hex files as input");
				}
				globalArgs.hexFileToRun = fopen(optarg,"r");
				getProgram(globalArgs.hexFileToRun);
				run();
				break;
				
			case 'e':
				if (strstr(optarg,".asm")==0){
					error("Wrong file input, encode takes .asm files as input");
				}
				/* Change in globalArgs */
				globalArgs.asmFileToEncode = optarg;
				/* Creation of the argv passing to python */
				char *argv_encode[2];
				argv_encode[0] = encoder_name;
				argv_encode[1] = globalArgs.asmFileToEncode;
				/* Python takeover */
				Py_SetProgramName(encoder_name);
				Py_Initialize();
				PySys_SetArgv(2,argv_encode);
				python_encoder = fopen(encoder_name,"r");
				PyRun_SimpleFile(python_encoder,encoder_name);
				Py_Finalize();
				puts("Encoding complete!");
				break;
				
			case 'd':
				if (strstr(optarg,".hex")==0){
					error("Wrong file input, decode takes .hex files as input");
				}
				/* Change in globalArgs */
				globalArgs.hexFileToDecode = optarg;
				/* Creation of the argv passing to python */
				char *argv_decode[2];
				argv_decode[0] = decoder_name;
				argv_decode[1] = globalArgs.hexFileToDecode;
				/* Python takeover */
				Py_SetProgramName(decoder_name);
				Py_Initialize();
				PySys_SetArgv(2,argv_decode);
				python_decoder = fopen(decoder_name,"r");
				PyRun_SimpleFile(python_decoder,decoder_name);
				Py_Finalize();
				puts("Decoding complete!");
				break;
				
			case 'l':
				if (strstr(optarg,".data")==0){
					error("Wrong file input, load takes .data files as input");
				}
				globalArgs.dataFileToLoad = fopen(optarg,"r");
				getData(globalArgs.dataFileToLoad);
				break;
			
			case 's':
				if (step==0){
					globalArgs.step = 1;
					step++;
					printf("Step mode enabled!, step = %d\n",step);
				}
				else{
					globalArgs.step = 0;
					step--;
					printf("Step mode disabled!, step = %d\n",step);
				}
				break;
			
			case 'h':
			case '?':
				display_usage();
				break;
				
			default:
				break;
		}
		
		opt = getopt_long( argc, argv, optString, longOpts, &longIndex );
	}
	
	return EXIT_SUCCESS;
}

