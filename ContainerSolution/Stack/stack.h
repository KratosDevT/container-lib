#pragma once

#include <stdexcept>
#include "deque.h"

namespace STDev
{
	template<typename T, typename Container = deque<T>>
	class stack
	{
	private:
		Container container_;

	public:
		stack() : container_() {}

		stack(const stack& other) : container_(other.container_) {}

		stack& operator=(const stack& other)
		{
			if (this != &other)
			{
				container_ = other.container_;
			}
			return *this;
		}

		stack(stack&& other) noexcept : container_(std::move(other.container_)) {}

		stack& operator=(stack&& other) noexcept
		{
			if (this != &other)
			{
				container_ = std::move(other.container_);
			}
			return *this;
		}

		~stack() = default;

		void push(const T& value)
		{
			container_.push_back(value);
		}

		void pop()
		{
			if (empty())
			{
				throw std::out_of_range("pop on empty stack");
			}
			container_.pop_back();
		}

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

		bool empty() const
		{
			return container_.empty();
		}

		size_t size() const
		{
			return container_.size();
		}

		void clear()
		{
			container_.clear();
		}

		void print() const
		{
			if (empty())
			{
				std::cout << "Empty stack" << std::endl;
				return;
			}

			std::cout << "Stack (top to bottom): ";
			for (size_t i = container_.size(); i > 0; --i)
			{
				std::cout << container_[i - 1];
				if (i > 1) std::cout << " -> ";
			}
			std::cout << std::endl;
		}
	};
}