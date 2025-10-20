#include <iostream>
#include <list>
#include "list.h"

int main()
{


	//std::list<int> a;
	STDev::list<int> b;

	//b.push_back(1);
	//b.push_back(2);
	b.push_front(3);
	b.push_back(2);
	b.push_back(5);
	b.print_visual(); //3 2 5

	//STDev::list<int>::iterator iter = b.begin();

	for (auto it = b.begin(); it != b.end(); ++it)
	{
		std::cout << *it << std::endl;
	}
}

