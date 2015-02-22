// C model of a hardware RAM-based Buddy allocator
// created by Hilda Xue, last edited 19 Feb 2015
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
		//printf("verti = %d, topsize = %d, row_base = %d ",verti,topsize,row_base);
		
		verti ++;
		topsize = topsize/8;
		//I want to give one row_base above
		//row_base = row_base + pow(2, (double)(3*(verti-1))); 
		row_base = row_base + pow(2,(double)(3*(verti - 2)));	
	}

	printf("row_base = %d \n",row_base);
	
	// horiz
	horiz = floor(saddr/topsize);
	
	// pnode_sel 
	pnode_sel = (saddr % topsize);// /2?
	printf("saddr %d, topsize %d\n",saddr,topsize);
	printf("pnode_sel = %d\n",pnode_sel);
	
	output.coo.verti = verti;
	output.coo.horiz = horiz;
	output.pnode_sel = pnode_sel;
	output.row_base = row_base;
	
	return output;
};