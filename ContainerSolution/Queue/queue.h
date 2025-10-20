#pragma once
#include "deque.h"
#include <stdexcept>

namespace STDev
{
	template<typename T, typename Container = deque<T>>
	class queue
	{
	private:
		Container container_;

	public:
		queue() : container_() {}

		queue(const queue& other) : container_(other.container_) {}

		queue& operator=(const queue& other)
		{
			if (this != &other)
			{
				container_ = other.container_;
			}
			return *this;
		}

		queue(queue&& other) noexcept : container_(std::move(other.container_)) {}

		queue& operator=(queue&& other) noexcept
		{
			if (this != &other)
			{
				container_ = std::move(other.container_);
			}
			return *this;
		}

		~queue() = default;

		void push(const T& value)
		{
			container_.push_back(value);
		}

		void pop()
		{
			if (empty())
			{
				throw std::out_of_range("pop on empty queue");
			}
			container_.pop_front();
		}

		T& front()
		{
			if (empty())
			{
				throw std::out_of_range("front on empty queue");
			}
			return container_.front();
		}

		const T& front() const
		{
			if (empty())
			{
				throw std::out_of_range("front on empty queue");
			}
			return container_.front();
		}

		T& back()
		{
			if (empty())
			{
				throw std::out_of_range("back on empty queue");
			}
			return container_.back();
		}

		const T& back() const
		{
			if (empty())
			{
				throw std::out_of_range("back on empty queue");
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
				std::cout << "Empty queue" << std::endl;
				return;
			}

			std::cout << "Queue (front to back): ";
			for (size_t i = 0; i < container_.size(); ++i)
			{
				std::cout << container_[i];
				if (i < container_.size() - 1) std::cout << " -> ";
			}
			std::cout << std::endl;
		}
	};
}