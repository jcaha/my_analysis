int a = 10;
int test(int b){
	int *p = &a;
	*p = b+1;
	return a; 
}
