// C model of a hardware RAM-based Buddy allocator
// created by Hilda Xue, last edited 21 Feb 2015
// this file includes a function which is part of the buddy allocator 
// this function marks bit for malloc/free down the tree
#include "header.h"

drone mark_allocation_down(drone input){

	drone output;
	int address,alvec_address;
	int mtree[32],mtree_copy[32];
	int reqsize = input.request_size;
	int topsize = NUM_MBLOCK/pow(8,input.coo.verti);
	int i;
	int local_bit_sel;
	int n_f;
	int shift,offset;
	
	printf("[Node Mark] Group (%d,%d) top node size = %d, size left to be marked = %d \n",input.coo.verti,input.coo.horiz,topsize, reqsize);
	printf("If using allocation vector [** %d **]\n",input.alvec);
	
	//1.calculate ram address 2.read block 3.map block
	if(input.direction == DOWN){
		output.row_base = input.row_base + pow(2, (double)(3*(input.coo.verti-1)));
	}else if(input.direction == UP){
		output.row_base = input.row_base - pow(2, (double)(3*(input.coo.verti)));
	}

	
	if (input.alvec == 0){
		address = output.row_base + input.coo.horiz;
		//printf("location:(%d,%d) address: %d \n",input.coo.verti,input.coo.horiz,address);
		printf("group address %d \n",address);
		
		tree_map(mtree,bram_read(address));
		tree_map(mtree_copy,bram_read(address));
	}else{
		address = floor(input.coo.horiz/16);
		printf("ALLOCATION VECTOR INSTANCE %d, group-like location (%d,%d) \n",address,input.coo.verti,input.coo.horiz);
		tree_map(mtree,vector_read(address));
		tree_map(mtree_copy,vector_read(address));
	}  
	

	//output direction is always DOWN
	output.direction = DOWN;
	output.alvec = 0;
	output.pnode_sel = 0;
	output.original_reqsize = output.original_reqsize;
	output.saddr = input.saddr;

	shift = 0; 
	n_f = 0;
	
	if(flag_first == 1){
		//if(topsize == 4){ 
		//shift = input.pnode_sel *4;
		//printf("previous node sel = %d \n",input.pnode_sel);
		//}
		//else{	
		shift = input.pnode_sel * 2;
		//printf("shift = %d first\n",shift);
		//}
		flag_first = 0;		
	}else{
		shift = 0;		
	}
	
	
	offset = shift/2 + n_f;
			printf("input.pnode_sel = %d\n",input.pnode_sel);
			printf("reqsize = %d shift = %d (1)\n",reqsize,shift);
	
	if(input.alvec == 1){
		if(input.original_reqsize == 1 && input.saddr % 2 != 0){		
			mtree[(input.coo.horiz % 16)*2 + 1] = flag_alloc;
		}else{
			mtree[(input.coo.horiz % 16)*2] = flag_alloc;
		} 
		output.request_size = 0;
	}else if(reqsize < topsize/8){   
		printf("**reqsize < topsize/8 \n"); 

		//write bits 14 in mtree to 1,1
		mtree[shift + 14] = flag_alloc;   
		output.request_size = input.request_size;
		
	}else if(topsize == 16){
		//topsize = 16 
		//need to use allocation vector
		n_f = floor(reqsize/(topsize/8));
		/*
		if(flag_alloc == 1){
			shift = input.pnode_sel * 2;
		}else{
			shift = 0;
		}
		*/
		for(i = shift; i < shift + 2*n_f; i ++){
			mtree[i + 14] = flag_alloc;
		}
		output.request_size = reqsize - n_f * (topsize/8);                                                            
		if(output.request_size != 0){
			mtree[shift + 2*n_f + 14] = flag_alloc; //don't really care
			output.alvec = 1;
		}		
		offset = shift/2 + n_f;
	}else if(topsize == 4){		
		//topsize = 4
		/*
		if(flag_alloc == 1){
			shift = input.pnode_sel * 4;
		}else{
			shift = 0;
		}
		*/
		n_f = floor(reqsize/(topsize/4));
		for(i = shift; i < shift + 4*n_f; i ++){
			mtree[i+ 14] = flag_alloc;
		}
		output.request_size = reqsize - n_f * (topsize/4);

		if(output.request_size != 0){
			mtree[shift + 4*n_f + 6] = flag_alloc;
			mtree[shift + 4*n_f + 6 + 1] = flag_alloc;
		}
		offset = shift/4 + n_f;
		printf("n_f = %d, shift = %d, utput.request_size = %d\n",n_f,shift,output.request_size);
		
	}else{
		printf("**in case : topsize = not 16 or 4\n");
		printf("reqsize = %d shift = %d (1)\n",reqsize,shift);
		n_f = floor(reqsize /(topsize/8));
		printf("reqsize = %d shift = %d (2)\n",reqsize,shift);
		/*
		if(flag_alloc == 1){
			shift = input.pnode_sel * 2;
		}else{
			shift = 0;
		}
		*/
		printf("reqsize = %d shift = %d (3)\n",reqsize,shift);
		for(i =shift; i <shift + 2*n_f; i ++){
			printf("i = %d\n",i);
			mtree[i + 14] = flag_alloc;
		}
		printf("reqsize = %d shift = %d (4)\n",reqsize,shift);
		output.request_size = reqsize - n_f * (topsize/8);
		printf("reqsize = %d shift = %d (5)\n",reqsize,shift);
		if(output.request_size != 0){
			mtree[shift + 2*n_f + 14] = 1; // don't really care in case of free
		}
		offset = shift/2 + n_f;
				printf("reqsize %d, topsize %d,n_f = %d, shift = %d, utput.request_size = %d\n",reqsize,topsize,n_f,shift,output.request_size);
		
	}
	
	
	if(output.request_size != 0){
		output.pnode_sel = offset;
		output.coo.verti = input.coo.verti + 1;
		output.coo.horiz = input.coo.horiz * 8 + output.pnode_sel;
		if(flag_alloc == 1){
			if(output.alvec == 0){
				output.pnode_sel = 0;
			}
		}
	}else{
		NULL; //mark down done
	} 

	update_group(mtree,input.alvec);
	
	
	
	if (flag_alloc == 1){		
		if(input.alvec == 0){			
			bram_write(address, tree_mapback(mtree)); 
		}else{				
			vector_write(address, tree_mapback(mtree)); 
		}
	}else{
		if(free_vcheck == 1){
			held_start_verti = input.coo.verti;	
			free_vcheck = 0;
		}
		//de-allocation, write the previous tree. after the write, update the hold regs
		
		if(output.request_size == 0){
			printf("downward marking finished \n");
			
			//there is a case of alvec == 1
			if(input.alvec == 0){
				//printf("updating for free, start verti = %d\n, current coo verti = %d\n",held_start_verti,input.coo.verti);
				bram_write(address,tree_mapback(mtree));
				for(i = input.coo.verti - 1; i >= held_start_verti; i = i -1){
					//printf("updating for free i = %d \n",i);
					//held_pnode_sel[i] = input.pnode_sel;
					if(i == input.coo.verti - 1 ){
						held_mtree[i].group[14 + held_pnode_sel[i]*2] = mtree[0];
						held_mtree[i].group[15 + held_pnode_sel[i]*2] = mtree[1];
					}else{
						held_mtree[i].group[14 + held_pnode_sel[i]*2] = held_mtree[i+1].group[0];
						held_mtree[i].group[15 + held_pnode_sel[i]*2] = held_mtree[i+1].group[1];
					}
					//held_address[i] = address;					
					update_group(held_mtree[i].group,0);	
					bram_write(held_address[i],tree_mapback(held_mtree[i].group));

				}			
			}else{							
				//end up in allocation vector
				vector_write(address,tree_mapback(mtree));
				
				if(mtree[(input.coo.horiz % 16)*2] == 1 || mtree[(input.coo.horiz % 16)*2 + 1] == 1){
					output.node_or = 1;
				}else{
					output.node_or = 0;
				}
				if(mtree[(input.coo.horiz % 16)*2] == 1 &&  mtree[(input.coo.horiz % 16)*2 + 1] == 1){
					output.node_and = 1;
				}else{
					output.node_and = 0;
				}
				
				if(input.request_size != input.original_reqsize){//input reqsize != 1 and we are in allocation vector
					
					held_mtree[input.coo.verti - 1].group[14 + held_pnode_sel[input.coo.verti - 1]*2] = output.node_or; 			
					held_mtree[input.coo.verti - 1].group[15 + held_pnode_sel[input.coo.verti - 1]*2] = output.node_and;	
					
					update_group(held_mtree[input.coo.verti - 1].group,0);	

					vector_write(held_address[input.coo.verti - 1],tree_mapback(held_mtree[input.coo.verti - 1].group));	
					for(i = input.coo.verti - 2; i >= held_start_verti; i--){
						held_mtree[i].group[14 + held_pnode_sel[i]*2] =held_mtree[i+1].group[0] ;
						held_mtree[i].group[15 + held_pnode_sel[i]*2] =held_mtree[i+1].group[1] ;
						update_group(held_mtree[i].group,1);	
						bram_write(held_address[i],tree_mapback(held_mtree[i].group));	
					}	
					
				}else{//if reqsize originally = 1
					NULL;
					//printf("just mark up\n");
				}
				
			}
		}else{
			//not the last one
			if(input.alvec == 0){
				update_group(mtree,input.alvec);
				//printf("input.coo.verti = %d\n",input.coo.verti);
				held_address[input.coo.verti] = address;				
				held_pnode_sel[input.coo.verti] = output.pnode_sel;
				copy_mtree(mtree,held_mtree,input.coo.verti);
			} else{NULL;}
		}
	}
	
	//decide about mark up
	if(input.request_size == input.original_reqsize){
		
		if (mtree[0] != mtree_copy[0] || mtree[1] != mtree_copy[1] || (input.original_reqsize == 1 && flag_use_alvector == 1)){
			//printf("Downward marking finished, upward marking began.  \n");
			//if in top group, no need to mark up
			if(input.coo.verti == 0){
				output.flag_markup = 0;
				printf("upward marking is NOT required (a)\n");
			}else{
				output.flag_markup = 1;
				printf("upward marking is required \n");
			}
			
			output.node_or = mtree[0];
			output.node_and = mtree[1];
			if(input.original_reqsize == 1 && input.alvec == 1){
				if(mtree[(input.coo.horiz % 16)*2] == 1 || mtree[(input.coo.horiz % 16)*2 + 1] == 1){
					printf("whatsup or 1\n");
					output.node_or = 1;
				}else{
					output.node_or = 0;
					printf("whatsup or 0\n");
				}

				if(mtree[(input.coo.horiz % 16)*2] == 1 &&  mtree[(input.coo.horiz % 16)*2 + 1] == 1){
					output.node_and = 1;
					printf("whatsup and 1\n");
				}else{
					output.node_and = 0;
					printf("whatsup and 0\n");
				}
			}

		}else{
			output.flag_markup = 0;
			printf("upward marking is NOT required (b)\n");
		}
	}
	else{
		output.flag_markup = input.flag_markup;
		output.node_or = input.node_or;
		output.node_and = input.node_and;
	}
	
	/*
	printf("need mark UP?:%d\n",output.flag_markup);

	printf("or %d,and %d\n", output.node_or,output.node_and);
	printf("mtree[0] %d,mtree[1] %d\n", mtree[0],mtree[1]);
	*/
	//pgroup(mtree_copy);
	//pgroup(mtree);
	
	printf("size left = %d\n",output.request_size);

	

	return output;
}