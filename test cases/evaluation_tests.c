void e_test0{
	
	FILE *f = fopen("test_results/test0.txt", "w");
	if (f == NULL)
	{
		printf("Error opening file!\n");
		//exit(1);
	}
	
	write_sum = 0;
	read_sum = 0;
	
	fprintf(f,"index size READ WRITE\n");
	for(i = 0;i <500;i++){
		
		saddr1 = alloc(1);
		fprintf(f,"[%d] ",i);
		fprintf(f,"  %d ",1);
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
}

void e_test1{
	
		FILE *f = fopen("test_results/test1.txt", "w");
	if (f == NULL)
	{
		printf("Error opening file!\n");
		//exit(1);
	}
	
	write_sum = 0;
	read_sum = 0;
	
	fprintf(f,"index size READ WRITE\n");
	for(i = 0;i <500;i++){
		size = i+1;
		
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
}

void e_test2{
	
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
}