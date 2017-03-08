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
		hash = hash * 131 + ch;   // Ҳ���Գ���31��131��1313��13131��131313..  
		// ����˵���˷��ֽ�Ϊλ���㼰�Ӽ����������Ч�ʣ��罫��ʽ���Ϊ��hash = hash << 7 + hash << 1 + hash + ch;  
		// ����ʵ��Intelƽ̨�ϣ�CPU�ڲ��Զ��ߵĴ���Ч�ʶ��ǲ��ģ�  
		// �ҷֱ������100�ڴε������������㣬���ֶ���ʱ�������Ϊ0�������Debug�棬�ֽ��λ�����ĺ�ʱ��Ҫ��1/3����  
		// ��ARM����RISCϵͳ��û�в��Թ�������ARM�ڲ�ʹ��Booth's Algorithm��ģ��32λ�����˷����㣬����Ч��������йأ�  
		// ������8-31λ��Ϊ1��0ʱ����Ҫ1��ʱ������  
		// ������16-31λ��Ϊ1��0ʱ����Ҫ2��ʱ������  
		// ������24-31λ��Ϊ1��0ʱ����Ҫ3��ʱ������  
		// ������Ҫ4��ʱ������  
		// ��ˣ���Ȼ��û��ʵ�ʲ��ԣ���������Ȼ��Ϊ����Ч���ϲ�𲻴�          
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
	if(!*str)        // �����ɱ�����ӣ��Ա�֤���ַ������ع�ϣֵ0  
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