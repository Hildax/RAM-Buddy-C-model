/*
C model of a hardware RAM-based Buddy allocator
created by Hilda Xue, last edited 21 Feb 2015
to compile:
gcc -o main main.c hw_functions.c sw_functions.c header.h allocation.c buddy_markdown.c buddy_markup.c buddy_search.c de_allocation.c free_info.c buddy_tracker.c safe_search.c size_gen.c -lm
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
	
	printf(" NAHHHHHHHHHHHHhhh %d\n", sizegen(262144));
	
	//check if separate allocation vector is required

	check_alvector();
	
	test_index = 0;
	
	FILE *f = fopen("result.txt", "w");
	if (f == NULL)
	{
		printf("Error opening file!\n");
		//exit(1);
	}

	for(i = 0;i <50;i++){
		saddr1 = alloc(sizegen(10*i + 1));
		fprintf(f,"test %d ", i);
		fprintf(f," address = %x\n",saddr1 * 16 + 4);
	}
	
	
	fclose(f);
	
	return 0;
}


