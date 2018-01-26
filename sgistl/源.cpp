#include"Dym_alloc.h"
#include"Default_alloc.h"
#include<iostream>
using std::cout;
using std::endl;
bool isout = true;
class A
{
public:
	A();
	~A();
	void test()
	{
		cout << a << endl;
	}
private:
	int a;
};

A::A()
{
	a = 11;
}

A::~A()
{
}
void fun() {
	cout << "bad" << endl;
}
int main()
{
	
	Default_alloc::set_fail_alloc_hander(fun);
	while (true)
	{
		void* ptr = Dym_alloc::allocate(sizeof(A));
		A* a = new(ptr)A;
	}
	system("pause");
	return 0;
}