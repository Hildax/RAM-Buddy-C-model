// C model of a hardware RAM-based Buddy allocator
// created by Hilda Xue, last edited 19 Feb 2015
// this file includes functions which will be in hardware 
// but not related to my algorithm
#include "header.h"

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



void check_alvector(void){
	int max_global_depth,max_group_depth,last_top_nodesize;

	max_global_depth = log2(TOTAL_MEM_BLOCKS);
	max_group_depth = ceil(max_global_depth/3) ;
	last_top_nodesize = TOTAL_MEM_BLOCKS/pow(8,max_group_depth);
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