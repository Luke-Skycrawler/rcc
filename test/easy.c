int main(int argc){
	int a;
	double b;
	char c;
	3 * 4;
	a = 3 * 4;
	b = 233;
	a + 3;
	a + 3.14;
	b + 2;
	c + 1;
	a = c + 1;
	printf("%d,%d\n",argc,a);
	// x + 4; // We can detect undeclared identifier now :)
	// f();
	return 0;
}

int f()
{
	printf("Hello world!%f\n",0.0);
	return 0;
}
