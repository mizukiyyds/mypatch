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
		//���õ� n<<1��n��ͬ��λ
		//mask/2�õ�n��n>>1��ͬ��λ
		//xor�õ����
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
 * \brief ����һ�λ�ָ������������ڻ�ָ���м�ִ��
 * ����������void���͵ĺ���ָ�룬�ú����ڻ�ָ���м�ִ��
 * ����Ϊ�����൱��ֱ�Ӳ���һ�������û�ָ��
 * ע�⣺���Ǹ���������Ĵ�����뻨ָ��
 * 
 *���̣���ʼ -> my_junk_code_end1 -> ���ص���ǩback -> ��ת�� to_end -> ��ת�� real_end ->next_user_code
 */
extern void insert_junk_code_1(void(*func1)(), void(*func2)())
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
		call  insert_junk_code_1_part2;

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
		push next_user_code;
		jmp ebx;
		next_user_code:
	}
};

extern void insert_junk_code_1_part2()
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
		mov eax, 0xffffffff;
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

	if (func1 != nullptr) {
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


	//�Զ���2 ���������ʼ������---------------------
	__asm {
	real_end:
		pop func2;
	}
	if (func2 != nullptr) func2();


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



