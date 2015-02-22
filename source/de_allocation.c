// C model of a hardware RAM-based Buddy allocator
// created by Hilda Xue, last edited on 22 Feb 2015
// this file includes a function which is calls allocator functions to perform "free" 
#include "header.h"

void de_alloc(int saddr,int reqsize){
	
	drone starting_drone;
	drone working_drone;
	mupdrone st_drone,mup_drone;
	
	freeprobe free_req; 
	
	printf("deallocation request: address = %d, size = %d \n",saddr,reqsize);
	
	//change flag
	flag_alloc = 0;
	free_vcheck = 1;
	free_req = get_free_info(saddr,reqsize);
	flag_first = 1;
	
	printf("free info: location (%d,%d), node_sel = %d, row base = %d \n",free_req.coo.verti,free_req.coo.horiz,free_req.pnode_sel,free_req.row_base);
	
	//------------------------
	
	//declarations
	/*
	if (flag_use_alvector == 1 && reqsize == 1){
		starting_drone.alvec = 1;		
	}else{
		starting_drone.alvec = 0;		
	}
*/	
	starting_drone.request_size = reqsize;
	starting_drone.coo = free_req.coo;
	starting_drone.row_base = free_req.row_base;
	starting_drone.alvec = free_req.alvec;//located_scope.alvec; //*******
	starting_drone.pnode_sel = free_req.pnode_sel;
	starting_drone.saddr = saddr;
	starting_drone.original_reqsize = reqsize;
	starting_drone.flag_markup = 0;
	starting_drone.node_or = 0;
	starting_drone.node_and = 0;
	starting_drone.direction = DOWN;

	working_drone = starting_drone;
	while(working_drone.request_size != 0){
		working_drone = mark_allocation_down(working_drone);
		//	printf("next coordinate:(%d,%d) \n",working_drone.coo.verti,working_drone.coo.horiz);
		//	printf("allocation vector: %d \n",working_drone.alvec);
		//	printf("size left %d \n",working_drone.request_size); 
	}

	if(working_drone.flag_markup == 1){
		st_drone.node_or = working_drone.node_or;
		st_drone.node_and = working_drone.node_and;
		st_drone.coo = free_req.coo;
		st_drone.done = 0;
		st_drone.row_base = free_req.row_base;

		mup_drone = st_drone;
		while(mup_drone.done == 0){
			mup_drone = mark_allocation_up(mup_drone);
		}

	}

	flag_alloc = 1;
	
	printf("deallocation finished\n\n");
}
