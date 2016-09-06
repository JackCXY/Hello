#pragma once
#ifndef _GRAPH_H
#define _GRAPH_H
#include "stdafx.h"
#include "common.h"

class graph
{
public:
	int v;
	vector<int> V;
	vector<vector<int > > Edge;
	int graph_id;
	int e;
public:
	graph(int gid, int num, vector<int> &VINFO, int e, vector<vector<int > > &einfo)
	{
		this->graph_id = gid;
		this->v = num;
		this->V = VINFO;

		this->e = e;
		this->Edge = einfo;
	}
	graph(const graph &g)
	{
		this->graph_id = g.graph_id;
		this->v = g.v;
		this->e = g.e;
		this->V = g.V;
		this->Edge = g.Edge;
	}
	graph(){}
	~graph(){}
	static vector<graph> readGraphMemory(const char *file, int total)
	{
		ifstream fr(file, ios::in);
		vector<graph> vg;
		int gid, v, e;
		int f, t, l;
		int vd;
		int count = 0;

		while (fr >> gid)
		{
			fr >> v >> e;
			vector<int> vc;
			for (int i = 0; i < v; i++)
			{
				fr >> vd;
				vc.push_back(vd); //insert the vertex label into vc
			}
			vector<int> adj(v, -1);
			vector<vector<int> > adjMatrix(v, adj);
			//vector<graph::edge> te;
			//vector<vector<graph::edge  > > ve(v, te);
			for (int i = 0; i < e; i++)
			{
				fr >> f >> t >> l;
				//graph::edge ef(t, l); graph::edge et(f, l);
				//ve[f].push_back(ef);
				//ve[t].push_back(et);
				adjMatrix[f][t] = l;
				adjMatrix[t][f] = l;
			}
			graph g(gid, v, vc, e, adjMatrix);
			vg.push_back(g);
			count++;
			if (count >= total)
				break;
		}
		fr.close();
		return vg;

	}

public:
	bool hasEdge(int from, int to, int &einfo)
	{
		return Edge[from][to] != -1;
	}
	int edgeinfo(int from, int to) //获取边(from, to)上的属性
	{
		assert(from < v && to < v);
		if (Edge[from][to] == -1)
		{
			cout << "no edge in " << from << " and " << to << endl;
		}
		return Edge[from][to];
	}
	int degreeVertex(const int &id)
	{
		int s = 0;
		for (int i = 0; i < v; i++)
		{
			if (Edge[id][i] != -1)
				s++;
		}
		return s;
	}
	int maxDegree()
	{
		int max = 0;
		for (int i = 0; i < v; i++)
		{
			int vd = degreeVertex(i);
			if (max < vd)
				max = vd;
		}
		return max;
	}
	stringInt branchGramVertexDegree(int id)
	{
		assert(id < v);
		int s = V[id];
		char ch[16]; // 图中顶点最大的度
		ch[0] = s;
		stringInt ms;
		int size = 0;

		for (int i = 0; i < v; i++)
		{
			if (Edge[id][i] != -1)
			{
				ch[size + 1] = Edge[id][i];
				size++;
			}
		}
		ch[size + 1] = '\0';
		sort(ch + 1, ch + size + 1);
		ms.str = ch;
		ms.degree = size;
		return ms;
	}
	map<string, twoTuple> branchGramSetDegree()
	{
		map<string, twoTuple> tg;
		map<string, twoTuple> ::iterator iter;
		
		for (int i = 0; i < v; i++)
		{
			stringInt s = branchGramVertexDegree(i);
			iter = tg.find(s.str);
			if (iter == tg.end())
			{
				tg.insert(pair<string, twoTuple>(s.str, twoTuple(1, s.degree)));
			}
			else
			{
				iter->second.row++;
			}
		}
		return tg;
	}

public:
	//the branch gram rooted: 
	string branchGramVertex(int id)
	{
		assert(id < v);
		int s = V[id];
		string branchSequence = common::inToString(s);
		vector<string> vs;
		for (int i = 0; i < v; i++)
		{
			if (Edge[id][i] != -1)
			{
				string temp = common::inToString(Edge[id][i]);
				vs.push_back(temp);
			}
		}
		sort(vs.begin(), vs.end());
		for (int i = 0; i < vs.size(); i++)
			branchSequence += vs[i];
		return branchSequence;
	}
	//the <branch, freq> tuples
	map<string, int> branchGramSet()
	{
		map<string, int> tg;
		map<string, int>::iterator iter;
		for (int i = 0; i < v; i++)
		{
			string s = branchGramVertex(i);
			iter = tg.find(s);
			if (iter == tg.end())
			{
				tg.insert(pair<string, int>(s, 1));
			}
			else
				iter->second++;
		}
		return tg;
	}
public:	
	map<string, int> vertexLabel()
	{
		map<string, int> mvl;
		map<string, int>::iterator iter;
		char ch[2];
		for (int i = 0; i < v; i++)
		{
			ch[0] = V[i]; ch[1] = '\0';
			string str = ch;
			iter = mvl.find(str);
			if (iter == mvl.end())
				mvl.insert(pair<string, int>(str, 1));
			else
			{
				iter->second++;
			}
		}
		return mvl;
	}

	map<string, int> edgeLabel()
	{
		map<string, int> mvl;
		map<string, int>::iterator iter;
		char ch[2];

		for (int j = 0; j < v; j++)
		{
			for (int i = j + 1; i < v; i++)
			{
				if (Edge[j][i] != -1)
				{
					ch[0] = Edge[j][i];
					ch[1] = '\0';
					string einfo = ch;
					iter = mvl.find(einfo);
					if (iter == mvl.end())
						mvl.insert(pair<string, int>(einfo, 1));
					else
					{
						iter->second++;
					}
				}
			}
		}
		return mvl;
	}

	vector<int> degreeSequence(int &maxDegree)
	{
		vector<int> ds;
		maxDegree = 0;

		for (int i = 0; i < v; i++)
		{
			int degree = this->degreeVertex(i);
			ds.push_back(degree);
			if (maxDegree < degree)
				maxDegree = degree;
		}
		return ds;
	}

public:
	void vertexDeleteDegree(vector<int> &vr, vector<int> &degree) //delete the vertex set in vr 
	{
		for (int i = 0; i < v; i++)
		{
			if (common::inVector(vr, i))
				continue;
			int td = 0;
			for (int j = 0; j < v; j++)
			{
				if (j != i && Edge[i][j] != -1 && !common::inVector(vr, j))
					td++;
			}
			degree.push_back(td);
		}
	}
	void deleteGraph(vector<graph> &vg)
	{
		int count = 0;
		for (int i = 0; i < v; i++)
		{
			for (int j = i + 1; j < v; j++)
			{
				if (this->Edge[i][j] != -1)
				{
					graph g = *this;
					g.Edge[i][j] = g.Edge[j][i] = -1;
					g.e--;
					vg.push_back(g);//delete one edge
				}
				
			}
		}
	}
};
#endif
