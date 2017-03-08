#pragma once 
#include"BitMap.h"
#include"HashFunc.h"
#include<string>
typedef long long  LongType;
struct __HashFunc1
{
	template<class T>  
	size_t BKDRHash(const T *str)  
{  
	register size_t hash = 0;  
	while (size_t ch = (size_t)*str++)  
	{         
		hash = hash * 131 + ch;   // 也可以乘以31、131、1313、13131、131313..  
		// 有人说将乘法分解为位运算及加减法可以提高效率，如将上式表达为：hash = hash << 7 + hash << 1 + hash + ch;  
		// 但其实在Intel平台上，CPU内部对二者的处理效率都是差不多的，  
		// 我分别进行了100亿次的上述两种运算，发现二者时间差距基本为0（如果是Debug版，分解成位运算后的耗时还要高1/3）；  
		// 在ARM这类RISC系统上没有测试过，由于ARM内部使用Booth's Algorithm来模拟32位整数乘法运算，它的效率与乘数有关：  
		// 当乘数8-31位都为1或0时，需要1个时钟周期  
		// 当乘数16-31位都为1或0时，需要2个时钟周期  
		// 当乘数24-31位都为1或0时，需要3个时钟周期  
		// 否则，需要4个时钟周期  
		// 因此，虽然我没有实际测试，但是我依然认为二者效率上差别不大          
	}  
	return hash;  
}  
	size_t operator()(const string& key)
	{
		return  BKDRHash(key.c_str());
	}
};
struct __HashFunc2
{
	template<class T>  
	size_t SDBMHash(const T *str)  
{  
	register size_t hash = 0;  
	while (size_t ch = (size_t)*str++)  
	{  
		hash = 65599 * hash + ch;         
		//hash = (size_t)ch + (hash << 6) + (hash << 16) - hash;  
	}  
	return hash;  
}
	size_t operator()(const string& key)
	{
		return  SDBMHash(key.c_str());
	}
};
struct __HashFunc3
{
	template<class T>  
	size_t RSHash(const T *str)  
{  
	register size_t hash = 0;  
	size_t magic = 63689;     
	while (size_t ch = (size_t)*str++)  
	{  
		hash = hash * magic + ch;  
		magic *= 378551;  
	}  
	return hash;  
}  
	size_t operator()(const string& key)
	{
		return  RSHash(key.c_str());
	}
};
struct __HashFunc4
{
	template<class T>  
	size_t APHash(const T *str)  
{  
	register size_t hash = 0;  
	size_t ch;  
	for (long i = 0; ch = (size_t)*str++; i++)  
	{  
		if ((i & 1) == 0)  
		{  
			hash ^= ((hash << 7) ^ ch ^ (hash >> 3));  
		}  
		else  
		{  
			hash ^= (~((hash << 11) ^ ch ^ (hash >> 5)));  
		}  
	}  
	return hash;  
}  
	size_t operator()(const string& key)
	{
		return  APHash(key.c_str());
	}
};;
struct __HashFunc5
{
	template<class T>  
	size_t JSHash(const T *str)  
{  
	if(!*str)        // 这是由本人添加，以保证空字符串返回哈希值0  
		return 0;  
	register size_t hash = 1315423911;  
	while (size_t ch = (size_t)*str++)  
	{  
		hash ^= ((hash << 5) + ch + (hash >> 2));  
	}  
	return hash;  
}  
	size_t operator()(const string& key)
	{
		return  JSHash(key.c_str());
	}
};
template<class K,
	class HashFunc1 = __HashFunc1,
	class HashFunc2 = __HashFunc2,
	class HashFunc3 = __HashFunc3,
	class HashFunc4 = __HashFunc4,
	class HashFunc5 = __HashFunc5>
class BloomFilter
{
	
public:
	BloomFilter(const LongType num)
		:_size(num*10)
		,_bt(num*10)
	{}
	void  Set(const  K& key)
	{
		size_t hash1 = HashFunc1()(key);
		size_t hash2 = HashFunc2()(key);
		size_t hash3 = HashFunc3()(key);
		size_t hash4 = HashFunc4()(key);
		size_t hash5 = HashFunc5()(key);
		_bt.Set(hash1);
		_bt.Set(hash2);
		_bt.Set(hash3);
		_bt.Set(hash4);
		_bt.Set(hash5);
	}
	bool Test(const  K& key)
	{
		size_t hash1 = HashFunc1()(key);
		size_t hash2 = HashFunc2()(key);
		size_t hash3 = HashFunc3()(key);
		size_t hash4 = HashFunc4()(key);
		size_t hash5 = HashFunc5()(key);
		if(_bt.Test(hash1) == false)
			return false;
		if(_bt.Test(hash2) == false)
			return false;
		if(_bt.Test(hash3) == false)
			return false;
		if(_bt.Test(hash4) == false)
			return false;
		return _bt.Test(hash5);
	}

protected:
	BitMap _bt;
	LongType _size;

};

void  TestBloomFilter()
{
	BloomFilter<string> bt(-1);
	char  * url1 = "http://www.cnblogs.com/-clq/archive/2012/05/31/2528153.html";
	char  * url2 = "http://www.cnblogs.com/-clq/archive/2012/05/31/2528111.html";
	char  * url3 = "http://www.cnblogs.com/-clq/archive/2012/05/31/2358111.html";
	char  * url4  = "http://www.cnblogs.com/-clq/archive/2012/05/31/24328111.html";
	bt.Set(url1);
	bt.Set(url2);
	bt.Set(url3);
	bt.Set("xiao");
	bt.Set("xiao1");
	cout<<url1<<"  "<<bt.Test(url1)<<endl;
	cout<<url2<<"  "<<bt.Test(url2)<<endl;
	cout<<url3<<"  "<<bt.Test(url3)<<endl;
	cout<<url4<<"  "<<bt.Test(url4)<<endl;
	cout<<"xiao ?"<<" "<<bt.Test("xiao")<<endl;
	cout<<"xiao1 ?"<<" "<<bt.Test("xiao1")<<endl;

}