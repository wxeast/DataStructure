#pragma  once

#include"stl_allocator.h"//�Լ�ʵ�ֵĿռ�������     git��ַ     https://github.com/wxeast/STL/blob/master/stl_allocator.h
#include"stl_construct.h"//https://github.com/wxeast/STL/blob/master/stl_construct.h
#include"stl_iterator.h"//https://github.com/wxeast/STL/blob/master/stl_iterator.h
#include<string>

template<class  T,class  Alloc =alloc>
class Vector
{
public:
	typedef T*  Iterator;
	typedef const T*  ConstIterator;

	Vector()
		:_start(NULL)
		,_finish(NULL)
		,_endOfCapacity(NULL)
	{}
	Iterator Begin()
	{
		return  _start;
	}
	Iterator End()
	{
		return _finish;
	}
	size_t  Size()
	{
		return  _finish -_start;
	}
	size_t  Capacity()
	{
		return  _endOfCapacity -_start;
	}
	void PushBack(const  T&  data)
	{
		//Ҫ����������
		if(Size()>= Capacity())
			//������
			_Expand(3*Size()+3);
		//�������ݵ�β��
		//ֱ�ӵ��ù��캯��
		Construct(_finish,data);
		++_finish;
	}

	void PopBack()
	{
		//Ҫ��vector�����ݵĸ�������0
		if(Size > 0)
		{
			//�ֽ�β��λ�õ�Ԫ���ͷ�
			Destroy(_finish);
			//����������
			--_finish;
		}
	}

	void Resize(size_t n,const  T&  data = T())
	{
		//���������
		// n <Size()
		if(n < Size())
		{
			Destroy(_start+n,_finish);
			_finish = _start +n ;
		}
		//     Size()<= n <Capacity
		else if(n < Capacity())
		{
			T *  end = _finish;
			_finish  =_start+ n;
			while(_end != _finish)
			{
				Construct(end,data);
				end++;
			}
		}
		// n >= Capacity
		else
		{
			_Expand(n);
			while(_finish != _endOfCapacity)
			{
				Construct(_finish,data);
				++_finish;
			}
		}
	}

	void Reserve(size_t n)
	{
		_Expand(n);
	}

	~Vector()
	{
		Destroy(_start,_finish);
		data_allocator::deallocate(_start,Capacity());
		_start =NULL;
		_finish = NULL;
		_endOfCapacity =NULL;
	}

protected:
	//����
	void _Expand(size_t  n)
	{
		if(n > Capacity())
		{
			size_t size  = Size();
			//����ռ�
			T *tem = data_allocator::allocate(n);
			for(int i = 0 ;i < size;++i)
			{
				Construct(tem+i,_start[i]);
			}
			//��ʾ�ĵ�����������
			Destroy(_start,_finish);
			data_allocator::deallocate(_start,size);

			_start  = tem;
			_finish =_start +size;
			_endOfCapacity  =_start + n;
		}
	}
protected:
	typedef simple_alloc<T,Alloc> data_allocator;
	T*  _start;//��ʾ����ͷָ�� 
	T*  _finish;//��ʾ����vector��Ԫ�صĸ���
	T*  _endOfCapacity;//���ٵĿռ��С
};

void TestVector()
{
	Vector<int> v;
	v.PushBack(1);
	v.PushBack(2);

	Vector<string> v1;
	v1.PushBack("5");
	v1.PushBack("6");

	cout<<"Vector:"<<Distance(v.Begin(), v.End())<<endl;
}