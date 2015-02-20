// C model of a hardware RAM-based Buddy allocator
// created by Hilda Xue, last edited 19 Feb 2015
// this file includes functions which help the software model
// but not required in hardware implementation
#include "header.h"

// map read node to local tree
void tree_map(int *tree_block_i,word tree_block){
	int i;
	for(i = 0; i < 32; i++){
		tree_block_i[i] = (tree_block >> i) & 0x1; 
	}  
}

// map local tree back 
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

// make a copye of the mapped local tree
void copy_mtree(int *input, holder *output, int index){
	int i;
	//printf("index is %d\n",index);
	for(i = 0; i<32; i++){		
		output[index].group[i] = input[i];
	}	
}

//print out a tree node group from "RAM"
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

//print out a tree node group from "allocation vector"
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
