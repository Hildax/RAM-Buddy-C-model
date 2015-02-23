// C model of a hardware RAM-based Buddy allocator
// created by Hilda Xue, last edited 22 Feb 2015
// this file includes a function which is calls allocator functions to perform "malloc" 
#include "header.h"

int alloc(int request_size){
	//declarations
	word tree_block;
	int mapped_block[32];
	scope starting_scope;
	scope located_scope;
	drone starting_drone;
	drone working_drone;
	mupdrone st_drone,mup_drone;

	flag_failed = 0;
	flag_alloc = 1;
	flag_first = 1;
	/*
	//initialise starting scope
	starting_scope.request_size = request_size;
	starting_scope.coo.verti = 0;
	starting_scope.coo.horiz = 0;
	starting_scope.direction = DOWN;
	starting_scope.pnode_sel = 0;
	starting_scope.pnode_sel_phy = 0;
	starting_scope.search_status = 0;
	starting_scope.tree_block = 0;
	starting_scope.row_base = 0;
	starting_scope.saddr = 0;
	starting_scope.alvec = 0;
*/
starting_scope = scope_gen(request_size);
	//search function
	printf("\n\nallocation request: size = %d \n",request_size);
	
	located_scope = starting_scope;
	while(located_scope.search_status == 0 && flag_failed == 0){
		located_scope = locate_block(located_scope);
		//printf("block detected, coordinate:(%d,%d), pnode_sel:%d \n",located_scope.coo.verti,located_scope.coo.horiz,located_scope.pnode_sel_phy);
		//printf("starting address is: %d \n", located_scope.saddr);
		//printf("end up in allocation vector?: %d \n",located_scope.alvec); 
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
			//printf("next coordinate:(%d,%d) \n",working_drone.coo.verti,working_drone.coo.horiz);
			//printf("allocation vector: %d \n",working_drone.alvec);
			//printf("size left %d \n",working_drone.request_size); 	
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