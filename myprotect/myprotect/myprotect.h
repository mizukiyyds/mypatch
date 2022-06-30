#pragma once






namespace protect
{

	static long long int_param1;
	static long long int_param2;
	static long long rand_n;


	void initialize();				//初始化函数
	extern void push_param(const long long& int_param1,const long long& int_param2);
	extern __forceinline long long get_rand(const long long &l,const long long &r);

	const long long protect_types=4;
	enum protect_type
	{
		type_default,
		type_nor,		//基于或非运算
		type_nand,		//基于与非运算
		type_rand,		//基于与非运算
	};
	__forceinline long long nor_(const long long &n,const long long &m);
	__forceinline long long nand_(const long long &n,const long long &m);
	__forceinline long long not_(const long long &n,protect_type = type_default);
	__forceinline long long and_(const long long &n,const long long &m,protect_type = type_default);
	__forceinline long long or_(const long long &n,const long long &m,protect_type = type_default);
	__forceinline long long xor_(const long long &n,const long long &m,protect_type = type_default);
	__forceinline long long xnor(const long long &n,const long long &m,protect_type = type_default);
	__forceinline long long shl_(const long long &n,const long long &m);
	__forceinline long long shr_(const long long &n,const long long &m);
	
	//<typename T> T operator+(T n,T m);
	__forceinline long long add(const long long &n,const long long &m,protect_type = type_default);
	__forceinline long long sub(const long long &n,const long long &m,protect_type = type_default);



	enum last_error
	{
		success,						//成功
		success_possible,				//操作完成，但可能结果不符合预期
		error_invalid_protect_type,		//无效的protect_type
	};
	static last_error last_error_code=success;




	


	
}

__forceinline void insert_junk_code_1(void(*func1)()=nullptr ,void(*func2)()=nullptr);
inline void insert_junk_code_1_part2();;