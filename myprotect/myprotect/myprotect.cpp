#include <windows.h>
#include <iostream>
using namespace std;


#include "myprotect.h"




namespace protect
{

	extern void initialize()
	{
		srand((unsigned int)(time(nullptr)));

	}
	void push_param(const long long& param1, const long long& param2)
	{
		int_param1 = param1;
		int_param2 = param2;
	}
	extern long long get_rand(const long long &l,const long long &r)
	{
		rand_n=l+rand()%(r-l+1);
		return rand_n;
	}


	extern long long nor_(const long long &n, const long long &m)
	{
		return ~(n | m);
	}
	extern long long nand_(const long long &n, const long long &m)
	{
		return ~(n & m);
	}
	extern long long not_(const long long &n, protect_type type)
	{
		if(type==type_default) type=type_nor;
		const protect_type org = type;
		if (type == type_rand)
		{
			while (type == type_rand||type == type_default) type = (protect_type)get_rand(0, protect_types - 1);
		}
		if (type == type_nor) return nor_(n, n);
		else if (type == type_nand) return nand_(n, n);
		last_error_code = error_invalid_protect_type;
		return -1;
	}
	extern long long and_(const long long &n, const long long &m, protect_type type)
	{
		if(type==type_default) type=type_nor;
		const protect_type org = type;
		if (type == type_rand)
		{
			while (type == type_rand||type == type_default) type = (protect_type)get_rand(0, protect_types - 1);
		}
		if (type == type_nor) return nor_(not_(n), not_(m));
		else if (type == type_nand) return not_(nand_(n, m), org);
		last_error_code = error_invalid_protect_type;
		return -1;
	}
	extern long long or_(const long long &n, const long long &m, protect_type type)
	{
		if(type==type_default) type=type_nor;
		const protect_type org = type;
		if (type == type_rand)
		{
			while (type == type_rand||type == type_default) type = (protect_type)get_rand(0, protect_types - 1);
		}
		if (type == type_nor) return not_(nor_(n, m), org);
		else if (type == type_nand) return nand_(not_(n), not_(m));
		last_error_code = error_invalid_protect_type;
		return -1;
	}
	extern long long xor_(const long long &n, const long long &m, protect_type type)
	{
		if(type==type_default) type=type_nor;
		const protect_type org = type;
		if (type == type_rand)
		{
			while (type == type_rand||type == type_default) type = (protect_type)get_rand(0, protect_types - 1);
		}
		if (type == type_nor) return or_(nor_(not_(n), m), nor_(n, not_(m)), org);
		else if (type == type_nand) return nand_(not_(and_(n, not_(m, org), org)), not_(and_(not_(n, org), m, org)));
		last_error_code = error_invalid_protect_type;
		return -1;
	}
	extern long long xnor(const long long &n, const long long &m, protect_type type)
	{
		if(type==type_default) type=type_nor;
		const protect_type org = type;
		if (type == type_rand)
		{
			while (type == type_rand||type == type_default) type = (protect_type)get_rand(0, protect_types - 1);
		}
		if (type == type_nor) return not_(xor_(n, m), org);
		else if (type == type_nand) return not_(xor_(n, m), org);
		last_error_code = error_invalid_protect_type;
		return -1;
	}
	extern long long shl_(const long long &n,const long long &m)
	{
		return n<<m;
	}
	extern long long shr_(const long long &n,const long long &m)
	{
		//111000110   n<<1
		//011100011   n
		//异或得到 n<<1与n不同的位
		//mask/2得到n与n>>1不同的位
		//xor得到结果
		long long result=n;
		for(int i=1;i<=m;++i)
		{
			long long sh=shl_(result,1);
			long long mask1=xor_(sh,result);
			mask1/=2;
			result= xor_(n,mask1);
		}
		return result;
	}

	extern long long add(const long long &n, const long long &m, protect_type type)
	{
		const protect_type org = type;
		return xor_(n, m, org) + shl_(and_(n, m, org),1);
	}
	extern long long sub(const long long &n, const long long &m, protect_type type)
	{
		return add(n,-m,type);
	}
	extern long long mul(const long long &n, const long long &m, protect_type type)
	{
		const protect_type org = type;
		
		last_error_code = error_invalid_protect_type;
		return -1;
	}
}







/**
 * \brief 插入一段花指令，让两个函数在花指令中间执行
 * 参数：两个void类型的函数指针，让函数在花指令中间执行
 * 参数为空则相当于直接插入一长段无用花指令
 * 注意：不是给函数里面的代码插入花指令
 * 
 *流程：开始 -> my_junk_code_end1 -> 返回到标签back -> 跳转到 to_end -> 跳转到 real_end ->next_user_code
 */
extern void insert_junk_code_1(void(*func1)(), void(*func2)())
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
		call  insert_junk_code_1_part2;

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
		push next_user_code;
		jmp ebx;
		next_user_code:
	}
};

extern void insert_junk_code_1_part2()
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
		mov eax, 0xffffffff;
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

	if (func1 != nullptr) {
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
		lea eax, insert_junk_code_1_part2;
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
	if (func2 != nullptr) func2();


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



