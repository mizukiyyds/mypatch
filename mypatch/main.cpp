#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
using namespace std;
int main()
{
	int pid;
	printf("����pid:");
	scanf("%d",&pid);
	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS,true,pid);
	if(handle==NULL)
	{
		cout<<"�򿪽��̳���\n";
		system("pause");
		return -1;
	}

	int addr=0;
	int len=0;
	char filter;
	//int data;
	char buffer[1001]={0};
	unsigned long actual_write_len=0;
	printf("д��Ļ�ַ:");
	scanf("%X",&addr);
	printf("д����ֽ���:");
	scanf("%d%c",&len,&filter);
	printf("���ݣ�ÿ���ֽ�֮��ո�:");
	//gets_s(buffer);
	for(int i=0;i<len;i++)
	{
		scanf("%x",&buffer[i]);
	}
	WriteProcessMemory(handle,(LPVOID)addr,buffer,len,&actual_write_len);
	printf("ʵ��д������ݳ��ȣ�%lu\n",actual_write_len);
	system("pause");
}