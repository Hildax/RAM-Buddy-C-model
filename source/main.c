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
	int i;

	for(i= 0; i < MAX_TREE_DEPTH +1; i++){
		overlord[i] = 0;
	}
	
	printf("Behav RAM-based Buddy Allocator\n");
	bram_init(TREE_RAM_LENGTH);
	vector_init(ALVEC_RAM_LENGTH);
	
	//check if separate allocation vector is required
	check_alvector();
	printf("\n");
	
	saddr1= alloc(400);	
	saddr2= alloc(32);
	de_alloc(saddr1,400);	
	saddr3= alloc(188);
	ptree(299640);
	saddr4= alloc(1);
	ptree(299640);
	saddr5= alloc(1);
	ptree(299640);	
	de_alloc(saddr5,1); //***
	ptree(299640);
	saddr6 = alloc(2);
	ptree(299640);
	saddr5= alloc(1);
	ptree(299640);
	
	return 0;
}
