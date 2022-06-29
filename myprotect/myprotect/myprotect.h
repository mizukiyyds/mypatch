#pragma once




void my_junk_code_begin1(void(*func1)() ,void(*func2)());
void my_junk_code_end1();


extern void (*junk_code_begin[101])();

extern void (*junk_code_end[101])();


void initialize();


/**
 * \brief 首先调用push_param传递参数，再用其他函数
 */
namespace protect
{

	static int int_param1;
	static int int_param2;
	static int rand_n;
	extern void push_param(const int& int_param1,const int& int_param2);
	extern __forceinline int get_rand(int l,int r);

	const int protect_types=4;
	enum protect_type
	{
		type_default,
		type_nor,		//基于或非运算
		type_nand,		//基于与非运算
		type_rand,		//基于与非运算
	};
	extern __forceinline int nor_(int n,int m);
	extern __forceinline int nand_(int n,int m);
	extern __forceinline int not_(int n,protect_type = type_default);
	extern __forceinline int and_(int n,int m,protect_type = type_default);
	extern __forceinline int or_(int n,int m,protect_type = type_default);
	extern __forceinline int xor_(int n,int m,protect_type = type_default);
	extern __forceinline int xnor(int n,int m,protect_type = type_default);
	extern __forceinline int shl_(int n);
	extern __forceinline int shr_(int n);
	
	//<typename T> T operator+(T n,T m);
	extern  __forceinline int add(int n,int m,protect_type = type_default);



	enum last_error
	{
		success,						//成功
		success_possible,				//操作完成，但可能结果不符合预期
		error_invalid_protect_type,		//无效的protect_type
	};
	static last_error last_error_code=success;

};