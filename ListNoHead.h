#pragma once 

#include"stl_allocator.h" //     https://github.com/wxeast/STL

//��ͷ������

template<class T>
struct  ListNoHeadNode
{
	T _data;//����ڵ��ֵ
	ListNoHeadNode<T> * _next;//�����nextָ��
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
		//���������
		Node * node = BuyNode(data);
		//��ǰ������û�нڵ�
		if(_phead == NULL)
		{
			_phead = _ptail = node;
		}
		//��ǰ�����нڵ�
		else 
		{
			_ptail->_next = node;
			_ptail =node;
		}
		++_size;
	}
	void PopBack()
	{
		//βɾ �Ļ�Ҫ��Ϊ�������
		//��ǰ����û�нڵ�
		if(_phead ==NULL)
		{
			return  ;
		}
		//��ǰ����ֻ��һ���ڵ�
		else if(_phead ==_ptail)
		{
			--_size;
			DeleteNode(_ptail);
			_phead = _ptail = NULL;
		}
		//��ǰ����ֹһ���ڵ�
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
		//�������
		//��ǰ����û�нڵ�
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
		//����qingkuang
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
	//�õ�β�ڵ��ǰһ���ڵ�
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
	Node*  _phead;//ͷ���
	Node*  _ptail;//β�ڵ�
	size_t  _size;//�ڵ�ĸ���
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