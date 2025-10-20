#include "vector.h"
#include <iostream>
#include <cassert>
#include <utility>

using namespace STDev;

// ============ TEST PUSH/POP ============

void test_push_back_basic()
{
	std::cout << "Test: push_back basico... ";
	vector<int> v;

	v.push_back(10);
	v.push_back(20);
	v.push_back(30);

	assert(v.size() == 3);
	assert(v[0] == 10);
	assert(v[1] == 20);
	assert(v[2] == 30);

	std::cout << "OK\n";
}

void test_push_back_resize()
{
	std::cout << "Test: push_back con resize automatico... ";
	vector<int> v;

	for (int i = 0; i < 20; i++)
	{
		v.push_back(i);
	}

	assert(v.size() == 20);
	for (int i = 0; i < 20; i++)
	{
		assert(v[i] == i);
	}

	std::cout << "OK\n";
}

void test_pop_back()
{
	std::cout << "Test: pop_back()... ";
	vector<int> v;

	v.push_back(1);
	v.push_back(2);
	v.push_back(3);
	v.push_back(4);

	assert(v.size() == 4);

	v.pop_back();
	assert(v.size() == 3);
	assert(v[2] == 3);

	v.pop_back();
	v.pop_back();
	assert(v.size() == 1);
	assert(v[0] == 1);

	std::cout << "OK\n";
}

void test_pop_back_empty()
{
	std::cout << "Test: pop_back su vector vuoto... ";
	vector<int> v;

	bool exception_thrown = false;
	try
	{
		v.pop_back();
	}
	catch (const std::out_of_range&)
	{
		exception_thrown = true;
	}
	assert(exception_thrown);

	std::cout << "OK\n";
}

// ============ TEST INSERT/ERASE ============

void test_insert_beginning()
{
	std::cout << "Test: insert all'inizio... ";
	vector<int> v;

	v.push_back(2);
	v.push_back(3);
	v.push_back(4);

	v.insert(1, 0);

	assert(v.size() == 4);
	assert(v[0] == 1);
	assert(v[1] == 2);
	assert(v[2] == 3);
	assert(v[3] == 4);

	std::cout << "OK\n";
}

void test_insert_middle()
{
	std::cout << "Test: insert nel mezzo... ";
	vector<int> v;

	v.push_back(1);
	v.push_back(2);
	v.push_back(4);
	v.push_back(5);

	v.insert(3, 2);

	assert(v.size() == 5);
	assert(v[0] == 1);
	assert(v[1] == 2);
	assert(v[2] == 3);
	assert(v[3] == 4);
	assert(v[4] == 5);

	std::cout << "OK\n";
}

void test_insert_end()
{
	std::cout << "Test: insert alla fine... ";
	vector<int> v;

	v.push_back(1);
	v.push_back(2);
	v.push_back(3);

	v.insert(4, 3);

	assert(v.size() == 4);
	assert(v[3] == 4);

	std::cout << "OK\n";
}

void test_erase_beginning()
{
	std::cout << "Test: erase dall'inizio... ";
	vector<int> v;

	v.push_back(1);
	v.push_back(2);
	v.push_back(3);
	v.push_back(4);

	v.erase(0);

	assert(v.size() == 3);
	assert(v[0] == 2);
	assert(v[1] == 3);
	assert(v[2] == 4);

	std::cout << "OK\n";
}

void test_erase_middle()
{
	std::cout << "Test: erase dal mezzo... ";
	vector<int> v;

	v.push_back(1);
	v.push_back(2);
	v.push_back(3);
	v.push_back(4);

	v.erase(2);

	assert(v.size() == 3);
	assert(v[0] == 1);
	assert(v[1] == 2);
	assert(v[2] == 4);

	std::cout << "OK\n";
}

void test_erase_end()
{
	std::cout << "Test: erase dalla fine... ";
	vector<int> v;

	v.push_back(1);
	v.push_back(2);
	v.push_back(3);

	v.erase(2);

	assert(v.size() == 2);
	assert(v[0] == 1);
	assert(v[1] == 2);

	std::cout << "OK\n";
}

// ============ TEST ACCESS ============

void test_operator_bracket()
{
	std::cout << "Test: operator[]... ";
	vector<int> v;

	v.push_back(10);
	v.push_back(20);
	v.push_back(30);

	assert(v[0] == 10);
	assert(v[1] == 20);
	assert(v[2] == 30);

	v[1] = 200;
	assert(v[1] == 200);

	std::cout << "OK\n";
}

void test_at_valid()
{
	std::cout << "Test: at() valido... ";
	vector<int> v;

	v.push_back(10);
	v.push_back(20);
	v.push_back(30);

	assert(v.at(0) == 10);
	assert(v.at(1) == 20);
	assert(v.at(2) == 30);

	v.at(1) = 200;
	assert(v.at(1) == 200);

	std::cout << "OK\n";
}

void test_at_exception()
{
	std::cout << "Test: at() con eccezione... ";
	vector<int> v;

	v.push_back(1);
	v.push_back(2);

	bool exception_thrown = false;
	try
	{
		v.at(10);
	}
	catch (const std::out_of_range&)
	{
		exception_thrown = true;
	}
	assert(exception_thrown);

	std::cout << "OK\n";
}

void test_front_back()
{
	std::cout << "Test: front() e back()... ";
	vector<int> v;

	v.push_back(10);
	v.push_back(20);
	v.push_back(30);

	assert(v.front() == 10);
	assert(v.back() == 30);

	v.front() = 100;
	v.back() = 300;

	assert(v.front() == 100);
	assert(v.back() == 300);

	std::cout << "OK\n";
}

void test_front_back_single()
{
	std::cout << "Test: front/back con 1 elemento... ";
	vector<int> v;

	v.push_back(42);

	assert(v.front() == 42);
	assert(v.back() == 42);
	assert(&v.front() == &v.back());

	std::cout << "OK\n";
}

// ============ TEST CAPACITY ============

void test_size_capacity()
{
	std::cout << "Test: size e capacity... ";
	vector<int> v;

	assert(v.size() == 0);
	assert(v.capacity() == 0);

	v.push_back(1);
	assert(v.size() == 1);
	assert(v.capacity() >= 1);

	size_t old_cap = v.capacity();
	for (int i = 0; i < 10; i++)
	{
		v.push_back(i);
	}
	assert(v.size() == 11);
	assert(v.capacity() >= old_cap);

	std::cout << "OK\n";
}

void test_empty()
{
	std::cout << "Test: empty()... ";
	vector<int> v;

	assert(v.empty());

	v.push_back(1);
	assert(!v.empty());

	v.pop_back();
	assert(v.empty());

	std::cout << "OK\n";
}

void test_clear()
{
	std::cout << "Test: clear()... ";
	vector<int> v;

	v.push_back(1);
	v.push_back(2);
	v.push_back(3);

	size_t old_cap = v.capacity();
	v.clear();

	assert(v.size() == 0);
	assert(v.empty());
	assert(v.capacity() == old_cap); // capacity rimane uguale

	v.push_back(10);
	assert(v.size() == 1);

	std::cout << "OK\n";
}

void test_reserve()
{
	std::cout << "Test: reserve()... ";
	vector<int> v;

	v.reserve(100);
	assert(v.capacity() >= 100);
	assert(v.size() == 0);

	for (int i = 0; i < 50; i++)
	{
		v.push_back(i);
	}
	assert(v.size() == 50);
	assert(v.capacity() >= 100);

	std::cout << "OK\n";
}

void test_shrink_to_fit()
{
	std::cout << "Test: shrink_to_fit()... ";
	vector<int> v;

	v.reserve(100);
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);

	assert(v.capacity() >= 100);

	v.shrink_to_fit();
	assert(v.capacity() == 3);
	assert(v.size() == 3);

	std::cout << "OK\n";
}

// ============ TEST COPY ============

void test_copy_constructor()
{
	std::cout << "Test: copy constructor... ";
	vector<int> v1;

	v1.push_back(1);
	v1.push_back(2);
	v1.push_back(3);

	vector<int> v2(v1);

	assert(v2.size() == 3);
	assert(v2[0] == 1);
	assert(v2[1] == 2);
	assert(v2[2] == 3);

	v1[0] = 999;
	assert(v2[0] == 1);

	std::cout << "OK\n";
}

void test_copy_assignment()
{
	std::cout << "Test: copy assignment... ";
	vector<int> v1;

	v1.push_back(1);
	v1.push_back(2);
	v1.push_back(3);

	vector<int> v2;
	v2.push_back(100);

	v2 = v1;

	assert(v2.size() == 3);
	assert(v2[0] == 1);
	assert(v2[1] == 2);
	assert(v2[2] == 3);

	v1[1] = 888;
	assert(v2[1] == 2);

	std::cout << "OK\n";
}

void test_self_assignment()
{
	std::cout << "Test: self assignment... ";
	vector<int> v;

	v.push_back(1);
	v.push_back(2);
	v.push_back(3);

	v = v;

	assert(v.size() == 3);
	assert(v[0] == 1);
	assert(v[1] == 2);
	assert(v[2] == 3);

	std::cout << "OK\n";
}

// ============ TEST MOVE ============

void test_move_constructor()
{
	std::cout << "Test: move constructor... ";
	vector<int> v1;

	v1.push_back(1);
	v1.push_back(2);
	v1.push_back(3);

	vector<int> v2(std::move(v1));

	assert(v2.size() == 3);
	assert(v2[0] == 1);
	assert(v2[1] == 2);
	assert(v2[2] == 3);

	assert(v1.size() == 0);
	assert(v1.empty());

	std::cout << "OK\n";
}

void test_move_assignment()
{
	std::cout << "Test: move assignment... ";
	vector<int> v1;

	v1.push_back(1);
	v1.push_back(2);
	v1.push_back(3);

	vector<int> v2;
	v2.push_back(999);

	v2 = std::move(v1);

	assert(v2.size() == 3);
	assert(v2[0] == 1);
	assert(v2[1] == 2);
	assert(v2[2] == 3);

	assert(v1.empty());

	std::cout << "OK\n";
}

// ============ TEST STRESS ============

void test_stress_large()
{
	std::cout << "Test: stress con molti elementi... ";
	vector<int> v;

	for (int i = 0; i < 1000; i++)
	{
		v.push_back(i);
	}

	assert(v.size() == 1000);
	for (int i = 0; i < 1000; i++)
	{
		assert(v[i] == i);
	}

	std::cout << "OK\n";
}

void test_stress_insert_erase()
{
	std::cout << "Test: stress insert/erase... ";
	vector<int> v;

	for (int i = 0; i < 10; i++)
	{
		v.push_back(i);
	}

	v.insert(99, 5);
	assert(v[5] == 99);
	assert(v.size() == 11);

	v.erase(5);
	assert(v[5] == 5);
	assert(v.size() == 10);

	for (int i = 0; i < 5; i++)
	{
		v.erase(0);
	}
	assert(v.size() == 5);
	assert(v[0] == 5);

	std::cout << "OK\n";
}

// ============ MAIN ============

int main()
{
	std::cout << "\n";
	std::cout << "TEST SUITE VECTOR - COMPLETA\n";
	std::cout << "\n";

	std::cout << "--- TEST PUSH/POP ---\n";
	test_push_back_basic();
	test_push_back_resize();
	test_pop_back();
	test_pop_back_empty();

	std::cout << "\n--- TEST INSERT/ERASE ---\n";
	test_insert_beginning();
	test_insert_middle();
	test_insert_end();
	test_erase_beginning();
	test_erase_middle();
	test_erase_end();

	std::cout << "\n--- TEST ACCESS ---\n";
	test_operator_bracket();
	test_at_valid();
	test_at_exception();
	test_front_back();
	test_front_back_single();

	std::cout << "\n--- TEST CAPACITY ---\n";
	test_size_capacity();
	test_empty();
	test_clear();
	test_reserve();
	test_shrink_to_fit();

	std::cout << "\n--- TEST COPY ---\n";
	test_copy_constructor();
	test_copy_assignment();
	test_self_assignment();

	std::cout << "\n--- TEST MOVE ---\n";
	test_move_constructor();
	test_move_assignment();

	std::cout << "\n--- TEST STRESS ---\n";
	test_stress_large();
	test_stress_insert_erase();

	std::cout << "\n";
	std::cout << "TUTTI I TEST SONO PASSATI!\n";
	std::cout << "\n";

	return 0;
}