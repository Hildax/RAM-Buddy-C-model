// C model of a hardware RAM-based Buddy allocator
// created by Hilda Xue, last edited 21/02/15
// this file includes a function which is part of the buddy allocator 
// this function marks bit for malloc/free up the tree


#include "header.h"

mupdrone mark_allocation_up(mupdrone input){
	mupdrone output;
	int address;
	int mtree[32];
	int old_or,old_and;
		
	output.done =input.done;

	output.coo.verti = input.coo.verti - 1;
	output.coo.horiz = floor(input.coo.horiz/8);
	output.row_base = input.row_base - pow(2, (double)(3*(output.coo.verti-1)));//verti - 1?
	address = input.row_base + output.coo.horiz;

	printf("[Node Mark (upwards)] Group (%d,%d) Group address %d\n",output.coo.verti,output.coo.horiz,address);
	
	tree_map(mtree,bram_read(address));
	//keep an old version of the top node
	old_or = mtree[0];
	old_and = mtree[1];

	mtree[14 + (input.coo.horiz % 8) * 2] = input.node_or;
	mtree[15 + (input.coo.horiz % 8) * 2] = input.node_and;

	update_group(mtree,0);
	
	bram_write(address, tree_mapback(mtree));
	
	output.node_or = mtree[0];
	output.node_and = mtree[1];

	if(output.coo.verti == 0 || (output.node_or == old_or && output.node_and == old_and)){
		output.done = 1;
		printf("Stop Going up\n");
	}

	/*
	printf("node to write (or) = %d\n",(output.coo.horiz % 8) * 2);
	printf("node_or = %d \nnode_and =%d \n", input.node_or,input.node_and);
	printf("Marking Upwards \n");
	printf("updated  ");
	pgroup(mtree);
	*/

	return output;
}