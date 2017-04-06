#pragma  once 
#include<assert.h>
#include"stl_allocator.h"//自己实现的空间配置器     git地址  https://github.com/wxeast/STL/blob/master/stl_allocator.h
#include"stl_construct.h"//https://github.com/wxeast/STL/blob/master/stl_construct.h
#include"stl_iterator.h" //https://github.com/wxeast/STL/blob/master/stl_iterator.h
//实现的带有头结点的双向链表
//链表的节点
template<class T>
struct  ListNode
{
	T _data;//链表节点的值
	ListNode<T> * _next;//链表的next指针
	ListNode<T> * _prev;//链表的prev指针
	ListNode(const T& data = T())
		:_data(data)
		,_next(NULL)
		,_prev(NULL)
	{}
};


//实现链表的迭代器
template<class T,class  Ref,class Ptr>
struct ListIterator
{
	typedef ListNode<T>  Node;
	typedef ListIterator<T,Ref,Ptr> Self;
	//成员变量
	Node*  _node;
	ListIterator(Node*  node =NULL)
		:_node(node)
	{}


	/////////////////////////////////////////////////////////
	//迭代器的五种型别    
	typedef typename InputIteratorTag			IteratorCategory;//迭代器的分类  在这里使用的是一个切片来
	//既可以用来表示单向迭代器，也可以表示的是双向迭代器
	typedef T									ValueType;//迭代器的内嵌类型
	typedef size_t								DifferenceType;//迭代器的距离的最大容量
	typedef Ptr									Pointer;//迭代器的内嵌指针(迭代器所指指的对象)
	typedef Ref									Reference;//迭代器的内嵌的内容
	////////////////////////////////////////////////////////

	Ref  operator*()
	{
		return _node->_data;
	}

	Ptr  operator->()
	{
		return  &(operator*());
	}

	Self& operator++()
	{
		_node = _node->_next;
		return *this;
	}
	Self operator++(int)
	{
		Node *  node= _node;
		_node = _node->_next;
		return  node;
	}

	bool operator!=(Self&  it)
	{
		return  _node!=it._node;
	}
};



//带有头结点的双向链表
template<class T,class Alloc = alloc>
class List
{
	typedef ListNode<T> Node;
public:
	typedef ListIterator<T,T&,T*> Iterator;
	typedef ListIterator<T,const T&,const T*> ConstIterator;
	List()
		:_phead(_BuyNode())
	{
		//让头节点的    前指针与后指针都指向自己
		_phead->_next = _phead;
		_phead->_prev = _phead;

	}
	
	void PushBack(const T& data)
	{
		Node *  node = _BuyNode(data);
		Node *  tail= _phead->_prev;//表示的为节点

		tail->_next =node;
		node->_prev = tail;

		node->_next = _phead;
		_phead->_prev =node;
	}
	void PopBack()
	{
		Node* tail = _phead->_prev;//表示的是 当前的尾节点
		Node* prev  = tail->_prev;//表示的是尾节点前的节点

		//表示的是链表内无节点
		if(tail == _phead)
			return;
		else
		{
			prev->_next = _phead;
			_phead->_prev = prev;
			_DestroyNode(tail);
		}
	
	}
	void PushFront(const T& data)
	{
		Node* node= _BuyNode(data);
		Node* head = _phead->_next;//得到的是当前的链表的第一个结点

		_phead->_next = node;
		node->_prev = _phead;

		node->_next = head;
		head->_prev = node;
	}
	void PopFront()
	{
		Node*  head  =_phead->_next;//得到当前链表的第一个节点
		Node*  next  = head->_next;//得到当前链表的第二个节点
		//表示的是当前的链表内没有一个节点
		if(head == _phead)
			return ;
		else
		{
			next->_prev = _phead;
			_phead->_next = next;
			_DestroyNode(head);
		}
	
	}
	Iterator Find(const T& data)
	{
		Node* cur  = _phead->_next;
		while(cur!=_phead)
		{
			if(cur->_data == data)
				return cur;
			cur=cur->_next;
		}
		return NULL;
	}




	Iterator Begin()
	{
		return _phead->_next;
	}
	Iterator End()
	{
		return _phead;
	}

	ConstIterator Begin()const
	{
		return _phead->_next;
	}
	ConstIterator End()const
	{
		return _phead;
	}
	~List()
	{
		Clear();
		_DestroyNode(_phead);
		_phead =NULL;
	}
	void Clear()
	{
		Node * cur = _phead->_next;
		while(cur!= _phead)
		{
			Node *del =cur;
			cur =cur->_next;
			_DestroyNode(del);
		}
	}
protected:
	Node*  _BuyNode(const T&  data =T())
	{
		Node *  node  = data_allocator::allocate();
		Construct(node,data);
		return  node;
	}
	void _DestroyNode(Node* del)
	{
		if(del)
		{
			Destroy(del);
			data_allocator::deallocate(del);
		}
		del = NULL;
	}
protected:
	Node* _phead;
	//专属的空间配置器
	typedef simple_alloc<Node,Alloc>  data_allocator;
};
void TestList()
{
	List<int> l;
	l.PushFront(1);
	l.PushFront(2);
	l.PushFront(3);
	l.PushFront(4);
	l.PushFront(5);
	

	List<int>::Iterator it   =l.Begin();
	while(it != l.End())
	{
		cout<<*it<<" ";
		++it;
	}
	cout<<endl;
	cout<<"List:"<<Distance(l.Begin(),l.End())<<endl;

	
}