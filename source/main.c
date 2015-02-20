/*
 C model of a hardware RAM-based Buddy allocator
 created by Hilda Xue, last edited 19 Feb 2015
 to compile:
 gcc -o main main.c hw_functions.c sw_functions.c header.h allocation.c buddy_markdown.c buddy_markup.c buddy_search.c de_allocation.c free_info.c -lm
*/
#include "header.h"

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
	ptree(0);
	ptree(1);
	ptree(10);
	
	de_alloc(saddr1,400);
    ptree(0);
	ptree(1);
	ptree(10);
	
	saddr3= alloc(200);
	ptree(0);
	ptree(1);
	ptree(10);

	
	return 0;
}

//mark allocation

//end of mark allocation function

