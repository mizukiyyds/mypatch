#pragma once






namespace protect
{

	//static long long int_param1;
	//static long long int_param2;
	static long long rand_n;


	void initialize(long long kernel_key=987654);				//��ʼ������
	//extern void push_param(const long long& int_param1, const long long& int_param2);
	extern __forceinline long long get_rand(const long long& l, const long long& r);


	//�Լ�����key �����ڼ��ܺͽ��ܣ������⵽���򱻵��ԡ����ģ�key�ᱻ�ı�
	static long long kernel_key= 123456;

	const long long protect_types = 4;
	enum protect_type
	{
		type_default,	//Ĭ�ϣ����ڻ�ǵ����㣩
		type_nor,		//���ڻ������
		type_nand,		//�����������
		type_rand,		//���ѡ�񣬷�֧��ͬ�������ͬ
	};
	__forceinline long long nor_(const long long& n, const long long& m);
	__forceinline long long nand_(const long long& n, const long long& m);
	__forceinline long long not_(const long long& n, protect_type = type_default);
	__forceinline long long and_(const long long& n, const long long& m, protect_type = type_default);
	__forceinline long long or_(const long long& n, const long long& m, protect_type = type_default);
	__forceinline long long xor_(const long long& n, const long long& m, protect_type = type_default);
	__forceinline long long xnor(const long long& n, const long long& m, protect_type = type_default);
	__forceinline long long shl_(const long long& n, const long long& m);
	__forceinline long long shr_(const long long& n, const long long& m);


	__forceinline long long add(const long long& n, const long long& m, const protect_type & = type_default);		//�����ӷ�
	__forceinline long long sub(const long long& n, const long long& m, const protect_type & = type_default);		//��������
	__forceinline long long mul(const long long& n, const long long& m, const protect_type & = type_default);		//�����˷�



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

__forceinline void insert_junk_code_1(void(*func1)() = nullptr, void(*func2)() = nullptr);
inline void insert_junk_code_1_part2();


//Ϊ�˷�ֹ����ֱ�ӱ��ɵ�����Ҫinline
//�����ԣ���ʱ�Ƿ񱻵��ԣ�
__forceinline void anti_debug();		


__forceinline void check_debugger_1();
__forceinline void check_debugger_2();
__forceinline void check_debugger_3();
__forceinline void check_debugger_4();	//���ĸ�������ֱ���˳���Ӱ������������ת��ʧ��



//-------------�ж��Ƿ񱻵��Է���-----------
//
__forceinline bool check_debugger_method_1();
__forceinline bool check_debugger_method_2();
__forceinline bool check_debugger_method_3();
__forceinline bool check_debugger_method_4();
//-----------------------------------------



//-------------�˳������-----------------
//
__forceinline void exit_1();
__forceinline void exit_2();
__forceinline void exit_3();
__forceinline void exit_4();
//----------------------------------------

