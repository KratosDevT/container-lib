#include "deque.h"
#include <iostream>
#include <cassert>

using namespace STDev;

void test_basic_push_back()
{
    std::cout << "Test: push_back basico... ";
    deque<int> deq1;

    deq1.push_back(1);
    deq1.push_back(2);
    deq1.push_back(3);

    assert(deq1.size() == 3);
    assert(deq1[0] == 1);
    assert(deq1[1] == 2);
    assert(deq1[2] == 3);

    std::cout << "OK\n";
    deq1.print_structure();
}

void test_basic_push_front()
{
    std::cout << "Test: push_front basico... ";
    deque<int> deq2;

    deq2.push_front(1);
    deq2.push_front(2);
    deq2.push_front(3);

    assert(deq2.size() == 3);
    assert(deq2[0] == 3);
    assert(deq2[1] == 2);
    assert(deq2[2] == 1);

    std::cout << "OK\n";
    deq2.print_structure();
}

void test_mixed_operations()
{
    std::cout << "Test: operazioni miste... ";
    deque<int> d;

    d.push_back(1);
    d.push_front(2);
    d.push_back(3);
    d.push_front(4);

    assert(d.size() == 4);
    assert(d[0] == 4);
    assert(d[1] == 2);
    assert(d[2] == 1);
    assert(d[3] == 3);

    std::cout << "OK\n";
    d.print_structure();
}

void test_empty()
{
    std::cout << "Test: empty()... ";
    deque<int> d;

    assert(d.empty());
    assert(d.size() == 0);

    d.push_back(1);
    assert(!d.empty());
    assert(d.size() == 1);

    std::cout << "OK\n";
}

void test_large_push_back()
{
    std::cout << "Test: molti push_back (blocchi multipli)... ";
    deque<int> d;

    // Inserisce più di BLOCK_SIZE elementi
    for (int i = 0; i < 25; i++)
    {
        d.push_back(i);
    }

    assert(d.size() == 25);
    for (int i = 0; i < 25; i++)
    {
        assert(d[i] == i);
    }

    std::cout << "OK\n";
    d.print_structure();
}

void test_large_push_front()
{
    std::cout << "Test: molti push_front (blocchi multipli)... ";
    deque<int> d;

    for (int i = 0; i < 25; i++)
    {
        d.push_front(i);
    }

    assert(d.size() == 25);
    for (int i = 0; i < 25; i++)
    {
        assert(d[i] == 24 - i);
    }

    std::cout << "OK\n";
}

void test_clear()
{
    std::cout << "Test: clear()... ";
    deque<int> d;

    d.push_back(1);
    d.push_back(2);
    d.push_back(3);

    d.clear();

    assert(d.size() == 0);
    assert(d.empty());

    // Dovrebbe poter essere riutilizzata
    d.push_back(10);
    assert(d.size() == 1);
    assert(d[0] == 10);

    std::cout << "OK\n";
}

void test_operator_access()
{
    std::cout << "Test: operator[] con const... ";
    deque<int> d;

    for (int i = 0; i < 10; i++)
    {
        d.push_back(i * 10);
    }

    const deque<int>& cd = d;
    assert(cd[5] == 50);

    d[3] = 999;
    assert(d[3] == 999);

    std::cout << "OK\n";
}

void test_stress()
{
    std::cout << "Test: stress test (100 elementi misti)... ";
    deque<int> d;

    // Alterna push_back e push_front
    for (int i = 0; i < 50; i++)
    {
        d.push_back(i);
        d.push_front(-i - 1);
    }

    assert(d.size() == 100);

    // Verifica che l'ordine sia corretto
    for (int i = 0; i < 50; i++)
    {
        assert(d[i] == -50 + i);
    }
    for (int i = 50; i < 100; i++)
    {
        assert(d[i] == i - 50);
    }

    std::cout << "OK\n";
}

void test_single_element()
{
    std::cout << "Test: singolo elemento... ";
    deque<int> d;

    d.push_back(42);
    assert(d.size() == 1);
    assert(d[0] == 42);

    d.clear();

    d.push_front(99);
    assert(d.size() == 1);
    assert(d[0] == 99);

    std::cout << "OK\n";
}

int main()
{
    std::cout << "=== Test Suite Deque ===\n\n";

    test_basic_push_back();
    test_basic_push_front();

    test_mixed_operations();

    test_empty();

    test_large_push_back();
    test_large_push_front();

    test_clear();

    test_operator_access();
    test_single_element();

    test_stress();

    std::cout << "\n=== Tutti i test passati! ===\n";

    return 0;
}