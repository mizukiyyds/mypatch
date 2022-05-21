#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
using namespace std;
int main()
{
	int pid;
	printf("进程pid:");
	scanf("%d",&pid);
	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS,true,pid);
	if(handle==NULL)
	{
		cout<<"打开进程出错\n";
		system("pause");
		return -1;
	}

	int addr=0;
	int len=0;
	char filter;
	//int data;
	char buffer[1001]={0};
	unsigned long actual_write_len=0;
	printf("写入的基址:");
	scanf("%X",&addr);
	printf("写入的字节数:");
	scanf("%d%c",&len,&filter);
	printf("数据（每个字节之间空格）:");
	//gets_s(buffer);
	for(int i=0;i<len;i++)
	{
		scanf("%x",&buffer[i]);
	}
	WriteProcessMemory(handle,(LPVOID)addr,buffer,len,&actual_write_len);
	printf("实际写入的数据长度：%lu\n",actual_write_len);
	system("pause");
}