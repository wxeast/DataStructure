#pragma  once

#include"stl_allocator.h"//自己实现的空间配置器     git地址     https://github.com/wxeast/STL/blob/master/stl_allocator.h
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
		//要是容量不足
		if(Size()>= Capacity())
			//先扩容
			_Expand(3*Size()+3);
		//插入数据到尾部
		//直接调用构造函数
		Construct(_finish,data);
		++_finish;
	}

	void PopBack()
	{
		//要是vector内数据的个数大于0
		if(Size > 0)
		{
			//现将尾部位置的元素释放
			Destroy(_finish);
			//将数量减少
			--_finish;
		}
	}

	void Resize(size_t n,const  T&  data = T())
	{
		//分三种情况
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
	//扩容
	void _Expand(size_t  n)
	{
		if(n > Capacity())
		{
			size_t size  = Size();
			//申请空间
			T *tem = data_allocator::allocate(n);
			for(int i = 0 ;i < size;++i)
			{
				Construct(tem+i,_start[i]);
			}
			//显示的调用析构函数
			Destroy(_start,_finish);
			data_allocator::deallocate(_start,size);

			_start  = tem;
			_finish =_start +size;
			_endOfCapacity  =_start + n;
		}
	}
protected:
	typedef simple_alloc<T,Alloc> data_allocator;
	T*  _start;//表示的是头指针 
	T*  _finish;//表示的是vector的元素的个数
	T*  _endOfCapacity;//开辟的空间大小
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