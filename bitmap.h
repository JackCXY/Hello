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
	** getR4(u16 i) ��ȡ����һ��i�к��ж��ٸ�����
	** getR3(u16 i) ��ȡ16λ������Ч��λ��
	** getR2(u16 i) �洢16λ���е�һ����Ч���ֵ�λ��
	** getR1(u16 i) �洢16λ���е�һ����Ч���ֵ�ֵ
	*/
	u32 getR1(u32 &i);
	u32 getR2(u32 &i);
	u32 getR3(u32 &i);
	u32 getR4(u32 &i);
	u32 *getGamaArr(u32 &i);
	/*
	�ú���ʵ�ֽ��룬��һ��16bit�����ֽ������
	@param ָ����Ҫ������������ֵĵڼ���
	@return  ���ش𰸣�-1��ʾ�������߼�����
	*/
	int uncode(int i, u32 value);
	u32* getBitmap();
	~bitmap();
private:
	bitmap();
};
#endif