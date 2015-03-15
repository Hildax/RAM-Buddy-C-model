#include "header.h"

int sizegen(int input){
	int output;
	
	//output = num of blocks
	//output = floor((input + 19)/16)
	
	output = floor((input + 19)/16);
	
	return output;
}