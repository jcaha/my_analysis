int test1(int a){
	int* p = &a;
	*p = *p + 1;
	return *p;
}

int test2(int a){
	int p = a;
	p = p + 1;
	return p;
}
