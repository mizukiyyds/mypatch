#pragma once






namespace protect
{

	//static long long int_param1;
	//static long long int_param2;
	static long long rand_n;


	void initialize(long long kernel_key=987654);				//初始化函数
	//extern void push_param(const long long& int_param1, const long long& int_param2);
	extern __forceinline long long get_rand(const long long& l, const long long& r);


	//自己设置key 。用于加密和解密，如果检测到程序被调试、更改，key会被改变
	static long long kernel_key= 123456;

	const long long protect_types = 4;
	enum protect_type
	{
		type_default,	//默认（基于或非的运算）
		type_nor,		//基于或非运算
		type_nand,		//基于与非运算
		type_rand,		//随机选择，分支不同但结果相同
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


	__forceinline long long add(const long long& n, const long long& m, const protect_type & = type_default);		//混淆加法
	__forceinline long long sub(const long long& n, const long long& m, const protect_type & = type_default);		//混淆减法
	__forceinline long long mul(const long long& n, const long long& m, const protect_type & = type_default);		//混淆乘法



	enum last_error
	{
		success,						//成功
		success_possible,				//操作完成，但可能结果不符合预期
		error_invalid_protect_type,		//无效的protect_type
		error_impossible,				//不可能执行到的地方被执行了
		error_initial_twice				//初始化函数initial被调用了两次或以上(key不能更改)
	};
	static last_error last_error_code = success;
	void print_last_error();







}

__forceinline void insert_junk_code_1(void(*func1)() = nullptr, void(*func2)() = nullptr);
inline void insert_junk_code_1_part2();


//为了防止函数直接被干掉，需要inline
//反调试（此时是否被调试）
__forceinline void anti_debug();		


__forceinline void check_debugger_1();
__forceinline void check_debugger_2();
__forceinline void check_debugger_3();
__forceinline void check_debugger_4();	//第四个方法不直接退出，影响程序后续流程转向失败



//-------------判断是否被调试方法-----------
//
__forceinline bool check_debugger_method_1();
__forceinline bool check_debugger_method_2();
__forceinline bool check_debugger_method_3();
__forceinline bool check_debugger_method_4();
//-----------------------------------------



//-------------退出或崩溃-----------------
//
__forceinline void exit_1();
__forceinline void exit_2();
__forceinline void exit_3();
__forceinline void exit_4();
//----------------------------------------

