#pragma once 
#include<assert.h>
#include<vector>
#include<string>
#include<map>
#include<queue>
//�ڽӾ���ʵ��ͼ
template<class V,class W>
class GraphMatrix
{
public:
	GraphMatrix(const V*  vertexs,const  size_t  n,const W& invalid = W(),
		bool  IsDirtected =false)
		:_verexts(vertexs,vertexs+n)
		,_IsDirected(IsDirtected)
		,_invalid(invalid)
	{
		_matrix = new W*[n];//Ϊ��������ռ�
		for(size_t i = 0 ;i < n;++i)
		{
			_matrix[i] = new W[n];
			for(size_t  j= 0 ;j < n;++j)
			{
				_matrix[i][j] =_invalid;//������ĳ�ʼֵ��ֵΪinvalid
			}
		}
		for(size_t  i= 0 ;i < _verexts.size();++i)
		{
			_getidex.insert(make_pair(_verexts[i],i));
		}
	}


	//ͨ�������Ķ�����Ѱ���±�
	int  GetIndex(const V& k)
	{
		//for(size_t i = 0 ;i < _verexts.size();++i)
		//{
		//	if(_verexts[i] == k)
		//	{
		//		return i;
		//	}
		//}
		////�������ʾ�����˴���
		//assert(false);
		//return  -1;

		return _getidex.find(k)->second;
	}
	//��ӱߣ��ڲ�ʵ�֣�
	void _AddEdge(const size_t src,const size_t dest,const  W& w)
	{
		
		//Ϊ�ڽӾ����ʾ�ı�����ֵ
		_matrix[src][dest] = w;
		//Ҫ�Ǹ�ͼΪ����ͼ�Ļ�
		if(_IsDirected == false)
		{
			_matrix[dest][src] = w;
		}
	}
	//��ӱ�
	void AddEdge(const  V& v1,const V& v2,const W& w)
	{
		int src= GetIndex(v1);
		int dest = GetIndex(v2);
		_AddEdge(src,dest,w);
	}
protected:
	vector<V> _verexts;// ���㼯��
	W** _matrix ;// �ڽӾ���   �ߵļ���
	bool _IsDirected;//�Ƿ�Ϊ������ͨͼ
	W   _invalid;//��Ч�ı�
	map<V,size_t> _getidex;
};

void TestMatrix()
{
	string  str[] = {"����","����","̫ԭ","�Ϻ�","�Ͼ�"};
	GraphMatrix<string ,double> g(str,sizeof(str)/sizeof(str[0]),0);
	g.AddEdge("����","����",123.2);		
	g.AddEdge("����","̫ԭ",1.2);
	g.AddEdge("����","�Ͼ�",345.4);
	g.AddEdge("����","�Ϻ�",999);
	g.AddEdge("̫ԭ","�Ϻ�",100);


}


//ʹ���ڽӱ���ʵ��ͼ

//��ʾ�ڽӱߵĽڵ�
template<class W>
struct LinkTableEdge
{
	size_t _src ;//��ʾ�ߵ�Դ�ڵ���±�
	size_t _dest;//��ʾ�ߵ�Ŀ��ڵ��±�
	LinkTableEdge<W> * _next;
	W _weight;//��ʾ���� �ߵ�Ȩֵ
	LinkTableEdge(const  size_t  src,const  size_t dest, const W& weight)
		:_src(src)
		,_dest(dest)
		,_next(NULL)
		,_weight(weight)
	{}

};


template<class V,class W>
class GraphLinkTable
{
	typedef LinkTableEdge<W> Edge;
public:
	GraphLinkTable(const V* verexts,const size_t n,const W& invalid = W()
		,bool IsDirtected =false)
		:_invalid(invalid)
		,_IsDirected(IsDirtected)
	{
		_verexts.resize(n);
		_verexts.assign(verexts,verexts+n);
		_LinkTables.resize(n,NULL);
	}
	//ͨ�������Ķ�����Ѱ���±�
	int  GetIndex(const V& k)
	{
		for(size_t i = 0 ;i < _verexts.size();++i)
		{
			if(_verexts[i] == k)
			{
				return i;
			}
		}
		//�������ʾ�����˴���
		assert(false);
		return  -1;
	}
	//����ڽӱ�(�ڲ�ʵ��)
	void _AddEdge(const  size_t src,const  size_t dest,const W& w)
	{
		Edge *  edge =  new Edge(src,dest,w);
		edge->_next = _LinkTables[src];
		_LinkTables[src] = edge;
	}
	void AddEdge(const V& v1,const V& v2,const W& w)
	{
		int src = GetIndex(v1);
		int dest = GetIndex(v2);
		_AddEdge(src,dest,w);
		if(_IsDirected ==false)
		{
			_AddEdge(dest,src,w);
		}
	}
	//������ȱ���  
	void DFS(const  V& v)
	{
		int* count  = new int[_verexts.size()];
		memset(count,0,sizeof(int)*_verexts.size());
		int n = GetIndex(v);
		_DFS(_LinkTables[n],count);
		cout<<endl;
		delete[] count;
	}
	//������ȱ���
	void GFS(const  V& v)
	{
		//��ʾ����
		int* count  = new int[_verexts.size()];
		memset(count,0,sizeof(int)*_verexts.size());
		int idex = GetIndex(v);
		queue<int> q;
		q.push(idex);
		count[idex] ++;
		while(q.empty()==false)
		{
			int pos  =q.front();
			q.pop();
			Edge *  edge  = _LinkTables[pos];
			cout<<pos<<" ";
			while(edge)
			{
				if(count[edge->_dest] == 0)
				{
					q.push(edge->_dest);
					count[edge->_dest]++;
				}
				edge  =edge->_next;
			}
			
		}
		cout<<endl;
		delete[] count;
	}
private:
	void _DFS(Edge * cur,int *count)
	{
		if(cur  == NULL)
		{
			return  ;
		}
		//�Ƚ�����������i
		if(count[cur->_src] == 0)
		{
			cout<<cur->_src<<" ";
			count[cur->_src]++;
			//���ʸö��㣬���������ʵ��
			//Ҫ�Ǹö�����ڽӶ���û�з��ʹ���
			while(cur)
			{	
				if(count[cur->_dest] == 0)
				{	
					_DFS(_LinkTables[cur->_dest],count);
				}
				cur= cur->_next;
			}
			while(cur&&count[cur->_dest] ==1)
			{
				cur =cur->_next;
			}
			_DFS(cur,count);
		}
		
	}
	

protected:
	vector<V> _verexts;//��ʾ�Ķ���ļ���
	vector<Edge*> _LinkTables;//�ڽӱ�
	bool _IsDirected;//��ʾ���ǲ�������ͼ
	W  _invalid;//��ʾ���� һ����Ч�ߵ�Ȩֵ
};
void TestLinkTable()
{
	string  str[] = {"����","����","̫ԭ","�Ϻ�","�Ͼ�"};
	GraphLinkTable<string ,double> g(str,sizeof(str)/sizeof(str[0]),0);
	g.AddEdge("����","����",123.2);		
	g.AddEdge("����","̫ԭ",1.2);
	g.AddEdge("����","�Ͼ�",345.4);
	g.AddEdge("����","�Ϻ�",999);
	g.AddEdge("̫ԭ","�Ϻ�",100);
	g.DFS("����");
	g.GFS("����");


}


