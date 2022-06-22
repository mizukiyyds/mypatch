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
	extern void push_param(const int& int_param1,const int& int_param2);
	enum protect_type;
	inline int nor_(int n,int m);
	inline int not_(int n);
	inline int and_(int n,int m);
	inline int or_(int n,int m);
	inline int xor_(int n,int m);
	inline int xnor(int n,int m);
	inline int shl_(int n);
	inline int shr_(int n);
	
	//<typename T> T operator+(T n,T m);
	extern inline int add(int n,int m);
};