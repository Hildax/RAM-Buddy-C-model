/*
C model of a hardware RAM-based Buddy allocator
created by Hilda Xue, last edited 21 Feb 2015
to compile:
gcc -o main main.c hw_functions.c sw_functions.c header.h allocation.c buddy_markdown.c buddy_markup.c buddy_search.c de_allocation.c free_info.c buddy_tracker.c safe_search.c -lm
*/
#include "header.h"

int main()
{
	freeprobe free_req;
	int saddr1,saddr2,saddr3,saddr4,saddr5,saddr6;
	int readtree;
	int mtree[32];
	int i,j;

	for(i= 0; i < MAX_TREE_DEPTH +1; i++){
		overlord[i] = 0;
	}
	
	printf("Behav RAM-based Buddy Allocator\n");
	bram_init(TREE_RAM_LENGTH);
	vector_init(ALVEC_RAM_LENGTH);
	
	//check if separate allocation vector is required
	check_alvector();
	printf("\n");
	
	for(i = 0;i <100;i++){
		/*
		if (i % 9 == 0){
			saddr2 = saddr1;
			j = 0;
		}
		j ++;
		*/
		j = 0;
		if(j == 3 ){
			printf("FREE[%d]", i);
			de_alloc(saddr2,400);
		}else{
			printf("ALLOCATION[%d] ",i);
			saddr1 = alloc(i+1);
		}		
		
		printf("READ COUNT = %d\n",read_count);
		printf("WRITE COUNT = %d\n\n",write_count);
	}
	

	
	return 0;
}
