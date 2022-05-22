#include <windows.h>
#include <iostream>
using namespace std;

LONG CALLBACK VectoredHandler(EXCEPTION_POINTERS* exception_info)
{
	printf("Eip -> %08X\n",exception_info->ContextRecord->Eip);
	return EXCEPTION_EXECUTE_HANDLER;
}

void SetHardwareBreakPoint()
{


}


int main()
{
	//CONTEXT ctx;
	//GetThreadContext(GetCurrentThread(),&ctx);

	//◊¢≤·“Ï≥£¥¶¿Ì
	PVOID handle = AddVectoredExceptionHandler(NULL,VectoredHandler);
	__asm int 3;
	RemoveVectoredExceptionHandler(handle);

	system("pause");
}
