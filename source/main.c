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
	
	//check if separate allocation vector is required
	check_alvector();
	printf("\n");
	
	FILE *f = fopen("test_results/test2.txt", "w");
	if (f == NULL)
	{
		printf("Error opening file!\n");
		//exit(1);
	}
	
	write_sum = 0;
	read_sum = 0;
	
	fprintf(f,"index size READ WRITE\n");
	for(i = 0;i <10;i++){
		size = 1000;
		
		saddr1 = alloc(size);//size
		fprintf(f,"[%d] ",i);
		fprintf(f,"  %d ",size);//size
		fprintf(f,"  %d ",read_count);
		fprintf(f,"  %d\n",write_count);
		
		write_sum = write_sum + write_count;
		read_sum = read_sum + read_count;
		
	}
	
	read_avg = read_sum/i;
	write_avg = write_sum/i;
	fprintf(f,"===============================\n");
	fprintf(f,"READ average  = %d\n",read_avg);
	fprintf(f,"WRITE average = %d\n",write_avg);
	
	
	
	fclose(f);
	
	return 0;
}
