#include <Windows.h>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <winternl.h>
#include <tlhelp32.h>
#include <Psapi.h>
using namespace std;


#include "myprotect.h"

DWORD getParentPID(DWORD pid);
int getProcessName(DWORD pid, LPSTR fname, DWORD sz);


namespace protect
{
	long long rand_n;
	long long kernel_key=123456;
	void initialize(long long key)
	{
		last_error_code = success;
		if (kernel_key != 123456) {
			last_error_code = error_initial_twice;
			return;
		}
		srand((unsigned int)(time(nullptr)));
		kernel_key = key;
		anti_debug();
		
	}
	// void push_param(const long long& param1, const long long& param2)
	// {
	// 	int_param1 = param1;
	// 	int_param2 = param2;
	// }




	/**
	 * \brief		打印不能直接被中文搜索引擎搜到的字符串
	 * \param func	自己的处理字符串的函数，可为空
	 * \param str	要加密的字符串
	 */
	
	void print_string(void(*func)(char* str),char* str)
	{
		if(func!=nullptr) func(str);
		if(str[0]>=0)printf("{'\\x%x'", str[0]);
		else printf("{-0x%x", -str[0]);
		for(int i=1;i<strlen(str);++i)
		{
			if(str[i]>=0) printf(",'\\x%x'",str[i]);
			else printf(",-0x%x",-str[i]);
		}
		printf(",0}\n");
	}





	long long get_rand(const long long& l, const long long& r)
	{
		rand_n = l + rand() % (r - l + 1);
		return rand_n;
	}


	long long nor_(const long long& n, const long long& m)
	{
		insert_junk_code_1();
		return ~(n | m);
	}
	long long nand_(const long long& n, const long long& m)
	{
		insert_junk_code_1();
		return ~(n & m);
	}
	long long not_(const long long& n, protect_type type)
	{
		if (type == type_default) type = type_nor;
		const protect_type org = type;
		if (type == type_rand)
		{
			while (type == type_rand || type == type_default) type = (protect_type)get_rand(0, protect_types - 1);
		}
		if (type == type_nor) return nor_(n, n);
		else if (type == type_nand) return nand_(n, n);
		last_error_code = error_invalid_protect_type;
		return -1;
	}
	long long and_(const long long& n, const long long& m, protect_type type)
	{
		if (type == type_default) type = type_nor;
		const protect_type org = type;
		if (type == type_rand)
		{
			while (type == type_rand || type == type_default) type = (protect_type)get_rand(0, protect_types - 1);
		}
		if (type == type_nor) return nor_(not_(n), not_(m));
		else if (type == type_nand) return not_(nand_(n, m), org);
		last_error_code = error_invalid_protect_type;
		return -1;
	}
	long long or_(const long long& n, const long long& m, protect_type type)
	{
		if (type == type_default) type = type_nor;
		const protect_type org = type;
		if (type == type_rand)
		{
			while (type == type_rand || type == type_default) type = (protect_type)get_rand(0, protect_types - 1);
		}
		if (type == type_nor) return not_(nor_(n, m), org);
		else if (type == type_nand) return nand_(not_(n), not_(m));
		last_error_code = error_invalid_protect_type;
		return -1;
	}
	long long xor_(const long long& n, const long long& m, protect_type type)
	{
		if (type == type_default) type = type_nor;
		const protect_type org = type;
		if (type == type_rand)
		{
			while (type == type_rand || type == type_default) type = (protect_type)get_rand(0, protect_types - 1);
		}
		if (type == type_nor) return or_(nor_(not_(n), m), nor_(n, not_(m)), org);
		else if (type == type_nand) return nand_(not_(and_(n, not_(m, org), org)), not_(and_(not_(n, org), m, org)));
		last_error_code = error_invalid_protect_type;
		return -1;
	}
	long long xnor(const long long& n, const long long& m, protect_type type)
	{
		if (type == type_default) type = type_nor;
		const protect_type org = type;
		if (type == type_rand)
		{
			while (type == type_rand || type == type_default) type = (protect_type)get_rand(0, protect_types - 1);
		}
		if (type == type_nor) return not_(xor_(n, m), org);
		else if (type == type_nand) return not_(xor_(n, m), org);
		last_error_code = error_invalid_protect_type;
		return -1;
	}
	// template<typename T>
	// T shl_(const T& n, const long long& m)
	// {
	// 	return n << m;
	// }
	long long shl_(const long long& n, const long long& m)
	{
		return n << m;
	}
	long long shr_(const long long& n, const long long& m)
	{
		//111000110   n<<1
		//011100011   n
		//异或得到 n<<1与n不同的位
		//mask/2得到n与n>>1不同的位
		//xor得到结果

		last_error_code = success;
		long long result = n;
		for (int i = 1; i <= m; ++i)
		{
			long long sh = shl_(result, 1);
			long long mask1 = xor_(sh, result);
			mask1 /= 2;
			result = xor_(result, mask1);
		}
		return result;
	}

	long long add(const long long& n, const long long& m, const protect_type& type)
	{
		last_error_code = success;
		const protect_type org = type;
		return xor_(n, m, org) + shl_(and_(n, m, org), 1);
	}
	long long sub(const long long& n, const long long& m, const protect_type& type)
	{
		last_error_code = success;
		return add(n, -m, type);
	}
	long long mul(const long long& n, const long long& m, const protect_type& type)
	{
		//a*b=(a/2)*(b*2)	注意除不尽的情况
		last_error_code = success;
		const protect_type org = type;
		long long result = 0;
		long long a = n;
		long long b = m;
		while (abs(b) >= 2)
		{
			//m为奇数
			if (and_(b, 1, org) == 1) {
				result += a;
				a = shl_(a, 1);
				b = shr_(b, 1);
			}
			else {
				a = shl_(a, 1);
				b = shr_(b, 1);
			}
			shr_(b, 1);
		}
		if (b == 0) return 0;
		else if (b == 1) return add(result, a, org);
		else if (b == -1) return add(result, -a, org);
		last_error_code = error_impossible;
		return -1;
	}

	/**
	 * \brief 小于返回-1，等于返回0，大于返回1
	 */
	long long cmp(const long long& n, const long long& m, const protect_type & type)
	{
		long long a=n;
		long long b=m;
		//if(a>=b) swap(a,b);

		//	not[(a-b)*3]<0	即a>=b
		long long tmp=not_(mul(sub(a,b,type),3,type));
		if(tmp<0&&tmp!=-1)
		{
			return 1;
		}
		//[(a-b)*4]>>1 <0	即a<b
		if(shr_(mul(sub(a,b,type),4,type),1)<0)
		{
			return -1;
		}
		return 0;
	}

	/**
	 * \brief 在调用前添加几条语句，再调用。尽量让成功时表达式为假，跳转。下面是例子\n
	 * void* p;							\n
	 * get_label_address(p,label1);		\n
	 * _if_(!(a == 123), p, type_rand);	\n
		{								\n
			cout << "false\n";			\n
		}								\n
		label1:							\n
		cout << "true\n";				\n
		当a==123时输出成功
	 * \param expression 表达式
	 * \param address 要跳转到的地址
	 * 
	 */
	extern void _if_(const long long& expression, void* address, const protect_type & type)
	{
		last_error_code=success;
		long long result = cmp(expression,0,type);
		//address=(void*)xor_((long long)address,result,type);
		address=(void*)((long long)address^result);
		if(result==1) return;
		else if(result==0)
		{
			//address=(void*)xor_((long long)address,result,type);
			address=(void*)((long long)address^result);
			__asm
			{
				nop;
				nop;
				nop;
				nop;
				nop;
				mov eax, address;
				leave;
				push eax;
				retn 4;
			}
		}	
		else
		{
			last_error_code=error_impossible;
		}
	}










	void print_last_error()
	{
		printf("last_error_code = %d\n", last_error_code);

	}
}







/**
 * \brief 插入一段花指令，让两个函数在花指令中间执行
 *
 * 参数：两个void类型的函数指针，让函数在花指令中间执行
 * 参数为空则相当于直接插入一长段无用花指令
 * 注意：不是给函数里面的代码插入花指令
 * 流程：开始 -> insert_junk_code_1_part2 -> 返回到标签back -> 跳转到 to_end -> 跳转到 real_end ->next_user_code
 */
void insert_junk_code_1(void(*func1)(), void(*func2)())
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
		push next_user_code;
		push func2;
		jmp ebx;

	next_user_code:
		//花指令结束，这接着就是后面的代码了
	}

};

void insert_junk_code_1_part2()
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
		ret;

		//下面输入任意垃圾代码-----
		__emit 0xff; __emit 0x25; __emit 0xE8; __emit 0xE8; __emit 0x68; __emit 0x69;
		__emit 0xf5; __emit 0x20; __emit 0xE8; __emit 0xE9; __emit 0x66; __emit 0x69;
		//-------------------------
	}
}




// void my_virtual_code_begin1()
// {
// 	int org_eax = 0;
// 	int org_ebx = 0;
// 	int org_ecx = 0;
// 	int org_edx = 0;
// 	__asm
// 	{
// 		mov org_eax, eax;
// 		mov org_ebx, ebx;
// 		mov org_ecx, ecx;
// 		mov org_edx, edx;
//
// 	}
//
// }



void anti_debug()
{
	insert_junk_code_1(check_debugger_1, check_debugger_2);
	insert_junk_code_1(check_debugger_3, check_debugger_4);
	// CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)check_debugger_1, NULL, NULL, NULL);
	// CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)check_debugger_2,NULL,NULL,NULL);
	// CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)check_debugger_3,NULL,NULL,NULL);
	// CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)check_debugger_4,NULL,NULL,NULL);

}

void check_debugger_1()
{
	if (check_debugger_method_1())
	{
		insert_junk_code_1(exit_1,exit_2);
		exit_1();
		exit_2();
		insert_junk_code_1(exit_3,exit_4);
		exit_3();
		exit_4();
	}
}

void check_debugger_2()
{
	if (check_debugger_method_2())
	{
		insert_junk_code_1(exit_1,exit_2);
		exit_1();
		exit_2();
		insert_junk_code_1(exit_3,exit_4);
		exit_3();
		exit_4();
	}
}
void check_debugger_3()
{
	if (check_debugger_method_3())
	{
		// exit_1();
		// exit_2();
		// exit_3();
		// exit_4();

		//假装没有发现被调试，修改了kernel_key，让后续解密失败
	}
}
void check_debugger_4()
{
	if (check_debugger_method_4())
	{
		insert_junk_code_1(exit_1,exit_2);
		exit_1();
		exit_2();
		insert_junk_code_1(exit_3,exit_4);
		exit_3();
		exit_4();
	}
}
bool check_debugger_method_1()
{
	__try
	{
		__asm int 3;
	}
	__except (1)
	{
		return false;
	}
	//运行环境异常
	char text[] = { '\xD4' ^ 0x55,'\xCB' ^ 0x55,'\xD0' ^ 0x55,'\xD0' ^ 0x55,'\xBB' ^ 0x55,'\xB7' ^ 0x55,'\xBE' ^ 0x55,'\xB3' ^ 0x55,'\xD2' ^ 0x55,'\xEC' ^ 0x55,'\xB3' ^ 0x55,'\xA3' ^ 0x55,'\x0' };
	//错误
	char title[] = { '\xB4' ^ 0x55,'\xED' ^ 0x55,'\xCE' ^ 0x55,'\xF3' ^ 0x55,'\x0' };
	for (int i = 0; i < strlen(text); ++i) text[i] ^= 0x55;
	for (int i = 0; i < strlen(title); ++i) title[i] ^= 0x55;
	MessageBoxA(0, text, title, MB_ICONERROR | MB_TOPMOST | MB_OK);
	return true;
}
bool check_debugger_method_2()
{
	__try
	{
		RaiseException(DBG_PRINTEXCEPTION_C, 0, 0, 0);
	}
	__except (GetExceptionCode() == DBG_PRINTEXCEPTION_C)
	{
		return false;
	}
	//运行环境异常
	char text[] = { '\xD4' ^ 0x55,'\xCB' ^ 0x55,'\xD0' ^ 0x55,'\xD0' ^ 0x55,'\xBB' ^ 0x55,'\xB7' ^ 0x55,'\xBE' ^ 0x55,'\xB3' ^ 0x55,'\xD2' ^ 0x55,'\xEC' ^ 0x55,'\xB3' ^ 0x55,'\xA3' ^ 0x55,'\x0' };
	//错误
	char title[] = { '\xB4' ^ 0x55,'\xED' ^ 0x55,'\xCE' ^ 0x55,'\xF3' ^ 0x55,'\x0' };
	for (int i = 0; i < strlen(text); ++i) text[i] ^= 0x55;
	for (int i = 0; i < strlen(title); ++i) title[i] ^= 0x55;
	MessageBoxA(0, text, title, MB_ICONERROR | MB_TOPMOST | MB_OK);
	return true;
}



bool check_debugger_method_3()
{
	DWORD pid = GetCurrentProcessId();
	DWORD  ppid = getParentPID(pid);
	int e = 0;
	char fname[MAX_PATH] = { 0 };
	//char explorer1[]="explorer";
	char explorer1[]={'\x65' ,'\x78' ,'\x70' ,'\x6C' ,'\x6F' ,'\x72' ,'\x65' ,'\x72','\x0'};
	//char explorer2[]="EXPLORER";
	char explorer2[]={'\x45' ,'\x58' ,'\x50' ,'\x4C' ,'\x4F' ,'\x52' ,'\x45' ,'\x52','\x0'};
	e = getProcessName(ppid, fname, MAX_PATH);

	if(strstr(fname,explorer1)!=nullptr||strstr(fname,explorer2)!=nullptr) return false;	
	else    //父进程不是explorer，可能被调试
	{
		protect::kernel_key=0;
	}
	return true;
}



bool check_debugger_method_4()
{
	BOOL bDebug = false;
	HANDLE hProcess = GetCurrentProcess();
	CheckRemoteDebuggerPresent(hProcess, &bDebug);
	if (bDebug)
	{

		//char text[]="应用程序正常初始化(0xc00000005)失败，请单击\"确定\",终止应用程序";
		char text[] = { '\xD3','\xA6' ,'\xD3' ,'\xC3' ,'\xB3' ,'\xCC' ,'\xD0' ,'\xF2' ,'\xD5' ,'\xFD' ,'\xB3' ,'\xA3' ,'\xB3' ,'\xF5' ,'\xCA' ,
			'\xBC' ,'\xBB' ,'\xAF' ,'\x28' ,'\x30' ,'\x78' ,'\x43' ,'\x30' ,'\x30' ,'\x30' ,'\x30' ,'\x30' ,'\x30' ,'\x30' ,'\x35' ,'\x29' ,'\xCA',
			'\xA7' ,'\xB0' ,'\xDC' ,'\xA3' ,'\xAC' ,'\xC7' ,'\xEB' ,'\xB5' ,'\xA5' ,'\xBB' ,'\xF7' ,'\x22' ,'\xC8' ,'\xB7' ,'\xB6' ,
			'\xA8'  ,'\x22' ,'\xA3','\xAC' ,'\xD6' ,'\xD5' ,'\xD6' ,'\xB9' ,'\xD3' ,'\xA6' ,'\xD3' ,'\xC3' ,'\xB3' ,'\xCC' ,'\xD0','\xF2','\x0'
		};
		//char title[]="应用程序错误";
		char title[] = { '\xD3' ,'\xA6' ,'\xD3' ,'\xC3' ,'\xB3' ,'\xCC' ,'\xD0' ,'\xF2' ,'\xB4' ,'\xED' ,'\xCE' ,'\xF3','\x0' };
		MessageBoxA(0, text, title, MB_ICONERROR | MB_TOPMOST | MB_OK);
		return true;
	}
	return false;
}









void exit_1()
{
	__asm
	{
		xor eax, eax;
		xor ecx, ecx;
		xor edx, edx;
		xor ebx, ebx;
		xor ebp, ebp;
		xor esp, esp;
		xor esi, esi;
		xor edi, edi;
	}

}

void exit_2()
{
	__asm
	{
		mov eax, 0xfedcba98;
		push eax;
		jmp eax;
		call eax;
		mov[eax], 0;
	}
}

void exit_3()
{
	insert_junk_code_1([]() {exit(0); ExitProcess(0); }, []() {TerminateProcess(GetCurrentProcess(), 0); });
}

void exit_4()
{
	__asm
	{
		int 3;
		int 3;
	}
}












DWORD getParentPID(DWORD pid)
{
	HANDLE h = NULL;
	PROCESSENTRY32 pe = { 0 };
	DWORD ppid = 0;
	pe.dwSize = sizeof(PROCESSENTRY32);
	h = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (Process32First(h, &pe))
	{
		do
		{
			if (pe.th32ProcessID == pid)
			{
				ppid = pe.th32ParentProcessID;
				break;
			}
		} while (Process32Next(h, &pe));
	}
	CloseHandle(h);
	return (ppid);
}

int getProcessName(DWORD pid, LPSTR fname, DWORD sz)
{
	HANDLE h = NULL;
	int e = 0;
	h = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
	if (h)
	{
		if (GetModuleFileNameEx(h, NULL, fname, sz) == 0) e = GetLastError();
		CloseHandle(h);
	}
	else e = GetLastError();
	return (e);
}