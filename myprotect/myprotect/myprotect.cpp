#include <windows.h>
#include <iostream>
using namespace std;


#include "myprotect.h"

void (*junk_code_begin[101])();
void (*junk_code_end[101])();


/**
 * \brief ���̣���ʼ -> my_junk_code_end1 -> ���ص���ǩback -> ��ת�� to_end -> ��ת�� real_end
 * ��ʼ�����봦����ֱ�ӵ����Լ��ĺ��������
 *
 */
void my_junk_code_begin1()
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
		call  my_junk_code_end1;

		//����������������----------------
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
		mov eax, 0xFFFFFFFF;		//����ָ��
		xor eax, eax;
		je to_end;				//�������ת������ָ�������
		//����������������----------------
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
	
	//--�Զ���1 ���������ʼ���������������-----------------
	__asm
	{
		//�����Ļ�ָ��
		push 0x114514
		mov ebx,0x114514*2;
		mov ecx,0x114514*3;
		mov edx,0x114514*4;
		sub edx,ecx;
		sub ebx,eax;
	}

	MessageBoxA(0,"���ǳ�ʼ��1","title",MB_OK);

	//-------------------------------------------------------


	__asm{
		leave;
		pop eax;				//��ջƽ��,��Ϊ������ԭ����ַ
		popad;				//�ָ��Ĵ���
		mov ebx, real_end;	//��������ַ����ebx
		retn;
		//����������������------------------
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


	//�Զ���2 ���������ʼ������---------------------
	__asm{
		real_end:
	}

	MessageBoxA(0,"���ǳ�ʼ��2","title",MB_OK);

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
