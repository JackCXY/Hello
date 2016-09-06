#ifndef _SEQ_SCAN_H
#define _SEQ_SCAN_H
#include "stdafx.h"
#include "graph.h"
#include "common.h"
class seqScan
{
public:
	bool candidateDegree(graph &ga, graph &gb);
	bool candidateBranch(graph &ga, graph &gb);
	bool candidateLabel(graph &ga, graph &gb);

	int unionStar(map<string, twoTuple> &sa, map<string, twoTuple> &sb, vector<int> &da, vector<int> &db)
	{
		da.clear();
		db.clear();
		map<string, twoTuple> ::iterator iter1, iter2;
		vector<string> s1, s2;
		vector<int> f1, f2;
		for (iter1 = sa.begin(); iter1 != sa.end(); iter1++)
		{
			s1.push_back(iter1->first);
			f1.push_back(iter1->second.row);
			da.push_back(iter1->second.value);
		}

		for (iter2 = sa.begin(); iter2 != sa.end(); iter2++)
		{
			s2.push_back(iter2->first);
			f2.push_back(iter2->second.row);
			db.push_back(iter2->second.value);
		}

		int i = 0, j = 0;
		int sz1 = s1.size(), sz2 = s2.size();
		int sum = 0;

		while (i < sz1 && j < sz2)
		{
			if (s1[i] == s2[j])
			{
				sum += min(f1[i], f2[j]);
				i++;
				j++;
			}
			else if (s1[i] < s2[j]) i++;
			else
				j++;
		}
		return sum;
	}

	bool candidateStar(graph &ga, graph &gb, int d)
	{
		map<string, twoTuple> sa = ga.branchGramSetDegree();
		map<string, twoTuple> sb = gb.branchGramSetDegree();
		map<string, int> la = ga.vertexLabel();
		map<string, int> lb = gb.vertexLabel();
		vector<int> d1, d2;
		int dg1, dg2;
		//test 1:
		int ul = common::unionStringSet(la, lb);
	
		d1 = ga.degreeSequence(dg1);
		d2 = gb.degreeSequence(dg2);

		int us = unionStar(sa, sb, d1, d2);
		int m1 = 2 * gb.v - ga.v - 2 * d - (dg1 - 1) * (ga.v - ul);
		int m2 = 2 * ga.v - gb.v - 2 * d - (dg2 - 1) * (gb.v - ul);
		
		if (us >= max(m1, m2))
			return true;
		else
			return false;


	}

	int seqScanStar(string db, string query, int count, int d)
	{
		vector<graph> vg = graph::readGraphMemory(db.c_str(), count);
		vector<graph> q = graph::readGraphMemory(query.c_str(), 50);

		int sum = 0;
		for (int i = 0; i < vg.size(); i++)
		{
			for (int j = 0; j < q.size(); j++)
			{
				if (candidateStar(vg[i], q[j], d))
					sum++;
			}

		}
		return sum;
	}
};
#endif
