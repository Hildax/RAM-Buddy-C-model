// C model of a hardware RAM-based Buddy allocator
// created by Hilda Xue, last edited 20 Feb 2015 13:30 
// this file includes a function which is part of the buddy allocator 
// this function marks bit for malloc/free up the tree
/* [Previous Bug]previously faulty because I forgot to write to BRAM the updated tree.
   NOTE.
   function-wsie, this function should check if the updated tree's top node changed. 
   If not, no change needs to be propagated. set done = 1.
*/

#include "header.h"

mupdrone mark_allocation_up(mupdrone input){
	mupdrone output;
	int address;
	int mtree[32];
	int old_or,old_and;

	output.done =input.done;

	output.coo.verti = input.coo.verti - 1;
	output.coo.horiz = floor(input.coo.horiz/8);
	output.row_base = input.row_base - pow(2, (double)(3*(output.coo.verti-1)));
	address = input.row_base + output.coo.horiz;

	printf("[Node Mark (upwards)] Group (%d,%d) \n",output.coo.verti,output.coo.horiz);
	
	tree_map(mtree,bram_read(address));
	//keep an old version of the top node
	old_or = mtree[0];
	old_and = mtree[1];
	
	mtree[14 + (output.coo.horiz % 8) * 2] = input.node_or;
	mtree[15 + (output.coo.horiz % 8) * 2] = input.node_and;

	update_group(mtree,0);
	
	bram_write(address, tree_mapback(mtree));
	
	output.node_or = mtree[0];
	output.node_and = mtree[1];

	if(output.coo.verti == 0 || (output.node_or == old_or && output.node_and == old_and)){
		output.done = 1;
		printf("Stop Going up\n");
	}

/*
	printf("14 + (output.coo.horiz % 8) * 2 = %d\n",14 + (output.coo.horiz % 8) * 2);
	printf("node_or = %d \nnode_and =%d \n", input.node_or,input.node_and);
	printf("Marking Upwards \n");
	pgroup(mtree);
*/
	return output;
}