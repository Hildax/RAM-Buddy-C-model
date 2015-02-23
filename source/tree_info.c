// C model of a hardware RAM-based Buddy allocator
// created by Hilda Xue, last edited on 23 Feb 2015
// this file includes a function which is part of the buddy allocator 
// this function computes info needed to start marking the tree for de-allocation
#include "header.h"

int main(void){
	int max_tree_depth,tree_ram_length;
	int i;
	int topsize;
	int verti;
	
	max_tree_depth = log2(TOTAL_MEM_BLOCKS);
	printf("max_tree_depth = %d\n",max_tree_depth);
	
	topsize = TOTAL_MEM_BLOCKS;
	verti = 0;

	while(topsize > 31){
		verti ++;
		topsize = topsize/8;
	}
	printf("deepest group's index = %d\n",verti);
	if(verti == 16){
		printf("allocation vector is required\n");
	}
	
	tree_ram_length = 0;
	for(i = 0; i < verti + 1; i++){
		tree_ram_length = pow(8,i) + tree_ram_length;
		printf("from depth 0 to depth %d, %d RAM words \n",i,tree_ram_length);
	}
	
	printf("tree_ram_length = %d\n",tree_ram_length);
	return 0;
	
}