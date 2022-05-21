#include <iostream>
#include <windows.h>

using namespace std;

int real_cnt = 0;					//正常情况下的数据内容之和


DWORD mythread(LPVOID lp_thread_parameter)
{
	while (1) {
		int address = 0x00401000;		//检查的起始地址
		const int len = 3000;			//检查的长度
		char data = 0;					//读取的内容
		int cnt = 0;					//数据之和
		for (int i = 0; i < len; i++)
		{
			ReadProcessMemory((HANDLE)-1, LPVOID(address + i), &data, 1, NULL);
			cnt += data;
		}
		if (cnt != real_cnt)
		{
			exit(-1);
		}
	}
}


int main()
{
	//需要在项目中设置基址,但碰到如下错误
	/*链接器工具错误 LNK1224
	无效的映像基址。你为映像指定了无效的基址。
	基址的大小必须为64KB， (最后四个十六进制数字必须为零)
	并且必须适合32位有符号或无符号值。
	 于是从0x00401000改成了0x00400000 编译成功
	 */
	//先计算一次，再开线程持续对比

	//调试了一下发现代码段是从00401000开始的
	int address = 0x00401000;		//检查的起始地址
	const int len = 3000;			//检查的长度
	char data = 0;				//读取的内容
	for (int i = 0; i < len; i++)
	{
		ReadProcessMemory((HANDLE)-1, LPVOID(address + i), &data, 1, NULL);
		real_cnt += data;
	}
	//-------------------------------------------------------

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)mythread, NULL, NULL, NULL);

	getchar();

	//下断发现程序立即退出，退出代码为-1


}
