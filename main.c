// C model of a hardware RAM-based Buddy allocator
// created by Hilda Xue, last edited 19 Feb 2015

#include <stdio.h>
#include <math.h>

//case changeable
#define REQUEST_SIZE 3
//example design parameters
#define WORDS_TOTAL 584
#define ALLO_VECTOR_LENGTH 64
#define NUM_MBLOCK 16384
//direction
#define UP 1
#define DOWN 0

#define max_global_depth1 log2(NUM_MBLOCK)

#define max_group_depth1 ceil(max_global_depth1/3) + 1

#define GD 30 //group depth, for example

//my types
typedef int word;

typedef struct BlockCoordi {
	int verti;
	int horiz;
} coordi;

typedef struct BlockScope {
	int request_size;
	coordi coo;
	int direction;
	int pnode_sel; 
	int pnode_sel_phy;
	int search_status; 
	word tree_block;
	int row_base;
	int saddr;
	int alvec;
} scope;

typedef struct AllocationDrone {
	int request_size;
	coordi coo;
	int direction;
	int row_base;
	int alvec;
	int pnode_sel;
	int saddr;
	int original_reqsize;
	int flag_markup;
	int node_or;
	int node_and;
} drone;

typedef struct MarkupDrone{
	int node_or;
	int node_and;
	coordi coo;
	int done;
	int row_base;
} mupdrone;

typedef struct FreeInfo{
	coordi coo;
	int pnode_sel;
	int row_base;	
}freeprobe;

typedef struct HolderType{
	int group[32];
}holder;

word bram[WORDS_TOTAL];
word vec_bram[ALLO_VECTOR_LENGTH];
int flag_use_alvector;
int flag_first;
int flag_alloc;
int free_vcheck;
int held_start_verti;


holder held_mtree[GD];

int held_pnode_sel[GD];
int held_address[GD];

int bram_read(int address);
void bram_write(int address, int content);
void bram_init(int length);
void vector_init(int length);
int vector_read(int address);
void vector_write(int address, int content);
void tree_map(int *tree_block_i,word tree_block);
scope locate_block(scope input);
void check_alvector(void);
drone mark_allocation_down(drone input);
mupdrone mark_allocation_up(mupdrone input);
void update_group(int *mtree,int alvec);
word tree_mapback(int *mtree);
int alloc(int request_size);
freeprobe get_free_info(int saddr,int reqsize);
void de_alloc(int saddr,int reqsize);
void copy_mtree(int *input, holder *output, int index);
void ptree(int address);
void pvec(int address);

int main()
{
	freeprobe free_req;
	int saddr1,saddr2,saddr3,saddr4;
	int readtree;
	int mtree[32];

	printf("Behav RAM-based Buddy Allocator\n");
	bram_init(WORDS_TOTAL);
	vector_init(ALLO_VECTOR_LENGTH);
	
	//check if separate allocation vector is required
	check_alvector();
	printf("\n");

	//saddr1 = alloc(400);
	//de_alloc(saddr2,4);

	//saddr2= alloc(32);
	

	//de_alloc(saddr1,400);

	
	saddr1= alloc(400);
	ptree(0);
	ptree(1);
	ptree(10);
	
	
	saddr2= alloc(32);
	
	de_alloc(saddr1,400);
	
	
	saddr3= alloc(200);
	

	
	return 0;
}

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
//mark allocation

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
	
	printf("[Node Mark] Group (%d,%d) top node size = %d, size left to be marked = %d \n",input.coo.verti,input.coo.horiz,topsize, input.request_size);
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
	}else{
		address = floor(input.coo.horiz/16);
		printf("ALLOCATION VECTOR INSTANCE %d \n",address);
	}  
	
	tree_map(mtree,bram_read(address));
	tree_map(mtree_copy,bram_read(address));
	//output direction is always DOWN
	output.direction = DOWN;
	output.alvec = 0;
	output.pnode_sel = 0;
	output.original_reqsize = output.original_reqsize;
	output.saddr = input.saddr;

	shift = 0;
	n_f = 0;
	if(flag_first == 1){
		shift = input.pnode_sel * 2;	
		flag_first = 0;
	}
	offset = shift/2 + n_f;

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
		mtree[14] = 1;   
		output.request_size = input.request_size;


	}else if(topsize == 16){
		//topsize = 16 
		//need to use allocation vector
		n_f = floor(reqsize/(topsize/8));

		if(flag_alloc == 1){
			shift = input.pnode_sel * 2;
		}else{
			shift = 0;
		}
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
		if(flag_alloc == 1){
			shift = input.pnode_sel * 4;
		}else{
			shift = 0;
		}
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
	}else{
		//printf("**in case : topsize = not 16 or 4\n");
		n_f = floor(reqsize /(topsize/8));
		if(flag_alloc == 1){
			shift = input.pnode_sel * 2;
		}else{
			shift = 0;
		}
		for(i =shift; i <shift + 2*n_f; i ++){
			mtree[i + 14] = flag_alloc;
		}
		output.request_size = reqsize - n_f * (topsize/8);
		if(output.request_size != 0){
			mtree[shift + 2*n_f + 14] = 1; // don't really care in case of free
		}
		offset = shift/2 + n_f;
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
				
				if(input.request_size == input.original_reqsize){
					held_mtree[input.coo.verti - 1].group[14 + held_pnode_sel[input.coo.verti - 1]*2] = output.node_or;
					held_mtree[input.coo.verti - 1].group[15 + held_pnode_sel[input.coo.verti - 1]*2] = output.node_and;				
					update_group(held_mtree[input.coo.verti - 1].group,0);	
					bram_write(held_address[i],tree_mapback(held_mtree[input.coo.verti - 1].group));	
					
					for(i = input.coo.verti - 2; i >= held_start_verti; i--){
						held_mtree[i].group[14 + held_pnode_sel[i]*2] =held_mtree[i+1].group[0] ;
						held_mtree[i].group[15 + held_pnode_sel[i]*2] =held_mtree[i+1].group[1] ;
						update_group(held_mtree[i].group,1);	
						bram_write(held_address[i],tree_mapback(held_mtree[i].group));	
					}				
				}else{
					NULL;
					printf("just mark up\n");
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
			printf("Downward marking finished, upward marking began.  \n");
			output.flag_markup = 1;
			output.node_or = mtree[0];
			output.node_and = mtree[1];
			if(input.original_reqsize == 1){
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
			printf("Downward marking finished, upward marking is not required. \n");
		}
	}
	else{
		output.flag_markup = input.flag_markup;
		output.node_or = input.node_or;
		output.node_and = input.node_and;
	}
	
	//printf("need mark UP?:%d\n",output.flag_markup);

	printf("or %d,and %d\n", output.node_or,output.node_and);
	
	return output;
}
//end of mark allocation function


int bram_read(int address){
	//  printf("bram entry %d has content %d\n", address, bram[address]);
	return bram[address];
}

void bram_write(int address, int content){
	bram[address] = content;
}

int vector_read(int address){
	//  printf("bram entry %d has content %d\n", address, bram[address]);
	return vec_bram[address];
}

void vector_write(int address, int content){
	vec_bram[address] = content;
}

void bram_init(int length){
	int i;

	for(i = 0; i < length; i++)
	bram[i] = 0;

	printf("bram has been initialised to 0\n");
}

void vector_init(int length){
	int i;

	for(i = 0; i < length; i++)
	vec_bram[i] = 0;

	printf("allocation vector has been initialised to 0\n");
}

void tree_map(int *tree_block_i,word tree_block){
	int i;
	for(i = 0; i < 32; i++){
		tree_block_i[i] = (tree_block >> i) & 0x1; 
	}  
}


word  tree_mapback(int *mtree){
	int i;
	int output = 0;
	word output_i;

	for(i = 0; i < 32; i++){
		if(mtree[i] == 1){
			output = output | ( mtree[i] << i );
		}
	}

	output_i = (word) output;

	return output_i;	 
}

void check_alvector(void){
	int max_global_depth,max_group_depth,last_top_nodesize;

	max_global_depth = log2(NUM_MBLOCK);
	max_group_depth = ceil(max_global_depth/3) ;
	last_top_nodesize = NUM_MBLOCK/pow(8,max_group_depth);
	printf("max depth: %d, max group depth: %d, last top node size: %d \n",max_global_depth, max_group_depth, last_top_nodesize);
	if(last_top_nodesize == 2){
		flag_use_alvector = 1;
	}else{ flag_use_alvector = 0;}
}

void update_group(int *mtree,int alvec){
	int i,j;
	if(alvec == 0){
		//depth 2
		//update OR tree (0,2,4,6...)
		i = 6;
		j = 14;

		for(j = 14; j<30; j = j +4){
			if(mtree[j] == 1 || mtree[j+2] == 1){
				mtree[i] = 1;
			}else{
				mtree[i] = 0;
			}
			i = i + 2;
		}

		//update AND tree (1,3,5,7...)
		i = 7;
		j = 15;
		for(j = 15; j <31; j = j +4){
			if(mtree[j] == 1 && mtree[j+2] == 1){
				mtree[i] = 1;
			}else{
				mtree[i] = 0;
			}
			i = i + 2;
		}

		//depth 1
		//or tree
		i = 2;
		j = 6;

		for(j = 6;j<14; j = j + 4){
			if(mtree[j] == 1 || mtree[j+2] == 1){
				mtree[i] = 1;
			}else{
				mtree[i] = 0;
			}
			i = i + 2;
		}

		//and tree

		i = 3;
		j = 7;
		for(j = 7; j <15; j = j +4){
			if(mtree[j] == 1 && mtree[j + 2] == 1){
				mtree[i] = 1;
			} else{
				mtree[i] = 0;
			}
			i = i + 2;
		}

		//depth 0
		if (mtree[2] == 1 || mtree[4] == 1){
			mtree[0] = 1;
		}else{
			mtree[0] = 0;
		}

		if (mtree[3] == 1 && mtree[5] ==1){
			mtree[1] = 1;
		}else{
			mtree[1] = 0;
		}
	}

}

int alloc(int request_size){
	//declarations
	word tree_block;
	int mapped_block[32];
	scope starting_scope;
	scope located_scope;
	drone starting_drone;
	drone working_drone;
	mupdrone st_drone,mup_drone;

	flag_alloc = 1;
	flag_first = 1;
	
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

	//search function


	printf("allocation request: size = %d \n",request_size);
	
	located_scope = starting_scope;
	while(located_scope.search_status == 0){
		located_scope = locate_block(located_scope);
		//printf("block detected, coordinate:(%d,%d), pnode_sel:%d \n",located_scope.coo.verti,located_scope.coo.horiz,located_scope.pnode_sel_phy);
		//printf("starting address is: %d \n", located_scope.saddr);
		//printf("end up in allocation vector?: %d \n",located_scope.alvec); 
	}

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
		printf("or %d,and %d\n", st_drone.node_or,st_drone.node_and);
		printf("or %d,and %d\n", working_drone.node_or,working_drone.node_and);
		printf("allocation finished, starting address = %d \n\n",located_scope.saddr);
	}

	return located_scope.saddr;

}

mupdrone mark_allocation_up(mupdrone input){
	mupdrone output;
	int address;
	int mtree[32];

	output.done =input.done;

	output.coo.verti = input.coo.verti - 1;
	output.coo.horiz = floor(input.coo.horiz/8);
	output.row_base = input.row_base - pow(2, (double)(3*(output.coo.verti-1)));
	address = input.row_base + output.coo.horiz;

	printf("[Node Mark (upwards)] Group (%d,%d) \n",output.coo.verti,output.coo.horiz);
	
	tree_map(mtree,bram_read(address));
	
	mtree[14 + (input.coo.horiz % 8) * 2] = input.node_or;
	mtree[15 + (input.coo.horiz % 8) * 2] = input.node_and;
	printf("or %d,and %d\n", output.node_or,output.node_and);
	update_group(mtree,0);

	output.node_or = mtree[0];
	output.node_and = mtree[1];

	if(output.coo.verti == 0){
		output.done = 1;
	}
	
	ptree(address);
	
	return output;
}


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
	}
	row_base = row_base + pow(2,(double)(3*(verti - 2)));
	//printf("row_base = %d \n",row_base);
	
	// horiz
	horiz = floor(saddr/topsize);
	
	// pnode_sel 
	pnode_sel = (saddr % topsize)/2;
	
	//printf("location (%d,%d)\n",verti,horiz);
	
	output.coo.verti = verti;
	output.coo.horiz = horiz;
	output.pnode_sel = pnode_sel;
	output.row_base = row_base;
	
	return output;
};

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
	
	//printf("free info: location (%d,%d), node_sel = %d, row base = %d \n",free_req.coo.verti,free_req.coo.horiz,free_req.pnode_sel,free_req.row_base);
	
	//------------------------
	
	//declarations

	if (flag_use_alvector == 1 && reqsize == 1){
		starting_drone.alvec = 1;		
	}else{
		starting_drone.alvec = 0;		
	}
	
	starting_drone.request_size = reqsize;
	starting_drone.coo = free_req.coo;
	starting_drone.row_base = free_req.row_base;
	//starting_drone.alvec = 0 ;//located_scope.alvec; //*******
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

void copy_mtree(int *input, holder *output, int index){
	int i;
	//printf("index is %d\n",index);
	for(i = 0; i<32; i++){		
		output[index].group[i] = input[i];
	}	
}

void ptree(int address){
	word read_tree;
	int mtree[32];
	int i;
	
	read_tree = bram_read(address);
	tree_map(mtree,read_tree);
	
	printf("group with address %d has content: \n",address);
	
	for(i = 0; i<30; i++){
		printf("%d",mtree[i]);
		if(i%2 != 0){
			printf(" ");
		}
		if(i == 1 || i == 5 || i == 13 || i ==29){
			printf(" \n");
		}
	}

	printf("\n");
}
void pvec(int address){
	word read_tree;
	int mtree[32];
	int i;
	
	read_tree = vector_read(address);
	tree_map(mtree,read_tree);
	
	printf("allocation vector group with address %d has content: ",address);
	
	for(i = 0; i<30; i++){
		printf("%d",mtree[i]);
		if(i%2 != 0){
			printf(" ");
		}
	}

	printf("\n\n");
}
