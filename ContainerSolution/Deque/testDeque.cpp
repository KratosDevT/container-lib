#include "deque.h"
#include <iostream>
#include <cassert>
#include <algorithm>
#include <numeric>

using namespace STDev;

// ============ TEST ITERATOR BASIC ============

void test_iterator_begin_end()
{
	std::cout << "Test: begin() e end()... ";
	deque<int> d;

	for (int i = 0; i < 10; i++)
	{
		d.push_back(i);
	}

	auto it = d.begin();
	assert(*it == 0);

	auto end_it = d.end();
	--end_it;
	assert(*end_it == 9);

	std::cout << "OK\n";
}

void test_iterator_increment()
{
	std::cout << "Test: operator++... ";
	deque<int> d;

	d.push_back(10);
	d.push_back(20);
	d.push_back(30);

	auto it = d.begin();
	assert(*it == 10);

	++it;
	assert(*it == 20);

	it++;
	assert(*it == 30);

	++it;
	assert(it == d.end());

	std::cout << "OK\n";
}

void test_iterator_decrement()
{
	std::cout << "Test: operator--... ";
	deque<int> d;

	d.push_back(10);
	d.push_back(20);
	d.push_back(30);

	auto it = d.end();
	--it;
	assert(*it == 30);

	it--;
	assert(*it == 20);

	--it;
	assert(*it == 10);
	assert(it == d.begin());

	std::cout << "OK\n";
}

void test_iterator_dereference()
{
	std::cout << "Test: operator* e operator->... ";
	deque<int> d;

	d.push_back(100);
	d.push_back(200);

	auto it = d.begin();
	assert(*it == 100);

	*it = 999;
	assert(d[0] == 999);

	std::cout << "OK\n";
}

void test_iterator_comparison()
{
	std::cout << "Test: operatori di confronto... ";
	deque<int> d;

	for (int i = 0; i < 5; i++)
	{
		d.push_back(i);
	}

	auto it1 = d.begin();
	auto it2 = d.begin();
	auto it3 = d.end();

	assert(it1 == it2);
	assert(it1 != it3);
	assert(it1 < it3);
	assert(it3 > it1);
	assert(it1 <= it2);
	assert(it1 >= it2);

	std::cout << "OK\n";
}

// ============ TEST RANDOM ACCESS ============

void test_iterator_arithmetic()
{
	std::cout << "Test: aritmetica iteratori... ";
	deque<int> d;

	for (int i = 0; i < 10; i++)
	{
		d.push_back(i * 10);
	}

	auto it = d.begin();

	it += 3;
	assert(*it == 30);

	it -= 1;
	assert(*it == 20);

	auto it2 = it + 5;
	assert(*it2 == 70);

	auto it3 = it2 - 2;
	assert(*it3 == 50);

	std::cout << "OK\n";
}

void test_iterator_subscript()
{
	std::cout << "Test: operator[]... ";
	deque<int> d;

	for (int i = 0; i < 10; i++)
	{
		d.push_back(i);
	}

	auto it = d.begin();

	assert(it[0] == 0);
	assert(it[5] == 5);
	assert(it[9] == 9);

	it += 3;
	assert(it[0] == 3);
	assert(it[2] == 5);

	std::cout << "OK\n";
}

void test_iterator_distance()
{
	std::cout << "Test: distanza tra iteratori... ";
	deque<int> d;

	for (int i = 0; i < 10; i++)
	{
		d.push_back(i);
	}

	auto it1 = d.begin();
	auto it2 = d.begin() + 5;

	assert(it2 - it1 == 5);
	assert(it1 - it2 == -5);

	auto it3 = d.end();
	assert(it3 - it1 == 10);

	std::cout << "OK\n";
}

// ============ TEST RANGE-BASED FOR ============

void test_range_based_for()
{
	std::cout << "Test: range-based for loop... ";
	deque<int> d;

	for (int i = 0; i < 5; i++)
	{
		d.push_back(i);
	}

	int sum = 0;
	for (const auto& val : d)
	{
		sum += val;
	}

	assert(sum == 10);

	// Modifica tramite reference
	for (auto& val : d)
	{
		val *= 2;
	}

	assert(d[0] == 0);
	assert(d[1] == 2);
	assert(d[2] == 4);
	assert(d[3] == 6);
	assert(d[4] == 8);

	std::cout << "OK\n";
}

// ============ TEST CONST_ITERATOR ============

void test_const_iterator()
{
	std::cout << "Test: const_iterator... ";
	deque<int> d;

	for (int i = 0; i < 5; i++)
	{
		d.push_back(i * 10);
	}

	const deque<int>& const_d = d;

	auto it = const_d.begin();
	assert(*it == 0);

	++it;
	assert(*it == 10);

	it += 3;
	assert(*it == 40);

	std::cout << "OK\n";
}

void test_cbegin_cend()
{
	std::cout << "Test: cbegin() e cend()... ";
	deque<int> d;

	for (int i = 0; i < 5; i++)
	{
		d.push_back(i);
	}

	int count = 0;
	for (auto it = d.cbegin(); it != d.cend(); ++it)
	{
		count++;
	}

	assert(count == 5);

	std::cout << "OK\n";
}

// ============ TEST STL ALGORITHMS ============

void test_stl_find()
{
	std::cout << "Test: std::find... ";
	deque<int> d;

	for (int i = 0; i < 10; i++)
	{
		d.push_back(i);
	}

	auto it = std::find(d.begin(), d.end(), 5);
	assert(it != d.end());
	assert(*it == 5);

	auto it2 = std::find(d.begin(), d.end(), 99);
	assert(it2 == d.end());

	std::cout << "OK\n";
}

void test_stl_count()
{
	std::cout << "Test: std::count... ";
	deque<int> d;

	d.push_back(1);
	d.push_back(2);
	d.push_back(1);
	d.push_back(3);
	d.push_back(1);

	int count = std::count(d.begin(), d.end(), 1);
	assert(count == 3);

	std::cout << "OK\n";
}

void test_stl_accumulate()
{
	std::cout << "Test: std::accumulate... ";
	deque<int> d;

	for (int i = 1; i <= 10; i++)
	{
		d.push_back(i);
	}

	int sum = std::accumulate(d.begin(), d.end(), 0);
	assert(sum == 55);

	std::cout << "OK\n";
}

void test_stl_sort()
{
	std::cout << "Test: std::sort... ";
	deque<int> d;

	d.push_back(5);
	d.push_back(2);
	d.push_back(8);
	d.push_back(1);
	d.push_back(9);
	d.push_back(3);

	std::sort(d.begin(), d.end());

	for (size_t i = 0; i < d.size() - 1; i++)
	{
		assert(d[i] <= d[i + 1]);
	}

	assert(d[0] == 1);
	assert(d[5] == 9);

	std::cout << "OK\n";
}

void test_stl_reverse()
{
	std::cout << "Test: std::reverse... ";
	deque<int> d;

	for (int i = 0; i < 5; i++)
	{
		d.push_back(i);
	}

	std::reverse(d.begin(), d.end());

	assert(d[0] == 4);
	assert(d[1] == 3);
	assert(d[2] == 2);
	assert(d[3] == 1);
	assert(d[4] == 0);

	std::cout << "OK\n";
}

void test_stl_copy()
{
	std::cout << "Test: std::copy... ";
	deque<int> d1;
	deque<int> d2;

	for (int i = 0; i < 5; i++)
	{
		d1.push_back(i);
		d2.push_back(0);
	}

	std::copy(d1.begin(), d1.end(), d2.begin());

	for (size_t i = 0; i < d1.size(); i++)
	{
		assert(d1[i] == d2[i]);
	}

	std::cout << "OK\n";
}

void test_stl_fill()
{
	std::cout << "Test: std::fill... ";
	deque<int> d;

	for (int i = 0; i < 10; i++)
	{
		d.push_back(0);
	}

	std::fill(d.begin(), d.end(), 42);

	for (size_t i = 0; i < d.size(); i++)
	{
		assert(d[i] == 42);
	}

	std::cout << "OK\n";
}

// ============ TEST EDGE CASES ============

void test_iterator_empty_deque()
{
	std::cout << "Test: iteratore su deque vuota... ";
	deque<int> d;

	assert(d.begin() == d.end());

	int count = 0;
	for (auto it = d.begin(); it != d.end(); ++it)
	{
		count++;
	}
	assert(count == 0);

	std::cout << "OK\n";
}

void test_iterator_single_element()
{
	std::cout << "Test: iteratore con singolo elemento... ";
	deque<int> d;

	d.push_back(42);

	auto it = d.begin();
	assert(*it == 42);

	++it;
	assert(it == d.end());

	std::cout << "OK\n";
}

void test_iterator_across_blocks()
{
	std::cout << "Test: iteratore attraverso blocchi multipli... ";
	deque<int> d;

	// Inserisci 25 elementi (più di 3 blocchi da 8)
	for (int i = 0; i < 25; i++)
	{
		d.push_back(i);
	}

	// Verifica che l'iterazione funzioni attraverso i blocchi
	int expected = 0;
	for (auto it = d.begin(); it != d.end(); ++it)
	{
		assert(*it == expected);
		expected++;
	}
	assert(expected == 25);

	std::cout << "OK\n";
}

void test_iterator_after_push_front()
{
	std::cout << "Test: iteratore dopo push_front... ";
	deque<int> d;

	d.push_back(5);
	d.push_back(6);
	d.push_back(7);
	d.push_front(4);
	d.push_front(3);
	d.push_front(2);
	d.push_front(1);

	int expected = 1;
	for (auto it = d.begin(); it != d.end(); ++it)
	{
		assert(*it == expected);
		expected++;
	}

	std::cout << "OK\n";
}

void test_iterator_modification()
{
	std::cout << "Test: modifica tramite iteratore... ";
	deque<int> d;

	for (int i = 0; i < 10; i++)
	{
		d.push_back(i);
	}

	// Moltiplica ogni elemento per 2
	for (auto it = d.begin(); it != d.end(); ++it)
	{
		*it *= 2;
	}

	for (size_t i = 0; i < d.size(); i++)
	{
		assert(d[i] == static_cast<int>(i) * 2);
	}

	std::cout << "OK\n";
}

// ============ TEST STRESS ============
void test_stress_iteration()
{
	std::cout << "Test: stress iterazione... ";
	deque<int> d;

	// Inserisci 1000 elementi
	for (int i = 0; i < 1000; i++)
	{
		d.push_back(i);
	}

	// Verifica iterazione completa
	int count = 0;
	for (auto it = d.begin(); it != d.end(); ++it)
	{
		assert(*it == count);
		count++;
	}
	assert(count == 1000);

	// Verifica iterazione inversa
	count = 999;
	for (auto it = d.end(); it != d.begin(); )
	{
		--it;
		assert(*it == count);
		count--;
	}

	std::cout << "OK\n";
}

void test_stress_stl_algorithms()
{
	std::cout << "Test: stress algoritmi STL... ";
	deque<int> d;

	// Crea deque con 500 elementi
	for (int i = 0; i < 500; i++)
	{
		d.push_back(i % 100);
	}

	// Sort
	std::sort(d.begin(), d.end());

	// Verifica ordinamento
	for (size_t i = 0; i < d.size() - 1; i++)
	{
		assert(d[i] <= d[i + 1]);
	}

	// Count
	int count = std::count(d.begin(), d.end(), 50);
	assert(count == 5);

	// Find
	auto it = std::find(d.begin(), d.end(), 99);
	assert(it != d.end());

	std::cout << "OK\n";
}

// ============ TEST COMPARISON WITH VECTOR ============
void test_iterator_compatibility()
{
	std::cout << "Test: compatibilità con algoritmi STL... ";
	deque<int> d;

	for (int i = 1; i <= 10; i++)
	{
		d.push_back(i);
	}

	// Test min_element
	auto min_it = std::min_element(d.begin(), d.end());
	assert(*min_it == 1);

	// Test max_element
	auto max_it = std::max_element(d.begin(), d.end());
	assert(*max_it == 10);

	// Test binary_search (dopo sort)
	std::sort(d.begin(), d.end());
	bool found = std::binary_search(d.begin(), d.end(), 5);
	assert(found);

	// Test lower_bound
	auto lower = std::lower_bound(d.begin(), d.end(), 5);
	assert(*lower == 5);

	// Test upper_bound
	auto upper = std::upper_bound(d.begin(), d.end(), 5);
	assert(*upper == 6);

	std::cout << "OK\n";
}

// ============ TEST VISUAL DEMONSTRATION ============

void test_visual_demonstration()
{
	std::cout << "\n=== DIMOSTRAZIONE ITERATORI ===" << std::endl;

	deque<int> d;

	std::cout << "\nCreazione deque con elementi 0-9:" << std::endl;
	for (int i = 0; i < 10; i++)
	{
		d.push_back(i);
	}

	std::cout << "Contenuto: ";
	for (auto it = d.begin(); it != d.end(); ++it)
	{
		std::cout << *it << " ";
	}
	std::cout << "\n" << std::endl;

	std::cout << "Iterazione con range-based for:" << std::endl;
	std::cout << "Contenuto: ";
	for (const auto& val : d)
	{
		std::cout << val << " ";
	}
	std::cout << "\n" << std::endl;

	std::cout << "Modifica: moltiplica per 2 ogni elemento" << std::endl;
	for (auto& val : d)
	{
		val *= 2;
	}

	std::cout << "Contenuto: ";
	for (auto it = d.begin(); it != d.end(); ++it)
	{
		std::cout << *it << " ";
	}
	std::cout << "\n" << std::endl;

	std::cout << "Random access con operator[]:" << std::endl;
	auto it = d.begin();
	std::cout << "it[0] = " << it[0] << std::endl;
	std::cout << "it[5] = " << it[5] << std::endl;
	std::cout << "it[9] = " << it[9] << std::endl;
	std::cout << std::endl;

	std::cout << "Aritmetica degli iteratori:" << std::endl;
	auto it1 = d.begin();
	auto it2 = it1 + 5;
	std::cout << "it2 = it1 + 5, *it2 = " << *it2 << std::endl;
	std::cout << "distanza (it2 - it1) = " << (it2 - it1) << std::endl;
	std::cout << std::endl;

	std::cout << "Uso con std::sort:" << std::endl;
	std::cout << "Prima:  ";
	for (const auto& val : d)
	{
		std::cout << val << " ";
	}
	std::cout << std::endl;

	std::reverse(d.begin(), d.end());

	std::cout << "Dopo reverse: ";
	for (const auto& val : d)
	{
		std::cout << val << " ";
	}
	std::cout << "\n" << std::endl;

	std::cout << "Uso con std::accumulate:" << std::endl;
	int sum = std::accumulate(d.begin(), d.end(), 0);
	std::cout << "Somma totale: " << sum << std::endl;
	std::cout << std::endl;

	d.print_structure();
}

// ============ MAIN ============

int main()
{
	std::cout << "\n========================================\n";
	std::cout << "TEST SUITE DEQUE ITERATORS\n";
	std::cout << "========================================\n\n";

	std::cout << "--- TEST ITERATOR BASIC ---\n";
	test_iterator_begin_end();
	test_iterator_increment();
	test_iterator_decrement();
	test_iterator_dereference();
	test_iterator_comparison();

	std::cout << "\n--- TEST RANDOM ACCESS ---\n";
	test_iterator_arithmetic();
	test_iterator_subscript();
	test_iterator_distance();

	std::cout << "\n--- TEST RANGE-BASED FOR ---\n";
	test_range_based_for();

	std::cout << "\n--- TEST CONST_ITERATOR ---\n";
	test_const_iterator();
	test_cbegin_cend();

	std::cout << "\n--- TEST STL ALGORITHMS ---\n";
	test_stl_find();
	test_stl_count();
	test_stl_accumulate();
	test_stl_sort();
	test_stl_reverse();
	test_stl_copy();
	test_stl_fill();

	std::cout << "\n--- TEST EDGE CASES ---\n";
	test_iterator_empty_deque();
	test_iterator_single_element();
	test_iterator_across_blocks();
	test_iterator_after_push_front();
	test_iterator_modification();

	std::cout << "\n--- TEST STRESS ---\n";
	test_stress_iteration();
	test_stress_stl_algorithms();

	std::cout << "\n--- TEST COMPATIBILITY ---\n";
	test_iterator_compatibility();

	std::cout << "\n========================================\n";
	std::cout << "TUTTI I TEST SONO PASSATI!\n";
	std::cout << "========================================\n";

	test_visual_demonstration();

	return 0;
}