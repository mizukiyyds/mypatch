#include <windows.h>
#include <iostream>
using namespace std;


#include "myprotect.h"

void (*junk_code_begin[101])();
void (*junk_code_end[101])();


#if defined(__GNUC__)
#define FORCEDINLINE  __attribute__((always_inline))
#else 
#define FORCEDINLINE
#endif

namespace protect
{


	void push_param(const int& param1,const int& param2)
	{
		int_param1=param1;
		int_param2=param2;
	}
	enum protect_type
	{
		type_nor	//基于或非运算模拟其他运算
	};
	inline int nor_(int n,int m)
	{
		return ~(n|m);
	}
	
	inline int not_(int n)
	{
		return nor_(n,n);
	}
	inline int and_(int n,int m)
	{
		return nor_(not_(n),not_(m));
	}
	inline int or_(int n,int m)
	{
		return not_(nor_(n,m));
	}
	inline int xor_(int n,int m)
	{
		return or_(nor_(not_(n),m),nor_(n,not_(m)));
	}
	inline int xnor(int n,int m)
	{
		return not_(xor_(n,m));
	}
	inline int shl_(int n)
	{
		__asm{
			shl n,0x1;
		}
		return n;
	}
	inline int shr_(int n)
	{
		__asm{
			shr n,0x1;
		}
		return n;
	}
	

	/**
	 * \brief 模拟加法
	 * n+m = n^m + (n&m)*2
	 * \return 
	 */
	/*template<typename T> 
	int operator+(int n,T m)
	{
		cout<<"模拟加法\n";
		int result=xor_(n,m)+shl_(and_(n,m));
		return result;
	}*/
	inline int add(int n,int m,protect_type type = type_nor)
	{
		if(type==type_nor)
		{
			return xor_(n,m)+shl_(and_(n,m));
		}
		
	}












	// #define not_(n) (nor_(n,n));
// #define and_(n, m) (nor_(not_(n),not_(m)));
// #define or_(n, m) (not_(nor_(n,m)));
// #define xor_(n, m) (or_(nor_(not_(n),m),nor_(n,not_(m))));
// #define xnor_(n, m) (not_(xor_(n,m)));
// #define shl_(n) (n<<1);
// #define shr_(n) (n>>1);

}

/**
 * \brief 流程：开始 -> my_junk_code_end1 -> 返回到标签back -> 跳转到 to_end -> 跳转到 real_end
 * 初始化代码处可以直接调用自己的函数，简洁
 *
 */
void my_junk_code_begin1(void(*func1)() ,void(*func2)())
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
		mov edx, func1;				//将func1传递给下面函数执行
		call  my_junk_code_end1;

		//任意输入垃圾代码----------------
		pushad;
		__emit 0xff; __emit 0x25; __emit 0xE8; __emit 0xE8; __emit 0x68; __emit 0x69;
		lea eax, [edx];
		shr edx, 66;
		call back;
		ret;
		__emit 0xff; __emit 0x25; __emit 0xE8; __emit 0xE8; __emit 0x68; __emit 0x69;
		nop
			popad;
		ret 0x114514;

		//--------------------------------

	back:
		mov eax, 0xFFFFFFFF;		//垃圾指令
		xor eax, eax;
		je to_end;				//恒成立跳转，跳向花指令即将结束
		//任意输入垃圾代码----------------
		mov ecx, 0x68e872;
		__emit 0xff; __emit 0x25; __emit 0xE8; __emit 0xE8; __emit 0x68; __emit 0x69;
		in eax, dx;
		pushad;
		push func1;
		call eax;
		mov ecx, 0xCC5dff25;
		in eax, dx;
		mov edx, 0x5F5E;
		__emit 0xff; __emit 0x25; __emit 0xE8; __emit 0xE8; __emit 0x68; __emit 0x69;
		popad;
		int 3;
		mov ecx, 0xff255f5e;
		ret 0xfe;
		__emit 0xff; __emit 0x25; __emit 0xE8; __emit 0xE8; __emit 0xE8; __emit 0x00;
		__emit 0x76; __emit 0x25; __emit 0x98; __emit 0x33; __emit 0x68; __emit 0x69;
		//--------------------------------
	to_end:
		push func2;
		jmp ebx;
	}
};

void my_junk_code_end1()
{
	void(*func1)() = nullptr;
	void(*func2)() = nullptr;
	//--自定义1 可以输入初始化代码或垃圾代码-----------------
	__asm
	{
		push edx;

		//添加你的花指令（不是任意指令）------
		push 0x114514
		mov ebx, 0x1919810;
		mov ecx, 0x5201314;
		mov edx, 0x654321;
		sub edx, ecx;
		sub ebx, eax;
		pop eax;
		//------------------------------------

		pop esi;				//esi保存edx传递的func1地址，从这里开始不要操作esi

		//添加你的花指令-----------------------
		mov eax,0xffffffff;
		push eax;
		mov ebx, 0x1;
		mov ecx, 0x2;
		sub ebx, eax;

		cmp ecx, ebx;	//一定相等
		//jnz FFFFFFFF	必定不会实现
		//0F 85 AD B3 BF FF
		__emit 0x0F;
		__emit 0x85;
		__emit 0xAD;
		__emit 0xB3;
		__emit 0xBF;
		__emit 0xFF;
		//------------

		pop eax;
		//----------------------------------------

		mov func1, esi;
	}

	if(func1!=nullptr){
		func1();
	}

	//-------------------------------------------------------


	__asm {
		leave;
		pop eax;			//堆栈平衡,因为不返回原来地址
		popad;				//恢复寄存器
		mov ebx, real_end;	//将结束地址放入ebx
		retn;
		//任意输入垃圾代码------------------
		pushad;
		ret;
		in eax, dx;
		pushad;
		__emit 0xff; __emit 0x25; __emit 0xE8; __emit 0xE8; __emit 0x68; __emit 0x69;
		lea eax,my_junk_code_end1;
		call eax;
		mov ecx, 00011451;
		mov edx, 41919810;
		ret;
		__emit 0xff; __emit 0x25; __emit 0xE8; __emit 0xE8; __emit 0x68; __emit 0x69;
		popad;
		ret 0x114514;
		popad;
		ret 114514;
		__emit 0x3f; __emit 0xa5; __emit 0x11; __emit 0x77; __emit 0x68; __emit 0x69;

		//-----------------------------------
	}


	//自定义2 可以输入初始化代码---------------------
	__asm {
	real_end:
		pop func2;
	}
	if(func2!=nullptr) func2();
	

	//-------------------------------------------------

	__asm {
		leave;
		ret;

		//下面输入任意垃圾代码-----
		__emit 0xff; __emit 0x25; __emit 0xE8; __emit 0xE8; __emit 0x68; __emit 0x69;
		__emit 0xf5; __emit 0x20; __emit 0xE8; __emit 0xE9; __emit 0x66; __emit 0x69;
		//-------------------------
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
	//virtual_end:

	}
}

void initialize()
{
	//junk_code_begin[1] = (void(*)())my_junk_code_begin1;
	//junk_code_end[1] = my_junk_code_end1;
}
