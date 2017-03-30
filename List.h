#pragma  once 
#include<assert.h>
#include"stl_allocator.h"//�Լ�ʵ�ֵĿռ������� 
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
//����ͷ����˫������
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
		Node*   tail=  _phead->_prev;//�õ����������β�ڵ�
		if(tail)//Ҫ��β�ڵ����
		{
			tail->_next = node;
			node->_prev =  tail; 
			node->_next = _phead;
		}
		else//β�ڵ㲻���ڡ�������ʾ���� ������ڲ�û�нڵ�
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
		if(tail == NULL)//��ʾ���ǵ�ǰ������Ϊ��
		{
			return  ;
		}
		else if(head == tail)//��ʾ�ĵ�ǰ������ֻ����ôһ���ڵ�
		{
			_phead->_next = _phead->_prev =NULL;
			DeleteNode(tail);
		}
		else//��ʾ��������ֻ��һ���ڵ�
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
		if(head ==NULL)//��ǰ�����޽ڵ�
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
	l.PrintList();
	ListNode<int> *  node = l.Find(10);
	
}