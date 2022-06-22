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
		type_nor	//���ڻ������ģ����������
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
	 * \brief ģ��ӷ�
	 * n+m = n^m + (n&m)*2
	 * \return 
	 */
	/*template<typename T> 
	int operator+(int n,T m)
	{
		cout<<"ģ��ӷ�\n";
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
 * \brief ���̣���ʼ -> my_junk_code_end1 -> ���ص���ǩback -> ��ת�� to_end -> ��ת�� real_end
 * ��ʼ�����봦����ֱ�ӵ����Լ��ĺ��������
 *
 */
void my_junk_code_begin1(void(*func1)() ,void(*func2)())
{
	int ret_address = 0;
	__asm
	{
		mov edx, back;
		mov ret_address, 0x114514;	//����ָ��
		mov ret_address, edx;		//ȡ�������ص�ַback
		push ret_address;			//���������ص�ַѹջ
		mov ret_address, 0x114514;	//����ָ��
		pushad;						//���浱ǰ�Ĵ���
		mov edx, func1;				//��func1���ݸ����溯��ִ��
		call  my_junk_code_end1;

		//����������������----------------
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
		mov eax, 0xFFFFFFFF;		//����ָ��
		xor eax, eax;
		je to_end;				//�������ת������ָ�������
		//����������������----------------
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
	//--�Զ���1 ���������ʼ���������������-----------------
	__asm
	{
		push edx;

		//�����Ļ�ָ���������ָ�------
		push 0x114514
		mov ebx, 0x1919810;
		mov ecx, 0x5201314;
		mov edx, 0x654321;
		sub edx, ecx;
		sub ebx, eax;
		pop eax;
		//------------------------------------

		pop esi;				//esi����edx���ݵ�func1��ַ�������￪ʼ��Ҫ����esi

		//�����Ļ�ָ��-----------------------
		mov eax,0xffffffff;
		push eax;
		mov ebx, 0x1;
		mov ecx, 0x2;
		sub ebx, eax;

		cmp ecx, ebx;	//һ�����
		//jnz FFFFFFFF	�ض�����ʵ��
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
		pop eax;			//��ջƽ��,��Ϊ������ԭ����ַ
		popad;				//�ָ��Ĵ���
		mov ebx, real_end;	//��������ַ����ebx
		retn;
		//����������������------------------
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


	//�Զ���2 ���������ʼ������---------------------
	__asm {
	real_end:
		pop func2;
	}
	if(func2!=nullptr) func2();
	

	//-------------------------------------------------

	__asm {
		leave;
		ret;

		//��������������������-----
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
