// C model of a hardware RAM-based Buddy allocator
// created by Hilda Xue, last edited on 23 Feb 2015
#include "header.h"

scope check_blocking_prep(scope input){
	scope output;
	int topsize = TOTAL_MEM_BLOCKS/pow(8,input.coo.verti);

	
	output.row_base = input.row_base + pow(2, (double)(3*(input.coo.verti-1)));
	
	output.coo.verti = input.coo.verti - 1;
	output.coo.horiz = floor( input.coo.horiz/8);
	output.direction = UP;
	output.pnode_sel = input.coo.horiz % 8;
	output.pnode_sel_phy = input.coo.horiz % 8;				
	output.saddr = input.saddr - input.pnode_sel_phy * topsize;	
	output.request_size = input.request_size;	
	output.search_status = 0;
	output.alvec = 0;

	return output;
	
}

scope check_blocking(scope input){
	scope output;
	int address, alvec_address;
	int mtree[32];
	int reqsize;
	int topsize;
	int flag_found;
	int i;
	int local_sel,local_bit_sel;  
	
	printf("[Check Blocking] ");
	printf("In (%d,%d) \n",input.coo.verti,input.coo.horiz);

	topsize = TOTAL_MEM_BLOCKS/pow(8,input.coo.verti);
	output.request_size = input.request_size;
	
	//compute row_base for input.direction == UP
	output.row_base = input.row_base - pow(2, (double)(3*(input.coo.verti)));

	//compute group address
	address = output.row_base + input.coo.horiz;
	output.tree_block = bram_read(address);
	tree_map(mtree,output.tree_block);  
	
	//checking node AND
	if(mtree[14 + 1 + input.pnode_sel_phy * 2] == 1){
		//if blocked, retain the input scope info
		output.coo = input.coo;
		output.direction = DOWN;
		output.pnode_sel = input.pnode_sel;
		output.pnode_sel_phy = input.pnode_sel_phy;
		output.saddr = input.saddr;
		output.row_base = output.row_base - pow(2, (double)(3*(input.coo.verti-1)));
		//set the flag
		flag_blocking = 1;
	}else{
		//if not blocked, move the scope up
		output.coo.verti = input.coo.verti - 1;
		output.coo.horiz = floor( input.coo.horiz/8);
		output.direction = UP;
		output.pnode_sel = input.coo.horiz % 8;
		output.pnode_sel_phy = input.coo.horiz % 8;				
		output.saddr = input.saddr - input.pnode_sel_phy * topsize;			
	}
	output.request_size = input.request_size;	
	output.search_status = 0;
	output.alvec = 0;
	
	return output;	
}