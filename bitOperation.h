#ifndef _BITOPERATION_H
#define _BITOPERATION_H
#include "stdafx.h"
class bitOperation
{

public:
	static inline void setbit(u64 *bitvec, u64 i, u64 value, u64 len)
	{
		u64 word = i / B64;  //确定索引
		u64 offset = i % B64;//确定索引内偏移量
		u64 j = B64 - offset;//确定索引内剩余的偏移量
		if (j >= len) //在一个B64内可以完全写入需要存储的bit位 
		{
			value = value << (j - len);
			bitvec[word] += value;
		}
		else //需要跨字节存储bit位
		{
			u64 tmp = value >> (len - j);
			bitvec[word] += tmp;//先存入value的高j位
			tmp = value << (B64 - len + j);
			bitvec[word + 1] += tmp;//存入最低位
		}
	}
	/*
	该函数实现bit的读取操作
	1.如果此次出报错肯定是bitvec数组越界
	** @param *bitvec指向被写入串的指针
	** @param *i 开始读取的起始位置
	** @param *len 被写入的长度（len < 64）
	** @return 返回读取到的值，
	*/
	static inline u64 getbit(const u64 *bitvec, const u64 &i, const int &len)
	{
		register u64 word = i / B64;  //确定索引
		register u64 offset = i % B64;//确定索引内偏移量
		register u64 j = B64 - offset;//确定索引内剩余的偏移量
		register u64 tmp;
		if (j >= len) //在一个B64内可以完全读出需要的bit位串
		{
			tmp = bitvec[word] << offset;
			tmp = tmp >> (B64 - len);
		}
		else //需要跨字节读取bit位串
		{
			tmp = bitvec[word] << offset;
			tmp = tmp >> (B64 - len);
			tmp += bitvec[word + 1] >> (B64 - (len - j));
		}
		return tmp;
	}
	//
	//popcount: 统计i中1的个数
	//优化popcount结果
	static inline u64 popcount(register u64 r)
	{
		//knuth
		r = ((r & 0xaaaaaaaaaaaaaaaa) >> 1) + (r & 0x5555555555555555);
		r = ((r & 0xcccccccccccccccc) >> 2) + (r & 0x3333333333333333);
		r = ((r >> 4) + r) & 0x0f0f0f0f0f0f0f0f;
		r *= 0x0101010101010101;
		r >>= 64 - 8;
		return r;
		
	}
	//编码长度为(len - 1), 例如6的编码长度为2
	static inline u32 logx(u64 num)
	{
		int len = 0;
		while ((num = num / 2) >= 1)
			len++;
		return len;
	}

	static int gamaLen(const vector<int> &vr, int left, int right)
	{
		int sum = 0;
		for (int i = left; i < right; i++)
		{
			sum += 2 * bitOperation::logx(vr[i]) + 1;
		}
		return sum;
	}

	static int fixLen(const vector<int> &vr, int left, int right)
	{
		int sum = 0;
		int max = vr[left];
		for (int i = left + 1; i < right; i++)
		{
			if (max < vr[i])
				max = vr[i];
		}
		return (right-left) * (bitOperation::logx(max) + 1);
	}
	static u32 encodingLen(vector<int> &value, bool &f, u32 &maxValue, u64 &ft, u64 &gt, u64 &ht)
	{
		u32 bitlen = 0;
		int size = value.size();
		int max = 0;
		for (int j = 0; j < size; j++)
		{
			if (max < value[j])
				max = value[j];
			bitlen += 2 * bitOperation::logx(value[j]) + 1;
		}

		if (maxValue < max)
			maxValue = max;
		ft += (bitOperation::logx(max) + 1) * size;
		gt += bitlen;
		ht += min(bitlen, (bitOperation::logx(max) + 1) * value.size());
		if (bitlen < (bitOperation::logx(max) + 1) * size)
			f = true;
		else
			f = false;

		bitlen = min(bitlen, (bitOperation::logx(max) + 1) * value.size());
		return bitlen;
	}
	static u32 encodingLen(vector<int> &value, bool &f, u32 &maxValue)
	{
		u32 bitlen = 0;
		int size = value.size();
		int max = 0;
		for (int j = 0; j < size; j++)
		{
			if (max < value[j])
				max = value[j];
			bitlen += 2 * bitOperation::logx(value[j]) + 1;
		}

		if (maxValue < max)
			maxValue = max;
		if (bitlen < (bitOperation::logx(max) + 1) * size)
			f = true;
		else
			f = false;

		bitlen = min(bitlen, (bitOperation::logx(max) + 1) * value.size());
		return bitlen;
	}

	static bool fixMoreGamma(vector<int> &value)
	{
		//return true;
		//return false;
		u32 bitlen = 0;
		int max = 0;
		u32 size = value.size();
		for (int j = 0; j < size; j++)
		{
			if (max < value[j])
				max = value[j];
			bitlen += 2 * bitOperation::logx(value[j]) + 1;
		}
		if (bitlen < (bitOperation::logx(max) + 1) * size)
			return true;
		else
			return false;
	}
};
#endif