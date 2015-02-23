void test1{
	saddr1= alloc(400);	
	saddr2= alloc(32);
	de_alloc(saddr1,400);	
	saddr3= alloc(188);
	saddr4= alloc(1);
	saddr5= alloc(1);	
	de_alloc(saddr5,1);
	saddr6 = alloc(2);
	
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

void test5{
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
	
}