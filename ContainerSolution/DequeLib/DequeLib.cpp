#include "deque.h"
#include <iostream>
#include <cassert>
#include <utility>

using namespace STDev;

// ============ TEST PUSH ============

void test_basic_push_back()
{
    std::cout << "Test: push_back basico... ";
    deque<int> d;

    d.push_back(10);
    d.push_front(7);
    d.push_front(6);
    d.push_front(5);
    d.push_front(4);
    d.push_front(3);
    d.push_front(2);
    d.push_front(1);
    d.push_front(0);
    d.push_front(7);
    d.pop_front();
    //assert(d.size() == 3);
    //assert(d[0] == 10);


    std::cout << "OK\n";

    d.print_structure();
}

void test_basic_push_front()
{
    std::cout << "Test: push_front basico... ";
    deque<int> d;

    d.push_front(10);
    d.push_front(20);
    d.push_front(30);

    assert(d.size() == 3);
    assert(d[0] == 30);
    assert(d[1] == 20);
    assert(d[2] == 10);

    std::cout << "OK\n";
}

void test_mixed_push()
{
    std::cout << "Test: push misti... ";
    deque<int> d;

    d.push_back(5);
    d.push_front(3);
    d.push_back(7);
    d.push_front(1);

    assert(d.size() == 4);
    assert(d[0] == 1);
    assert(d[1] == 3);
    assert(d[2] == 5);
    assert(d[3] == 7);

    std::cout << "OK\n";
}

void test_large_push_back()
{
    std::cout << "Test: molti push_back... ";
    deque<int> d;

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
}

void test_large_push_front()
{
    std::cout << "Test: molti push_front... ";
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

// ============ TEST POP ============

void test_pop_back()
{
    std::cout << "Test: pop_back()... ";
    deque<int> d;

    d.push_back(1);
    d.push_back(2);
    d.push_back(3);
    d.push_back(4);
    d.push_back(5);

    assert(d.size() == 5);

    d.pop_back();
    assert(d.size() == 4);
    assert(d.back() == 4);

    d.pop_back();
    d.pop_back();
    assert(d.size() == 2);
    assert(d[0] == 1);
    assert(d[1] == 2);

    std::cout << "OK\n";
}

void test_pop_front()
{
    std::cout << "Test: pop_front()... ";
    deque<int> d;

    d.push_back(1);
    d.push_back(2);
    d.push_back(3);
    d.push_back(4);
    d.push_back(5);

    assert(d.size() == 5);

    d.pop_front();
    assert(d.size() == 4);
    assert(d.front() == 2);

    d.pop_front();
    d.pop_front();
    assert(d.size() == 2);
    assert(d[0] == 4);
    assert(d[1] == 5);

    std::cout << "OK\n";
}

void test_pop_until_empty()
{
    std::cout << "Test: pop fino a vuoto... ";
    deque<int> d;

    d.push_back(1);
    d.push_back(2);
    d.push_back(3);

    d.pop_back();
    d.pop_back();
    d.pop_back();

    assert(d.empty());
    assert(d.size() == 0);

    std::cout << "OK\n";
}

void test_pop_mixed()
{
    std::cout << "Test: pop misti... ";
    deque<int> d;

    for (int i = 1; i <= 10; i++)
    {
        d.push_back(i);
    }

    d.pop_front();  // rimuove 1
    d.pop_back();   // rimuove 10
    d.pop_front();  // rimuove 2
    d.pop_back();   // rimuove 9

    assert(d.size() == 6);
    assert(d.front() == 3);
    assert(d.back() == 8);

    std::cout << "OK\n";
}

// ============ TEST FRONT/BACK ============

void test_front_back()
{
    std::cout << "Test: front() e back()... ";
    deque<int> d;

    d.push_back(10);
    d.push_back(20);
    d.push_back(30);

    assert(d.front() == 10);
    assert(d.back() == 30);

    d.front() = 100;
    d.back() = 300;

    assert(d.front() == 100);
    assert(d.back() == 300);
    assert(d[0] == 100);
    assert(d[2] == 300);

    std::cout << "OK\n";
}

void test_front_back_single()
{
    std::cout << "Test: front/back con 1 elemento... ";
    deque<int> d;

    d.push_back(42);

    assert(d.front() == 42);
    assert(d.back() == 42);
    assert(&d.front() == &d.back());

    std::cout << "OK\n";
}

// ============ TEST AT ============

void test_at_valid()
{
    std::cout << "Test: at() valido... ";
    deque<int> d;

    d.push_back(10);
    d.push_back(20);
    d.push_back(30);

    assert(d.at(0) == 10);
    assert(d.at(1) == 20);
    assert(d.at(2) == 30);

    d.at(1) = 200;
    assert(d.at(1) == 200);

    std::cout << "OK\n";
}

void test_at_exception()
{
    std::cout << "Test: at() con eccezione... ";
    deque<int> d;

    d.push_back(1);
    d.push_back(2);

    bool exception_thrown = false;
    try
    {
        d.at(10);
    }
    catch (const std::out_of_range&)
    {
        exception_thrown = true;
    }
    assert(exception_thrown);

    exception_thrown = false;
    try
    {
        d.at(2);
    }
    catch (const std::out_of_range&)
    {
        exception_thrown = true;
    }
    assert(exception_thrown);

    std::cout << "OK\n";
}

// ============ TEST COPY ============

void test_copy_constructor()
{
    std::cout << "Test: copy constructor... ";
    deque<int> d1;

    d1.push_back(1);
    d1.push_back(2);
    d1.push_back(3);
    d1.push_front(0);

    deque<int> d2(d1);

    assert(d2.size() == 4);
    assert(d2[0] == 0);
    assert(d2[1] == 1);
    assert(d2[2] == 2);
    assert(d2[3] == 3);

    // Modifica d1 non deve influenzare d2
    d1[0] = 999;
    assert(d2[0] == 0);

    d1.push_back(100);
    assert(d2.size() == 4);

    std::cout << "OK\n";
}

void test_copy_assignment()
{
    std::cout << "Test: copy assignment... ";
    deque<int> d1;

    d1.push_back(1);
    d1.push_back(2);
    d1.push_back(3);

    deque<int> d2;
    d2.push_back(100);
    d2.push_back(200);

    d2 = d1;

    assert(d2.size() == 3);
    assert(d2[0] == 1);
    assert(d2[1] == 2);
    assert(d2[2] == 3);

    d1[1] = 888;
    assert(d2[1] == 2);

    std::cout << "OK\n";
}

void test_self_assignment()
{
    std::cout << "Test: self assignment... ";
    deque<int> d;

    d.push_back(1);
    d.push_back(2);
    d.push_back(3);

    d = d;

    assert(d.size() == 3);
    assert(d[0] == 1);
    assert(d[1] == 2);
    assert(d[2] == 3);

    std::cout << "OK\n";
}

// ============ TEST MOVE ============

void test_move_constructor()
{
    std::cout << "Test: move constructor... ";
    deque<int> d1;

    d1.push_back(1);
    d1.push_back(2);
    d1.push_back(3);

    deque<int> d2(std::move(d1));

    assert(d2.size() == 3);
    assert(d2[0] == 1);
    assert(d2[1] == 2);
    assert(d2[2] == 3);

    assert(d1.empty());
    assert(d1.size() == 0);

    std::cout << "OK\n";
}

void test_move_assignment()
{
    std::cout << "Test: move assignment... ";
    deque<int> d1;

    d1.push_back(1);
    d1.push_back(2);
    d1.push_back(3);

    deque<int> d2;
    d2.push_back(999);

    d2 = std::move(d1);

    assert(d2.size() == 3);
    assert(d2[0] == 1);
    assert(d2[1] == 2);
    assert(d2[2] == 3);

    assert(d1.empty());

    std::cout << "OK\n";
}

// ============ TEST UTILITIES ============

void test_empty()
{
    std::cout << "Test: empty()... ";
    deque<int> d;

    assert(d.empty());
    assert(d.size() == 0);

    d.push_back(1);
    assert(!d.empty());
    assert(d.size() == 1);

    d.pop_back();
    assert(d.empty());

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
    std::cout << "Test: operator[]... ";
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

// ============ TEST STRESS ============

void test_stress_alternate()
{
    std::cout << "Test: stress alternato... ";
    deque<int> d;

    for (int i = 0; i < 50; i++)
    {
        d.push_back(i);
        d.push_front(-i - 1);
    }

    assert(d.size() == 100);

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

void test_stress_push_pop()
{
    std::cout << "Test: stress push/pop... ";
    deque<int> d;

    // Push molti elementi
    for (int i = 0; i < 100; i++)
    {
        d.push_back(i);
    }

    // Pop metà da dietro
    for (int i = 0; i < 50; i++)
    {
        d.pop_back();
    }

    assert(d.size() == 50);
    assert(d.front() == 0);
    assert(d.back() == 49);

    // Pop metà da davanti
    for (int i = 0; i < 25; i++)
    {
        d.pop_front();
    }

    assert(d.size() == 25);
    assert(d.front() == 25);
    assert(d.back() == 49);

    std::cout << "OK\n";
}

// ============ MAIN ============

int main()
{
    std::cout << "\n";
    std::cout << "TEST SUITE DEQUE - COMPLETA\n";
    std::cout << "\n";

    std::cout << "--- TEST PUSH ---\n";
    test_basic_push_back();
    test_basic_push_front();
    test_mixed_push();
    test_large_push_back();
    test_large_push_front();

    std::cout << "\n--- TEST POP ---\n";
    test_pop_back();
    test_pop_front();
    test_pop_until_empty();
    test_pop_mixed();

    std::cout << "\n--- TEST FRONT/BACK ---\n";
    test_front_back();
    test_front_back_single();

    std::cout << "\n--- TEST AT ---\n";
    test_at_valid();
    test_at_exception();

    std::cout << "\n--- TEST COPY ---\n";
    test_copy_constructor();
    test_copy_assignment();
    test_self_assignment();

    std::cout << "\n--- TEST MOVE ---\n";
    test_move_constructor();
    test_move_assignment();

    std::cout << "\n--- TEST UTILITIES ---\n";
    test_empty();
    test_clear();
    test_operator_access();

    std::cout << "\n--- TEST STRESS ---\n";
    test_stress_alternate();
    test_stress_push_pop();

    std::cout << "\n";
    std::cout << "TUTTI I TEST SONO PASSATI!\n";
    std::cout << "\n";

    return 0;
}