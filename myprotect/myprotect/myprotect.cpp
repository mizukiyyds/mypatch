#include <windows.h>
#include <iostream>
using namespace std;


#include "myprotect.h"

void (*junk_code_begin[101])();
void (*junk_code_end[101])();


/**
 * \brief 流程：开始 -> my_junk_code_end1 -> 返回到标签back -> 跳转到 to_end -> 跳转到 real_end
 * 初始化代码处可以直接调用自己的函数，简洁
 *
 */
void my_junk_code_begin1()
{
	int ret_address = 0;
	__asm
	{
		mov edx, back;
		mov ret_address, 0x114514;	//垃圾指令
		mov ret_address, edx;		//取真正返回地址back
		push ret_address;			//将真正返回地址压栈
		mov ret_address, 0x114514;	//垃圾指令
		pushad;						//保存当前寄存器
		call  my_junk_code_end1;

		//任意输入垃圾代码----------------
		pushad;
		__emit 0xff;__emit 0x25;__emit 0xE8;__emit 0xE8;__emit 0x68;__emit 0x69;
		lea eax,[edx];
		shr edx,66;
		call back;
		ret;
		__emit 0xff;__emit 0x25;__emit 0xE8;__emit 0xE8;__emit 0x68;__emit 0x69;
		nop
		popad;
		ret 0x114514;

		//--------------------------------

	back:
		mov eax, 0xFFFFFFFF;		//垃圾指令
		xor eax, eax;
		je to_end;				//恒成立跳转，跳向花指令即将结束
		//任意输入垃圾代码----------------
		mov ecx,0x68e872;
		__emit 0xff;__emit 0x25;__emit 0xE8;__emit 0xE8;__emit 0x68;__emit 0x69;
		in eax,dx;
		pushad;
		call my_junk_code_begin1;
		mov ecx,0xCC5dff25;
		in eax,dx;
		mov edx,0x5F5E;
		__emit 0xff;__emit 0x25;__emit 0xE8;__emit 0xE8;__emit 0x68;__emit 0x69;
		popad;
		int 3;
		mov ecx,0xff255f5e;
		ret 0xfe;
		__emit 0xff;__emit 0x25;__emit 0xE8;__emit 0xE8;__emit 0xE8;__emit 0x00;
		__emit 0x76;__emit 0x25;__emit 0x98;__emit 0x33;__emit 0x68;__emit 0x69;
		//--------------------------------
	to_end:
		jmp ebx;
	}
};

void my_junk_code_end1()
{
	
	//--自定义1 可以输入初始化代码或垃圾代码-----------------
	__asm
	{
		//添加你的花指令
		push 0x114514
		mov ebx,0x114514*2;
		mov ecx,0x114514*3;
		mov edx,0x114514*4;
		sub edx,ecx;
		sub ebx,eax;
	}

	MessageBoxA(0,"我是初始化1","title",MB_OK);

	//-------------------------------------------------------


	__asm{
		leave;
		pop eax;				//堆栈平衡,因为不返回原来地址
		popad;				//恢复寄存器
		mov ebx, real_end;	//将结束地址放入ebx
		retn;
		//任意输入垃圾代码------------------
		pushad;
		ret;
		in eax,dx;
		pushad;
		__emit 0xff;__emit 0x25;__emit 0xE8;__emit 0xE8;__emit 0x68;__emit 0x69;
		call my_junk_code_begin1;
		mov ecx,00011451;
		mov edx,41919810;
		ret;
		__emit 0xff;__emit 0x25;__emit 0xE8;__emit 0xE8;__emit 0x68;__emit 0x69;
		popad;
		ret 0x114514;
		popad;
		ret 114514;
		__emit 0x3f;__emit 0xa5;__emit 0x11;__emit 0x77;__emit 0x68;__emit 0x69;

		//-----------------------------------
	}


	//自定义2 可以输入初始化代码---------------------
	__asm{
		real_end:
	}

	MessageBoxA(0,"我是初始化2","title",MB_OK);

	//----------------------------------------------------------

	__asm{
		leave;
		ret;
	}
}




void my_virtual_code_begin1()
{
	int org_eax = 0;
	int org_ebx = 0;
	int org_ecx = 0;
	int org_edx = 0;
	__asm
	{
		mov org_eax, eax;
		mov org_ebx, ebx;
		mov org_ecx, ecx;
		mov org_edx, edx;




	}

}
void my_virtual_code_end1()
{
	__asm
	{
	virtual_end:

	}
}

void initialize()
{
	junk_code_begin[1]=my_junk_code_begin1;
	junk_code_end[1]=my_junk_code_end1;
}
