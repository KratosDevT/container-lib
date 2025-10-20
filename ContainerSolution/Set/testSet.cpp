#include "set.h"
#include <iostream>
#include <cassert>
#include <utility>
#include <vector>
#include <algorithm>

using namespace STDev;

// ==================== TEST FUNCTIONS ====================

void test_basic_insert()
{
	std::cout << "Test: inserimento base... ";
	set<int> s;

	assert(s.empty());
	assert(s.size() == 0);

	bool inserted = s.insert(5);
	assert(inserted);
	assert(s.size() == 1);
	assert(s.contains(5));

	inserted = s.insert(3);
	assert(inserted);
	assert(s.size() == 2);

	inserted = s.insert(7);
	assert(inserted);
	assert(s.size() == 3);

	// Tentativo di inserire duplicato
	inserted = s.insert(5);
	assert(!inserted);
	assert(s.size() == 3);

	std::cout << "OK\n";
}

void test_find_and_contains()
{
	std::cout << "Test: find e contains... ";
	set<int> s;

	s.insert(10);
	s.insert(20);
	s.insert(30);

	assert(s.find(10));
	assert(s.find(20));
	assert(s.find(30));
	assert(!s.find(15));
	assert(!s.find(100));

	assert(s.contains(20));
	assert(!s.contains(25));

	std::cout << "OK\n";
}

void test_erase()
{
	std::cout << "Test: erase... ";
	set<int> s;

	s.insert(1);
	s.insert(2);
	s.insert(3);
	s.insert(4);
	s.insert(5);

	assert(s.size() == 5);

	bool erased = s.erase(3);
	assert(erased);
	assert(s.size() == 4);
	assert(!s.contains(3));

	erased = s.erase(3);
	assert(!erased);
	assert(s.size() == 4);

	erased = s.erase(1);
	assert(erased);
	assert(s.size() == 3);

	erased = s.erase(5);
	assert(erased);
	assert(s.size() == 2);

	assert(s.contains(2));
	assert(s.contains(4));

	std::cout << "OK\n";
}

void test_clear()
{
	std::cout << "Test: clear... ";
	set<int> s;

	s.insert(1);
	s.insert(2);
	s.insert(3);
	s.insert(4);
	s.insert(5);

	assert(s.size() == 5);

	s.clear();

	assert(s.empty());
	assert(s.size() == 0);
	assert(!s.contains(1));
	assert(!s.contains(3));

	// Reinserimento dopo clear
	s.insert(10);
	assert(s.size() == 1);
	assert(s.contains(10));

	std::cout << "OK\n";
}

void test_copy_constructor()
{
	std::cout << "Test: copy constructor... ";
	set<int> s1;

	s1.insert(1);
	s1.insert(2);
	s1.insert(3);

	set<int> s2(s1);

	assert(s2.size() == 3);
	assert(s2.contains(1));
	assert(s2.contains(2));
	assert(s2.contains(3));

	// Modifica s1 non influenza s2
	s1.insert(4);
	assert(s1.size() == 4);
	assert(s2.size() == 3);
	assert(!s2.contains(4));

	std::cout << "OK\n";
}

void test_copy_assignment()
{
	std::cout << "Test: copy assignment... ";
	set<int> s1;

	s1.insert(1);
	s1.insert(2);
	s1.insert(3);

	set<int> s2;
	s2.insert(99);

	s2 = s1;

	assert(s2.size() == 3);
	assert(s2.contains(1));
	assert(s2.contains(2));
	assert(s2.contains(3));
	assert(!s2.contains(99));

	std::cout << "OK\n";
}

void test_move_constructor()
{
	std::cout << "Test: move constructor... ";
	set<int> s1;

	s1.insert(1);
	s1.insert(2);
	s1.insert(3);

	set<int> s2(std::move(s1));

	assert(s2.size() == 3);
	assert(s2.contains(1));
	assert(s2.contains(2));
	assert(s2.contains(3));

	assert(s1.empty());
	assert(s1.size() == 0);

	std::cout << "OK\n";
}

void test_move_assignment()
{
	std::cout << "Test: move assignment... ";
	set<int> s1;

	s1.insert(1);
	s1.insert(2);
	s1.insert(3);

	set<int> s2;
	s2.insert(999);

	s2 = std::move(s1);

	assert(s2.size() == 3);
	assert(s2.contains(1));
	assert(s2.contains(2));
	assert(s2.contains(3));

	assert(s1.empty());

	std::cout << "OK\n";
}

void test_iterator()
{
	std::cout << "Test: iterator... ";
	set<int> s;

	s.insert(5);
	s.insert(2);
	s.insert(8);
	s.insert(1);
	s.insert(9);

	// Test in-order traversal
	std::vector<int> values;
	for (auto it = s.begin(); it != s.end(); ++it)
	{
		values.push_back(*it);
	}

	assert(values.size() == 5);

	// Verifica che sia ordinato
	bool is_sorted = true;
	for (size_t i = 1; i < values.size(); i++)
	{
		if (values[i] <= values[i - 1])
		{
			is_sorted = false;
			break;
		}
	}
	assert(is_sorted);

	// Range-based for loop
	int count = 0;
	for (const auto& val : s)
	{
		count++;
		(void)val;
	}
	assert(count == 5);

	std::cout << "OK\n";
}

void test_empty_operations()
{
	std::cout << "Test: operazioni su set vuoto... ";
	set<int> s;

	assert(s.empty());
	assert(!s.find(1));
	assert(!s.erase(1));

	s.clear(); // Non deve crashare
	assert(s.empty());

	std::cout << "OK\n";
}

void test_single_element()
{
	std::cout << "Test: set con singolo elemento... ";
	set<int> s;

	s.insert(42);
	assert(s.size() == 1);
	assert(s.contains(42));

	bool erased = s.erase(42);
	assert(erased);
	assert(s.empty());

	std::cout << "OK\n";
}

void test_stress_sequential()
{
	std::cout << "Test: stress sequenziale... ";
	set<int> s;

	const int N = 1000;

	for (int i = 0; i < N; i++)
	{
		bool inserted = s.insert(i);
		assert(inserted);
	}

	assert(s.size() == N);

	for (int i = 0; i < N; i++)
	{
		assert(s.contains(i));
	}

	std::cout << "OK\n";
}

void test_stress_random()
{
	std::cout << "Test: stress random... ";
	set<int> s;

	std::vector<int> values;
	for (int i = 0; i < 500; i++)
		values.push_back(i);

	std::random_shuffle(values.begin(), values.end());

	for (int v : values)
	{
		s.insert(v);
	}

	assert(s.size() == 500);

	std::random_shuffle(values.begin(), values.end());

	for (int i = 0; i < 250; i++)
	{
		s.erase(values[i]);
	}

	assert(s.size() == 250);

	for (int i = 250; i < 500; i++)
	{
		assert(s.contains(values[i]));
	}

	std::cout << "OK\n";
}

void test_duplicates()
{
	std::cout << "Test: gestione duplicati... ";
	set<int> s;

	s.insert(5);
	s.insert(5);
	s.insert(5);

	assert(s.size() == 1);
	assert(s.contains(5));

	s.insert(3);
	s.insert(7);
	s.insert(3);
	s.insert(7);

	assert(s.size() == 3);

	std::cout << "OK\n";
}

void test_different_types()
{
	std::cout << "Test: tipi diversi... ";

	// Set di stringhe
	set<std::string> string_set;
	string_set.insert("hello");
	string_set.insert("world");
	string_set.insert("cpp");

	assert(string_set.size() == 3);
	assert(string_set.contains("hello"));
	assert(!string_set.contains("java"));

	// Set di double
	set<double> double_set;
	double_set.insert(3.14);
	double_set.insert(2.71);
	double_set.insert(1.41);

	assert(double_set.size() == 3);
	assert(double_set.contains(3.14));

	std::cout << "OK\n";
}

void test_order_verification()
{
	std::cout << "Test: verifica ordinamento... ";
	set<int> s;

	std::vector<int> unsorted = { 50, 30, 70, 20, 40, 60, 80, 10, 25, 35 };

	for (int val : unsorted)
	{
		s.insert(val);
	}

	std::vector<int> sorted_result;
	for (const auto& val : s)
	{
		sorted_result.push_back(val);
	}

	// Verifica che sia ordinato
	for (size_t i = 1; i < sorted_result.size(); i++)
	{
		assert(sorted_result[i] > sorted_result[i - 1]);
	}

	std::cout << "OK\n";
}

void test_visual_demonstration()
{
	std::cout << "\n=== DIMOSTRAZIONE VISIVA ===" << std::endl;

	set<int> s;

	std::cout << "\nInserimento: 5, 3, 7, 1, 9, 4, 6" << std::endl;
	s.insert(5);
	s.insert(3);
	s.insert(7);
	s.insert(1);
	s.insert(9);
	s.insert(4);
	s.insert(6);

	std::cout << "\nStruttura dell'albero:" << std::endl;
	s.print_tree();

	std::cout << "Elementi in ordine: ";
	for (const auto& val : s)
	{
		std::cout << val << " ";
	}
	std::cout << "\n" << std::endl;

	std::cout << "Rimozione di 3 e 7..." << std::endl;
	s.erase(3);
	s.erase(7);

	s.print_tree();

	std::cout << "Elementi rimasti: ";
	for (const auto& val : s)
	{
		std::cout << val << " ";
	}
	std::cout << "\n" << std::endl;
}

// ==================== MAIN ====================

int main()
{
	std::cout << "\n";
	std::cout << "========================================" << std::endl;
	std::cout << "   SET DATA STRUCTURE TEST SUITE" << std::endl;
	std::cout << "========================================" << std::endl;
	std::cout << "\n";

	std::cout << "--- TEST BASE ---\n";
	test_basic_insert();
	test_find_and_contains();
	test_erase();
	test_clear();

	std::cout << "\n--- TEST COPY/MOVE ---\n";
	test_copy_constructor();
	test_copy_assignment();
	test_move_constructor();
	test_move_assignment();

	std::cout << "\n--- TEST ITERATOR ---\n";
	test_iterator();

	std::cout << "\n--- TEST EDGE CASES ---\n";
	test_empty_operations();
	test_single_element();
	test_duplicates();

	std::cout << "\n--- TEST STRESS ---\n";
	test_stress_sequential();
	test_stress_random();

	std::cout << "\n--- TEST VARI ---\n";
	test_different_types();
	test_order_verification();

	std::cout << "\n";
	std::cout << "========================================" << std::endl;
	std::cout << "       TUTTI I TEST SONO PASSATI!" << std::endl;
	std::cout << "========================================" << std::endl;

	test_visual_demonstration();

	return 0;
}