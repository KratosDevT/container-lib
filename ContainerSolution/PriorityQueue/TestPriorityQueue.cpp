#include "priority_queue.h"
#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <functional>

using namespace STDev;

//==================== UTILITIES ====================

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
	section_header("TEST 1: BASIC OPERATIONS (MAX-HEAP)");

	priority_queue<int> pq;

	// Test empty
	test_assert(pq.empty(), "New priority_queue is empty");
	test_assert(pq.size() == 0, "New priority_queue size is 0");

	// Test push
	pq.push(5);
	test_assert(!pq.empty(), "Queue not empty after push");
	test_assert(pq.size() == 1, "Size is 1 after push");
	test_assert(pq.top() == 5, "Top is correct after single push");

	// Test max-heap property
	pq.push(3);
	pq.push(8);
	pq.push(1);
	pq.push(10);
	pq.push(7);

	test_assert(pq.size() == 6, "Size is 6 after multiple pushes");
	test_assert(pq.top() == 10, "Top is maximum element (10)");
	test_assert(pq.is_valid_heap(), "Heap property is valid");

	// Test pop
	pq.pop();
	test_assert(pq.top() == 8, "After pop, top is second largest (8)");
	test_assert(pq.size() == 5, "Size decreased after pop");
	test_assert(pq.is_valid_heap(), "Heap property valid after pop");

	pq.pop();
	test_assert(pq.top() == 7, "After second pop, top is 7");

	// Pop all elements
	pq.pop(); // 5
	pq.pop(); // 3
	pq.pop(); // 1
	test_assert(pq.empty(), "Queue is empty after popping all");
}

void test_min_heap()
{
	section_header("TEST 2: MIN-HEAP (with greater<int>)");

	// MIN-HEAP: usa std::greater come comparator
	priority_queue<int, std::vector<int>, std::greater<int>> min_pq;

	min_pq.push(5);
	min_pq.push(3);
	min_pq.push(8);
	min_pq.push(1);
	min_pq.push(10);

	test_assert(min_pq.top() == 1, "Min-heap: top is minimum element (1)");
	test_assert(min_pq.is_valid_heap(), "Min-heap: heap property is valid");

	min_pq.pop();
	test_assert(min_pq.top() == 3, "Min-heap: after pop, top is 3");

	min_pq.pop();
	test_assert(min_pq.top() == 5, "Min-heap: after second pop, top is 5");
}

void test_constructor_from_range()
{
	section_header("TEST 3: CONSTRUCTOR FROM RANGE");

	std::vector<int> vec = { 4, 2, 9, 1, 7, 5, 3 };

	// Construct priority_queue da iterator range
	priority_queue<int> pq(vec.begin(), vec.end());

	test_assert(pq.size() == 7, "Size matches input vector");
	test_assert(pq.top() == 9, "Top is maximum from input (9)");
	test_assert(pq.is_valid_heap(), "Heap constructed from range is valid");

	// Verifica che tutti gli elementi siano presenti (ordinamento decrescente)
	std::vector<int> extracted;
	while (!pq.empty())
	{
		extracted.push_back(pq.top());
		pq.pop();
	}

	bool correct_order = true;
	for (size_t i = 1; i < extracted.size(); ++i)
	{
		if (extracted[i - 1] < extracted[i])
		{
			correct_order = false;
			break;
		}
	}
	test_assert(correct_order, "Elements extracted in descending order");
}

void test_initializer_list()
{
	section_header("TEST 4: INITIALIZER LIST CONSTRUCTOR");

	priority_queue<int> pq = { 4, 2, 9, 1, 7, 5, 3 };

	test_assert(pq.size() == 7, "Size matches initializer list");
	test_assert(pq.top() == 9, "Top is maximum (9)");
	test_assert(pq.is_valid_heap(), "Heap from initializer_list is valid");
}

void test_copy_and_move()
{
	section_header("TEST 5: COPY AND MOVE SEMANTICS");

	priority_queue<int> pq1;
	pq1.push(5);
	pq1.push(3);
	pq1.push(8);
	pq1.push(1);

	// Copy constructor
	priority_queue<int> pq2(pq1);
	test_assert(pq2.size() == 4, "Copy constructor: size matches");
	test_assert(pq2.top() == 8, "Copy constructor: top matches");
	test_assert(pq1.size() == 4, "Copy constructor: original unchanged");

	// Modify copy
	pq2.push(10);
	test_assert(pq2.top() == 10, "Copy modified independently");
	test_assert(pq1.top() == 8, "Original still has old top");

	// Copy assignment
	priority_queue<int> pq3;
	pq3.push(99);
	pq3 = pq1;
	test_assert(pq3.size() == 4, "Copy assignment: size matches");
	test_assert(pq3.top() == 8, "Copy assignment: top matches");

	// Move constructor
	priority_queue<int> pq4(std::move(pq1));
	test_assert(pq4.size() == 4, "Move constructor: destination has data");
	test_assert(pq4.top() == 8, "Move constructor: top correct");
	test_assert(pq1.size() == 0, "Move constructor: source is empty");

	// Move assignment
	priority_queue<int> pq5;
	pq5.push(99);
	pq5 = std::move(pq2);
	test_assert(pq5.size() == 5, "Move assignment: destination has data");
	test_assert(pq5.top() == 10, "Move assignment: top correct");
	test_assert(pq2.size() == 0, "Move assignment: source is empty");
}

void test_emplace()
{
	section_header("TEST 6: EMPLACE");

	struct Point
	{
		int x, y;
		Point(int x_, int y_) : x(x_), y(y_) {}
		bool operator<(const Point& other) const { return x < other.x; }
		bool operator>(const Point& other) const { return x > other.x; }
	};

	priority_queue<Point> pq;

	// Emplace costruisce in-place senza temporanei
	pq.emplace(5, 10);
	pq.emplace(3, 20);
	pq.emplace(8, 30);

	test_assert(pq.size() == 3, "Emplace: size correct");
	test_assert(pq.top().x == 8, "Emplace: top has max x value");
}

void test_custom_comparator()
{
	section_header("TEST 7: CUSTOM COMPARATOR");

	// Lambda comparator per ordinare stringhe per lunghezza
	auto comp = [](const std::string& a, const std::string& b)
		{
			return a.length() < b.length(); // Max-heap basato su lunghezza
		};

	priority_queue<std::string, std::vector<std::string>, decltype(comp)> pq(comp);

	pq.push("hi");
	pq.push("hello");
	pq.push("world");
	pq.push("c++");
	pq.push("priority");

	test_assert(pq.top() == "priority", "Custom comparator: longest string on top");
	test_assert(pq.top().length() == 8, "Custom comparator: length is 8");

	pq.pop();
	test_assert(pq.top() == "hello" || pq.top() == "world", "After pop: 5-char string on top");
}

void test_edge_cases()
{
	section_header("TEST 8: EDGE CASES");

	priority_queue<int> pq;

	// Single element
	pq.push(42);
	test_assert(pq.top() == 42, "Single element: top is correct");
	test_assert(pq.size() == 1, "Single element: size is 1");
	test_assert(pq.is_valid_heap(), "Single element: heap is valid");

	pq.pop();
	test_assert(pq.empty(), "After pop single element: queue is empty");

	// Duplicate elements
	priority_queue<int> pq2;
	pq2.push(5);
	pq2.push(5);
	pq2.push(5);
	test_assert(pq2.size() == 3, "Duplicates: all inserted");
	test_assert(pq2.top() == 5, "Duplicates: top is correct");

	// Exception test - top on empty
	priority_queue<int> empty_pq;
	bool exception_thrown = false;
	try
	{
		empty_pq.top();
	}
	catch (const std::out_of_range&)
	{
		exception_thrown = true;
	}
	test_assert(exception_thrown, "top() throws on empty queue");

	// Exception test - pop on empty
	exception_thrown = false;
	try
	{
		empty_pq.pop();
	}
	catch (const std::out_of_range&)
	{
		exception_thrown = true;
	}
	test_assert(exception_thrown, "pop() throws on empty queue");
}

void test_stress()
{
	section_header("TEST 9: STRESS TEST");

	priority_queue<int> pq;
	const int N = 1000;

	// Insert N elements
	for (int i = 0; i < N; ++i)
	{
		pq.push(i);
	}

	test_assert(pq.size() == N, "Stress: all elements inserted");
	test_assert(pq.top() == N - 1, "Stress: top is maximum");
	test_assert(pq.is_valid_heap(), "Stress: heap is valid after insertions");

	// Extract all in descending order
	bool correct_order = true;
	int prev = N;
	while (!pq.empty())
	{
		int current = pq.top();
		if (current >= prev)
		{
			correct_order = false;
			break;
		}
		prev = current;
		pq.pop();
	}

	test_assert(correct_order, "Stress: all elements extracted in correct order");
	test_assert(pq.empty(), "Stress: queue is empty after extracting all");
}

void test_heap_sort_simulation()
{
	section_header("TEST 10: HEAP SORT SIMULATION");

	std::vector<int> unsorted = { 5, 2, 8, 1, 9, 3, 7, 4, 6 };
	priority_queue<int> pq(unsorted.begin(), unsorted.end());

	std::vector<int> sorted;
	while (!pq.empty())
	{
		sorted.push_back(pq.top());
		pq.pop();
	}

	// Verify descending order (max-heap extracts in descending order)
	bool is_sorted = true;
	for (size_t i = 1; i < sorted.size(); ++i)
	{
		if (sorted[i - 1] < sorted[i])
		{
			is_sorted = false;
			break;
		}
	}

	test_assert(is_sorted, "Heap sort: elements in descending order");
	test_assert(sorted.size() == unsorted.size(), "Heap sort: all elements present");
}

void test_clear_and_swap()
{
	section_header("TEST 11: CLEAR AND SWAP");

	priority_queue<int> pq1;
	pq1.push(5);
	pq1.push(3);
	pq1.push(8);

	// Clear
	pq1.clear();
	test_assert(pq1.empty(), "Clear: queue is empty");
	test_assert(pq1.size() == 0, "Clear: size is 0");

	// Refill
	pq1.push(10);
	pq1.push(20);

	priority_queue<int> pq2;
	pq2.push(100);
	pq2.push(200);
	pq2.push(300);

	// Swap
	pq1.swap(pq2);
	test_assert(pq1.size() == 3, "Swap: pq1 has pq2's size");
	test_assert(pq1.top() == 300, "Swap: pq1 has pq2's top");
	test_assert(pq2.size() == 2, "Swap: pq2 has pq1's size");
	test_assert(pq2.top() == 20, "Swap: pq2 has pq1's top");
}

void test_kth_largest()
{
	section_header("TEST 12: PRACTICAL USE CASE - K-th Largest Element");

	// Use case: trovare i K elementi più grandi
	std::vector<int> data = { 3, 2, 1, 5, 6, 4, 7, 8, 9 };
	int k = 3;

	// Min-heap di size K per tenere i K elementi più grandi
	priority_queue<int, std::vector<int>, std::greater<int>> min_heap;

	for (int num : data)
	{
		min_heap.push(num);
		if (min_heap.size() > static_cast<size_t>(k))
		{
			min_heap.pop();
		}
	}

	// Il top del min-heap è il K-esimo elemento più grande
	test_assert(min_heap.size() == k, "K-th largest: heap has K elements");
	test_assert(min_heap.top() == 7, "K-th largest: 3rd largest is 7");

	std::cout << "K-th largest use case: Finding 3rd largest in array" << std::endl;
	std::cout << "Result: " << min_heap.top() << " (correct: 7, 8, 9 are the 3 largest)" << std::endl;
}

void test_merge_k_sorted_arrays()
{
	section_header("TEST 13: PRACTICAL USE CASE - Merge K Sorted Arrays");

	// Use case: merge multiple sorted arrays
	std::vector<std::vector<int>> arrays = {
		{1, 4, 7},
		{2, 5, 8},
		{3, 6, 9}
	};

	struct Element
	{
		int value;
		int array_idx;
		int elem_idx;

		bool operator>(const Element& other) const
		{
			return value > other.value; // Min-heap per value
		}
	};

	priority_queue<Element, std::vector<Element>, std::greater<Element>> min_heap;

	// Insert first element of each array
	for (size_t i = 0; i < arrays.size(); ++i)
	{
		if (!arrays[i].empty())
		{
			min_heap.emplace(Element{ arrays[i][0], static_cast<int>(i), 0 });
		}
	}

	std::vector<int> merged;
	while (!min_heap.empty())
	{
		Element top = min_heap.top();
		min_heap.pop();
		merged.push_back(top.value);

		// Se c'è un altro elemento nello stesso array, aggiungilo
		int next_idx = top.elem_idx + 1;
		if (next_idx < static_cast<int>(arrays[top.array_idx].size()))
		{
			min_heap.emplace(Element{ arrays[top.array_idx][next_idx], top.array_idx, next_idx });
		}
	}

	test_assert(merged.size() == 9, "Merge K arrays: all elements merged");

	bool is_sorted = true;
	for (size_t i = 1; i < merged.size(); ++i)
	{
		if (merged[i - 1] > merged[i])
		{
			is_sorted = false;
			break;
		}
	}
	test_assert(is_sorted, "Merge K arrays: result is sorted");

	std::cout << "Merged result: ";
	for (int x : merged) std::cout << x << " ";
	std::cout << std::endl;
}

void test_visual_demonstration()
{
	section_header("VISUAL DEMONSTRATION");

	std::cout << "\n=== MAX-HEAP EXAMPLE ===" << std::endl;
	priority_queue<int> max_pq;

	std::vector<int> values = { 5, 3, 8, 1, 9, 2, 7 };
	std::cout << "Inserting: ";
	for (int val : values)
	{
		std::cout << val << " ";
		max_pq.push(val);
	}
	std::cout << std::endl;

	max_pq.print_heap();
	max_pq.print_tree();

	std::cout << "\n=== MIN-HEAP EXAMPLE ===" << std::endl;
	priority_queue<int, std::vector<int>, std::greater<int>> min_pq;

	for (int val : values)
	{
		min_pq.push(val);
	}

	min_pq.print_heap();
	min_pq.print_tree();

	std::cout << "\n=== EXTRACTING ELEMENTS ===" << std::endl;
	std::cout << "Max-heap extraction (descending): ";
	while (!max_pq.empty())
	{
		std::cout << max_pq.top() << " ";
		max_pq.pop();
	}
	std::cout << std::endl;

	std::cout << "Min-heap extraction (ascending): ";
	while (!min_pq.empty())
	{
		std::cout << min_pq.top() << " ";
		min_pq.pop();
	}
	std::cout << std::endl;
}

// ==================== MAIN ====================

int main3()
{
	std::cout << "\n";
	std::cout << "========================================" << std::endl;
	std::cout << "  PRIORITY_QUEUE TEST SUITE" << std::endl;
	std::cout << "========================================" << std::endl;

	// Run all tests
	test_basic_operations();
	test_min_heap();
	test_constructor_from_range();
	test_initializer_list();
	test_copy_and_move();
	test_emplace();
	test_custom_comparator();
	test_edge_cases();
	test_stress();
	test_heap_sort_simulation();
	test_clear_and_swap();
	test_kth_largest();
	test_merge_k_sorted_arrays();

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