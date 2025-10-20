#include <iostream>
#include "stack.h"

int main()
{
	std::cout << "=== TEST STACK ===" << std::endl;
	STDev::stack<int> s;

	// Push elements
	std::cout << "\nPushing 10, 20, 30, 40, 50" << std::endl;
	s.push(10);
	s.push(20);
	s.push(30);
	s.push(40);
	s.push(50);

	s.print();
	std::cout << "Size: " << s.size() << std::endl;
	std::cout << "Top: " << s.top() << std::endl;

	// Pop elements
	std::cout << "\nPopping 2 elements..." << std::endl;
	s.pop();
	s.pop();
	s.print();
	std::cout << "Size: " << s.size() << std::endl;
	std::cout << "Top: " << s.top() << std::endl;

	// Copy stack
	std::cout << "\nCopying stack..." << std::endl;
	STDev::stack<int> s2 = s;
	s2.print();

	// Clear
	std::cout << "\nClearing original stack..." << std::endl;
	s.clear();
	std::cout << "Original stack empty: " << (s.empty() ? "Yes" : "No") << std::endl;
	std::cout << "Copied stack empty: " << (s2.empty() ? "Yes" : "No") << std::endl;
	s2.print();

	return 0;
}