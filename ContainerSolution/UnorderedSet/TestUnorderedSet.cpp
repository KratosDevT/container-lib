#include "unordered_set.h"
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

	unordered_set<int> s;

	// Test empty
	test_assert(s.empty(), "New set is empty");
	test_assert(s.size() == 0, "New set size is 0");

	// Test insert
	bool inserted = s.insert(5);
	test_assert(inserted, "Insert new element returns true");
	test_assert(!s.empty(), "Set not empty after insert");
	test_assert(s.size() == 1, "Set size is 1 after insert");

	// Test find
	test_assert(s.find(5), "Find existing element");
	test_assert(!s.find(10), "Don't find non-existing element");

	// Test contains
	test_assert(s.contains(5), "Contains existing element");
	test_assert(!s.contains(10), "Doesn't contain non-existing element");

	// Test multiple inserts
	s.insert(3);
	s.insert(7);
	s.insert(1);
	test_assert(s.size() == 4, "Size is 4 after multiple inserts");

	// Test duplicate insert
	inserted = s.insert(5);
	test_assert(!inserted, "Insert duplicate returns false");
	test_assert(s.size() == 4, "Size unchanged after duplicate insert");

	// Test erase
	bool erased = s.erase(5);
	test_assert(erased, "erase() returns true on existing element");
	test_assert(s.size() == 3, "Size decreased after erase");
	test_assert(!s.find(5), "Element not found after erase");

	bool not_erased = s.erase(999);
	test_assert(!not_erased, "erase() returns false on non-existing element");

	// Test clear
	s.clear();
	test_assert(s.empty(), "Set is empty after clear");
	test_assert(s.size() == 0, "Size is 0 after clear");
}

void test_copy_and_assignment()
{
	section_header("TEST 2: COPY AND ASSIGNMENT");

	unordered_set<int> s1;
	s1.insert(1);
	s1.insert(2);
	s1.insert(3);

	// Test copy constructor
	unordered_set<int> s2(s1);
	test_assert(s2.size() == 3, "Copy constructor: size matches");
	test_assert(s2.contains(1), "Copy constructor: data matches");
	test_assert(s2.contains(2), "Copy constructor: data matches");
	test_assert(s2.contains(3), "Copy constructor: data matches");

	// Modify copy, ensure original unchanged
	s2.insert(4);
	test_assert(s1.size() == 3, "Original unchanged after copy modification");
	test_assert(s2.size() == 4, "Copy modified independently");

	// Test copy assignment
	unordered_set<int> s3;
	s3.insert(99);
	s3 = s1;
	test_assert(s3.size() == 3, "Copy assignment: size matches");
	test_assert(s3.contains(1), "Copy assignment: data matches");
	test_assert(!s3.contains(99), "Copy assignment: old data removed");
}

void test_move_semantics()
{
	section_header("TEST 3: MOVE SEMANTICS");

	unordered_set<int> s1;
	s1.insert(1);
	s1.insert(2);
	s1.insert(3);

	// Test move constructor
	unordered_set<int> s2(std::move(s1));
	test_assert(s2.size() == 3, "Move constructor: destination has data");
	test_assert(s1.size() == 0, "Move constructor: source is empty");
	test_assert(s1.empty(), "Move constructor: source is empty");

	// Test move assignment
	unordered_set<int> s3;
	s3.insert(99);
	s3 = std::move(s2);
	test_assert(s3.size() == 3, "Move assignment: destination has data");
	test_assert(s2.size() == 0, "Move assignment: source is empty");
	test_assert(s3.contains(1), "Move assignment: data correct");
}

void test_iterators()
{
	section_header("TEST 4: ITERATORS");

	unordered_set<int> s;
	s.insert(5);
	s.insert(2);
	s.insert(8);
	s.insert(1);
	s.insert(9);

	// Test iteration (order may vary)
	std::vector<int> values;
	for (auto it = s.begin(); it != s.end(); ++it)
	{
		values.push_back(*it);
	}

	test_assert(values.size() == 5, "Iterator visits all elements");

	// Verify all values are present
	bool all_found = true;
	std::vector<int> expected = { 1, 2, 5, 8, 9 };
	for (int val : expected)
	{
		if (std::find(values.begin(), values.end(), val) == values.end())
		{
			all_found = false;
			break;
		}
	}
	test_assert(all_found, "All values found via iterator");

	// Test range-based for loop
	int count = 0;
	for (const auto& val : s)
	{
		count++;
		(void)val;
	}
	test_assert(count == 5, "Range-based for loop works");

	// Test const iterator
	const unordered_set<int>& const_s = s;
	count = 0;
	for (auto it = const_s.begin(); it != const_s.end(); ++it)
	{
		count++;
	}
	test_assert(count == 5, "Const iterator works");
}

void test_hash_collisions()
{
	section_header("TEST 5: HASH COLLISIONS");

	unordered_set<int> s;

	// Insert many elements to force collisions
	for (int i = 0; i < 100; i++)
	{
		s.insert(i);
	}

	test_assert(s.size() == 100, "All elements inserted despite collisions");

	// Verify all elements
	bool all_found = true;
	for (int i = 0; i < 100; i++)
	{
		if (!s.contains(i))
		{
			all_found = false;
			break;
		}
	}
	test_assert(all_found, "All elements findable after collisions");

	std::cout << "\nLoad factor: " << s.load_factor() << std::endl;
	std::cout << "Bucket count: " << s.bucket_count() << std::endl;
}

void test_rehashing()
{
	section_header("TEST 6: AUTOMATIC REHASHING");

	unordered_set<int> s;

	size_t initial_buckets = s.bucket_count();
	std::cout << "Initial bucket count: " << initial_buckets << std::endl;

	// Insert enough elements to trigger rehashing
	for (int i = 0; i < 50; i++)
	{
		s.insert(i);
	}

	size_t final_buckets = s.bucket_count();
	std::cout << "Final bucket count: " << final_buckets << std::endl;

	test_assert(final_buckets > initial_buckets, "Bucket count increased after rehashing");
	test_assert(s.size() == 50, "All elements present after rehashing");
	test_assert(s.load_factor() <= 0.75, "Load factor maintained");

	// Verify all elements still accessible
	bool all_found = true;
	for (int i = 0; i < 50; i++)
	{
		if (!s.contains(i))
		{
			all_found = false;
			break;
		}
	}
	test_assert(all_found, "All elements accessible after rehashing");
}

void test_edge_cases()
{
	section_header("TEST 7: EDGE CASES");

	unordered_set<int> s;

	// Insert duplicate
	s.insert(5);
	bool inserted = s.insert(5);
	test_assert(!inserted, "Duplicate insert returns false");
	test_assert(s.size() == 1, "Duplicate insert doesn't increase size");

	// Erase from empty
	unordered_set<int> empty_set;
	bool erased = empty_set.erase(999);
	test_assert(!erased, "Erase from empty set returns false");

	// Single element operations
	unordered_set<int> single;
	single.insert(42);
	single.erase(42);
	test_assert(single.empty(), "Single element erase leaves empty set");

	// Clear already empty set
	empty_set.clear();
	test_assert(empty_set.empty(), "Clear on empty set is safe");
}

void test_different_types()
{
	section_header("TEST 8: DIFFERENT DATA TYPES");

	// Test with strings
	unordered_set<std::string> string_set;
	string_set.insert("hello");
	string_set.insert("world");
	string_set.insert("cpp");

	test_assert(string_set.size() == 3, "String set works");
	test_assert(string_set.contains("hello"), "String contains works");

	// Test with double (note: floating point keys are generally not recommended for sets)
	unordered_set<int> int_set;
	int_set.insert(1);
	int_set.insert(2);
	int_set.insert(3);

	test_assert(int_set.size() == 3, "Int set works");
	test_assert(int_set.contains(2), "Int contains works");
}

void test_stress()
{
	section_header("TEST 9: STRESS TEST");

	unordered_set<int> s;

	const int N = 1000;

	// Insert many elements
	for (int i = 0; i < N; i++)
	{
		s.insert(i);
	}

	test_assert(s.size() == N, "Stress: all elements inserted");

	// Verify all elements
	bool all_found = true;
	for (int i = 0; i < N; i++)
	{
		if (!s.contains(i))
		{
			all_found = false;
			break;
		}
	}
	test_assert(all_found, "Stress: all elements findable");

	// Delete half
	for (int i = 0; i < N / 2; i++)
	{
		s.erase(i);
	}

	test_assert(s.size() == N / 2, "Stress: correct size after deletions");

	// Verify remaining elements
	bool remaining_correct = true;
	for (int i = N / 2; i < N; i++)
	{
		if (!s.contains(i))
		{
			remaining_correct = false;
			break;
		}
	}
	test_assert(remaining_correct, "Stress: remaining elements correct");
}

void test_uniqueness()
{
	section_header("TEST 10: UNIQUENESS ENFORCEMENT");

	unordered_set<int> s;

	std::vector<int> values = { 1, 2, 3, 1, 2, 4, 5, 3, 1 };

	for (int val : values)
	{
		s.insert(val);
	}

	test_assert(s.size() == 5, "Set contains only unique elements");
	test_assert(s.contains(1), "Contains 1");
	test_assert(s.contains(2), "Contains 2");
	test_assert(s.contains(3), "Contains 3");
	test_assert(s.contains(4), "Contains 4");
	test_assert(s.contains(5), "Contains 5");
}

void test_visual_demonstration()
{
	section_header("VISUAL DEMONSTRATION");

	unordered_set<int> s;

	std::cout << "\nInserting elements: 5, 3, 7, 1, 9" << std::endl;
	s.insert(5);
	s.insert(3);
	s.insert(7);
	s.insert(1);
	s.insert(9);

	s.print_structure();

	std::cout << "Elements via iterator:" << std::endl;
	for (const auto& val : s)
	{
		std::cout << val << " ";
	}
	std::cout << "\n" << std::endl;

	std::cout << "Erasing 3 and 7..." << std::endl;
	s.erase(3);
	s.erase(7);

	s.print_structure();

	std::cout << "Trying to insert duplicate 5..." << std::endl;
	bool inserted = s.insert(5);
	std::cout << "Insert successful: " << (inserted ? "Yes" : "No") << std::endl;

	s.print_structure();
}

// ==================== MAIN ====================

int main()
{
	std::cout << "\n";
	std::cout << "========================================" << std::endl;
	std::cout << "    UNORDERED_SET TEST SUITE" << std::endl;
	std::cout << "========================================" << std::endl;

	// Run all tests
	test_basic_operations();
	test_copy_and_assignment();
	test_move_semantics();
	test_iterators();
	test_hash_collisions();
	test_rehashing();
	test_edge_cases();
	test_different_types();
	test_stress();
	test_uniqueness();

	// Print test summary
	g_stats.print_summary();

	// Visual demonstration
	test_visual_demonstration();

	std::cout << "\n";
	std::cout << "========================================" << std::endl;
	std::cout << "        ALL TESTS COMPLETE" << std::endl;
	std::cout << "========================================" << std::endl;
	std::cout << "\n";

	return 0;
}