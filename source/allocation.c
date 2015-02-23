// C model of a hardware RAM-based Buddy allocator
// created by Hilda Xue, last edited 23 Feb 2015
// this file includes a function which is calls allocator functions to perform "malloc" 
#include "header.h"

int alloc(int request_size){
	//declarations
	int i;
	word tree_block;
	int mapped_block[32];
	scope starting_scope;
	scope bcheck_scope;
	scope located_scope;
	
	drone starting_drone;
	drone working_drone;
	mupdrone st_drone,mup_drone;

	flag_failed = 0;
	flag_alloc = 1;
	flag_first = 1;
	flag_blocking = 0;
	/*
	for(i = 0; i < MAX_TREE_DEPTH +1; i ++){
		printf("overlord %d has group addr %d \n",i,overlord[i]);
	}
	*/
	
	starting_scope = scope_gen(request_size);
	
	located_scope = starting_scope;
	
	if(starting_scope.coo.verti != 0){
		bcheck_scope = check_blocking_prep(starting_scope);
		
		while(flag_blocking == 0 && bcheck_scope.coo.verti >= 0){
			bcheck_scope = check_blocking(bcheck_scope);
		}
		if(flag_blocking == 1){			
			located_scope = bcheck_scope;	
			printf("found blocking node \n");
		}	
	}	
	//search function
	printf("\nallocation request: size = %d \n",request_size);
	
	
	while(located_scope.search_status == 0 && flag_failed == 0){
		located_scope = locate_block(located_scope);
	}
	
	if(flag_failed == 0){
		
		starting_drone.request_size = starting_scope.request_size;
		starting_drone.coo = located_scope.coo;
		starting_drone.row_base = located_scope.row_base;
		starting_drone.alvec = located_scope.alvec;
		starting_drone.pnode_sel = located_scope.pnode_sel_phy;
		starting_drone.saddr = located_scope.saddr;
		starting_drone.original_reqsize = starting_scope.request_size;
		starting_drone.flag_markup = 0;
		starting_drone.node_or = 0;
		starting_drone.node_and = 0;
		starting_drone.direction = DOWN;

		working_drone = starting_drone;
		while(working_drone.request_size != 0){
			working_drone = mark_allocation_down(working_drone);
		}

		if(working_drone.flag_markup == 1){
			st_drone.node_or = working_drone.node_or;
			st_drone.node_and = working_drone.node_and;
			st_drone.coo = located_scope.coo;
			st_drone.done = 0;
			st_drone.row_base = located_scope.row_base;

			mup_drone = st_drone;
			while(mup_drone.done == 0){
				mup_drone = mark_allocation_up(mup_drone);
			}
		}
		printf("allocation finished, starting address = %d \n\n",located_scope.saddr);
	}
	malloc_update(starting_scope.request_size, located_scope.group_addr);
	
	return located_scope.saddr;

}