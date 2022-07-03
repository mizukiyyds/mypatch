#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>

#include "myprotect.h"
using namespace std;

char input_s[101]={};
//正确key为mizukiyyds
char encrypt_s[101]={-0x39,-0x31,-0x13,-0x9,-0x35,-0x31,-0x11,-0x11,-0x27,-0x5,0};
//
// void encrypt(char* str)
// {
// 	// int i=0;
// 	// long long tmp=(long long)str[i];
// 	// cout<<tmp<<endl;
// 	// tmp=109;
// 	// tmp= protect::sub(tmp,1, protect::type_rand);
// 	// cout<<tmp<<endl;
// 	// tmp= protect::shl_(tmp,2);
// 	// cout<<tmp<<endl;
// 	// tmp= protect::add(tmp,2, protect::type_rand);
// 	// cout<<tmp<<endl;
// 	// tmp= protect::shr_(tmp,1);
// 	// cout<<tmp<<endl;
// 	// tmp=xor_(tmp, protect::kernel_key%256, protect::type_rand);
// 	// cout<<tmp<<endl;
// 		//str[i]=tmp;
// 	for(int i = 0; i < strlen(str); i++)
// 	{
// 		using namespace protect;
//
// 		//int时 shr_没效果
// 		//long long时 都出错，但在别的地方不出错
// 		//发现bug:在基本逻辑运算中插入insert_junk_code_1有时出现错误
//
// 		// long long tmp=str[i];
// 		// tmp=sub(tmp,1,type_rand);
// 		// cout<<tmp<<endl;
// 		// tmp=shl_(tmp,2);
// 		// cout<<tmp<<endl;
// 		// tmp=add(tmp,2,type_rand);
// 		// cout<<tmp<<endl;
// 		// tmp=shr_(tmp,1);
// 		// cout<<tmp<<endl;
// 		// tmp=xor_(tmp,kernel_key%256,type_rand);
// 		// cout<<tmp<<endl;
// 		// str[i]=tmp;
//
// 		//str[i]={ { [ (str[i]-1) << 2 ] + 2 } >> 1 } ^ ( kernel_key % 256 )
// 		 long long result=xor_(shr_(add(shl_(sub(str[i],1,type_rand),2),2,type_rand),1),kernel_key%256,type_rand);
// 		 str[i]=result;
// 	}
// }

bool check(char* input_s,char *encrypt_s)
{
	using namespace protect;	
	if(strlen(input_s)!=strlen(encrypt_s)) return false;
	for(int i=0;i<strlen(input_s);i++)
	{
		char tmp=xor_(shr_(add(shl_(sub(input_s[i],1,type_rand),2),2,type_rand),1),kernel_key%256,type_rand);
		if(cmp(tmp,encrypt_s[i])!=0) return false;
		
	}
	return true;
}

extern "C" void func1()
{
	{
		
		using namespace protect;
		//生成加密字符串
		//char str[1001]="mizukiyyds";
		//print_string(encrypt,str);
		//system("pause");


		void* p=nullptr;
		get_label_address(p,label1);
		while(1)
		{
			cout<<"输入key：";
			cin>>input_s;
			_if_(!(check(input_s,encrypt_s)), p, type_rand);
			{
				cout << "false\n";
				system("pause");
				system("cls");
				continue;
			}
			label1:
			break;
			
		}
		return;
	}
}
void func2()
{
	//二次验证
	using namespace protect;

	//result="success";
	char result[1001]={-0x5,-0x9,-0x25,-0x25,-0x29,-0x5,-0x5,0};
	for(int i=0;i<strlen(input_s);i++)
	{
		//注意越界，正199（11000111）变为-57
		// long long tmp=input_s[i];
		// tmp=sub(tmp,1,type_rand);
		// tmp=shl_(tmp,2);
		// tmp=add(tmp,2,type_rand);
		// tmp=shr_(tmp,1);
		// tmp=xor_(tmp,kernel_key%128,type_rand);
		// input_s[i]=tmp;

		input_s[i]=(char)xor_(shr_(add(shl_(sub(input_s[i],1,type_rand),2),2,type_rand),1),kernel_key%256,type_rand);
	}
	cout<<"结果：";
	for(int i=0;i<strlen(result);i++){
		long long tmp=256;
		/*
		 //负数转正数，扩展
		tmp=add(tmp,sub(input_s[i],encrypt_s[i],type_rand),type_rand);
		tmp=add(tmp,result[i],type_rand);

		 tmp=add(tmp,0,type_rand);
		 tmp=xor_(tmp,kernel_key%256,type_rand);
		 tmp=mul(tmp,2,type_rand);
		 tmp=sub(tmp,2,type_rand);
		 tmp=shr_(tmp,2);
		 tmp=add(tmp,1,type_rand);
		 */

		tmp=add(shr_(sub(mul(xor_(add(add(add(tmp,sub(input_s[i],encrypt_s[i],type_rand),type_rand),result[i],type_rand),0,type_rand),kernel_key%256,type_rand),2,type_rand),2,type_rand),2),1,type_rand);
		putchar(tmp);
	}
	putchar('\n');
}
void func3()
{
	
}
void func4()
{
	
}
void func5()
{
	
}
int main()
{
	insert_junk_code_1(func3,func4);
	protect::initialize(20220702);
	insert_junk_code_1(func5);
	system("color 3e");
	insert_junk_code_1(func1,func2);
	system("pause");
}