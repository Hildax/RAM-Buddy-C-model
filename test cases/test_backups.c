void test1{
	FILE *f = fopen("case1024_test_1.txt", "w");
	if (f == NULL)
	{
		printf("Error opening file!\n");
		//exit(1);
	}
//1
	saddr1 = alloc(67);
	fprintf(f,"test %d ",test_index);
	fprintf(f," address = %d\n",saddr1);
	//2
	saddr2= alloc(67);
	fprintf(f,"test %d ",test_index);
	fprintf(f," address = %d\n",saddr2);
	//3
	saddr3 = alloc(67);
	fprintf(f,"test %d ",test_index);
	fprintf(f," address = %d\n",saddr3);
	//4
	saddr4 = alloc(67);
	fprintf(f,"test %d ",test_index);
	fprintf(f," address = %d\n",saddr4);
//5
	de_alloc(saddr1,67);
	fprintf(f,"test %d ",test_index);
	fprintf(f," free\n");	
	//6
	de_alloc(saddr2,67);
	fprintf(f,"test %d ",test_index);
	fprintf(f," free\n");	
	//7
	saddr5 = alloc(256);
	fprintf(f,"test %d ",test_index);
	fprintf(f," address = %d\n",saddr5);
	//8
	saddr5 = alloc(67);
	fprintf(f,"test %d ",test_index);
	fprintf(f," address = %d\n",saddr5);
	
	fclose(f);
}

void test2{
	FILE *f = fopen("case1024_test_2.txt", "w");
	if (f == NULL)
	{
		printf("Error opening file!\n");
		//exit(1);
	}

	saddr1= alloc(400);	
	fprintf(f,"test %d ",test_index);
	fprintf(f," address = %d\n",saddr1);
	
	saddr2= alloc(32);
	fprintf(f,"test %d ",test_index);
	fprintf(f," address = %d\n",saddr2);
	
	de_alloc(saddr1,400);	
	fprintf(f,"test %d ",test_index);
	fprintf(f," free\n");
	
	saddr3= alloc(188);
	fprintf(f,"test %d ",test_index);
	fprintf(f," address = %d\n",saddr3);

	saddr4= alloc(1);
	fprintf(f,"test %d ",test_index);
	fprintf(f," address = %d\n",saddr4);
	
	saddr5= alloc(1);	
	fprintf(f,"test %d ",test_index);
	fprintf(f," address = %d\n",saddr5);
	
	de_alloc(saddr5,1);
	fprintf(f,"test %d ",test_index);
	fprintf(f," free\n");
	
	saddr6 = alloc(2);
	fprintf(f,"test %d ",test_index);
	fprintf(f," address = %d\n",saddr6);
	
	fclose(f);
	
}

void(test2){
	
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
}

void test3{
	for(i = 0;i <27;i++){
		saddr1 = alloc(15);
	}
	
	ptree(0);
	ptree(1);
	ptree(9);
	ptree(73);
	pvec(0);
	
}

void test4{
	
	de_alloc(288,15);
	pvec(0);
	ptree(73);
	saddr1 = alloc(1);
	ptree(73);
	saddr1 = alloc(15);
	pvec(0);	
}

