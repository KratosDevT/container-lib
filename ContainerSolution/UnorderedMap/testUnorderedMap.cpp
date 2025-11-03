#include "unordered_map.h"
#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <algorithm>

using namespace STDev;

// ==================== UTILITIES ====================

class TestStats
{
private:
	int total = 0;
	int passed = 0;
	int failed = 0;

public:
	void record_pass() { passed++; total++; }
	void record_fail() { failed++; total++; }

	void print_summary() const
	{
		std::cout << "\n========================================" << std::endl;
		std::cout << "TEST SUMMARY" << std::endl;
		std::cout << "========================================" << std::endl;
		std::cout << "Total tests:  " << total << std::endl;
		std::cout << "Passed:       " << passed << " (" << (total > 0 ? (passed * 100 / total) : 0) << "%)" << std::endl;
		std::cout << "Failed:       " << failed << std::endl;
		std::cout << "========================================\n" << std::endl;
	}
};

TestStats g_stats;

void test_assert(bool condition, const std::string& test_name)
{
	if (condition)
	{
		std::cout << "[PASS] " << test_name << std::endl;
		g_stats.record_pass();
	}
	else
	{
		std::cout << "[FAIL] " << test_name << std::endl;
		g_stats.record_fail();
	}
}

void section_header(const std::string& section_name)
{
	std::cout << "\n========================================" << std::endl;
	std::cout << section_name << std::endl;
	std::cout << "========================================" << std::endl;
}

// ==================== TEST FUNCTIONS ====================

void test_basic_operations()
{
	section_header("TEST 1: BASIC OPERATIONS");

	unordered_map<int, std::string> m;

	// Test empty
	test_assert(m.empty(), "New map is empty");
	test_assert(m.size() == 0, "New map size is 0");

	// Test insert
	m.insert(5, "five");
	test_assert(!m.empty(), "Map not empty after insert");
	test_assert(m.size() == 1, "Map size is 1 after insert");

	// Test find
	test_assert(m.find(5), "Find existing key");
	test_assert(!m.find(10), "Don't find non-existing key");

	// Test at
	try
	{
		test_assert(m.at(5) == "five", "at() returns correct value");
	}
	catch (...)
	{
		test_assert(false, "at() throws on existing key");
	}

	try
	{
		m.at(999);
		test_assert(false, "at() should throw on non-existing key");
	}
	catch (const std::out_of_range&)
	{
		test_assert(true, "at() throws out_of_range on non-existing key");
	}

	// Test multiple inserts
	m.insert(3, "three");
	m.insert(7, "seven");
	m.insert(1, "one");
	test_assert(m.size() == 4, "Size is 4 after multiple inserts");

	// Test erase
	bool erased = m.erase(5);
	test_assert(erased, "erase() returns true on existing key");
	test_assert(m.size() == 3, "Size decreased after erase");
	test_assert(!m.find(5), "Key not found after erase");

	bool not_erased = m.erase(999);
	test_assert(!not_erased, "erase() returns false on non-existing key");

	// Test clear
	m.clear();
	test_assert(m.empty(), "Map is empty after clear");
	test_assert(m.size() == 0, "Size is 0 after clear");
}

void test_operator_bracket()
{
	section_header("TEST 2: OPERATOR[]");

	unordered_map<int, std::string> m;

	// Test insert via operator[]
	m[10] = "ten";
	test_assert(m.size() == 1, "operator[] inserts new key");
	test_assert(m[10] == "ten", "operator[] returns correct value");

	// Test modify via operator[]
	m[10] = "TEN";
	test_assert(m[10] == "TEN", "operator[] modifies existing value");
	test_assert(m.size() == 1, "Size unchanged after modification");

	// Test default value creation
	std::string val = m[999];
	test_assert(m.size() == 2, "operator[] creates new entry");
	test_assert(m.find(999), "New key exists");
}

void test_copy_and_assignment()
{
	section_header("TEST 3: COPY AND ASSIGNMENT");

	unordered_map<int, std::string> m1;
	m1.insert(1, "one");
	m1.insert(2, "two");
	m1.insert(3, "three");

	// Test copy constructor
	unordered_map<int, std::string> m2(m1);
	test_assert(m2.size() == 3, "Copy constructor: size matches");
	test_assert(m2.at(1) == "one", "Copy constructor: data matches");
	test_assert(m2.at(2) == "two", "Copy constructor: data matches");
	test_assert(m2.at(3) == "three", "Copy constructor: data matches");

	// Modify copy, ensure original unchanged
	m2.insert(4, "four");
	test_assert(m1.size() == 3, "Original unchanged after copy modification");
	test_assert(m2.size() == 4, "Copy modified independently");

	// Test copy assignment
	unordered_map<int, std::string> m3;
	m3.insert(99, "ninety-nine");
	m3 = m1;
	test_assert(m3.size() == 3, "Copy assignment: size matches");
	test_assert(m3.at(1) == "one", "Copy assignment: data matches");
	test_assert(!m3.find(99), "Copy assignment: old data removed");
}

void test_move_semantics()
{
	section_header("TEST 4: MOVE SEMANTICS");

	unordered_map<int, std::string> m1;
	m1.insert(1, "one");
	m1.insert(2, "two");
	m1.insert(3, "three");

	// Test move constructor
	unordered_map<int, std::string> m2(std::move(m1));
	test_assert(m2.size() == 3, "Move constructor: destination has data");
	test_assert(m1.size() == 0, "Move constructor: source is empty");
	test_assert(m1.empty(), "Move constructor: source is empty");

	// Test move assignment
	unordered_map<int, std::string> m3;
	m3.insert(99, "ninety-nine");
	m3 = std::move(m2);
	test_assert(m3.size() == 3, "Move assignment: destination has data");
	test_assert(m2.size() == 0, "Move assignment: source is empty");
	test_assert(m3.at(1) == "one", "Move assignment: data correct");
}

void test_iterators()
{
	section_header("TEST 5: ITERATORS");

	unordered_map<int, std::string> m;
	m.insert(5, "five");
	m.insert(2, "two");
	m.insert(8, "eight");
	m.insert(1, "one");
	m.insert(9, "nine");

	// Test iteration (order may vary)
	std::vector<int> keys;
	for (auto it = m.begin(); it != m.end(); ++it)
	{
		keys.push_back(it->first);
	}

	test_assert(keys.size() == 5, "Iterator visits all elements");

	// Verify all keys are present
	bool all_found = true;
	std::vector<int> expected = { 1, 2, 5, 8, 9 };
	for (int key : expected)
	{
		if (std::find(keys.begin(), keys.end(), key) == keys.end())
		{
			all_found = false;
			break;
		}
	}
	test_assert(all_found, "All keys found via iterator");

	// Test range-based for loop
	int count = 0;
	for (const auto& pair : m)
	{
		count++;
		(void)pair;
	}
	test_assert(count == 5, "Range-based for loop works");

	// Test const iterator
	const unordered_map<int, std::string>& const_m = m;
	count = 0;
	for (auto it = const_m.begin(); it != const_m.end(); ++it)
	{
		count++;
	}
	test_assert(count == 5, "Const iterator works");
}

void test_hash_collisions()
{
	section_header("TEST 6: HASH COLLISIONS");

	unordered_map<int, int> m;

	// Insert many elements to force collisions
	for (int i = 0; i < 100; i++)
	{
		m.insert(i, i * 2);
	}

	test_assert(m.size() == 100, "All elements inserted despite collisions");

	// Verify all elements
	bool all_correct = true;
	for (int i = 0; i < 100; i++)
	{
		if (!m.find(i) || m.at(i) != i * 2)
		{
			all_correct = false;
			break;
		}
	}
	test_assert(all_correct, "All elements retrievable after collisions");

	std::cout << "\nLoad factor: " << m.load_factor() << std::endl;
	std::cout << "Bucket count: " << m.bucket_count() << std::endl;
}

void test_rehashing()
{
	section_header("TEST 7: AUTOMATIC REHASHING");

	unordered_map<int, int> m;

	size_t initial_buckets = m.bucket_count();
	std::cout << "Initial bucket count: " << initial_buckets << std::endl;

	// Insert enough elements to trigger rehashing
	for (int i = 0; i < 50; i++)
	{
		m.insert(i, i);
	}

	size_t final_buckets = m.bucket_count();
	std::cout << "Final bucket count: " << final_buckets << std::endl;

	test_assert(final_buckets > initial_buckets, "Bucket count increased after rehashing");
	test_assert(m.size() == 50, "All elements present after rehashing");
	test_assert(m.load_factor() <= 0.75, "Load factor maintained");

	// Verify all elements still accessible
	bool all_found = true;
	for (int i = 0; i < 50; i++)
	{
		if (!m.find(i) || m.at(i) != i)
		{
			all_found = false;
			break;
		}
	}
	test_assert(all_found, "All elements accessible after rehashing");
}

void test_edge_cases()
{
	section_header("TEST 8: EDGE CASES");

	unordered_map<int, int> m;

	// Insert duplicate key
	m.insert(5, 100);
	m.insert(5, 200);
	test_assert(m.size() == 1, "Duplicate insert doesn't increase size");
	test_assert(m.at(5) == 200, "Duplicate insert updates value");

	// Erase from empty
	unordered_map<int, int> empty_map;
	bool erased = empty_map.erase(999);
	test_assert(!erased, "Erase from empty map returns false");

	// Single element operations
	unordered_map<int, int> single;
	single.insert(42, 84);
	single.erase(42);
	test_assert(single.empty(), "Single element erase leaves empty map");

	// Clear already empty map
	empty_map.clear();
	test_assert(empty_map.empty(), "Clear on empty map is safe");
}

void test_different_types()
{
	section_header("TEST 9: DIFFERENT DATA TYPES");

	// Test with strings
	unordered_map<std::string, int> string_map;
	string_map.insert("hello", 1);
	string_map.insert("world", 2);
	string_map.insert("cpp", 3);

	test_assert(string_map.size() == 3, "String key map works");
	test_assert(string_map.at("hello") == 1, "String key retrieval works");

	// Test with double (note: floating point keys are generally not recommended)
	unordered_map<int, double> double_map;
	double_map.insert(1, 3.14);
	double_map.insert(2, 2.71);

	test_assert(double_map.size() == 2, "Double value map works");
	test_assert(double_map.at(1) == 3.14, "Double value retrieval works");
}

void test_stress()
{
	section_header("TEST 10: STRESS TEST");

	unordered_map<int, int> m;

	const int N = 1000;

	// Insert many elements
	for (int i = 0; i < N; i++)
	{
		m.insert(i, i * 2);
	}

	test_assert(m.size() == N, "Stress: all elements inserted");

	// Verify all elements
	bool all_correct = true;
	for (int i = 0; i < N; i++)
	{
		if (!m.find(i) || m.at(i) != i * 2)
		{
			all_correct = false;
			break;
		}
	}
	test_assert(all_correct, "Stress: all elements correct");

	// Delete half
	for (int i = 0; i < N / 2; i++)
	{
		m.erase(i);
	}

	test_assert(m.size() == N / 2, "Stress: correct size after deletions");

	// Verify remaining elements
	bool remaining_correct = true;
	for (int i = N / 2; i < N; i++)
	{
		if (!m.find(i) || m.at(i) != i * 2)
		{
			remaining_correct = false;
			break;
		}
	}
	test_assert(remaining_correct, "Stress: remaining elements correct");
}

void test_visual_demonstration()
{
	section_header("VISUAL DEMONSTRATION");

	unordered_map<int, std::string> m;

	std::cout << "\nInserting elements: 5, 3, 7, 1, 9" << std::endl;
	m.insert(5, "five");
	m.insert(3, "three");
	m.insert(7, "seven");
	m.insert(1, "one");
	m.insert(9, "nine");

	m.print_structure();

	std::cout << "Elements via iterator:" << std::endl;
	for (const auto& pair : m)
	{
		std::cout << pair.first << " => " << pair.second << std::endl;
	}

	std::cout << "\n\nErasing 3 and 7..." << std::endl;
	m.erase(3);
	m.erase(7);

	m.print_structure();
}

// ==================== MAIN ====================

int main()
{
	std::cout << "\n";
	std::cout << "========================================" << std::endl;
	std::cout << "  UNORDERED_MAP TEST SUITE" << std::endl;
	std::cout << "========================================" << std::endl;

	// Run all tests
	test_basic_operations();
	test_operator_bracket();
	test_copy_and_assignment();
	test_move_semantics();
	test_iterators();
	test_hash_collisions();
	test_rehashing();
	test_edge_cases();
	test_different_types();
	test_stress();

	// Print test summary
	g_stats.print_summary();

	// Visual demonstration
	test_visual_demonstration();

	std::cout << "\n";
	std::cout << "========================================" << std::endl;
	std::cout << "      ALL TESTS COMPLETE" << std::endl;
	std::cout << "========================================" << std::endl;
	std::cout << "\n";

	return 0;
}