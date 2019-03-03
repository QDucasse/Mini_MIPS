#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>

#define SIZE_MEM = 36
#define CACHE_LINES = 8

/* We are using a memory 36-bytes memory
 * Therefore we need an 8-line cache
 * adresses are decoded as follows:
 * tag(1 bit) - line_index(3 bits) - offset(1 bit)  = 5 bits
 * and every line in the cache is coded as follows:
 * validity(1 bit) - tag (1 bit) - block1(8 bits) - block2(8 bits) = 18 bits
 */

int memory[ SIZE_MEM ];
int cache[ CACHE_LINES ];

void initialize_cache (){
	int i = 0;
	for (i;i<CACHE_LINES;i++){
		cache[i] = 0b000000000000000000;
	}
}

int get_data(int adress){
	a_tag      = (adress & 0b10000) >> 4; 						/* tag ---------- address */
	a_lindex   = (adress & 0b01110) >> 1;						/* line index --- address */
	a_offset   = (adress & 0b00001);							/* block offset - address */
	c_validity = (cache[lindex] & 0b100000000000000000) >> 17;	/* validity bit - cache */
	c_tag      = (cache[lindex] & 0b010000000000000000) >> 16;	/* tag bit ------ cache */
	c_block1   = (cache[lindex] & 0b001111111100000000) >> 8;	/* block1 value - cache */
	c_block2   = (cache[lindex] & 0b000000000011111111);		/* block2 value - cache */
	if((validity==1)&(c_tag==a_tag)){ /* correct tag and line is valid -> information is already there */
		if (a_offset){
			return c_block2;
		}
		else{
			return c_block1;
		}
	}
	else{ /* information isnt there */
		int block1_value = memory[a_tag + a_lindex];
		int block2_value = memory[a_tag + a_lindex + 1];
		cache[lindex] = (0b1 << 17) + (bin) (a_tag << 16) + (bin) (block1_value << 8) + (bin) (block2_value)
	}
}

int write_data(int adress, int data){
	get_data(adress); /* ensures the data we want to write is in the cache */
	a_tag      = (adress & 0b10000) >> 4; 
	a_lindex   = (adress & 0b01110) >> 1;
	a_offset   = (adress & 0b00001);
	c_validity = (cache[lindex] & 0b100000000000000000) >> 17;
	c_tag      = (cache[lindex] & 0b010000000000000000) >> 16;
	c_block1   = (cache[lindex] & 0b001111111100000000) >> 8;
	c_block2   = (cache[lindex] & 0b000000000011111111);
	if(a_offset){
		cache[lindex] = (0b1 << 17) + (bin) (a_tag << 16) + (bin) (c_block1 << 8) + (bin) (data);
		memory[adress] = data; /* writethrough */
	}
	else{
		cache[lindex] = (0b1 << 17) + (bin) (a_tag << 16) + (bin) (data << 8) + (bin) (c_block2);		
		memory[adress] = data; /* writethrough */
	}
}












