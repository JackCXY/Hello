#pragma once 
#ifndef _BITMAP_H
#define _BITMAP_H
#include "stdafx.h"
//can be with u8
class bitmap
{
private:
	u32 *bmap;
	static bitmap* instance;
public:
	static bitmap* getInstance();
	/*
	the introduction about the four following functions
	** getR4(u16 i) 提取出来一个i中含有多少个整数
	** getR3(u16 i) 提取16位串中有效的位数
	** getR2(u16 i) 存储16位串中第一个有效数字的位数
	** getR1(u16 i) 存储16位串中第一个有效数字的值
	*/
	u32 getR1(u32 &i);
	u32 getR2(u32 &i);
	u32 getR3(u32 &i);
	u32 getR4(u32 &i);
	u32 *getGamaArr(u32 &i);
	/*
	该函数实现解码，把一个16bit的数字解码出来
	@param 指明需要解码出来的数字的第几个
	@return  返回答案，-1表示函数有逻辑错误
	*/
	int uncode(int i, u32 value);
	u32* getBitmap();
	~bitmap();
private:
	bitmap();
};
#endif