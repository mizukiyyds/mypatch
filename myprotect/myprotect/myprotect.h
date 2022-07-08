#pragma once



namespace protect
{

	//static long long int_param1;
	//static long long int_param2;
	//extern void push_param(const long long& int_param1, const long long& int_param2);


	extern long long rand_n;
	__forceinline extern long long get_rand(const long long& l, const long long& r);

	extern void initialize(long long kernel_key=987654);				//��ʼ������

	//��initial��������key �����ڼ��ܺͽ��ܣ������⵽���򱻵��ԡ����ģ�key���Ϊ0��0���κ������Ϊ�䱾�����ܻ�ʧ��
	extern long long kernel_key;


	extern void print_string(void(*func)(char* str),char* str);


	const long long protect_types = 4;
	enum protect_type
	{
		type_default,	//Ĭ�ϣ����ڻ�ǵ����㣩
		type_nor,		//���ڻ������
		type_nand,		//�����������
		type_rand,		//���ѡ�񣬷�֧��ͬ�������ͬ
	};
	__forceinline extern long long nor_(const long long& n, const long long& m);
	__forceinline extern long long nand_(const long long& n, const long long& m);
	__forceinline extern long long not_(const long long& n, protect_type type = type_default);
	__forceinline extern long long and_(const long long& n, const long long& m, protect_type type = type_default);
	__forceinline extern long long or_(const long long& n, const long long& m, protect_type type = type_default);
	__forceinline extern long long xor_(const long long& n, const long long& m, protect_type type = type_default);
	__forceinline extern long long xnor(const long long& n, const long long& m, protect_type type = type_default);

	__forceinline extern long long shl_(const long long& n, const long long& m);
	__forceinline extern long long shr_(const long long& n, const long long& m);

	__forceinline extern long long add(const long long& n, const long long& m, const protect_type &type = type_default);		//�����ӷ�
	__forceinline extern long long sub(const long long& n, const long long& m, const protect_type &type = type_default);		//��������
	__forceinline extern long long mul(const long long& n, const long long& m, const protect_type &type = type_default);		//�����˷�
	__forceinline extern long long cmp(const long long& n, const long long& m, const protect_type &type = type_default);		//�����Ƚ�
	extern void if_(const long long& expression, void* address, const protect_type &type = type_default);		//����if��push retʵ����ת



	enum last_error
	{
		success,						//�ɹ�
		success_possible,				//������ɣ������ܽ��������Ԥ��
		error_invalid_protect_type,		//��Ч��protect_type
		error_impossible,				//������ִ�е��ĵط���ִ����
		error_initial_twice				//��ʼ������initial�����������λ�����(key���ܸ���)
	};
	static last_error last_error_code = success;
	void print_last_error();







}

__forceinline extern void insert_junk_code_1(void(*func1)() = nullptr, void(*func2)() = nullptr);
inline extern void insert_junk_code_1_part2();


//Ϊ�˷�ֹ����ֱ�ӱ��ɵ�����Ҫinline
//�����ԣ���ʱ�Ƿ񱻵��ԣ�
__forceinline extern void anti_debug();		


__forceinline extern void check_debugger_1();
__forceinline extern void check_debugger_2();
__forceinline extern void check_debugger_3();
__forceinline extern void check_debugger_4();	//���ĸ�������ֱ���˳���Ӱ������������ת��ʧ��



//-------------�ж��Ƿ񱻵��Է���-----------
//
__forceinline extern bool check_debugger_method_1();
__forceinline extern bool check_debugger_method_2();
__forceinline extern bool check_debugger_method_3();
__forceinline extern bool check_debugger_method_4();
//-----------------------------------------



//-------------�˳������-----------------
//
__forceinline extern void exit_1();
__forceinline extern void exit_2();
__forceinline extern void exit_3();
__forceinline extern void exit_4();
//----------------------------------------





//��б�ܺ��治���ж���ո�asm��Ҫ����������ÿһ��ǰҪ��__asm
//ʹ�þ�ʽ C ע�� ( /* comment */) �����ǻ��ʽע�� ( ; comment) ���� C ע�� ( // comment)

//ѹջ�ٵ������ѱ�ǩ��ַ���ݸ�ָ��
#define GET_LABEL_ADDRESS(p,label) do{		\
	void* tmp_p=0;							\
	__asm{									\
	__asm push label						\
	__asm pop tmp_p							\
	}										\
	p=tmp_p;								\
}while(0)



/**
 * \brief ����if��ʼ��־��������ʽ��������������ת��IF_END(mark)
 * ����ʹ���ʽ��������������if�м�����ݣ�Ч������
 * \param expression	�жϵı��ʽ
 * \param mark			����ĸ��ͷ�ı�ǩ����IF_END���Ӧ���������Ѿ������ı�ǩ��
 */
#define IF_BEGIN(expression,mark) do{			\
	void* tmp_addr=nullptr;						\
	GET_LABEL_ADDRESS(tmp_addr,mark);			\
	if_(expression,tmp_addr,protect::type_rand);

/**
 * \brief ����if������־
 * \param mark ����ĸ��ͷ�ı�ǩ����IF_BEGIN���Ӧ���������Ѿ������ı�ǩ��
 */
#define IF_END(mark)						\
	mark:									\
	break;									\
	}while(0);								\




/**
 * \brief ����if_else��ʼ��־��������ʽ��������������ת��ELSE(mark)
 * ����ʹ���ʽ��������������if�м�����ݣ�Ч������
 * \param expression	�жϵı��ʽ
 * \param mark			����ĸ��ͷ�ı�ǩ����ELSE���Ӧ���������Ѿ������ı�ǩ��
 */
#define IF_ELSE_BEGIN(expression,mark) do{		\
	void* tmp_addr=nullptr;						\
	GET_LABEL_ADDRESS(tmp_addr,mark);			\
	if_(expression,tmp_addr,protect::type_rand);

/**
 * \brief ����if_else��else��ʼ��־
 * \param mark ����ĸ��ͷ�ı�ǩ����IF_ELSE_BEGIN���Ӧ���������Ѿ������ı�ǩ��
 */
#define ELSE(mark)								\
	break;										\
	mark:										\
	__asm nop

/**
 * \brief ����if_else������־
 */
#define IF_ELSE_END()							\
	}while(0);

