#include <iostream>
#include <windows.h>

#include "myprotect.h"
using namespace std;
int main()
{
	MessageBoxA(0, "Hello World", "Hello", MB_OK);
	char s1[100]="initial_begin";
	initialize();
	char s2[100]="initial_end";
	junk_code_begin[1]();
	system("pause");

}