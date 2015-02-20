// C model of a hardware RAM-based Buddy allocator
// created by Hilda Xue, last edited 19 Feb 2015
// this file includes a function which is part of the buddy allocator 
// this function marks bit for malloc/free up the tree
#include "header.h"

mupdrone mark_allocation_up(mupdrone input){
	mupdrone output;
	int address;
	int mtree[32];

	output.done =input.done;

	output.coo.verti = input.coo.verti - 1;
	output.coo.horiz = floor(input.coo.horiz/8);
	output.row_base = input.row_base - pow(2, (double)(3*(output.coo.verti-1)));
	address = input.row_base + output.coo.horiz;

	printf("[Node Mark (upwards)] Group (%d,%d) \n",output.coo.verti,output.coo.horiz);
	
	tree_map(mtree,bram_read(address));
	
	mtree[14 + (input.coo.horiz % 8) * 2] = input.node_or;
	mtree[15 + (input.coo.horiz % 8) * 2] = input.node_and;
	printf("or %d,and %d\n", output.node_or,output.node_and);
	update_group(mtree,0);

	output.node_or = mtree[0];
	output.node_and = mtree[1];

	if(output.coo.verti == 0){
		output.done = 1;
	}
	
	printf("Marking Upwards \n");
	ptree(address);

	return output;
}