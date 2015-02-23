// C model of a hardware RAM-based Buddy allocator
// created by Hilda Xue, last edited on 23 Feb 2015
// tracker which helps to improve the buddy tree search efficiency
// FIRST FIT principle
#include "header.h"

void malloc_update(int size, int group_addr){
	int overlord_index = get_index(size);
	
	if(flag_alloc == 1){
		overlord[overlord_index] = group_addr;
	}else{
		if(overlord[overlord_index] > group_addr){
			overlord[overlord_index] = group_addr;
		}
	}
	//printf("overlord %d has group addr %d \n",overlord_index,group_addr);
}

scope scope_gen(int size){
	scope output;
	int addr;
	
	addr = overlord[get_index(size)];
	
	output.request_size = size;	
	output.direction = DOWN;
	output.search_status = 0;
	output.alvec = 0;
	
	output.coo = get_coo(addr,size).coo;
	output.pnode_sel =  output.coo.horiz % 8;
	output.pnode_sel_phy = output.coo.horiz % 8;// important
	output.row_base = get_coo(addr,size).row_base;
	output.saddr = output.coo.horiz * get_coo(addr,size).topsize;

	return output;
}

getcoo_type get_coo(int addr, int size){
	getcoo_type output;
	int topsize;
	int verti,horiz;
	int row_base,correlated_row_base;
	
	//initialization
	topsize = TOTAL_MEM_BLOCKS;
	verti = 0;
	horiz = 0;
	row_base = 0;
	
	if(addr != 0){  //--------------------------------- NOT ZERO!
		// verti
		while(size <= topsize/16){
			verti ++;
			topsize = topsize/8;
			row_base = row_base + pow(2,(double)(3*(verti - 2)));	
		}
		correlated_row_base = row_base + pow(2,(double)(3*(verti - 1)));		
		horiz = addr - correlated_row_base;
	}
	
	output.coo.verti = verti;
	output.coo.horiz = horiz;
	output.row_base = row_base;//of one above
	output.topsize = topsize;

	return output;
}

int get_index(int size){
	
	int depth,overlord_index;
	int dsize;
	
	depth = 0;
	dsize = TOTAL_MEM_BLOCKS;
	
	for(depth = 0; depth < MAX_TREE_DEPTH + 1; depth ++){
		if(size < dsize/pow(2,depth)+1 && size > dsize/(pow(2,depth + 1))){
			overlord_index = depth;
		}
	}
	return overlord_index;
}