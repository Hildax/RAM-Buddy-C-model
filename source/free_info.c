// C model of a hardware RAM-based Buddy allocator
// created by Hilda Xue, last edited 21 Feb 2015
// this file includes a function which is part of the buddy allocator 
// this function computes info needed to start marking the tree for de-allocation
#include "header.h"

freeprobe get_free_info(int saddr,int reqsize){
	freeprobe output;
	int topsize;
	int verti,horiz,pnode_sel,row_base;
	
	//initialization
	topsize = NUM_MBLOCK;
	verti = 0;
	horiz = 0;
	pnode_sel = 0;
	row_base = 0; 
	
	// verti
	while(reqsize <= topsize/16){
		verti ++;
		topsize = topsize/8;
		row_base = row_base + pow(2,(double)(3*(verti - 2)));	
	}
	
	// horiz
	horiz = floor(saddr/topsize);
	
	// pnode_sel 
	pnode_sel = (saddr % topsize);
	
	output.coo.verti = verti;
	output.coo.horiz = horiz;
	output.pnode_sel = pnode_sel;
	output.row_base = row_base;
	
	return output;
};