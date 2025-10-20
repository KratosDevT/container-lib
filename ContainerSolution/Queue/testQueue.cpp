#include <iostream>
#include "queue.h" 

int main()
{
	std::cout << "\n\n=== TEST QUEUE ===" << std::endl;
	STDev::queue<int> q;

	// Push elements
	std::cout << "\nEnqueuing 10, 20, 30, 40, 50" << std::endl;
	q.push(10);
	q.push(20);
	q.push(30);
	q.push(40);
	q.push(50);

	q.print();
	std::cout << "Size: " << q.size() << std::endl;
	std::cout << "Front: " << q.front() << std::endl;
	std::cout << "Back: " << q.back() << std::endl;

	// Pop elements
	std::cout << "\nDequeuing 2 elements..." << std::endl;
	q.pop();
	q.pop();
	q.print();
	std::cout << "Size: " << q.size() << std::endl;
	std::cout << "Front: " << q.front() << std::endl;
	std::cout << "Back: " << q.back() << std::endl;

	// Copy queue
	std::cout << "\nCopying queue..." << std::endl;
	STDev::queue<int> q2 = q;
	q2.print();

	// Move queue
	std::cout << "\nMoving queue..." << std::endl;
	STDev::queue<int> q3 = std::move(q2);
	q3.print();
	std::cout << "Moved-from queue empty: " << (q2.empty() ? "Yes" : "No") << std::endl;

	// Test with strings
	std::cout << "\n\n=== TEST WITH STRINGS ===" << std::endl;
	STDev::queue<std::string> strQueue;
	strQueue.push("First");
	strQueue.push("Second");
	strQueue.push("Third");
	strQueue.print();

	return 0;
}