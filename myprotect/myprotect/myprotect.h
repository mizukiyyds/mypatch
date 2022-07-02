#pragma once


//压栈再弹出，把标签地址传递给指针
#define get_label_address(address,label) __asm push label __asm pop address

//#define if_(expression,label,type)  void* label_addr;\get_label_address(label_addr,label);\_if_(expression,label_addr,type);


namespace protect
{

	//static long long int_param1;
	//static long long int_param2;
	//extern void push_param(const long long& int_param1, const long long& int_param2);


	extern long long rand_n;
	extern __forceinline long long get_rand(const long long& l, const long long& r);

	extern void initialize(long long kernel_key=987654);				//初始化函数

	//用initial函数设置key 。用于加密和解密，如果检测到程序被调试、更改，key会变为0
	extern long long kernel_key;


	extern void print_string(void(*func)(char* str),char* str);


	const long long protect_types = 4;
	enum protect_type
	{
		type_default,	//默认（基于或非的运算）
		type_nor,		//基于或非运算
		type_nand,		//基于与非运算
		type_rand,		//随机选择，分支不同但结果相同
	};
	__forceinline extern long long nor_(const long long& n, const long long& m);
	__forceinline extern long long nand_(const long long& n, const long long& m);
	__forceinline extern long long not_(const long long& n, protect_type type = type_default);
	__forceinline extern long long and_(const long long& n, const long long& m, protect_type type = type_default);
	__forceinline extern long long or_(const long long& n, const long long& m, protect_type type = type_default);
	__forceinline extern long long xor_(const long long& n, const long long& m, protect_type type = type_default);
	__forceinline extern long long xnor(const long long& n, const long long& m, protect_type type = type_default);

	// template<typename T>
	// __forceinline extern T shl_(const T& n, const int m);
	__forceinline extern long long shl_(const long long& n, const long long& m);
	__forceinline extern long long shr_(const long long& n, const long long& m);


	__forceinline extern long long add(const long long& n, const long long& m, const protect_type &type = type_default);		//混淆加法
	__forceinline extern long long sub(const long long& n, const long long& m, const protect_type &type = type_default);		//混淆减法
	__forceinline extern long long mul(const long long& n, const long long& m, const protect_type &type = type_default);		//混淆乘法

	__forceinline extern long long cmp(const long long& n, const long long& m, const protect_type &type = type_default);		//混淆比较
	extern void _if_(const long long& expression, void* address, const protect_type &type = type_default);		//混淆if，push ret实现跳转



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

__forceinline extern void insert_junk_code_1(void(*func1)() = nullptr, void(*func2)() = nullptr);
inline extern void insert_junk_code_1_part2();


//为了防止函数直接被干掉，需要inline
//反调试（此时是否被调试）
__forceinline extern void anti_debug();		


__forceinline extern void check_debugger_1();
__forceinline extern void check_debugger_2();
__forceinline extern void check_debugger_3();
__forceinline extern void check_debugger_4();	//第四个方法不直接退出，影响程序后续流程转向失败



//-------------判断是否被调试方法-----------
//
__forceinline extern bool check_debugger_method_1();
__forceinline extern bool check_debugger_method_2();
__forceinline extern bool check_debugger_method_3();
__forceinline extern bool check_debugger_method_4();
//-----------------------------------------



//-------------退出或崩溃-----------------
//
__forceinline extern void exit_1();
__forceinline extern void exit_2();
__forceinline extern void exit_3();
__forceinline extern void exit_4();
//----------------------------------------

