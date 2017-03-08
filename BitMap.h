#pragma once

#include<vector>
class BitMap
{
public:
	BitMap(const size_t & n)//n�ڲ���ʾ���ݵĸ���  
	{
		_bt.resize((n>>5)+1);//���ٿռ�
	}
	void  Set(const size_t  value)//�����ڲ�ֵ
	{
		size_t idex = value>>5;//�ҵ������洢���Ǹ����͵ķ�Χ��
		size_t num  = value %32; //�ҵ������ε��ĸ�λ��
		_bt[idex] |= (1<<num);//����λ�����ó�  1��
	}
	void ReSet(const size_t value)//����
	{
		size_t idex =value>>5;
		size_t num  =value%32;
		_bt[idex] &= ~(1<<num);//��λ������ �� 0 ��
	}
	bool Test(const size_t value)
	{
		size_t idex = value>>5;
		size_t num = value%32;
		if(_bt[idex] & (1<<num))//���λ���ϵ�����  1  
			return true;//��ʾ ����
		return false;//��ʾ ������
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