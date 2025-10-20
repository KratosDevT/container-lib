#include "map.h"
#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include <algorithm>
#include <string>
#include <cassert>
#include <iomanip> 

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

	STDev::map<int, std::string> m;

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

	STDev::map<int, std::string> m;

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

	STDev::map<int, std::string> m1;
	m1.insert(1, "one");
	m1.insert(2, "two");
	m1.insert(3, "three");

	// Test copy constructor
	STDev::map<int, std::string> m2(m1);
	test_assert(m2.size() == 3, "Copy constructor: size matches");
	test_assert(m2.at(1) == "one", "Copy constructor: data matches");
	test_assert(m2.at(2) == "two", "Copy constructor: data matches");
	test_assert(m2.at(3) == "three", "Copy constructor: data matches");

	// Modify copy, ensure original unchanged
	m2.insert(4, "four");
	test_assert(m1.size() == 3, "Original unchanged after copy modification");
	test_assert(m2.size() == 4, "Copy modified independently");

	// Test copy assignment
	STDev::map<int, std::string> m3;
	m3.insert(99, "ninety-nine");
	m3 = m1;
	test_assert(m3.size() == 3, "Copy assignment: size matches");
	test_assert(m3.at(1) == "one", "Copy assignment: data matches");
	test_assert(!m3.find(99), "Copy assignment: old data removed");
}

void test_move_semantics()
{
	section_header("TEST 4: MOVE SEMANTICS");

	STDev::map<int, std::string> m1;
	m1.insert(1, "one");
	m1.insert(2, "two");
	m1.insert(3, "three");

	// Test move constructor
	STDev::map<int, std::string> m2(std::move(m1));
	test_assert(m2.size() == 3, "Move constructor: destination has data");
	test_assert(m1.size() == 0, "Move constructor: source is empty");
	test_assert(m1.empty(), "Move constructor: source is empty");

	// Test move assignment
	STDev::map<int, std::string> m3;
	m3.insert(99, "ninety-nine");
	m3 = std::move(m2);
	test_assert(m3.size() == 3, "Move assignment: destination has data");
	test_assert(m2.size() == 0, "Move assignment: source is empty");
	test_assert(m3.at(1) == "one", "Move assignment: data correct");
}

void test_iterators()
{
	section_header("TEST 5: ITERATORS");

	STDev::map<int, std::string> m;
	m.insert(5, "five");
	m.insert(2, "two");
	m.insert(8, "eight");
	m.insert(1, "one");
	m.insert(9, "nine");

	// Test in-order traversal
	std::vector<int> keys;
	for (auto it = m.begin(); it != m.end(); ++it)
	{
		keys.push_back(it->first);
	}

	bool in_order = true;
	for (size_t i = 1; i < keys.size(); i++)
	{
		if (keys[i] <= keys[i - 1])
		{
			in_order = false;
			break;
		}
	}
	test_assert(in_order, "Iterator traversal is in-order");
	test_assert(keys.size() == 5, "Iterator visits all elements");

	// Test range-based for loop
	int count = 0;
	for (const auto& pair : m)
	{
		count++;
		(void)pair; // Suppress unused warning
	}
	test_assert(count == 5, "Range-based for loop works");

	// Test const iterator
	const STDev::map<int, std::string>& const_m = m;
	count = 0;
	for (auto it = const_m.begin(); it != const_m.end(); ++it)
	{
		count++;
	}
	test_assert(count == 5, "Const iterator works");
}

void test_stress_sequential()
{
	section_header("TEST 6: STRESS TEST - SEQUENTIAL INSERT");

	STDev::map<int, int> m;

	const int N = 1000;

	// Insert in order (worst case for unbalanced BST)
	for (int i = 0; i < N; i++)
	{
		m.insert(i, i * 2);
	}

	test_assert(m.size() == N, "Sequential insert: all elements inserted");

	// Verify all elements
	bool all_found = true;
	for (int i = 0; i < N; i++)
	{
		if (!m.find(i) || m.at(i) != i * 2)
		{
			all_found = false;
			break;
		}
	}
	test_assert(all_found, "Sequential insert: all elements findable");

	// Test RB-Tree properties
	if (m.is_valid_rb_tree())
	{
		test_assert(true, "RB-Tree properties maintained after 1000 sequential inserts");
	}
	else
	{
		test_assert(false, "RB-Tree properties violated!");
	}
}

void test_stress_random()
{
	section_header("TEST 7: STRESS TEST - RANDOM INSERT/DELETE");

	STDev::map<int, int> m;

	std::vector<int> values;
	for (int i = 0; i < 500; i++)
		values.push_back(i);

	// Shuffle
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(values.begin(), values.end(), g);

	// Random inserts
	for (int v : values)
	{
		m.insert(v, v * 2);
	}

	test_assert(m.size() == 500, "Random insert: all elements inserted");

	// Random deletes (half)
	std::shuffle(values.begin(), values.end(), g);
	for (int i = 0; i < 250; i++)
	{
		m.erase(values[i]);
	}

	test_assert(m.size() == 250, "Random delete: correct size after deletions");

	// Verify remaining elements
	bool correct = true;
	for (int i = 250; i < 500; i++)
	{
		if (!m.find(values[i]))
		{
			correct = false;
			break;
		}
	}
	test_assert(correct, "Random delete: remaining elements correct");

	// Test RB-Tree properties
	if (m.is_valid_rb_tree())
	{
		test_assert(true, "RB-Tree properties maintained after random ops");
	}
	else
	{
		test_assert(false, "RB-Tree properties violated after random ops!");
	}
}

void test_edge_cases()
{
	section_header("TEST 8: EDGE CASES");

	STDev::map<int, int> m;

	// Insert duplicate key
	m.insert(5, 100);
	m.insert(5, 200);
	test_assert(m.size() == 1, "Duplicate insert doesn't increase size");
	test_assert(m.at(5) == 200, "Duplicate insert updates value");

	// Erase from empty
	STDev::map<int, int> empty_map;
	bool erased = empty_map.erase(999);
	test_assert(!erased, "Erase from empty map returns false");

	// Single element operations
	STDev::map<int, int> single;
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
	STDev::map<std::string, int> string_map;
	string_map.insert("hello", 1);
	string_map.insert("world", 2);
	string_map.insert("cpp", 3);

	test_assert(string_map.size() == 3, "String key map works");
	test_assert(string_map.at("hello") == 1, "String key retrieval works");

	// Test with double
	STDev::map<double, std::string> double_map;
	double_map.insert(3.14, "pi");
	double_map.insert(2.71, "e");

	test_assert(double_map.size() == 2, "Double key map works");
	test_assert(double_map.at(3.14) == "pi", "Double key retrieval works");
}

void test_bst_vs_rbtree()
{
	section_header("TEST 10: BST vs RB-TREE COMPARISON");

	const int N = 100;

	// Create both types
	STDev::map<int, int, STDev::TreeType::BinarySearchTree> bst;
	STDev::map<int, int, STDev::TreeType::RedBlackTree> rbt;

	// Insert sequential (worst case for BST)
	for (int i = 0; i < N; i++)
	{
		bst.insert(i, i);
		rbt.insert(i, i);
	}

	test_assert(bst.size() == N, "BST: all elements inserted");
	test_assert(rbt.size() == N, "RBT: all elements inserted");

	// Both should find all elements (but RBT should be much faster)
	bool bst_correct = true, rbt_correct = true;
	for (int i = 0; i < N; i++)
	{
		if (!bst.find(i)) bst_correct = false;
		if (!rbt.find(i)) rbt_correct = false;
	}

	test_assert(bst_correct, "BST: all elements findable");
	test_assert(rbt_correct, "RBT: all elements findable");

	std::cout << "\nNote: BST becomes a linked list with sequential inserts," << std::endl;
	std::cout << "      while RB-Tree maintains balance." << std::endl;
}

// ==================== BENCHMARKS ====================

void benchmark_insert()
{
	section_header("BENCHMARK 1: INSERT PERFORMANCE");

	const int N = 10000;
	std::vector<int> values;
	for (int i = 0; i < N; i++)
		values.push_back(i);

	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(values.begin(), values.end(), g);

	// Benchmark RB-Tree
	auto start = std::chrono::high_resolution_clock::now();
	STDev::map<int, int, STDev::TreeType::RedBlackTree> rb;
	for (int v : values)
		rb.insert(v, v);
	auto end = std::chrono::high_resolution_clock::now();
	auto duration_rb = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	// Benchmark BST
	start = std::chrono::high_resolution_clock::now();
	STDev::map<int, int, STDev::TreeType::BinarySearchTree> bst;
	for (int v : values)
		bst.insert(v, v);
	end = std::chrono::high_resolution_clock::now();
	auto duration_bst = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	std::cout << "\nInserting " << N << " random elements:" << std::endl;
	std::cout << "  RB-Tree:  " << duration_rb.count() << " ms" << std::endl;
	std::cout << "  BST:      " << duration_bst.count() << " ms" << std::endl;

	rb.print_tree_info();
}

void benchmark_search()
{
	section_header("BENCHMARK 2: SEARCH PERFORMANCE");

	const int N = 10000;
	const int SEARCHES = 100000;

	std::vector<int> values;
	for (int i = 0; i < N; i++)
		values.push_back(i);

	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(values.begin(), values.end(), g);

	// Build trees
	STDev::map<int, int, STDev::TreeType::RedBlackTree> rb;
	STDev::map<int, int, STDev::TreeType::BinarySearchTree> bst;

	for (int v : values)
	{
		rb.insert(v, v);
		bst.insert(v, v);
	}

	// Generate random search keys
	std::vector<int> search_keys;
	for (int i = 0; i < SEARCHES; i++)
		search_keys.push_back(rand() % N);

	// Benchmark RB-Tree search
	auto start = std::chrono::high_resolution_clock::now();
	int rb_found = 0;
	for (int key : search_keys)
		if (rb.find(key)) rb_found++;
	auto end = std::chrono::high_resolution_clock::now();
	auto duration_rb = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	// Benchmark BST search
	start = std::chrono::high_resolution_clock::now();
	int bst_found = 0;
	for (int key : search_keys)
		if (bst.find(key)) bst_found++;
	end = std::chrono::high_resolution_clock::now();
	auto duration_bst = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	std::cout << "\nSearching " << SEARCHES << " random keys in " << N << " elements:" << std::endl;
	std::cout << "  RB-Tree:  " << duration_rb.count() << " ms (found: " << rb_found << ")" << std::endl;
	std::cout << "  BST:      " << duration_bst.count() << " ms (found: " << bst_found << ")" << std::endl;
}

void benchmark_worst_case()
{
	section_header("BENCHMARK 3: WORST CASE (SEQUENTIAL INSERT)");

	const int N = 10000;  // Aumentato da 5000

	// Sequential insert (worst case for BST)
	std::cout << "\n--- INSERIMENTO SEQUENZIALE ---" << std::endl;

	auto start = std::chrono::high_resolution_clock::now();
	STDev::map<int, int, STDev::TreeType::RedBlackTree> rb;
	for (int i = 0; i < N; i++)
		rb.insert(i, i);
	auto end = std::chrono::high_resolution_clock::now();
	auto duration_rb = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	start = std::chrono::high_resolution_clock::now();
	STDev::map<int, int, STDev::TreeType::BinarySearchTree> bst;
	for (int i = 0; i < N; i++)
		bst.insert(i, i);
	end = std::chrono::high_resolution_clock::now();
	auto duration_bst = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	std::cout << "Inserting " << N << " sequential elements:" << std::endl;
	std::cout << "  RB-Tree:  " << duration_rb.count() << " ms" << std::endl;
	std::cout << "  BST:      " << duration_bst.count() << " ms";

	if (duration_bst.count() > duration_rb.count())
	{
		double ratio = (double)duration_bst.count() / (duration_rb.count() > 0 ? duration_rb.count() : 1);
		std::cout << "  (" << (int)(ratio) << "x SLOWER!)";
	}
	std::cout << std::endl;

	// Now search in worst case - QUI SI VEDE LA DIFFERENZA!
	std::cout << "\n--- RICERCA NEL WORST CASE ---" << std::endl;
	std::vector<int> search_keys;
	const int SEARCHES = 50000;  // Aumentato
	for (int i = 0; i < SEARCHES; i++)
		search_keys.push_back(rand() % N);

	start = std::chrono::high_resolution_clock::now();
	int rb_found = 0;
	for (int key : search_keys)
		if (rb.find(key)) rb_found++;
	end = std::chrono::high_resolution_clock::now();
	duration_rb = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	start = std::chrono::high_resolution_clock::now();
	int bst_found = 0;
	for (int key : search_keys)
		if (bst.find(key)) bst_found++;
	end = std::chrono::high_resolution_clock::now();
	duration_bst = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	std::cout << "Searching " << SEARCHES << " random keys in worst-case tree:" << std::endl;
	std::cout << "  RB-Tree:  " << duration_rb.count() << " ms (found: " << rb_found << ")" << std::endl;
	std::cout << "  BST:      " << duration_bst.count() << " ms (found: " << bst_found << ")";

	if (duration_bst.count() > duration_rb.count())
	{
		double ratio = (double)duration_bst.count() / (duration_rb.count() > 0 ? duration_rb.count() : 1);
		std::cout << "  (" << (int)(ratio) << "x SLOWER!)";
	}
	std::cout << std::endl;

	std::cout << "\nNote: BST diventa una lista (O(n)) con inserimenti sequenziali!" << std::endl;
	std::cout << "      RB-Tree mantiene balance (O(log n))" << std::endl;
}

void benchmark_comparison_visual()
{
	section_header("BENCHMARK 5: VISUAL COMPARISON");

	std::cout << "\n=== CRESCITA DELLA COMPLESSITA ===" << std::endl;
	std::cout << "Inserimenti sequenziali + ricerca dell'ultimo elemento:\n" << std::endl;

	std::vector<int> sizes = { 100, 500, 1000, 2000, 5000, 10000 };

	std::cout << "N\tRB-Tree(ms)\tBST(ms)\t\tRatio" << std::endl;
	std::cout << "---------------------------------------------------" << std::endl;

	for (int N : sizes)
	{
		// RB-Tree
		auto start = std::chrono::high_resolution_clock::now();
		STDev::map<int, int, STDev::TreeType::RedBlackTree> rb;
		for (int i = 0; i < N; i++)
			rb.insert(i, i);
		// Cerca l'ultimo elemento (worst case per BST)
		for (int i = 0; i < 1000; i++)
			rb.find(N - 1);
		auto end = std::chrono::high_resolution_clock::now();
		auto rb_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

		// BST
		start = std::chrono::high_resolution_clock::now();
		STDev::map<int, int, STDev::TreeType::BinarySearchTree> bst;
		for (int i = 0; i < N; i++)
			bst.insert(i, i);
		// Cerca l'ultimo elemento (deve attraversare tutta la lista!)
		for (int i = 0; i < 1000; i++)
			bst.find(N - 1);
		end = std::chrono::high_resolution_clock::now();
		auto bst_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

		double ratio = (double)bst_time / (rb_time > 0 ? rb_time : 1);

		std::cout << N << "\t" << rb_time << "\t\t" << bst_time << "\t\t"
			<< std::fixed << std::setprecision(1) << ratio << "x" << std::endl;
	}

	std::cout << "\nNotare come il BST peggiora LINEARMENTE (O(n))," << std::endl;
	std::cout << "mentre RB-Tree rimane LOGARITMICO (O(log n))!" << std::endl;
}

void benchmark_delete()
{
	section_header("BENCHMARK 4: DELETE PERFORMANCE");

	const int N = 5000;
	std::vector<int> values;
	for (int i = 0; i < N; i++)
		values.push_back(i);

	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(values.begin(), values.end(), g);

	// Build trees
	STDev::map<int, int, STDev::TreeType::RedBlackTree> rb;
	STDev::map<int, int, STDev::TreeType::BinarySearchTree> bst;

	for (int v : values)
	{
		rb.insert(v, v);
		bst.insert(v, v);
	}

	// Shuffle for random deletion order
	std::shuffle(values.begin(), values.end(), g);

	// Benchmark RB-Tree delete
	auto start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N / 2; i++)
		rb.erase(values[i]);
	auto end = std::chrono::high_resolution_clock::now();
	auto duration_rb = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	// Benchmark BST delete
	start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N / 2; i++)
		bst.erase(values[i]);
	end = std::chrono::high_resolution_clock::now();
	auto duration_bst = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	std::cout << "\nDeleting " << (N / 2) << " random elements from " << N << " elements:" << std::endl;
	std::cout << "  RB-Tree:  " << duration_rb.count() << " ms" << std::endl;
	std::cout << "  BST:      " << duration_bst.count() << " ms" << std::endl;

	// Verify RB-Tree is still valid
	if (rb.is_valid_rb_tree())
	{
		std::cout << "\n[OK] RB-Tree properties maintained after deletions" << std::endl;
	}
}

// ==================== MAIN ====================

int main()
{
	std::cout << "\n";
	std::cout << "========================================" << std::endl;
	std::cout << "   MAP DATA STRUCTURE TEST SUITE" << std::endl;
	std::cout << "========================================" << std::endl;

	// Run all tests
	test_basic_operations();
	test_operator_bracket();
	test_copy_and_assignment();
	test_move_semantics();
	test_iterators();
	test_stress_sequential();
	test_stress_random();
	test_edge_cases();
	test_different_types();
	test_bst_vs_rbtree();

	// Print test summary
	g_stats.print_summary();

	// Run benchmarks
	std::cout << "\n";
	std::cout << "========================================" << std::endl;
	std::cout << "         PERFORMANCE BENCHMARKS" << std::endl;
	std::cout << "========================================" << std::endl;

	benchmark_insert();
	benchmark_search();
	benchmark_worst_case();
	benchmark_delete();
	benchmark_comparison_visual();

	std::cout << "\n";
	std::cout << "========================================" << std::endl;
	std::cout << "           ALL TESTS COMPLETE" << std::endl;
	std::cout << "========================================" << std::endl;
	std::cout << "\n";

	return 0;
}