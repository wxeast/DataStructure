#pragma  once 
#include<assert.h>
#include"stl_allocator.h"//�Լ�ʵ�ֵĿռ�������     git��ַ  https://github.com/wxeast/STL/blob/master/stl_allocator.h
#include"stl_construct.h"//https://github.com/wxeast/STL/blob/master/stl_construct.h
#include"stl_iterator.h" //https://github.com/wxeast/STL/blob/master/stl_iterator.h
//ʵ�ֵĴ���ͷ����˫������
//����Ľڵ�
template<class T>
struct  ListNode
{
	T _data;//����ڵ��ֵ
	ListNode<T> * _next;//�����nextָ��
	ListNode<T> * _prev;//�����prevָ��
	ListNode(const T& data = T())
		:_data(data)
		,_next(NULL)
		,_prev(NULL)
	{}
};


//ʵ������ĵ�����
template<class T,class  Ref,class Ptr>
struct ListIterator
{
	typedef ListNode<T>  Node;
	typedef ListIterator<T,Ref,Ptr> Self;
	//��Ա����
	Node*  _node;
	ListIterator(Node*  node =NULL)
		:_node(node)
	{}


	/////////////////////////////////////////////////////////
	//�������������ͱ�    
	typedef typename InputIteratorTag			IteratorCategory;//�������ķ���  ������ʹ�õ���һ����Ƭ��
	//�ȿ���������ʾ�����������Ҳ���Ա�ʾ����˫�������
	typedef T									ValueType;//����������Ƕ����
	typedef size_t								DifferenceType;//�������ľ�����������
	typedef Ptr									Pointer;//����������Ƕָ��(��������ָָ�Ķ���)
	typedef Ref									Reference;//����������Ƕ������
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



//����ͷ����˫������
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
		//��ͷ�ڵ��    ǰָ�����ָ�붼ָ���Լ�
		_phead->_next = _phead;
		_phead->_prev = _phead;

	}
	
	void PushBack(const T& data)
	{
		Node *  node = _BuyNode(data);
		Node *  tail= _phead->_prev;//��ʾ��Ϊ�ڵ�

		tail->_next =node;
		node->_prev = tail;

		node->_next = _phead;
		_phead->_prev =node;
	}
	void PopBack()
	{
		Node* tail = _phead->_prev;//��ʾ���� ��ǰ��β�ڵ�
		Node* prev  = tail->_prev;//��ʾ����β�ڵ�ǰ�Ľڵ�

		//��ʾ�����������޽ڵ�
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
		Node* head = _phead->_next;//�õ����ǵ�ǰ������ĵ�һ�����

		_phead->_next = node;
		node->_prev = _phead;

		node->_next = head;
		head->_prev = node;
	}
	void PopFront()
	{
		Node*  head  =_phead->_next;//�õ���ǰ����ĵ�һ���ڵ�
		Node*  next  = head->_next;//�õ���ǰ����ĵڶ����ڵ�
		//��ʾ���ǵ�ǰ��������û��һ���ڵ�
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
	//ר���Ŀռ�������
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