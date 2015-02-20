// C model of a hardware RAM-based Buddy allocator
// created by Hilda Xue, last edited 19 Feb 2015
// this file includes a function which is part of the buddy allocator 
// locate_block function searches for the allocation address deciding group of nodes
#include "header.h"

//search function
scope locate_block(scope input){
	scope output;
	int address, alvec_address;
	int mtree[32];
	int reqsize;
	int topsize;
	int flag_found;
	int i;
	int local_sel,local_bit_sel;  

	topsize = NUM_MBLOCK/pow(8,input.coo.verti);
	printf("[Group Search] ");
	printf("New Group (%d,%d) top node size = %d \n",input.coo.verti,input.coo.horiz,topsize);

	reqsize = input.request_size;
	output.request_size = input.request_size;

	//1.calculate ram address 2.read block 3.map block
	if(input.direction == DOWN){
		output.row_base = input.row_base + pow(2, (double)(3*(input.coo.verti-1)));
	}else if(input.direction == UP){
		output.row_base = input.row_base - pow(2, (double)(3*(input.coo.verti)));
	}

	if (input.alvec == 0){
		address = output.row_base + input.coo.horiz;
		printf("Group Address: %d \n",address);
	}else{
		address = floor(input.coo.horiz/16); 
		printf("ALLOCATION VECTOR INSTANCE %d \n",address);
	}

	output.tree_block = bram_read(address);
	tree_map(mtree,output.tree_block);  
	output.alvec = 0;
	flag_found = 0;
	
	//allocation will happen in some lever further down
	//find the first available p in lowest local level
	if(reqsize <= topsize/16){

		output.search_status = 0;
		output.direction = DOWN;

		if(input.direction == DOWN){
			i = 15; //odd number, because checking AND nodes
		}else if(input.direction == UP){
			i = input.pnode_sel*2 + 17; //skip the just checked node
		}

		while(flag_found == 0 && i < 31){
			if(mtree[i] == 0){
				flag_found = 1;
				output.pnode_sel = (i-15)/2;
				output.pnode_sel_phy = (i-15)/2;

			}else{
				i = i+2;
			}
		}

		if(flag_found == 1){
			output.coo.verti = input.coo.verti + 1;
			output.coo.horiz = input.coo.horiz * 8 + output.pnode_sel;
			output.saddr = input.saddr + output.pnode_sel * (topsize/8);
			//------allocation vector
			if(topsize/16 == 1 && flag_use_alvector == 1){
				output.alvec = 1; 
			}      
			//-----end allocation vector
		}else{
			printf("didn't find allocatable node, going up \n");
			output.coo.verti = input.coo.verti - 1;
			output.coo.horiz = floor( input.coo.horiz/8);
			output.direction = UP;
			output.pnode_sel = input.pnode_sel;
			output.pnode_sel_phy = input.pnode_sel_phy;
			output.saddr = input.saddr - input.pnode_sel * topsize;
		}
		
	}else {
		if(input.alvec == 1){
			output.alvec = 1;
			// if(mtree[input.pnode_sel*2] == 0){
			printf("horiz YU 16 = %d, address = %d\n",input.coo.horiz % 16,address);
			pvec(address);
			if (mtree[(input.coo.horiz % 16)*2] == 0){ 
				printf("bit0 !!\n");
				flag_found = 1;
				local_bit_sel = 0;
			}else{
				//	if(mtree[input.pnode_sel*2+1] == 0){
				if(mtree[(input.coo.horiz % 16)*2 +1] ==0){ 
					printf("bit1 !!\n");
					flag_found = 1;
					local_bit_sel = 1;
				}
			}			
		}

		if(input.alvec == 0){
			//allocation will happen in this level, can only go up if no node available
			//try to find empty node
			if ( reqsize <= topsize/8 ){
				if(input.direction == DOWN){
					i = 14;
				}else if(input.direction == UP){
					//not supposed to happen, but can be used for other things
					i = input.pnode_sel*2 + 16; 
				}    
				while(flag_found == 0 && i < 30){
					if(mtree[i] == 0){
						printf("[[[[i = %d]]]]",i);
						flag_found = 1;
						local_sel = (i-14)/2;
						output.saddr = input.saddr + local_sel * (topsize/8);
						output.pnode_sel = local_sel;
						output.pnode_sel_phy = local_sel;
					}else{
						i = i+2;
					} 
				}    
				
			}else{

				if ( reqsize <= topsize/4){

					if(input.direction == DOWN){
						i = 6;
					}
					while(flag_found == 0 && i < 14){
						if(mtree[i] == 0){
							flag_found = 1;
							local_sel = (i-6)/2;
							output.saddr = input.saddr + local_sel * (topsize/4);
							output.pnode_sel = local_sel;
							output.pnode_sel_phy = local_sel * 2;
						}else{
							i = i+2;
						}
					}
				}else{
					if ( reqsize <= topsize/2){
						if(input.direction == DOWN){
							i = 2;
						}    
						while(flag_found == 0 && i < 6){
							if(mtree[i] == 0){
								flag_found = 1;
								local_sel = (i-2)/2;
								output.saddr = input.saddr + local_sel * (topsize/2);
								output.pnode_sel = local_sel;
								output.pnode_sel_phy = local_sel * 4;
							}else{
								i = i+2;
							}
						}
					}else{
						NULL;
					}
				}
			}
		}
		if(flag_found == 1){
			
			output.search_status = 1;
			//found allocatable node, return allocatable tree block coordinate
			output.coo.verti = input.coo.verti;
			output.coo.horiz = input.coo.horiz;
			if(input.alvec == 0){
				output.saddr = input.saddr + output.pnode_sel_phy * (topsize/8);

			}else{
				output.saddr = input.saddr + local_bit_sel;
				output.pnode_sel = local_bit_sel;
				output.pnode_sel_phy = local_bit_sel;
			}
			
		}else{
			printf("didn't find allocatable node, going up \n");
			output.coo.verti = input.coo.verti - 1;
			output.coo.horiz = floor( input.coo.horiz/8);
			output.direction = UP;
			output.pnode_sel = input.pnode_sel;
			output.pnode_sel_phy = input.pnode_sel_phy;
			output.saddr = input.saddr - input.pnode_sel_phy * topsize;
		}     
	} 

	if(output.search_status == 1){
		output.row_base = input.row_base;
	}   

	return output;
}