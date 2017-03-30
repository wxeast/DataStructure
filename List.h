#pragma  once 
#include<assert.h>
#include"stl_allocator.h"//自己实现的空间配置器 
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
//带有头结点的双向链表
template<class T,class Alloc = alloc>
class List
{
	typedef ListNode<T> Node;
public:
	List()
		:_phead(BuyNode())
	{
		new(_phead)Node();
	}
	
	void PushBack(const T& data)
	{
		Node *  node = BuyNode(data);
		Node*   tail=  _phead->_prev;//得到的是链表的尾节点
		if(tail)//要是尾节点存在
		{
			tail->_next = node;
			node->_prev =  tail; 
			node->_next = _phead;
		}
		else//尾节点不存在、、、表示的是 链表的内部没有节点
		{
			_phead->_next = node;
			node->_next = _phead;
			node->_prev = _phead;
		}
		_phead ->_prev = node;	
	}
	void PopBack()
	{
		Node* tail = _phead->_prev;
		Node* head = _phead->_next;
		if(tail == NULL)//表示的是当前的链表为空
		{
			return  ;
		}
		else if(head == tail)//表示的当前的链表只有这么一个节点
		{
			_phead->_next = _phead->_prev =NULL;
			DeleteNode(tail);
		}
		else//表示的是链表不只有一个节点
		{
			Node * prev =tail->_prev;
			prev->_next = _phead;
			_phead->_prev = prev;
			DeleteNode(tail);
		}
	}
	void PushFront(const T& data)
	{
		Node*  node = BuyNode(data);
		Node* head = _phead->_next;
		_phead->_next = node;
		node->_prev =_phead; 
		if(head)
		{
			head->_prev  =node;
			node->_next  =head;
		}
		else
		{
			node->_next = _phead;
			_phead->_prev  =node;
		}
		
		
	}
	void PopFront()
	{
		Node*  head  =_phead->_next;
		if(head ==NULL)//当前链表无节点
		{
			return ;
		}
		else 
		{
			Node* next =  head->_next;
			_phead->_next = next;
			if(next)
				next->_prev = _phead;
		}

	}
	Node*  Find(const T& data)
	{
		Node* cur  = _phead->_next;
		while(cur&&cur!=_phead)
		{
			if(cur->_data == data)
				return cur;
			cur=cur->_next;
		}
		return NULL;
	}
	~List()
	{
		Node* cur  = _phead->_next;
		while(cur&&cur!=_phead)
		{
			Node *del  = cur;
			cur->_next;
			DeleteNode(del);
		}
		DeleteNode(_phead);
	}
	void PrintList()
	{
		Node*  cur =_phead->_next;
		while(cur&&cur!=_phead)
		{
			cout<<cur->_data<<" ";
			cur =cur->_next;
		}
		cout<<endl;
	}
	Node*  GetHead()
	{
		return _phead;
	}
protected:
	Node*  BuyNode(const T&  data =T())
	{
		Node *  node  = data_allocator::allocate();
		new(node)Node(data);
		return  node;
	}
	void DeleteNode(Node* del)
	{
		if(del)
		{
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
	l.PrintList();
	ListNode<int> *  node = l.Find(10);
	
}