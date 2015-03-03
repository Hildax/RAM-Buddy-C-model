// C model of a hardware RAM-based Buddy allocator
// created by Hilda Xue, last edited on 22 Feb 2015
// this file includes a function which is part of the buddy allocator 
// this function computes info needed to start marking the tree for de-allocation
#include "header.h"

freeprobe get_free_info(int saddr,int reqsize){
	freeprobe output;
	int topsize;
	int verti,horiz,pnode_sel,row_base,alvec;
	int correlated_row_base;
	
	//initialization
	topsize = TOTAL_MEM_BLOCKS;
	verti = 0;
	horiz = 0;
	pnode_sel = 0;
	row_base = 0; 
	alvec = 0;

	// verti
	while(reqsize <= topsize/16){
		verti ++;
		topsize = topsize/8;
		row_base = row_base + pow(2,(double)(3*(verti - 2)));	
	}

	// horiz
	horiz = floor(saddr/topsize);

	if(topsize == 2){
		pnode_sel = saddr % 2;
		alvec = 1;
	}else if(topsize == 4){
		pnode_sel = 2 * (saddr % 4);
		alvec = 0;
	}else{				
		pnode_sel = floor((saddr % topsize) / (topsize/8));
		alvec = 0;
	}
	
	correlated_row_base = row_base + pow(2,(double)(3*(verti-1)));

	output.virtual_gaddr = correlated_row_base + horiz;
	
	if(alvec == 0){
		output.group_addr = correlated_row_base + horiz;
	}else{
		output.group_addr = horiz % 16;
	}
	
	output.coo.verti = verti;
	output.coo.horiz = horiz;
	output.pnode_sel = pnode_sel;
	output.row_base = row_base;
	output.alvec = alvec;

	return output;
};