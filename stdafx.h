// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//
#pragma once
#pragma warning( disable : 4996) 

#include <stdio.h>
#include <cstdlib>
#include <tchar.h>
#include <assert.h>
#include <cmath>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <stack>
#include <assert.h>
#include <iostream>
#include <sstream>
#include <set>
#include <ctime>
#include <queue>
#include <functional>
#include <algorithm>
#include <random>
#include <io.h>
#include <list>
#include <sys\stat.h>
#include <stdint.h>
#include <direct.h>
#include <bitset>

#define  mkdir _mkdir
#define  ASSERT assert

typedef unsigned char uchar;
typedef unsigned long long u64;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;
typedef unsigned int uint;

#define B64 64
#define B32 32
#define B16 16
#define MAXNODES 63
#define MINNODES 32
#define BBS	32
#define VBS 32
#define EBS 32
#define RATE 4

static int position = 1;
const int MAX = 200000;
const std::string AIDS = "d:\\data\\database\\AIDS";
const std::string AIDS_Query = "d:\\data\\query\\AIDS";
const std::string AIDS_100 = "d:\\data\\query\\AIDS_100";
const std::string Query_400 = "d:\\data\\query\\m1200";
const std::string Pub_400 = "d:\\data\\database\\m400";
const std::string Pub_2400 = "d:\\data\\database\\m2400";
const std::string Query_4 = "d:\\data\\query\\m4";
const std::string AIDS_temp = "d:\\data\\query\\temp";
const std::string S100K = "d:\\data\\database\\S100K.E30.D50.L5";
using namespace std;
struct BTuple
{
	int column;
	int value;
	BTuple(){}
	BTuple(int c, int v)
	{
		column = c;
		value = v;
	}
	bool operator< (const BTuple &t) const
	{
		if (column < t.column) return true;
		else if (column > t.column) return false;
		else
				return value < t.value;
	}
	bool operator== (const BTuple &t) const 
	{
		if (column == t.column) return true;
		else
			return false;
	}
	bool operator>(const BTuple &t) const 
	{
		if (column > t.column) return true;
		else
		{
			if (column < t.column) return false;
			else
				return value > t.value;
		}
	}
	BTuple(const BTuple &t)
	{
		this->column = t.column;
		this->value = t.value;
	}
	BTuple & operator = (const BTuple &t)
	{
		if (this == &t)
			return *this;
		this->column = t.column;
		this->value = t.value;
		return *this;
	}
};
struct twoTuple
{
	u64 row;
	u64 value;
	bool operator< (const twoTuple &t) const
	{
		return row < t.row || (row == t.row && value <= t.value);
	}
	bool operator= (const twoTuple &t) const 
	{
		return row == t.row;
	}
	twoTuple(u64 r, u64 v) : row(r), value(v) { }
	twoTuple(){}
	
};

struct sTuple
{
	u64 bitLen;
	u64 codeLen;
	u64 codeNumber;
	u64 gamaCounts;
	u64 fixCounts;
	u64 maxValue;
	sTuple()
	{
		bitLen = codeLen = codeNumber = gamaCounts = fixCounts = maxValue = 0;
	}
	sTuple(const u64 &b, const u64 &c, const u64 &n, const u64 &g, const u64 &f, const u64 &m) 
		: bitLen(b), codeLen(c), codeNumber(n), gamaCounts(g), fixCounts(f), maxValue(m) {}
};

struct stringInt
{
	string str;
	int degree;
	stringInt(string s, int d) : str(s), degree(d) { }
	stringInt(){}
};