#pragma  once 
#include <assert.h>
#include<vector>
#include<assert.h>
#include<string>
enum Status//表示每个节点的状态
{
	EXIST,
	DELETE,
	EMPTY
};
template<class K,class V>
struct HashTableNode
{
	K  _key;
	V _value;
	Status _status;
	HashTableNode(const K& key = K(),const  V& value = V())
		:_key(key)
		,_value(value)
		,_status(EMPTY)
	{}
};

template<class K>
struct __HashFunc
{
	size_t operator()(const K& key)
	{
		return key;
	}
};

template<>
struct __HashFunc<string>
{	
	size_t BKDRHash(const char* str)
	{
		register size_t  hash =  0;
		while(*str)
		{
			hash = hash*131 +*str;
			++str;
		}
		return hash;
	}
	size_t operator()(const string & key)
	{
		return BKDRHash(key.c_str());
	}
};



template<class K,class V,class HashFunc = __HashFunc<K>>
class HashTable
{
	typedef  HashTableNode<K,V> Node;
public:
	HashTable(size_t n)
		:_size(0)
	{
		assert(n>0);
		_table.resize(n);
	}
	~HashTable()
	{}
	//插入函数
	pair<Node* , bool>  Insert(const K& key,const  V& value =  V())
	{
		_CheckCapacity();
		Node * node = Find(key);//先查找
		if(node)//如果找到的话就返回插入失败
		{
			return make_pair(node,false);
		}
		size_t idex = _HashFunc(key);//得到哈希地址
		while(_table[idex]._status == EXIST)
		{
			idex++;//找到合适的位置
			if(idex==_table.size())//如果到结尾 
			{
				idex = 0 ;//从头重新来过 
			}
		}//直到找到合适的位置
		_table[idex]._key = key;
		_table[idex]._value  =value;//插入数据;
		_table[idex]._status =  EXIST;
		++_size;
		return  make_pair(&_table[idex],true);
	}
	//删除函数 
	bool Remove(const K& key)
	{
		Node*  node  = Find(key);//先查找数据 
		if(node)//找到的话 
		{
			node->_status =  DELETE;//将该位置的状态改为删除后的状态 
			--_size;//存入数据的个数--
			return true;
		}
		return false;
	}
	//查找函数
	Node * Find(const  K& key)
	{
		size_t  idex=  _HashFunc(key);
		while(_table[idex]._status!= EMPTY)
		{
			if(_table[idex]._key == key)
				return  &_table[idex];

			++idex;
			if(idex == _table.size())
				idex=  0 ;
		}
		return NULL;
	}
private:
	//检查容量 
	void _CheckCapacity()
	{
		if(_table.size() == 0 )
		{
			_table.resize(7);
			return ;
		}
		//如果负载因子大于 7的话，，就对 该哈希表进行扩容
		if (_size*10 / _table.size() >= 7)
		{
			size_t newsize = _GetNextprime(_table.size());
			HashTable<K,V,HashFunc> tem(newsize);
			for(size_t i = 0 ;i< _table.size();++i)
			{
				if(_table[i]._status == EXIST)
					tem.Insert(_table[i]._key,_table[i]._value);
			}
			_Swap(tem);
		}
	}
	void _Swap(HashTable<K,V,HashFunc>& tem)
	{
		_table.swap(tem._table);
		std::swap(_size,tem._size);
	}
	//得到下个素数，，，，，一般情况下，哈希表的长度都是素数 ，，，
	size_t  _GetNextprime(size_t n)
	{
		const int _PrimeSize= 28;//下面为一个素数表 
		static const unsigned long _PrimeList[_PrimeSize] =
		{
			53ul, 97ul, 193ul, 389ul, 769ul,
			1543ul, 3079ul, 6151ul, 12289ul, 24593ul,
			49157ul, 98317ul, 196613ul, 393241ul,
			786433ul,
			1572869ul, 3145739ul, 6291469ul, 12582917ul,
			25165843ul,
			50331653ul, 100663319ul, 201326611ul, 402653189ul,
			805306457ul,
			1610612741ul, 3221225473ul, 4294967291ul
		};
		for(size_t i = 0 ;i< _PrimeSize;++i)
		{
			if(n < _PrimeList[i])
				return _PrimeList[i];//找到要找的下一个素数 
		}
		assert(false);
	}
	//得到哈希地址 
	size_t  _HashFunc(const K& key)
	{
		HashFunc hf;
		size_t ret = hf(key);
		return ret % _table.size();
	}
private:
	vector<Node> _table;//用一个vector容器来存储 
	size_t _size;//存储数据的个数 
};
void TestHashTable()
{
	HashTable<int,int> ht(10);
	ht.Insert(89);
	ht.Insert(18);
	ht.Insert(49);
	ht.Insert(58);
	ht.Insert(9);
	ht.Insert(1);
	ht.Insert(3);
	ht.Insert(2);
	ht.Insert(4);
	ht.Insert(5);
	ht.Insert(6);
	ht.Insert(7);
	ht.Insert(8);
	ht.Insert(9);
	ht.Insert(10);
	ht.Remove(1);
	HashTable<string,string,__HashFunc<string>> ht1(1);
	ht1.Insert("左边","left");

	ht1.Insert("左边","left");
}