#pragma once 
#include<assert.h>
#include<vector>
#include<string>
#include<map>
#include<queue>
//邻接矩阵实现图
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
		_matrix = new W*[n];//为矩阵申请空间
		for(size_t i = 0 ;i < n;++i)
		{
			_matrix[i] = new W[n];
			for(size_t  j= 0 ;j < n;++j)
			{
				_matrix[i][j] =_invalid;//将矩阵的初始值赋值为invalid
			}
		}
		for(size_t  i= 0 ;i < _verexts.size();++i)
		{
			_getidex.insert(make_pair(_verexts[i],i));
		}
	}


	//通过给定的顶点来寻找下标
	int  GetIndex(const V& k)
	{
		//for(size_t i = 0 ;i < _verexts.size();++i)
		//{
		//	if(_verexts[i] == k)
		//	{
		//		return i;
		//	}
		//}
		////到这里表示出现了错误
		//assert(false);
		//return  -1;

		return _getidex.find(k)->second;
	}
	//添加边（内部实现）
	void _AddEdge(const size_t src,const size_t dest,const  W& w)
	{
		
		//为邻接矩阵表示的边来赋值
		_matrix[src][dest] = w;
		//要是该图为无向图的话
		if(_IsDirected == false)
		{
			_matrix[dest][src] = w;
		}
	}
	//添加边
	void AddEdge(const  V& v1,const V& v2,const W& w)
	{
		int src= GetIndex(v1);
		int dest = GetIndex(v2);
		_AddEdge(src,dest,w);
	}
protected:
	vector<V> _verexts;// 顶点集合
	W** _matrix ;// 邻接矩阵   边的集合
	bool _IsDirected;//是否为无向连通图
	W   _invalid;//无效的边
	map<V,size_t> _getidex;
};

void TestMatrix()
{
	string  str[] = {"西安","北京","太原","上海","南京"};
	GraphMatrix<string ,double> g(str,sizeof(str)/sizeof(str[0]),0);
	g.AddEdge("西安","北京",123.2);		
	g.AddEdge("西安","太原",1.2);
	g.AddEdge("西安","南京",345.4);
	g.AddEdge("西安","上海",999);
	g.AddEdge("太原","上海",100);


}


//使用邻接表来实现图

//表示邻接边的节点
template<class W>
struct LinkTableEdge
{
	size_t _src ;//表示边的源节点的下标
	size_t _dest;//表示边的目标节点下标
	LinkTableEdge<W> * _next;
	W _weight;//表示的是 边的权值
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
	//通过给定的顶点来寻找下标
	int  GetIndex(const V& k)
	{
		for(size_t i = 0 ;i < _verexts.size();++i)
		{
			if(_verexts[i] == k)
			{
				return i;
			}
		}
		//到这里表示出现了错误
		assert(false);
		return  -1;
	}
	//添加邻接边(内部实现)
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
	//深度优先遍历  
	void DFS(const  V& v)
	{
		int* count  = new int[_verexts.size()];
		memset(count,0,sizeof(int)*_verexts.size());
		int n = GetIndex(v);
		_DFS(_LinkTables[n],count);
		cout<<endl;
		delete[] count;
	}
	//广度优先遍历
	void GFS(const  V& v)
	{
		//表示计数
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
		//先将这个顶点访问i
		if(count[cur->_src] == 0)
		{
			cout<<cur->_src<<" ";
			count[cur->_src]++;
			//访问该顶点，，，并往深处实现
			//要是该顶点的邻接顶点没有访问过了
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
	vector<V> _verexts;//表示的顶点的集合
	vector<Edge*> _LinkTables;//邻接表
	bool _IsDirected;//表示的是不是无向图
	W  _invalid;//表示的是 一个无效边的权值
};
void TestLinkTable()
{
	string  str[] = {"西安","北京","太原","上海","南京"};
	GraphLinkTable<string ,double> g(str,sizeof(str)/sizeof(str[0]),0);
	g.AddEdge("西安","北京",123.2);		
	g.AddEdge("西安","太原",1.2);
	g.AddEdge("西安","南京",345.4);
	g.AddEdge("西安","上海",999);
	g.AddEdge("太原","上海",100);
	g.DFS("西安");
	g.GFS("西安");


}


