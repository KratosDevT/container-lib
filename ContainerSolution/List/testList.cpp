#include "list.h"
#include <iostream>
#include <cassert>
#include <utility>

using namespace STDev;

// ============ TEST INSERT ============

void test_insert_at_begin()
{
	std::cout << "Test: insert all'inizio... ";
	list<int> l;

	l.push_back(2);
	l.push_back(3);
	l.push_back(4);

	auto it = l.begin();
	l.insert(it, 1);

	assert(l.size() == 4);
	assert(l.front() == 1);

	auto verify = l.begin();
	assert(*verify++ == 1);
	assert(*verify++ == 2);
	assert(*verify++ == 3);
	assert(*verify++ == 4);

	std::cout << "OK\n";
}

void test_insert_at_middle()
{
	std::cout << "Test: insert nel mezzo... ";
	list<int> l;

	l.push_back(1);
	l.push_back(2);
	l.push_back(4);
	l.push_back(5);

	auto it = l.begin();
	++it;
	++it;

	l.insert(it, 3);

	assert(l.size() == 5);

	auto verify = l.begin();
	assert(*verify++ == 1);
	assert(*verify++ == 2);
	assert(*verify++ == 3);
	assert(*verify++ == 4);
	assert(*verify++ == 5);

	std::cout << "OK\n";
}

void test_insert_at_end()
{
	std::cout << "Test: insert alla fine... ";
	list<int> l;

	l.push_back(1);
	l.push_back(2);
	l.push_back(3);

	l.insert(l.end(), 4);

	assert(l.size() == 4);
	assert(l.back() == 4);

	auto verify = l.begin();
	assert(*verify++ == 1);
	assert(*verify++ == 2);
	assert(*verify++ == 3);
	assert(*verify++ == 4);

	std::cout << "OK\n";
}

void test_insert_multiple()
{
	std::cout << "Test: inserimenti multipli... ";
	list<int> l;

	l.push_back(10);

	auto it = l.begin();
	l.insert(it, 5);

	it = l.begin();
	++it;
	l.insert(it, 7);

	it = l.begin();
	l.insert(it, 3);

	l.insert(l.end(), 15);

	assert(l.size() == 5);

	auto verify = l.begin();
	assert(*verify++ == 3);
	assert(*verify++ == 5);
	assert(*verify++ == 7);
	assert(*verify++ == 10);
	assert(*verify++ == 15);

	std::cout << "OK\n";
}

void test_insert_return_iterator()
{
	std::cout << "Test: insert ritorna iteratore... ";
	list<int> l;

	l.push_back(1);
	l.push_back(3);

	auto it = l.begin();
	++it;

	auto new_it = l.insert(it, 2);

	assert(*new_it == 2);
	++new_it;
	assert(*new_it == 3);

	std::cout << "OK\n";
}

// ============ TEST ERASE ============

void test_erase_at_begin()
{
	std::cout << "Test: erase dall'inizio... ";
	list<int> l;

	l.push_back(1);
	l.push_back(2);
	l.push_back(3);
	l.push_back(4);

	auto it = l.begin();
	auto next = l.erase(it);

	assert(l.size() == 3);
	assert(l.front() == 2);
	assert(*next == 2);

	auto verify = l.begin();
	assert(*verify++ == 2);
	assert(*verify++ == 3);
	assert(*verify++ == 4);

	std::cout << "OK\n";
}

void test_erase_at_middle()
{
	std::cout << "Test: erase dal mezzo... ";
	list<int> l;

	l.push_back(1);
	l.push_back(2);
	l.push_back(3);
	l.push_back(4);
	l.push_back(5);

	auto it = l.begin();
	++it;
	++it;

	auto next = l.erase(it);

	assert(l.size() == 4);
	assert(*next == 4);

	auto verify = l.begin();
	assert(*verify++ == 1);
	assert(*verify++ == 2);
	assert(*verify++ == 4);
	assert(*verify++ == 5);

	std::cout << "OK\n";
}

void test_erase_at_end()
{
	std::cout << "Test: erase dalla fine... ";
	list<int> l;

	l.push_back(1);
	l.push_back(2);
	l.push_back(3);

	auto it = l.begin();
	++it;
	++it;

	auto next = l.erase(it);

	assert(l.size() == 2);
	assert(l.back() == 2);
	assert(next == l.end());

	std::cout << "OK\n";
}

void test_erase_multiple()
{
	std::cout << "Test: erase multipli... ";
	list<int> l;

	for (int i = 1; i <= 10; i++)
	{
		l.push_back(i);
	}

	auto it = l.begin();
	while (it != l.end())
	{
		if (*it % 2 == 0)
		{
			it = l.erase(it);
		}
		else
		{
			++it;
		}
	}

	assert(l.size() == 5);

	auto verify = l.begin();
	assert(*verify++ == 1);
	assert(*verify++ == 3);
	assert(*verify++ == 5);
	assert(*verify++ == 7);
	assert(*verify++ == 9);

	std::cout << "OK\n";
}

void test_erase_all()
{
	std::cout << "Test: erase tutti gli elementi... ";
	list<int> l;

	l.push_back(1);
	l.push_back(2);
	l.push_back(3);

	auto it = l.begin();
	while (it != l.end())
	{
		it = l.erase(it);
	}

	assert(l.empty());
	assert(l.size() == 0);

	std::cout << "OK\n";
}

void test_erase_exception()
{
	std::cout << "Test: erase sentinel (exception)... ";
	list<int> l;

	l.push_back(1);
	l.push_back(2);

	bool exception_thrown = false;
	try
	{
		l.erase(l.end());
	}
	catch (const std::out_of_range&)
	{
		exception_thrown = true;
	}

	assert(exception_thrown);
	assert(l.size() == 2);

	std::cout << "OK\n";
}

// ============ TEST SPLICE ============

void test_splice_basic()
{
	std::cout << "Test: splice base... ";
	list<int> l1;
	list<int> l2;

	l1.push_back(1);
	l1.push_back(2);
	l1.push_back(5);

	l2.push_back(3);
	l2.push_back(4);

	auto it = l1.begin();
	++it;
	++it;

	l1.splice(it, l2);

	assert(l1.size() == 5);
	assert(l2.empty());
	assert(l2.size() == 0);

	auto verify = l1.begin();
	assert(*verify++ == 1);
	assert(*verify++ == 2);
	assert(*verify++ == 3);
	assert(*verify++ == 4);
	assert(*verify++ == 5);

	std::cout << "OK\n";
}

void test_splice_at_begin()
{
	std::cout << "Test: splice all'inizio... ";
	list<int> l1;
	list<int> l2;

	l1.push_back(4);
	l1.push_back(5);
	l1.push_back(6);

	l2.push_back(1);
	l2.push_back(2);
	l2.push_back(3);

	l1.splice(l1.begin(), l2);

	assert(l1.size() == 6);
	assert(l2.empty());

	auto verify = l1.begin();
	assert(*verify++ == 1);
	assert(*verify++ == 2);
	assert(*verify++ == 3);
	assert(*verify++ == 4);
	assert(*verify++ == 5);
	assert(*verify++ == 6);

	std::cout << "OK\n";
}

void test_splice_at_end()
{
	std::cout << "Test: splice alla fine... ";
	list<int> l1;
	list<int> l2;

	l1.push_back(1);
	l1.push_back(2);
	l1.push_back(3);

	l2.push_back(4);
	l2.push_back(5);
	l2.push_back(6);

	l1.splice(l1.end(), l2);

	assert(l1.size() == 6);
	assert(l2.empty());

	auto verify = l1.begin();
	for (int i = 1; i <= 6; i++)
	{
		assert(*verify++ == i);
	}

	std::cout << "OK\n";
}

void test_splice_empty_source()
{
	std::cout << "Test: splice da lista vuota... ";
	list<int> l1;
	list<int> l2;

	l1.push_back(1);
	l1.push_back(2);

	l1.splice(l1.end(), l2);

	assert(l1.size() == 2);
	assert(l2.empty());

	std::cout << "OK\n";
}

void test_splice_to_empty()
{
	std::cout << "Test: splice verso lista vuota... ";
	list<int> l1;
	list<int> l2;

	l2.push_back(1);
	l2.push_back(2);
	l2.push_back(3);

	l1.splice(l1.begin(), l2);

	assert(l1.size() == 3);
	assert(l2.empty());

	auto verify = l1.begin();
	assert(*verify++ == 1);
	assert(*verify++ == 2);
	assert(*verify++ == 3);

	std::cout << "OK\n";
}

void test_splice_single_element()
{
	std::cout << "Test: splice con un elemento... ";
	list<int> l1;
	list<int> l2;

	l1.push_back(1);
	l1.push_back(3);

	l2.push_back(2);

	auto it = l1.begin();
	++it;

	l1.splice(it, l2);

	assert(l1.size() == 3);
	assert(l2.empty());

	auto verify = l1.begin();
	assert(*verify++ == 1);
	assert(*verify++ == 2);
	assert(*verify++ == 3);

	std::cout << "OK\n";
}

void test_splice_multiple_operations()
{
	std::cout << "Test: splice multipli... ";
	list<int> l1;
	list<int> l2;
	list<int> l3;

	l1.push_back(1);
	l1.push_back(5);

	l2.push_back(2);
	l2.push_back(3);

	l3.push_back(4);

	auto it = l1.begin();
	++it;

	l1.splice(it, l2);

	it = l1.begin();
	++it; ++it; ++it;

	l1.splice(it, l3);

	assert(l1.size() == 5);
	assert(l2.empty());
	assert(l3.empty());

	auto verify = l1.begin();
	for (int i = 1; i <= 5; i++)
	{
		assert(*verify++ == i);
	}

	std::cout << "OK\n";
}

// ============ TEST COMBINED OPERATIONS ============

void test_insert_erase_combined()
{
	std::cout << "Test: insert ed erase combinati... ";
	list<int> l;

	l.push_back(1);
	l.push_back(3);
	l.push_back(5);

	auto it = l.begin();
	++it;
	l.insert(it, 2);

	it = l.begin();
	++it; ++it; ++it;
	l.insert(it, 4);

	assert(l.size() == 5);

	it = l.begin();
	++it; ++it;
	l.erase(it);

	assert(l.size() == 4);

	auto verify = l.begin();
	assert(*verify++ == 1);
	assert(*verify++ == 2);
	assert(*verify++ == 4);
	assert(*verify++ == 5);

	std::cout << "OK\n";
}

void test_splice_then_modify()
{
	std::cout << "Test: splice e poi modifica... ";
	list<int> l1;
	list<int> l2;

	l1.push_back(1);
	l1.push_back(4);

	l2.push_back(2);
	l2.push_back(3);

	auto it = l1.begin();
	++it;
	l1.splice(it, l2);

	l1.push_back(5);

	it = l1.begin();
	++it; ++it;
	l1.erase(it);

	assert(l1.size() == 4);

	auto verify = l1.begin();
	assert(*verify++ == 1);
	assert(*verify++ == 2);
	assert(*verify++ == 4);
	assert(*verify++ == 5);

	std::cout << "OK\n";
}

// ============ TEST COPY/MOVE WITH NEW OPERATIONS ============

void test_copy_after_insert()
{
	std::cout << "Test: copy dopo insert... ";
	list<int> l1;

	l1.push_back(1);
	l1.push_back(3);

	auto it = l1.begin();
	++it;
	l1.insert(it, 2);

	list<int> l2(l1);

	assert(l2.size() == 3);

	auto verify = l2.begin();
	assert(*verify++ == 1);
	assert(*verify++ == 2);
	assert(*verify++ == 3);

	std::cout << "OK\n";
}

void test_move_after_splice()
{
	std::cout << "Test: move dopo splice... ";
	list<int> l1;
	list<int> l2;

	l1.push_back(1);
	l1.push_back(2);

	l2.push_back(3);
	l2.push_back(4);

	l1.splice(l1.end(), l2);

	list<int> l3(std::move(l1));

	assert(l3.size() == 4);
	assert(l1.empty());

	auto verify = l3.begin();
	assert(*verify++ == 1);
	assert(*verify++ == 2);
	assert(*verify++ == 3);
	assert(*verify++ == 4);

	std::cout << "OK\n";
}

// ============ TEST STRESS ============

void test_stress_insert_erase()
{
	std::cout << "Test: stress insert/erase... ";
	list<int> l;

	for (int i = 0; i < 100; i++)
	{
		l.push_back(i);
	}

	auto it = l.begin();
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10 && it != l.end(); j++)
		{
			++it;
		}
		if (it != l.end())
		{
			l.insert(it, 999);
		}
	}

	it = l.begin();
	while (it != l.end())
	{
		if (*it == 999)
		{
			it = l.erase(it);
		}
		else
		{
			++it;
		}
	}

	assert(l.size() == 100);

	std::cout << "OK\n";
}

void test_stress_splice()
{
	std::cout << "Test: stress splice... ";
	list<int> result;

	for (int i = 0; i < 10; i++)
	{
		list<int> temp;
		for (int j = 0; j < 10; j++)
		{
			temp.push_back(i * 10 + j);
		}
		result.splice(result.end(), temp);
	}

	assert(result.size() == 100);

	auto it = result.begin();
	for (int i = 0; i < 100; i++)
	{
		assert(*it++ == i);
	}

	std::cout << "OK\n";
}

// ============ TEST VISUAL DEMONSTRATION ============

void test_visual_demonstration()
{
	std::cout << "\n=== DIMOSTRAZIONE VISIVA ===" << std::endl;

	list<int> l1;
	list<int> l2;

	std::cout << "\n--- Test INSERT ---" << std::endl;
	std::cout << "Creazione lista: 1, 3, 5" << std::endl;
	l1.push_back(1);
	l1.push_back(3);
	l1.push_back(5);
	l1.print_visual();

	std::cout << "Insert 2 dopo 1..." << std::endl;
	auto it = l1.begin();
	++it;
	l1.insert(it, 2);
	l1.print_visual();

	std::cout << "Insert 4 dopo 3..." << std::endl;
	it = l1.begin();
	++it; ++it; ++it;
	l1.insert(it, 4);
	l1.print_visual();

	std::cout << "\n--- Test ERASE ---" << std::endl;
	std::cout << "Rimozione elemento 3..." << std::endl;
	it = l1.begin();
	++it; ++it;
	l1.erase(it);
	l1.print_visual();

	std::cout << "\n--- Test SPLICE ---" << std::endl;
	std::cout << "Creazione seconda lista: 10, 20, 30" << std::endl;
	l2.push_back(10);
	l2.push_back(20);
	l2.push_back(30);
	l2.print_visual();

	std::cout << "Splice l2 alla fine di l1..." << std::endl;
	l1.splice(l1.end(), l2);

	std::cout << "Lista l1 dopo splice:" << std::endl;
	l1.print_visual();

	std::cout << "Lista l2 dopo splice (dovrebbe essere vuota):" << std::endl;
	l2.print_visual();
}

// ============ MAIN ============

int main()
{
	std::cout << "\n========================================\n";
	std::cout << "TEST SUITE LIST - ENHANCED VERSION\n";
	std::cout << "========================================\n\n";

	std::cout << "--- TEST INSERT ---\n";
	test_insert_at_begin();
	test_insert_at_middle();
	test_insert_at_end();
	test_insert_multiple();
	test_insert_return_iterator();

	std::cout << "\n--- TEST ERASE ---\n";
	test_erase_at_begin();
	test_erase_at_middle();
	test_erase_at_end();
	test_erase_multiple();
	test_erase_all();
	test_erase_exception();

	std::cout << "\n--- TEST SPLICE ---\n";
	test_splice_basic();
	test_splice_at_begin();
	test_splice_at_end();
	test_splice_empty_source();
	test_splice_to_empty();
	test_splice_single_element();
	test_splice_multiple_operations();

	std::cout << "\n--- TEST COMBINED ---\n";
	test_insert_erase_combined();
	test_splice_then_modify();

	std::cout << "\n--- TEST COPY/MOVE ---\n";
	test_copy_after_insert();
	test_move_after_splice();

	std::cout << "\n--- TEST STRESS ---\n";
	test_stress_insert_erase();
	test_stress_splice();

	std::cout << "\n========================================\n";
	std::cout << "TUTTI I TEST SONO PASSATI!\n";
	std::cout << "========================================\n";

	test_visual_demonstration();

	return 0;
}