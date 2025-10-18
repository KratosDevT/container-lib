#include "map.h"
#include <iostream>
#include <string>

using namespace STDev;

void test_basic_operations()
{
	std::cout << "\n========== TEST: Basic Operations ==========\n";

	map<int, std::string> m;

	std::cout << "Inserting elements...\n";
	m.insert(5, "five");
	m.insert(3, "three");
	m.insert(7, "seven");
	m.insert(1, "one");
	m.insert(9, "nine");

	std::cout << "Size: " << m.size() << " (expected: 5)\n";
	std::cout << "Empty: " << (m.empty() ? "yes" : "no") << " (expected: no)\n\n";

	m.print_tree();
}

void test_find()
{
	std::cout << "\n========== TEST: Find Operations ==========\n";

	map<int, std::string> m;
	m.insert(10, "ten");
	m.insert(20, "twenty");
	m.insert(30, "thirty");

	std::cout << "Find 20: " << (m.find(20) ? "found" : "not found") << " (expected: found)\n";
	std::cout << "Find 99: " << (m.find(99) ? "found" : "not found") << " (expected: not found)\n";
}

void test_operator_bracket()
{
	std::cout << "\n========== TEST: Operator[] ==========\n";

	map<std::string, int> m;

	std::cout << "Using operator[] to insert and modify...\n";
	m["apple"] = 5;
	m["banana"] = 3;
	m["cherry"] = 8;

	std::cout << "apple: " << m["apple"] << " (expected: 5)\n";
	std::cout << "banana: " << m["banana"] << " (expected: 3)\n";

	std::cout << "\nModifying banana...\n";
	m["banana"] = 10;
	std::cout << "banana: " << m["banana"] << " (expected: 10)\n";

	std::cout << "\nAccessing non-existent key 'date'...\n";
	int date_value = m["date"];
	std::cout << "date: " << date_value << " (expected: 0 - default constructed)\n";
	std::cout << "Size after accessing 'date': " << m.size() << " (expected: 4)\n\n";

	m.print_tree();
}

void test_at_method()
{
	std::cout << "\n========== TEST: at() Method ==========\n";

	map<int, std::string> m;
	m.insert(1, "one");
	m.insert(2, "two");
	m.insert(3, "three");

	std::cout << "Using at() to access existing keys:\n";
	std::cout << "at(1): " << m.at(1) << " (expected: one)\n";
	std::cout << "at(2): " << m.at(2) << " (expected: two)\n";

	std::cout << "\nModifying through at():\n";
	m.at(2) = "TWO";
	std::cout << "at(2) after modification: " << m.at(2) << " (expected: TWO)\n";

	std::cout << "\nTrying to access non-existent key with at():\n";
	try
	{
		std::string val = m.at(999);
		std::cout << "ERROR: Should have thrown exception!\n";
	}
	catch (const std::out_of_range& e)
	{
		std::cout << "Caught exception: " << e.what() << " (expected behavior)\n";
	}

	std::cout << "Size after exception: " << m.size() << " (expected: 3 - no insertion)\n";
}

void test_iterator_basic()
{
	std::cout << "\n========== TEST: Iterator Basic ==========\n";

	map<int, std::string> m;
	m.insert(5, "five");
	m.insert(2, "two");
	m.insert(8, "eight");
	m.insert(1, "one");
	m.insert(3, "three");

	std::cout << "Map contents (using iterator):\n";
	std::cout << "Expected order: 1, 2, 3, 5, 8\n";
	std::cout << "Actual order: ";

	for (auto it = m.begin(); it != m.end(); ++it)
	{
		std::cout << it->first << " ";
	}
	std::cout << "\n\n";

	std::cout << "Key-Value pairs:\n";
	for (auto it = m.begin(); it != m.end(); ++it)
	{
		std::cout << "  [" << it->first << ": " << it->second << "]\n";
	}
}

void test_iterator_range_based()
{
	std::cout << "\n========== TEST: Range-Based For Loop ==========\n";

	map<std::string, int> grades;
	grades["Alice"] = 85;
	grades["Bob"] = 92;
	grades["Charlie"] = 78;
	grades["Diana"] = 95;

	std::cout << "Student grades (in alphabetical order):\n";
	for (const auto& pair : grades)
	{
		std::cout << "  " << pair.first << ": " << pair.second << "\n";
	}
}

void test_iterator_modification()
{
	std::cout << "\n========== TEST: Iterator Modification ==========\n";

	map<int, int> m;
	m.insert(1, 10);
	m.insert(2, 20);
	m.insert(3, 30);

	std::cout << "Original values:\n";
	for (const auto& pair : m)
	{
		std::cout << "  " << pair.first << ": " << pair.second << "\n";
	}

	std::cout << "\nDoubling all values using iterator:\n";
	for (auto it = m.begin(); it != m.end(); ++it)
	{
		it->second *= 2;
	}

	std::cout << "Modified values:\n";
	for (const auto& pair : m)
	{
		std::cout << "  " << pair.first << ": " << pair.second << "\n";
	}
}

void test_const_iterator()
{
	std::cout << "\n========== TEST: Const Iterator ==========\n";

	map<int, std::string> m;
	m.insert(10, "ten");
	m.insert(20, "twenty");
	m.insert(30, "thirty");

	const map<int, std::string>& const_ref = m;

	std::cout << "Using const_iterator on const reference:\n";
	for (auto it = const_ref.begin(); it != const_ref.end(); ++it)
	{
		std::cout << "  [" << it->first << ": " << it->second << "]\n";
	}

	std::cout << "\nUsing cbegin()/cend():\n";
	for (auto it = m.cbegin(); it != m.cend(); ++it)
	{
		std::cout << "  [" << it->first << ": " << it->second << "]\n";
	}
}

void test_iterator_empty_map()
{
	std::cout << "\n========== TEST: Iterator on Empty Map ==========\n";

	map<int, std::string> m;

	std::cout << "Iterating over empty map:\n";
	int count = 0;
	for (auto it = m.begin(); it != m.end(); ++it)
	{
		count++;
	}
	std::cout << "Iterations: " << count << " (expected: 0)\n";

	std::cout << "\nUsing range-based for on empty map:\n";
	count = 0;
	for (const auto& pair : m)
	{
		count++;
	}
	std::cout << "Iterations: " << count << " (expected: 0)\n";
}

void test_iterator_single_element()
{
	std::cout << "\n========== TEST: Iterator Single Element ==========\n";

	map<int, std::string> m;
	m.insert(42, "answer");

	std::cout << "Iterating over single element:\n";
	for (const auto& pair : m)
	{
		std::cout << "  [" << pair.first << ": " << pair.second << "]\n";
	}
}

void test_copy_constructor()
{
	std::cout << "\n========== TEST: Copy Constructor ==========\n";

	map<int, std::string> m1;
	m1.insert(1, "one");
	m1.insert(2, "two");
	m1.insert(3, "three");

	std::cout << "Original map m1:\n";
	m1.print_tree();

	map<int, std::string> m2(m1);

	std::cout << "Copied map m2:\n";
	m2.print_tree();

	std::cout << "Modifying m2...\n";
	m2.insert(4, "four");

	std::cout << "\nAfter modification:\n";
	std::cout << "m1 size: " << m1.size() << " (expected: 3)\n";
	std::cout << "m2 size: " << m2.size() << " (expected: 4)\n";
}

void test_assignment_operator()
{
	std::cout << "\n========== TEST: Assignment Operator ==========\n";

	map<int, std::string> m1;
	m1.insert(10, "ten");
	m1.insert(20, "twenty");

	map<int, std::string> m2;
	m2.insert(100, "hundred");
	m2.insert(200, "two hundred");
	m2.insert(300, "three hundred");

	std::cout << "Before assignment:\n";
	std::cout << "m1 size: " << m1.size() << "\n";
	std::cout << "m2 size: " << m2.size() << "\n\n";

	m1 = m2;

	std::cout << "After m1 = m2:\n";
	std::cout << "m1 size: " << m1.size() << " (expected: 3)\n";
	std::cout << "m2 size: " << m2.size() << " (expected: 3)\n";

	std::cout << "\nm1 content:\n";
	m1.print_tree();

	std::cout << "Testing self-assignment...\n";
	m1 = m1;
	std::cout << "m1 size after self-assignment: " << m1.size() << " (expected: 3)\n";
}

void test_erase()
{
	std::cout << "\n========== TEST: Erase Operations ==========\n";

	map<int, std::string> m;
	m.insert(50, "fifty");
	m.insert(30, "thirty");
	m.insert(70, "seventy");
	m.insert(20, "twenty");
	m.insert(40, "forty");
	m.insert(60, "sixty");
	m.insert(80, "eighty");

	std::cout << "Initial tree:\n";
	m.print_tree();

	std::cout << "Erasing leaf node (20)...\n";
	bool erased = m.erase(20);
	std::cout << "Erased: " << (erased ? "yes" : "no") << " (expected: yes)\n";
	std::cout << "Size: " << m.size() << " (expected: 6)\n";
	m.print_tree();

	std::cout << "Erasing node with one child (30)...\n";
	erased = m.erase(30);
	std::cout << "Erased: " << (erased ? "yes" : "no") << " (expected: yes)\n";
	std::cout << "Size: " << m.size() << " (expected: 5)\n";
	m.print_tree();

	std::cout << "Erasing node with two children (70)...\n";
	erased = m.erase(70);
	std::cout << "Erased: " << (erased ? "yes" : "no") << " (expected: yes)\n";
	std::cout << "Size: " << m.size() << " (expected: 4)\n";
	m.print_tree();

	std::cout << "Attempting to erase non-existent key (999)...\n";
	erased = m.erase(999);
	std::cout << "Erased: " << (erased ? "yes" : "no") << " (expected: no)\n";
	std::cout << "Size: " << m.size() << " (expected: 4)\n";
}

void test_clear()
{
	std::cout << "\n========== TEST: Clear Operation ==========\n";

	map<int, std::string> m;
	m.insert(1, "one");
	m.insert(2, "two");
	m.insert(3, "three");
	m.insert(4, "four");
	m.insert(5, "five");

	std::cout << "Before clear:\n";
	std::cout << "Size: " << m.size() << " (expected: 5)\n";
	std::cout << "Empty: " << (m.empty() ? "yes" : "no") << " (expected: no)\n";

	m.clear();

	std::cout << "\nAfter clear:\n";
	std::cout << "Size: " << m.size() << " (expected: 0)\n";
	std::cout << "Empty: " << (m.empty() ? "yes" : "no") << " (expected: yes)\n";

	m.print_tree();

	std::cout << "Inserting after clear...\n";
	m.insert(100, "hundred");
	std::cout << "Size: " << m.size() << " (expected: 1)\n";
	m.print_tree();
}

void test_duplicate_keys()
{
	std::cout << "\n========== TEST: Duplicate Keys ==========\n";

	map<int, std::string> m;
	m.insert(5, "first_five");

	std::cout << "Initial value for key 5: " << m[5] << " (expected: first_five)\n";

	m.insert(5, "second_five");

	std::cout << "After re-inserting key 5: " << m[5] << " (expected: second_five)\n";
	std::cout << "Size: " << m.size() << " (expected: 1)\n";
}

void test_complex_scenario()
{
	std::cout << "\n========== TEST: Complex Scenario ==========\n";

	map<std::string, int> grades;

	std::cout << "Adding student grades...\n";
	grades["Alice"] = 85;
	grades["Bob"] = 92;
	grades["Charlie"] = 78;
	grades["Diana"] = 95;
	grades["Eve"] = 88;

	grades.print_tree();

	std::cout << "Updating Bob's grade using at()...\n";
	grades.at("Bob") = 97;
	std::cout << "Bob's new grade: " << grades.at("Bob") << "\n";

	std::cout << "\nRemoving Charlie...\n";
	grades.erase("Charlie");
	std::cout << "Charlie in map: " << (grades.find("Charlie") ? "yes" : "no") << " (expected: no)\n";

	std::cout << "\nFinal grades using iterator:\n";
	for (const auto& student : grades)
	{
		std::cout << "  " << student.first << ": " << student.second << "\n";
	}
}

void test_iterator_postfix()
{
	std::cout << "\n========== TEST: Iterator Postfix Increment ==========\n";

	map<int, std::string> m;
	m.insert(1, "one");
	m.insert(2, "two");
	m.insert(3, "three");

	std::cout << "Testing postfix increment (it++):\n";
	auto it = m.begin();
	std::cout << "First element: " << it->first << "\n";

	auto old_it = it++;
	std::cout << "After it++, old iterator points to: " << old_it->first << " (expected: 1)\n";
	std::cout << "Current iterator points to: " << it->first << " (expected: 2)\n";
}

int main()
{
	std::cout << "==========================================\n";
	std::cout << "   STDev::map Test Suite                 \n";
	std::cout << "==========================================\n";

	try
	{
		test_basic_operations();
		test_find();
		test_operator_bracket();
		test_at_method();
		test_iterator_basic();
		test_iterator_range_based();
		test_iterator_modification();
		test_const_iterator();
		test_iterator_empty_map();
		test_iterator_single_element();
		test_iterator_postfix();
		test_copy_constructor();
		test_assignment_operator();
		test_erase();
		test_clear();
		test_duplicate_keys();
		test_complex_scenario();

		std::cout << "\n==========================================\n";
		std::cout << "   All tests completed successfully!    \n";
		std::cout << "==========================================\n";
	}
	catch (const std::exception& e)
	{
		std::cerr << "\n[ERROR] Exception caught: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}