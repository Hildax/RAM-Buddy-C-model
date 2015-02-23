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
	/*
	saddr1= alloc(400);	
	saddr2= alloc(32);
	de_alloc(saddr1,400);	
	saddr3= alloc(188);
	saddr4= alloc(1);
	saddr5= alloc(1);	
	de_alloc(saddr5,1);
	saddr6 = alloc(2);
	*/
	/*
	for(i = 0;i<20;i++){
		printf("allocation %d\n",i);
		saddr1 = alloc(15);
		
		ptree(0);
		ptree(1);

	}
	
	de_alloc(16,15);
			ptree(0);
		ptree(1);
saddr1 = alloc(15);
	
	ptree(2);
	ptree(18);
	de_alloc(288,15);
	ptree(2);
	ptree(18);
	saddr1 = alloc(15);
	
	saddr1 = alloc(1);
	de_alloc(15,1);
	saddr1 = alloc(1);
	*/
	/*
	for(i = 0;i <27;i++){
		saddr1 = alloc(15);
	}
	*/
	/*	ptree(0);
	ptree(1);
	ptree(9);*/
	//ptree(73);
	
	
	//pvec(0);
	/*
	de_alloc(288,15);
	//	pvec(0);
	//ptree(73);
	saddr1 = alloc(1);
	//ptree(73);
	saddr1 = alloc(15);
*/
	//pvec(0);
	
	//printf("tree depth (max) %d\n",MAX_TREE_DEPTH);
	
	
	printf("1\n");
	saddr1 = alloc(67);
	
	printf("2\n");
	saddr2= alloc(67);


	printf("3\n");
	saddr3 = alloc(67);

	printf("4\n");
	saddr4 = alloc(67);
	
	printf("5\n");
	de_alloc(saddr1,67);
	
	printf("6\n");
	de_alloc(saddr2,67);
	
	printf("7\n");
	saddr5 = alloc(256);

	printf("8\n");
	saddr5 = alloc(67);
	
	return 0;
}
