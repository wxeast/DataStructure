#pragma once 

#include"stl_allocator.h" //     https://github.com/wxeast/STL

//无头单链表

template<class T>
struct  ListNoHeadNode
{
	T _data;//链表节点的值
	ListNoHeadNode<T> * _next;//链表的next指针
	ListNoHeadNode(const T& data = T())
		:_data(data)
		,_next(NULL)
	{}
};


template<class T,class  Alloc  = alloc>
class ListNoHead
{
	typedef ListNoHeadNode<T> Node;
public:
	ListNoHead()
		:_phead(NULL)
		,_ptail(NULL)
		,_size(0)
	{}
	void PushBack(const  T& data)
	{
		//分两种情况
		Node * node = BuyNode(data);
		//当前的链表没有节点
		if(_phead == NULL)
		{
			_phead = _ptail = node;
		}
		//当前链表有节点
		else 
		{
			_ptail->_next = node;
			_ptail =node;
		}
		++_size;
	}
	void PopBack()
	{
		//尾删 的话要分为三种情况
		//当前链表没有节点
		if(_phead ==NULL)
		{
			return  ;
		}
		//当前链表只有一个节点
		else if(_phead ==_ptail)
		{
			--_size;
			DeleteNode(_ptail);
			_phead = _ptail = NULL;
		}
		//当前链表不止一个节点
		else
		{
			Node * prev= GetTailPrev();
			prev->_next = NULL;
			DeleteNode(_ptail);
			_ptail = prev;
			--_size;
		}
	}
	void PushFront(const T& data)
	{
		Node*  node = BuyNode(data);
		//两种情况
		//当前链表没有节点
		if(_phead ==NULL)
		{
			_phead = _ptail = node;
		}
		else
		{
			node->_next = _phead;
			_phead = node;
		}
		++_size;
		
	}
	void PopFront()
	{
		//三种qingkuang
		if(_phead == NULL)
			return;
		else if(_phead == _ptail)
		{
			DeleteNode(_phead);
			_phead =_ptail =NULL;
			--_size;
		}
		else
		{
			Node * del = _phead;
			_phead =_phead->_next;
			DeleteNode(del);
			--_size;
		}
	}
	Node * Find(const T&  data)
	{
		Node*  cur = _phead;
		while(cur)
		{
			if(cur->_data == data)
				return  cur;
			cur =cur->_next;
		}
		return NULL;
	}
	void PrintList()
	{
		Node*  cur = _phead;
		while(cur)
		{
			cout<<cur->_data<<" ";
			cur= cur->_next;
		}
		cout<<endl;
	}
	~ListNoHead()
	{
		Node* cur =_phead;
		while(cur)
		{
			Node*  del = cur;
			cur= cur->_next;
			DeleteNode(del);
		}
		_phead = _ptail =NULL;
		_size = 0;
	}
protected:
	//得到尾节点的前一个节点
	Node *GetTailPrev()
	{
		if(_size < 2)
			return  _phead;
		Node*  cur =_phead;
		while(cur->_next != _ptail)
		{
			cur= cur->_next;
		}
		return cur;
	}
	Node * BuyNode(const T  &  data = T())
	{
		Node* node = data_allocator::allocate();
		new(node)Node(data);
		return node;
	}
	void DeleteNode(Node* del)
	{
		if(del)
		{
			data_allocator::deallocate(del);
		}
		del=NULL;
	}
protected:
	Node*  _phead;//头结点
	Node*  _ptail;//尾节点
	size_t  _size;//节点的个数
	typedef simple_alloc<Node,Alloc> data_allocator;




};

void TestListNoHead()
{
	ListNoHead<int> l;
	l.PushBack(1);
	l.PushBack(2);
	l.PushBack(3);
	l.PushBack(4);
	l.PushBack(5);
	l.PrintList();
	l.PopBack();
	l.PrintList();
	l.PopBack();
	l.PrintList();
	l.PopBack();
	l.PrintList();
	l.PopBack();
	l.PrintList();
	ListNoHeadNode<int> *  node = l.Find(10);

}