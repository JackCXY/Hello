#ifndef _TEST_H
#define _TEST_H

#include "rankSelect.h"
#include "bitIndex.h"
#include "graph.h"
#include "branchTree.h"
#include "succBitTree.h"
#include "multiQSTree.h"

class test
{
public:
	void testRankIndex()
	{
		rankSelect *r = new rankSelect();
		bitIndex *b = new bitIndex();
		b->test();
	}
	void testBranchTree(const char *db, int total)
	{
		branchTree *bTree = new branchTree();
		bTree->buildBranchTree(db, total);

		sTuple bt(0, 0, 0, 0, 0, 0), vt(0, 0, 0, 0, 0, 0), et(0, 0, 0, 0, 0, 0);
		u64 idx = 0;
		u32 maxve = 0;
		u64 bft = 0, bgt = 0, bht = 0; 
		u64 lft = 0, lgt = 0, lht = 0;
		bTree->treeBitEncodingLen(bt, vt, et, maxve, idx, bft, bgt, bht, lft, lgt, lht);

		cout << "branch:(fix, gamma, h):"	<< bft * 1.0 / bt.codeNumber << " " << bgt * 1.0 / bt.codeNumber << " " << bht * 1.0 / bt.codeNumber << endl;
		cout << "label:(fix, gamma, h):"	<< lft * 1.0 / (vt.codeNumber + et.codeNumber) 
											<< " " << lgt * 1.0 / (vt.codeNumber + et.codeNumber) 
											<< " " << lht * 1.0 / (vt.codeNumber + et.codeNumber) << endl;
	}
	void testDeleteDegree()
	{
		int v = 5, e = 5;
		vector<int> V(v, 1);
		vector<int> adj(v, -1);
		vector<vector<int > > E(v, adj); 
		E[0][1] = 1; E[1][0] = 1; E[0][2] = 1; E[2][0] = 1; E[1][2] = 1; E[2][1] = 1; E[2][3] = 1; E[3][2] = 1; E[1][4] = 1; E[4][1] = 1;
		graph q(0, v, V, e, E);  int max; 
		vector<int> dq1 = q.degreeSequence(max);
		vector<int> idx; 
		for (int i = 0; i < 2; i++)
			idx.push_back(i);
		vector<int> dqd; q.vertexDeleteDegree(idx, dqd);
		int dvmin = common::deleteEdge(dq1, dqd);
		while (true)
		{
			dqd.clear();
			bool flag = common::next_data(2, dq1.size(), idx);
			if (!flag)  break;
			q.vertexDeleteDegree(idx, dqd);
			int tmp = common::deleteEdge(dq1, dqd);
			if (tmp < dvmin)
			{
				dvmin = tmp;
			}
		}
		
	}

	void testQSuccinctTree(string db, string query, int total)
	{
		const string treeIndex = "./index/test.idx";
		vector<graph> queryDB = graph::readGraphMemory(query.c_str(), 50);
		clock_t start, end;

		start = clock();
		branchTree *bt = new branchTree();
		bt->buildBranchTree(db, total);
		cout << "Indexing building done" << endl;
		end = clock();
		cout << "build time:(s)" << (end - start) * 1.0 / CLOCKS_PER_SEC << endl;
		cout << bt->branchTreeSize() << endl;
		sTuple BT(0, 0, 0, 0, 0, 0);
		sTuple VT(0, 0, 0, 0, 0, 0);
		sTuple ET(0, 0, 0, 0, 0, 0);
		u64 idx; 
		u32 maxve;
		bt->treeBitEncodingLen(BT, VT, ET, maxve, idx);
		cout << "BT:" << BT.bitLen << " " << BT.codeLen << " " << BT.codeNumber << " " << BT.fixCounts
			<< " " << BT.gamaCounts << " " << BT.maxValue << endl;
		cout << "VT:" << VT.bitLen << " " << VT.codeLen << " " << VT.codeNumber << " " << VT.fixCounts
			<< " " << VT.gamaCounts << " " << VT.maxValue << endl;
		cout << "ET:" << ET.bitLen << " " << ET.codeLen << " " << ET.codeNumber << " " << ET.fixCounts
			<< " " << ET.gamaCounts << " " << ET.maxValue << endl;
		cout << "maxve, idx:" << maxve << " " << idx << endl;
		for (int d = 1; d <= 5; d++)
		{
			start = clock();
			int sum = 0;
			for (int j = 0; j <queryDB.size(); j++)
			{
				vector<int> result;
				result = bt->queryGraph(queryDB[j], d);
				sum += result.size();
			}
			end = clock();
			cout << "average Size and query time(s):" << d << " " << sum *1.0 / queryDB.size() << " " << (end - start) * 1.0 / CLOCKS_PER_SEC << endl;
		}
		bt->writeIntoDisk(treeIndex);
		if (bt) delete bt;
		cout << "write the index done" << endl;
		
		succBitTree *st = new succBitTree(BT, VT, ET, maxve, idx);
		st->buildSuccBitTree(treeIndex);
		cout << "succinctTree build done: " << st->succinctBitTreeSize() << endl;


		for (int d = 1; d <= 5; d++)
		{
			start = clock();
			int sum = 0;
			for (int j = 0; j <queryDB.size(); j++)
			{
				vector<int> result;
				result = st->queryGraph(queryDB[j], d);
				sum += result.size();
			}

			end = clock();
			cout << "average Size and query time(s):" << d << " " << sum *1.0 / queryDB.size() << " " << (end - start) * 1.0 / CLOCKS_PER_SEC << endl;
		}

		if (st) delete st;
	}
	void testMultiQSTree(const string db, const string query, int total)
	{
		vector<graph> queryDB = graph::readGraphMemory(query.c_str(), 50);
		clock_t start, end;
		//multiQSTree *mt = new multiQSTree("./area/AIDS");
		//mt->multiBuildSuccTree("./area/AIDS");
		multiQSTree *mt = new multiQSTree("./area/AIDS", 2);
		mt->multiBuildSuccTree(db, total); // ¹¹½¨Íê±Ï
		u32 ve,  t, b, l; 
		cout << "build done and begin query: " << mt->multiSuccTreeSize() << endl;
		mt->multiSuccTreeSize(ve, t, b, l);
		cout << "the size of each part(rank, lr, others): " << ve* 1.0 / (1024.0 * 1024.0) << " " <<  t * 1.0 / (1024.0 * 1024.0) << " " << b * 1.0 / (1024.0 * 1024.0) << " " << l * 1.0 / (1024.0 * 1024.0) << endl;
		//cout << "build done and begin query: " << endl;
	
		//mt->multiSuccTreeSize(t, b, l);
		//cout << t << " " << b << " " << l << endl;
		//cout << "B, L, O:" << b * 1.0 / (1024.0 * 1024.0) << " " << l * 1.0 / (1024.0 * 1024.0) << " " << (t - b - l) * 1.0 / (1024.0 * 1024.0) << endl;
		for (int d = 1; d <= 5; d++)
		{
			start = clock();
			int sum = 0;
			for (int j = 0; j <queryDB.size(); j++)
			{
				vector<int> result;
				result = mt->multiSuccTreeQuery(queryDB[j], d, true);
				sum += result.size();
			}
			end = clock();
			 cout << "average Size and query time(s):" << d << " " << sum * 1.0 / queryDB.size() << " " << (end - start) * 1.0 / CLOCKS_PER_SEC << endl;
		}
		if (mt) delete mt;
	}

};
#endif