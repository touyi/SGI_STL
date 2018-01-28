#include<vector>
#include<iostream>
#include"TDequene.h"
using std::cout;
using std::endl;
int main()
{
	TDequene<int> que;
	que.push_back(123);
	
	que.push_back(123);
	que.pop_front();
	que.push_front(2);
	for (auto i = que.begin(); i != que.end(); i++)
	{
		cout << *i << endl;
	}
	system("pause");
	return 0;
}