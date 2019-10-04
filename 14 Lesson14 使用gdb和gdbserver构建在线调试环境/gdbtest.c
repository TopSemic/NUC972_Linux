#include <stdio.h>
int main() 
{
	char s[64] = "Welcome to www.topsemic.com";
	int a = 1;
	int c = a*2;
	int *ptr = NULL;
	printf("s is :%s\n", s);
	printf("c is : %d\n", c);
	*ptr = 20;
	printf("%d\n",*ptr);
	return 0;
}