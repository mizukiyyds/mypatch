#include <iostream>
#include <windows.h>

using namespace std;

int real_cnt = 0;					//��������µ���������֮��


DWORD mythread(LPVOID lp_thread_parameter)
{
	while (1) {
		int address = 0x00401000;		//������ʼ��ַ
		const int len = 3000;			//���ĳ���
		char data = 0;					//��ȡ������
		int cnt = 0;					//����֮��
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
	//��Ҫ����Ŀ�����û�ַ,���������´���
	/*���������ߴ��� LNK1224
	��Ч��ӳ���ַ����Ϊӳ��ָ������Ч�Ļ�ַ��
	��ַ�Ĵ�С����Ϊ64KB�� (����ĸ�ʮ���������ֱ���Ϊ��)
	���ұ����ʺ�32λ�з��Ż��޷���ֵ��
	 ���Ǵ�0x00401000�ĳ���0x00400000 ����ɹ�
	 */
	//�ȼ���һ�Σ��ٿ��̳߳����Ա�

	//������һ�·��ִ�����Ǵ�00401000��ʼ��
	int address = 0x00401000;		//������ʼ��ַ
	const int len = 3000;			//���ĳ���
	char data = 0;				//��ȡ������
	for (int i = 0; i < len; i++)
	{
		ReadProcessMemory((HANDLE)-1, LPVOID(address + i), &data, 1, NULL);
		real_cnt += data;
	}
	//-------------------------------------------------------

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)mythread, NULL, NULL, NULL);

	getchar();

	//�¶Ϸ��ֳ��������˳����˳�����Ϊ-1


}
