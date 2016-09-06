#ifndef _MULTIQSTREE_H
#define _MULTIQSTREE_H
#include "succBitTree.h"

class multiQSTree
{
public:
	map<string, branchTree *> mbt;
	map<string, succBitTree *> mst;
	
	vertexHash *vh;
	edgeHash *eh;
	branchHash *bh;
	int maxv;  // the dividision poionts
	int maxe; 
	int l; // the area length
	string prefix; //the data area path
	
public:
	multiQSTree(string prefix, int l)
	{
		bh = branchHash::getInstance();
		vh = vertexHash::getInstance();
		eh = edgeHash::getInstance();
		maxv = maxe = 0;
		this->l = l;
		this->prefix = prefix;
		mbt.clear();
		mst.clear();
#if 0
		if (_access(path.c_str(), 0) == -1)
		{
			if (_mkdir(path.c_str()) == -1)
			{
				cout << "mkdir error in path:" << path << endl;
			}
		}
 
		if (access(path.c_str(), F_OK | R_OK) == -1)
		{
			if (mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1)
			{
				cout << "mkdir error in" << path << endl;
			}
		}

		return path;
#endif
		if (_access(prefix.c_str(), 0) == -1)
		{
			if (_mkdir(prefix.c_str()) == -1)
			{
				cout << "mkdir error in" << endl;
			}
		}
		
	}
	~multiQSTree()
	{
		for (map<string, branchTree *> ::iterator iter = mbt.begin(); iter != mbt.end(); ++iter) //delete the space
		{
			if (iter->second)
			{
				delete iter->second;
				iter->second = NULL;
			}
		}
		ofstream fout(prefix + "/dfo");
		fout << maxv << "\t" << maxe << "\t" << l << endl;
		fout << mst.size() << endl;
		for (map<string, succBitTree *> ::iterator iter = mst.begin(); iter != mst.end(); ++iter)
		{
			fout << iter->first << endl;
			if (iter->second)
			{
				delete iter->second;
				iter->second = NULL;
			}
		}
		fout.close();
		if (bh) bh->sequenceHash(prefix + "/bhf"); bh = NULL;
		if (vh) vh->sequenceHash(prefix + "/vhf"); vh = NULL;
		if (eh) eh->sequenceHash(prefix + "/ehf"); eh = NULL;
	}
	multiQSTree(string prefix)
	{
		string bhf = prefix + "/bhf";
		string vhf = prefix + "/vhf";
		string ehf = prefix + "/ehf";
	
		bh = branchHash::getInstance();
		vh = vertexHash::getInstance();
		eh = edgeHash::getInstance();
		bh->readHashTable(bhf);
		vh->readHashTable(vhf);
		eh->readHashTable(ehf);
		this->prefix = prefix;
	}
	
public:
	//(x, y) ---->(y + x, y - x)
	inline void transformXYtoUV(int &x, int &y)
	{
		int temp = x;
		x = y + x;
		y = y - temp;
	}
	void staticGraphDataBase(const char *input, int total); //统计数据库信息<v, e>
	void position(int tv, int te, int &x, int &y); //ensure the boundary of tv and te
	void writeIntoDisk(string dinfo, string bhf, string vhf, string ehf); //保存数据信息(maxv, maxe, l) 和 hash表， 在析构函数里面
	void writeTreeIntoDisk(); //default into: "./area/**/bt.idx"
	void readTreeFromDisk(); 

public:
	inline string enchodingArea(int &x, int &y)
	{
		int ax, ay;
		position(x, y, ax, ay);
		stringstream ss;
		ss << ax << ay;
		string path = prefix + "/" + ss.str();
		if (_access(path.c_str(), 0) == -1)
		{
			if (mkdir(path.c_str()) == -1)  // 构建该目录
			{
				cout << prefix << " " << ss.str() << endl;
				cout << "mkdir error in path" << endl;
			}
		}
		return path;
	}
	inline void unionArea(int v, int e, int &lx, int &rx, int &ly, int &ry, int d)
	{
		int lv = v;
		int rv = v;
		int le = e - d;
		int re = e + d;
		position(lv, le, lx, ly);
		position(rv, re, rx, ry);
	}
	inline vector<string> unionArea(int v, int e, int d)
	{
		vector<string> vs;
		int lx, rx, ly, ry;
		unionArea(v, e, lx, rx, ly, ry, d);

		for (int i = lx; i <= rx; i++)
		{
			for (int j = ly; j <= ry; j++)
			{
				string path = common::inToString(prefix, i, j);
				vs.push_back(path);
			}
		}

		return vs;
	}
	inline vector<int> multiSuccTreeQuery(graph &g, int d)
	{
		vector<string> queryArea;
		queryArea = unionArea(g.v, g.e, d);
		map<string, succBitTree *> ::iterator iter;
		vector<int> rs;
		for (int i = 0; i < queryArea.size(); i++)
		{
			iter = mst.find(queryArea[i]);
			if (iter != mst.end() && iter->second)
			{
				vector<int> tmp = iter->second->queryGraph(g, d);
				for (int j = 0; j < tmp.size(); j++)
					rs.push_back(tmp[j]);
			}
		}
		return rs;
	}

	inline vector<int> multiSuccTreeQuery(graph &g, int d, bool flag)
	{
		vector<int> rs = this->multiSuccTreeQuery(g, d);
		sort(rs.begin(), rs.end());
		vector<graph> vg;
		g.deleteGraph(vg);
		for (int i = 0; i < vg.size(); i++)
		{
			vector<int> temp = this->multiSuccTreeQuery(vg[i], d + 1);
			common::unionSet(temp, rs); //may use too much time?
		}
		return rs;
	}

public:
	void multiBuildSuccTree(string db, int total); 
	void multiBuildSuccTree(string prefix); // 通过标识区域，识别各个子树，然后直接构建
	double multiSuccTreeSize();
	void multiSuccTreeSize(u32 &ve, u32 &t, u32 &b, u32 &l)
	{
		ve = t = b = l = 0;
		map<string, succBitTree *> ::iterator iter;
		for (iter = mst.begin(); iter != mst.end(); ++iter)
			iter->second->succinctBitTreeSize(ve, t, b, l);
		return ;
	}

};

#endif