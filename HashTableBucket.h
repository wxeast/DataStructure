#pragma once 
#include<iostream>
using namespace std;
#include<vector>
#include<assert.h>
namespace Bucket
{
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

	template<class K,class V>
	struct HashTableNode
	{
		pair<K,V> _kv;//��һ��pair�ṹ 
		HashTableNode<K,V> * _next;
		HashTableNode(const pair<K,V>& kv)
			:_kv(kv)
			,_next(NULL)
		{}
	};
	//ʵ�ֵ�����
	template<class K,class V,class Ref,class Ptr>
	struct HashTableIterator
	{
		typedef HashTableNode<K,V> Node;
		typedef HashTableIterator<K,V,Ref,Ptr> Self;
		Node* _node;
		HashTable<K,V> * _ht;
		HashTableIterator(Node* node,HashTable<K,V> * ht)
			:_node(node)
			,_ht(ht)
		{}
		Self& operator++()
		{
			_node =_Next();
			return *this;
		}
		Self operator++(int)
		{
			Node* cur =_node;
			_node =_Next();
			return cur;
		}
		Self& operator--()
		{
			_node = _Prev();
			return *this;
		}
		Self operator--(int)
		{
			Node* cur =_node;
			_node = _Prev();
			return cur;
		}
		Ptr operator->()
		{
			return  &_node->_kv;
		}
		Ref & operator*()
		{
			return  _node->_kv;
		}
		Node* _Next()
		{
			if(_node->_next)
			{
				return _node->_next;
			}
			size_t idex = _ht->_HashFunc((_node->_kv).first)+1;
			for(;idex < _ht->_tables.size();++idex)
			{
				if(_ht->_tables[idex])
				{
					return _ht->_tables[idex];
				}
			}
			return NULL;
		}
		Node* _Prev()
		{
		
		}
		bool operator!=(const Self& tem)
		{
			return (_node!= tem._node);
		}
	};
	//HashFunc������ʾ��ͬ���͵�key���������ϣ��ַ
	template<class K,class V,class HashFunc = __HashFunc<K>>
	class HashTable
	{
		typedef HashTableNode<K,V> Node;
	public:
		typedef HashTableIterator<K,V,pair<K,V>&,pair<K,V>*> Iterator;
		typedef HashTableIterator<K,V,const pair<K,V>&,const pair<K,V>*> const_Iterator;
		friend struct Iterator;
		friend struct const_Iterator;

		
		HashTable()
			:_size(0)
		{}
		~HashTable()
		{
			Destory();
			_tables.clear();
		}
		//��պ���
		void Clear()
		{
			Destory();
			_size =0 ;
		}
		//���ٺ��� 
		void Destory()
		{
			for(size_t i = 0 ;i< _tables.size();++i)
			{
				Node* cur = _tables[i];
				while(cur)
				{
					Node* del = cur;
					cur=cur->_next;
					delete del;
				}
				_tables[i] = NULL;
			}
		}
		HashTable(const size_t n )
			:_size(0)
		{
			assert(n>0);
			_tables.resize(n);
			
		}
		//���뺯��
		pair<Iterator ,bool> Insert(const pair<K,V>& kv)
		{
			//�������
			_CheckCapacity();
			//�Ȳ��� 
			Node * cur = Find(kv.first)._node;
			if(cur)//�ҵ�����ʧ��
			{
				return make_pair(Iterator(cur,this),false);//���ظýڵ�ĵ������������Ҳ���ʧ�� 
			}
			Node * node = new Node(kv);//�½��ڵ� 
			size_t idex = _HashFunc(kv.first);
			cur = _tables[idex];
			if(cur)
			{
				node->_next =  cur;
			}
			_tables[idex] = node;
			++_size;
			return  make_pair(Iterator(node,this),true);//���������Ľڵ�ĵ���������������ɹ�
		}
		//���Һ���
		Iterator Find(const K& key)
		{
			size_t idex = _HashFunc(key);//�ҵ���Ӧ�Ĺ�ϣ��ַ 
			Node* cur = _tables[idex];
			while(cur)
			{
				if(cur->_kv.first == key)//�ҵ��Ļ�
					return Iterator(cur,this);//���ظ�λ�õĵ�����
				cur =cur ->_next;
			}
			return Iterator(NULL,this);//û֪���Ļ���������һ��ΪNULL�ĵ�����

		}
		bool Erase(const K& key)
		{
			Node * cur =Find(key)._node;
			if(cur)
			{
				size_t idex = _HashFunc(key);
				if(_tables[idex] == cur)
				{
					
					delete cur ;
					_tables[idex] = NULL;
				}
				else
				{
					Node* parent = _tables[idex];
					while(parent->_next != cur)
					{
						parent->_next = cur->_next;
						delete cur;
					}
				}
				--_size;
				return true;
			}
			return false;
		}
		Iterator  Begin()
		{
			for(size_t i =0 ;i< _tables.size();++i)
			{
				Node* cur = _tables[i];
				if(cur)
				{
					return  Iterator(cur,this);
				}
			}
			return  Iterator(NULL,this);
		}
		const_Iterator  Begin()const
		{
			for(size_t i =0 ;i< _tables.size();++i)
			{
				Node* cur = _tables[i];
				if(cur)
				{
					return  Iterator(cur,this);
				}
			}
			return  Iterator(NULL,this);
		}
		const_Iterator End()const
		{
			return  Iterator(NULL,this);
		}
		Iterator End()
		{
			return  Iterator(NULL,this);
		}
		//ʹ�õ����������ϣ���ڵ����� 
		void  Print()
		{
			Iterator it =Begin();
			while(it!= End())
			{
				cout<<it->first<<" ,"<<it->second<<endl;
				++it;
			}
			cout<<endl;
		}
	protected:
		//��ȡ��һ������ 
		size_t _GetNextPrime(const  size_t n)
		{
			const int _PrimeSize= 28;//������Ϊ�����ṩ��������ĳ���
			//������
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
			for(size_t i = 0 ;i < _PrimeSize;++i)
			{
				if(_PrimeList[i] > n)//�ҵ���һ������
				{
					return  _PrimeList[i];
				}
			}
			return _PrimeList[_PrimeSize-1];//���Ҫ���Ѿ�������ˣ�����ֱ�ӷ�������ֵ

		}
		//������� �������ж��Ƿ���Ҫ������ 
		void _CheckCapacity()
		{
			if(_tables.size() == _size)//�����������Ϊ1�Ļ�����Ҫ���� ����
			{
				size_t newsize = _GetNextPrime(_tables.size());//�õ���һ�� ���� ��Ĺ�ϣ��ĳ���
				HashTable<K,V,HashFunc> tem(newsize);//����һ���³��ȵĹ�ϣ��
				for(size_t i = 0 ;i < _tables.size();++i)//��ԭ��ϣ���ڵ�����ȫ�����������뵽���ɵ���������� 
				{
					Node* cur = _tables[i];
					while(cur)
					{
						tem.Insert(cur->_kv);
						cur = cur->_next;
					}
				}
				_Swap(tem);//��������� ���������Ǿͽ�ԭ��ϣ������������ɵ��¶������� ����  
			}
		}
		//������������HashTable�Ķ�������  
		void _Swap(HashTable<K,V>& tem)
		{
			_tables.swap(tem._tables);
			std::swap(_size,tem._size);
		}
		//���
		size_t _HashFunc(const K& key)
		{
			HashFunc hf;
			size_t ret = hf(key);
			return ret % _tables.size();
		}
	protected:
		vector<Node*> _tables;//��ϣ����ڲ��������ݣ��洢���������ڵ�ĵ�һ��ָ��
		size_t _size;//�洢�ڵ������ 
	};
	void TestHashTable()
	{
		HashTable<int,int> ht(4);
		ht.Print();
		ht.Insert(make_pair(1,0));
		ht.Insert(make_pair(2,0));
		ht.Insert(make_pair(3,0));
		ht.Insert(make_pair(4,0));
		ht.Insert(make_pair(5,0));
		ht.Insert(make_pair(6,0));
		ht.Insert(make_pair(7,0));
		ht.Insert(make_pair(8,0));
		ht.Insert(make_pair(9,0));
		ht.Insert(make_pair(33,0));
		ht.Insert(make_pair(23,0));
		ht.Insert(make_pair(13,0));
		ht.Insert(make_pair(83,0));
		ht.Erase(3);
		ht.Print();
		
		HashTable<string,string,__HashFunc<string>> ht1(4);
		ht1.Insert(make_pair("���","left"));
		ht1.Insert(make_pair("�ұ�","right"));
		ht1.Insert(make_pair("һ","one"));
		ht1.Insert(make_pair("��","two"));
		ht1.Insert(make_pair("��","three"));
		ht1.Print();

	}


	template<class K,class V,class HashFunc = __HashFunc<K>>
	class UnorderMap
	{
		typedef HashTable<K,V,HashFunc> Hash;
	public:
		typedef   typename Hash::Iterator  Iterator;
		typedef	  typename Hash::const_Iterator  const_Iterator;
		UnorderMap()
		{}
		//���뺯�� 
		pair<Iterator,bool> Insert(const pair<K,V> & kv)
		{
			return _ht.Insert(kv);//ֱ�ӵ��� hash��ĺ���
		}
		bool Erase(const K& key)
		{
			return  _ht.Erase(key);//���� ɾ������ 
		}
		Iterator  Find(const  K& key)
		{
			return _ht.Find(key);//����   ���Һ��� 
		}
		Iterator Begin()
		{
			return  _ht.Begin();//ֱ�ӵ���  
		}
		const_Iterator Begin()const 
		{
			return  _ht.Begin();
		}
		Iterator End()
		{
			return _ht.End();
		}
		const_Iterator End()const 
		{
			return _ht.End();
		}
		void Print()
		{
			_ht.Print();
		}
	protected:
		Hash _ht;
	};
	void TestUnoederMap()
	{
		UnorderMap<int,int> ht;
		ht.Insert(make_pair(1,0));
		ht.Insert(make_pair(2,0));
		ht.Insert(make_pair(3,0));
		ht.Insert(make_pair(4,0));
		ht.Insert(make_pair(5,0));
		ht.Insert(make_pair(6,0));
		ht.Insert(make_pair(7,0));
		ht.Insert(make_pair(8,0));
		ht.Insert(make_pair(9,0));
		ht.Insert(make_pair(33,0));
		ht.Insert(make_pair(23,0));
		ht.Insert(make_pair(13,0));
		ht.Insert(make_pair(83,0));
		ht.Erase(3);
		ht.Print();

		UnorderMap<string,string,__HashFunc<string>> ht1;
		ht1.Insert(make_pair("���","left"));
		ht1.Insert(make_pair("�ұ�","right"));
		ht1.Insert(make_pair("һ","one"));
		ht1.Insert(make_pair("��","two"));
		ht1.Insert(make_pair("��","three"));
		ht1.Print();

	}
};

namespace bucket
{
	enum Status
	{
		HEAD,
		LAST,
		FIRST,
		ELSE
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

	template<class K,class V>
	struct HashTableNode
	{
		pair<K,V> _kv;
		HashTableNode<K,V> * _prev;
		HashTableNode<K,V> * _next;
		Status _status;
		HashTableNode(const K& key  = K(),const V& value = V())
			:_kv(make_pair(key,value))
			,_prev(NULL)
			,_next(NULL)
			,_status(ELSE)
		{}
	};
	template<class K,class V,class Ref,class Ptr>
	struct HashTableIterator
	{
		typedef HashTableNode<K,V> Node;
		typedef HashTableIterator<K,V,Ref,Ptr> Self;
		Node* _node;
		HashTableIterator(Node* node)
			:_node(node)
		{}
		Self& operator++()
		{
			_node =_Next();
			return *this;
		}
		Self operator++(int)
		{
			Node* cur =_node;
			_node =_Next();
			return cur;
		}
		Self& operator--()
		{
			_node = _Prev();
			return *this;
		}
		Self operator--(int)
		{
			Node* cur =_node;
			_node = _Prev();
			return cur;
		}
		Ptr operator->()
		{
			return  &_node->_kv;
		}
		Ref & operator*()
		{
			return  _node->_kv;
		}
		Node* _Next()
		{
			if(_node->_next)
			{
				return _node->_next;
			}
			else
			{
				Node *cur = _node;
				while(cur->_status != FIRST)
				{
					cur= cur->_prev;
				}
				cur= cur->_prev;
				if(cur->_status ==LAST)
				{
					return NULL;
				}
				cur++;
				while(cur&&cur->_next ==NULL)
				{
					if(cur->_status== LAST)
					{
						return NULL;
					}
					cur++;
				}
				if(cur)
				{
					return cur->_next;
				}
				return NULL;
				
			}
		}
		Node* _Prev() 
		{
			if(_node->_status!=FIRST)
			{
				return _node->_prev;
			}
			else
			{
				Node* cur= _node->_prev;
				if(_node->_prev==FIRST && cur->_status==HEAD)
				{
					return NULL;
				}
				else
				{
					cur--;
					while(cur->_next==NULL)
					{
						if(cur->_status==FIRST)
							return NULL;
						cur--;
					}
					while(cur->_next)
					{
						cur= cur->_next;
					}
					return cur;
				}
			}
		}
		bool operator!=(const Self& tem)
		{
			return (_node!= tem._node);
		}
	};
	
	template<class K,class V,class HashFunc = __HashFunc<K>>
	class HashTable
	{
		typedef HashTableNode<K,V> Node;
	public:
		typedef HashTableIterator<K,V,pair<K,V>&,pair<K,V>*> Iterator;
		typedef HashTableIterator<K,V,const pair<K,V>&,const pair<K,V>*> const_Iterator;
		
		HashTable()
			:_size(0)
		{}
		~HashTable()
		{
			Destory();
		}
		void Clear()
		{
			Destory();
		}
		void Destory()
		{
			for(size_t i = 0 ;i< _tables.size();++i)
			{
				Node* cur = _tables[i]._next;
				while(cur)
				{
					Node* del = cur;
					cur=cur->_next;
					delete del;
				}
				_tables[i]._next  = NULL;
			}
		}
		HashTable(const size_t n )
			:_size(0)
		{
			assert(n>0);
			_tables.resize(n);
			_tables[n-1]._status =  LAST;
			_tables[0]._status = HEAD;
		}
		pair<Iterator ,bool> Insert(const K& key,const V& value)
		{
			_CheckCapacity();
			Node * node = Find(key)._node;
			if(node)
			{
				return make_pair(node,false);
			}
			Node * newNode = new Node(key,value);
			size_t idex = _HashFunc(key);
			newNode->_status = FIRST;
			newNode->_prev = &_tables[idex];
			Node * cur = _tables[idex]._next ; 
			if(cur)
			{
				cur->_status =  ELSE;
				cur->_prev = newNode;
			}
			_tables[idex]._next = newNode;
			newNode->_next = cur;
			++_size;
			return  make_pair(newNode,true);
		}
		Iterator Find(const  K& key)
		{
			size_t idex = _HashFunc(key);
			Node* cur = _tables[idex]._next;
			while(cur)
			{
				if(cur->_kv.first == key)
					return cur;
				cur =cur ->_next;
			}
			return NULL;

		}
		bool Erase(const K& key)
		{
			Node * cur =Find(key)._node;
			if(cur)
			{
				if(cur->_status!= FIRST)
				{
					cur->_prev ->_next = cur->_next;
				}
				else
				{
					Node* next = cur->_next;
					if(next)
					{
						next->_status = FIRST;
						next->_prev = cur->_prev;
					}
					cur->_prev->_next = NULL;
				}
				--_size;
				delete cur;
			}
			return false;
		}
		Iterator  Begin()
		{
			Node* cur = &_tables[0];
			while(cur->_next==NULL)
			{
				if(cur->_status == LAST)
				{
					return NULL;
				}
				cur++;
			}
			return cur->_next;
		}
		const_Iterator  Begin()const
		{
			Node* cur = &_tables[0];
			while(cur->_next==NULL)
			{
				if(cur->_status == LAST)
				{
					return NULL;
				}
				cur++;
			}
			return cur->_next;
		}
		const_Iterator End()const
		{
			return  NULL;
		}
		Iterator End()
		{
			return  NULL;
		}
		void  Print()
		{
			Iterator it =Begin();
			while(it!= End())
			{
				cout<<it->first<<" ,"<<it->second<<endl;
				++it;
			}
			cout<<endl;
		}
	protected:
		size_t _GetNextPrime(const  size_t n)
		{
			const int _PrimeSize= 28;
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
			for(size_t i = 0 ;i < _PrimeSize;++i)
			{
				if(_PrimeList[i] > n)
				{
					return  _PrimeList[i];
				}
			}
			return _PrimeList[_PrimeSize-1];

		}
		void _CheckCapacity()
		{
			if(_tables.size() == 0)
			{
				_tables.resize(7);
				_tables[6]._status =  LAST;
				_tables[0]._status = HEAD;
				return;
			}
			if(_tables.size() == _size)
			{
				size_t newsize = _GetNextPrime(_tables.size());
				HashTable<K,V,HashFunc> tem(newsize);
				for(size_t i = 0 ;i < _tables.size();++i)
				{
					Node* cur = _tables[i]._next;
					while(cur)
					{
						tem.Insert(cur->_kv.first,cur->_kv.second);
						cur = cur->_next;
					}
				}
				_Swap(tem);
			}
		}
		void _Swap(HashTable<K,V>& tem)
		{
			_tables.swap(tem._tables);
			std::swap(_size,tem._size);
		}
		size_t _HashFunc(const K& key)
		{
			HashFunc hf;
			size_t ret = hf(key);
			return ret % _tables.size();
		}
	protected:
		vector<Node> _tables;
		size_t _size;

	};
	
	
	void TestHashTable()
	{
		HashTable<int,int> ht(10);
		ht.Print();
		ht.Insert(89,0);
		ht.Insert(18,0);
		ht.Insert(49,0);
		ht.Insert(58,0);
		ht.Insert(9,0);
		ht.Insert(1,0);
		ht.Insert(3,0);
		ht.Insert(2,0);
		ht.Insert(4,0);
		//ht.Erase(3);
		ht.Insert(5,0);
		ht.Insert(6,0);
		ht.Insert(7,0);
		ht.Insert(8,0);
		ht.Insert(9,0);
		ht.Print();
		
		HashTable<string,string,__HashFunc<string>> ht1(4);
		ht1.Insert("���","left");
		ht1.Insert("�ұ�","right");
		ht1.Insert("һ","one");
		ht1.Insert("��","two");
		ht1.Insert("��","three");
		ht1.Print();

	}
};