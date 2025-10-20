#pragma once

#include <stdexcept>
#include "deque.h"

namespace STDev
{
	template<typename T, typename Container = STDev::deque<T>>
	class stack
	{
	private:
		Container container_;

	public:
		// Costruttore di default
		stack() : container_() {}

		// Copy constructor
		stack(const stack& other) : container_(other.container_) {}

		// Copy assignment
		stack& operator=(const stack& other)
		{
			if (this != &other)
			{
				container_ = other.container_;
			}
			return *this;
		}

		// Move constructor
		stack(stack&& other) noexcept : container_(std::move(other.container_)) {}

		// Move assignment
		stack& operator=(stack&& other) noexcept
		{
			if (this != &other)
			{
				container_ = std::move(other.container_);
			}
			return *this;
		}

		// Distruttore (default va bene)
		~stack() = default;

		// Push element
		void push(const T& value)
		{
			container_.push_back(value);
		}

		// Pop element
		void pop()
		{
			if (empty())
			{
				throw std::out_of_range("pop on empty stack");
			}
			container_.pop_back();
		}

		// Access top element
		T& top()
		{
			if (empty())
			{
				throw std::out_of_range("top on empty stack");
			}
			return container_.back();
		}

		const T& top() const
		{
			if (empty())
			{
				throw std::out_of_range("top on empty stack");
			}
			return container_.back();
		}

		// Capacity
		bool empty() const
		{
			return container_.empty();
		}

		size_t size() const
		{
			return container_.size();
		}

		// Utility: clear all elements
		void clear()
		{
			container_.clear();
		}

		// Debug: print stack contents (top to bottom)
		void print() const
		{
			if (empty())
			{
				std::cout << "Empty stack" << std::endl;
				return;
			}

			std::cout << "Stack (top to bottom): ";
			// Stampiamo al contrario per mostrare il top per primo
			for (size_t i = container_.size(); i > 0; --i)
			{
				std::cout << container_[i - 1];
				if (i > 1) std::cout << " -> ";
			}
			std::cout << std::endl;
		}
	};
}