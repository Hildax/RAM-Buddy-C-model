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
	int i,j,size;
	int write_sum,read_sum,write_avg,read_avg;

	for(i= 0; i < MAX_TREE_DEPTH +1; i++){
		overlord[i] = 0;
	}
	
	printf("Behav RAM-based Buddy Allocator\n");
	bram_init(TREE_RAM_LENGTH);
	vector_init(ALVEC_RAM_LENGTH);
	
	/*
	bram_write(10,0xCCCD555);
	bram_write(1,0x1c045);
	bram_write(0,0x4045);
	ptree(0);
	ptree(1);
	ptree(10);
	*/
	saddr1 = alloc(137);
	saddr2 = alloc(4);
	
	
	//check if separate allocation vector is required
	/*
	check_alvector();
	printf("\n");
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
	*/
	return 0;
}
