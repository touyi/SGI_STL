#include<vector>
#include<iostream>
#include"TDequene.h"
using std::cout;
using std::endl;
int main()
{
	TDequene<int> que;
	for (int i = 0; i < 1000; i++)
	{
		que.push_back(i);
	}
	for (int i = 0; i < 500; i++)
	{
		que.pop_back();
	}
	for (auto i = que.begin(); i != que.end(); i++)
	{
		cout << *i << endl;
	}
	system("pause");
	return 0;
}