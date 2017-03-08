#pragma once

#include<vector>
class BitMap
{
public:
	BitMap(const size_t & n)//n内部表示数据的个数  
	{
		_bt.resize((n>>5)+1);//开辟空间
	}
	void  Set(const size_t  value)//设置内部值
	{
		size_t idex = value>>5;//找到该数存储在那个整型的范围内
		size_t num  = value %32; //找到在整形的哪个位上
		_bt[idex] |= (1<<num);//将这位置设置成  1；
	}
	void ReSet(const size_t value)//重置
	{
		size_t idex =value>>5;
		size_t num  =value%32;
		_bt[idex] &= ~(1<<num);//将位置设置 成 0 ；
	}
	bool Test(const size_t value)
	{
		size_t idex = value>>5;
		size_t num = value%32;
		if(_bt[idex] & (1<<num))//如果位置上的量是  1  
			return true;//表示 存在
		return false;//表示 不存在
	}
	
protected:
	vector<size_t> _bt;
};



void TestBitMap()
{
	BitMap bt((size_t ) -1);
	bt.Set(1);
	bt.Set(10432);
	bt.Set(1034);
	bt.Set(320);
	cout<<"1 ?"<<bt.Test(1)<<endl;
	cout<<"10432 ?"<<bt.Test(10432)<<endl;
	cout<<"1034 ?"<<bt.Test(1034)<<endl;
	cout<<"321 ?"<<bt.Test(321)<<endl;
	bt.ReSet(1034);
	cout<<"1034 ?"<<bt.Test(1034)<<endl;
}