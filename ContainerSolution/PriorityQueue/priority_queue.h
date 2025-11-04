#pragma once

#include <functional>
#include <vector>
#include <utility>
#include <stdexcept>
#include <iostream>

#ifndef STDEV_MACROS
#define STDEV_MACROS

#define STDEV_BEGIN \
namespace STDev \
{

#define STDEV_END \
}// STDEV_END

#endif // !STDEV_MACROS

STDEV_BEGIN

/*
 * PRIORITY_QUEUE - Binary Heap Implementation
 *
 * Container Adapter basato su vector che mantiene gli elementi in un heap.
 * Di default è un MAX-HEAP (elemento più grande sempre in cima).
 *
 * STRUTTURA HEAP (Max-Heap esempio):
 *         90
 *        /  \
 *       50   30
 *      / \   /
 *     20 10 15
 *
 * Array interno: [90, 50, 30, 20, 10, 15]
 * Index:          0   1   2   3   4   5
 *
 * FORMULE:
 * - Parent di i:      (i-1)/2
 * - Left child di i:  2*i + 1
 * - Right child di i: 2*i + 2
 *
 * COMPLESSITÀ:
 * - push():  O(log n) - Insert + heapify up
 * - pop():   O(log n) - Remove top + heapify down
 * - top():   O(1)     - Access root
 * - empty(): O(1)
 * - size():  O(1)
 */

	template<typename T,
	typename Container = std::vector<T>,
	typename Compare = std::less<T>>  // less = max-heap
	class priority_queue
{
private:
	Container c;        // Container sottostante (default: vector)
	Compare comp;       // Comparator (less = max-heap, greater = min-heap)

	// Helper functions per navigare il binary heap
	static size_t parent(size_t i) { return (i - 1) / 2; }
	static size_t left_child(size_t i) { return 2 * i + 1; }
	static size_t right_child(size_t i) { return 2 * i + 2; }

	/*
	 * HEAPIFY UP (Bubble Up)
	 * Dopo un insert, risale l'elemento fino a ripristinare heap property.
	 * Chiamato dopo push_back al container.
	 */
	void heapify_up(size_t idx)
	{
		while (idx > 0)
		{
			size_t parent_idx = parent(idx);

			// Se parent è "minore" (rispetto al comparator) dell'elemento corrente
			// Nota: comp(parent, current) == true significa parent < current in max-heap
			if (comp(c[parent_idx], c[idx]))
			{
				std::swap(c[parent_idx], c[idx]);
				idx = parent_idx;
			}
			else
			{
				break; // Heap property ripristinata
			}
		}
	}

	/*
	 * HEAPIFY DOWN (Bubble Down / Percolate Down)
	 * Dopo aver rimosso la root, scende l'elemento fino a ripristinare heap property.
	 * Chiamato dopo pop (root viene sostituita con last element).
	 */
	void heapify_down(size_t idx)
	{
		size_t size = c.size();

		while (true)
		{
			size_t largest = idx;
			size_t left = left_child(idx);
			size_t right = right_child(idx);

			// Trova il più grande tra idx, left, right
			// comp(largest, left) == true significa largest < left (quindi left è più grande)
			if (left < size && comp(c[largest], c[left]))
				largest = left;

			if (right < size && comp(c[largest], c[right]))
				largest = right;

			// Se idx era già il più grande, stop
			if (largest == idx)
				break;

			// Altrimenti swap e continua
			std::swap(c[idx], c[largest]);
			idx = largest;
		}
	}

public:
	// ==================== CONSTRUCTORS ====================

	priority_queue() : c(), comp() {}

	explicit priority_queue(const Compare& compare)
		: c(), comp(compare)
	{}

	// Constructor da range [first, last) - Heapify in O(n)
	template<typename InputIt>
	priority_queue(InputIt first, InputIt last, const Compare& compare = Compare())
		: c(first, last), comp(compare)
	{
		make_heap();
	}

	// Constructor da initializer_list
	priority_queue(std::initializer_list<T> init, const Compare& compare = Compare())
		: c(init), comp(compare)
	{
		make_heap();
	}

	// ==================== COPY & MOVE ====================

	// Copy constructor
	priority_queue(const priority_queue& other)
		: c(other.c), comp(other.comp)
	{}

	// Move constructor
	priority_queue(priority_queue&& other) noexcept
		: c(std::move(other.c)), comp(std::move(other.comp))
	{}

	// Copy assignment
	priority_queue& operator=(const priority_queue& other)
	{
		if (this != &other)
		{
			c = other.c;
			comp = other.comp;
		}
		return *this;
	}

	// Move assignment
	priority_queue& operator=(priority_queue&& other) noexcept
	{
		if (this != &other)
		{
			c = std::move(other.c);
			comp = std::move(other.comp);
		}
		return *this;
	}

	// ==================== ELEMENT ACCESS ====================

	/*
	 * top() - Accesso all'elemento con priorità più alta
	 * O(1) - È sempre il primo elemento (root del heap)
	 */
	const T& top() const
	{
		if (c.empty())
			throw std::out_of_range("STDev::priority_queue::top: queue is empty");
		return c.front();
	}

	// ==================== CAPACITY ====================

	bool empty() const { return c.empty(); }
	size_t size() const { return c.size(); }

	// ==================== MODIFIERS ====================

	/*
	 * push() - Insert elemento
	 * O(log n) - Push_back + heapify_up
	 */
	void push(const T& value)
	{
		c.push_back(value);
		heapify_up(c.size() - 1);
	}

	void push(T&& value)
	{
		c.push_back(std::move(value));
		heapify_up(c.size() - 1);
	}

	/*
	 * emplace() - Construct in-place
	 * O(log n) - Costruisce l'elemento direttamente nel container
	 */
	template<typename... Args>
	void emplace(Args&&... args)
	{
		c.emplace_back(std::forward<Args>(args)...);
		heapify_up(c.size() - 1);
	}

	/*
	 * pop() - Rimuove elemento con priorità più alta
	 * O(log n) - Swap root con last, pop_back, heapify_down
	 */
	void pop()
	{
		if (c.empty())
			throw std::out_of_range("STDev::priority_queue::pop: queue is empty");

		// Swap root (max) con l'ultimo elemento
		std::swap(c.front(), c.back());

		// Rimuovi l'ultimo (ex-root)
		c.pop_back();

		// Ripristina heap property dalla root
		if (!c.empty())
			heapify_down(0);
	}

	/*
	 * clear() - Rimuove tutti gli elementi
	 * O(n)
	 */
	void clear()
	{
		c.clear();
	}

	/*
	 * swap() - Scambia contenuti con altra priority_queue
	 * O(1)
	 */
	void swap(priority_queue& other) noexcept
	{
		using std::swap;
		swap(c, other.c);
		swap(comp, other.comp);
	}

	// ==================== HEAP CONSTRUCTION ====================

	/*
	 * make_heap() - Costruisce heap da container non ordinato
	 * O(n) - Più efficiente che fare n push!
	 *
	 * Algoritmo: Heapify-down da ultimo nodo interno fino alla root
	 * Ultimo nodo interno = parent dell'ultimo nodo = (size/2) - 1
	 */
	void make_heap()
	{
		if (c.size() <= 1)
			return;

		// Start dall'ultimo nodo interno e vai verso root
		for (int i = static_cast<int>(c.size() / 2) - 1; i >= 0; --i)
		{
			heapify_down(static_cast<size_t>(i));
		}
	}

	// ==================== DEBUG & VISUALIZATION ====================

	/*
	 * print_heap() - Visualizza heap come array
	 */
	void print_heap() const
	{
		std::cout << "\n=== PRIORITY QUEUE (HEAP) ===" << std::endl;
		std::cout << "Size: " << c.size() << std::endl;
		std::cout << "Heap type: " << (std::is_same<Compare, std::less<T>>::value ? "MAX-HEAP" : "MIN-HEAP") << std::endl;
		std::cout << "\nArray representation: [";

		for (size_t i = 0; i < c.size(); ++i)
		{
			std::cout << c[i];
			if (i < c.size() - 1)
				std::cout << ", ";
		}
		std::cout << "]" << std::endl;

		if (!c.empty())
			std::cout << "Top (highest priority): " << c.front() << std::endl;

		std::cout << "==============================\n" << std::endl;
	}

	/*
	 * print_tree() - Visualizza heap come albero
	 */
	void print_tree() const
	{
		if (c.empty())
		{
			std::cout << "Empty priority queue" << std::endl;
			return;
		}

		std::cout << "\n=== HEAP TREE STRUCTURE ===" << std::endl;
		print_tree_recursive(0, 0);
		std::cout << "===========================\n" << std::endl;
	}

	/*
	 * is_valid_heap() - Verifica se il container rispetta heap property
	 * Utile per debugging e testing
	 */
	bool is_valid_heap() const
	{
		for (size_t i = 0; i < c.size(); ++i)
		{
			size_t left = left_child(i);
			size_t right = right_child(i);

			// Check left child
			if (left < c.size() && comp(c[i], c[left]))
				return false;

			// Check right child
			if (right < c.size() && comp(c[i], c[right]))
				return false;
		}
		return true;
	}

private:
	// Helper per print_tree
	void print_tree_recursive(size_t idx, int depth) const
	{
		if (idx >= c.size())
			return;

		// Print right subtree
		size_t right = right_child(idx);
		if (right < c.size())
			print_tree_recursive(right, depth + 1);

		// Print current node with indentation
		for (int i = 0; i < depth; ++i)
			std::cout << "    ";
		std::cout << c[idx] << std::endl;

		// Print left subtree
		size_t left = left_child(idx);
		if (left < c.size())
			print_tree_recursive(left, depth + 1);
	}
};

// ==================== NON-MEMBER FUNCTIONS ====================

template<typename T, typename Container, typename Compare>
void swap(priority_queue<T, Container, Compare>& lhs,
	priority_queue<T, Container, Compare>& rhs) noexcept
{
	lhs.swap(rhs);
}

STDEV_END
