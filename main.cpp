#include "test.h"
int main()
{
	test * t = new test();
	//t->testBitIndex();
	//t->testRankIndex();
	//t->testQSuccinctTree(AIDS, AIDS_Query, 100);
	t->testMultiQSTree(AIDS, AIDS, 10000);
	//getchar();
	//t->testDeleteDegree();
	//t->testBranchTree(S100K.c_str(), 100000);
	getchar();
	return 0;
}