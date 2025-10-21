#pragma once
#include <utility>
#include <functional>
#include <vector>
#include <memory> 
#include <stack>
#include <iterator>
#include <algorithm>
#include <cmath>

namespace STDev
{
	// ========================================================================
	// BUBBLE SORT
	// ========================================================================
	template<typename RandomIt, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
	void bubble_sort(RandomIt first, RandomIt last, Compare comp = Compare())
	{
		if (first == last) return;
		bool swapped;
		for (auto i = first; i != last - 1; ++i)
		{
			swapped = false;
			auto end = last - std::distance(first, i) - 1;
			for (auto j = first; j != end; ++j)
			{
				if (comp(*(j + 1), *j))
				{
					std::swap(*j, *(j + 1));
					swapped = true;
				}
			}
			if (!swapped) break;
		}
	}

	// ========================================================================
	// QUICK SORT
	// ========================================================================
	template<typename RandomIt, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
	void quick_sort(RandomIt first, RandomIt last, Compare comp = Compare())
	{
		if (first == last || std::distance(first, last) <= 1) return;

		std::stack<std::pair<RandomIt, RandomIt>> stack;
		stack.push({ first, last });

		while (!stack.empty())
		{
			auto [begin, end] = stack.top();
			stack.pop();

			if (begin == end || std::distance(begin, end) <= 1)
				continue;

			auto pivotPosition = begin;
			auto pivot = *(end - 1);

			for (auto j = begin; j != end - 1; ++j)
			{
				if (comp(*j, pivot))
				{
					std::swap(*pivotPosition, *j);
					++pivotPosition;
				}
			}

			std::swap(*pivotPosition, *(end - 1));

			if (std::distance(pivotPosition + 1, end) > std::distance(begin, pivotPosition))
			{
				stack.push({ pivotPosition + 1, end });
				stack.push({ begin, pivotPosition });
			}
			else
			{
				stack.push({ begin, pivotPosition });
				stack.push({ pivotPosition + 1, end });
			}
		}
	}

	// ========================================================================
	// INSERTION SORT
	// ========================================================================
	template<typename RandomIt, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
	void insertion_sort(RandomIt first, RandomIt last, Compare comp = Compare())
	{
		if (first == last || std::distance(first, last) <= 1)
			return;

		for (auto i = first + 1; i != last; ++i)
		{
			auto key = *i;
			auto j = i;

			while (j != first && comp(key, *(j - 1)))
			{
				*j = *(j - 1);
				--j;
			}
			*j = key;
		}
	}

	// ========================================================================
	// SELECTION SORT
	// ========================================================================
	template<typename RandomIt, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
	void selection_sort(RandomIt first, RandomIt last, Compare comp = Compare())
	{
		if (first == last || std::distance(first, last) <= 1)
			return;

		for (auto i = first; i != last - 1; ++i)
		{
			auto min_it = i;
			for (auto j = i + 1; j != last; ++j)
			{
				if (comp(*j, *min_it))
					min_it = j;
			}
			if (min_it != i)
				std::swap(*i, *min_it);
		}
	}

	// ========================================================================
	// MERGE SORT
	// ========================================================================
	namespace detail
	{
		template<typename RandomIt, typename Compare>
		void merge(RandomIt first, RandomIt mid, RandomIt last, Compare comp)
		{
			using ValueType = typename std::iterator_traits<RandomIt>::value_type;

			std::vector<ValueType> left(first, mid);
			std::vector<ValueType> right(mid, last);

			auto left_it = left.begin();
			auto right_it = right.begin();
			auto dest = first;

			while (left_it != left.end() && right_it != right.end())
			{
				if (comp(*left_it, *right_it))
				{
					*dest = *left_it;
					++left_it;
				}
				else
				{
					*dest = *right_it;
					++right_it;
				}
				++dest;
			}

			while (left_it != left.end())
			{
				*dest = *left_it;
				++left_it;
				++dest;
			}

			while (right_it != right.end())
			{
				*dest = *right_it;
				++right_it;
				++dest;
			}
		}
	}

	template<typename RandomIt, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
	void merge_sort(RandomIt first, RandomIt last, Compare comp = Compare())
	{
		auto size = std::distance(first, last);
		if (size <= 1)
			return;

		auto mid = first + size / 2;

		merge_sort(first, mid, comp);
		merge_sort(mid, last, comp);

		detail::merge(first, mid, last, comp);
	}

	// ========================================================================
	// HEAP SORT
	// ========================================================================
	namespace detail
	{
		template<typename RandomIt, typename Compare>
		void heapify(RandomIt first, RandomIt last, RandomIt root, Compare comp)
		{
			auto size = std::distance(first, last);
			auto root_idx = std::distance(first, root);
			auto largest_idx = root_idx;
			auto left_idx = 2 * root_idx + 1;
			auto right_idx = 2 * root_idx + 2;

			if (left_idx < size && comp(*(first + largest_idx), *(first + left_idx)))
				largest_idx = left_idx;

			if (right_idx < size && comp(*(first + largest_idx), *(first + right_idx)))
				largest_idx = right_idx;

			if (largest_idx != root_idx)
			{
				std::swap(*(first + root_idx), *(first + largest_idx));
				heapify(first, last, first + largest_idx, comp);
			}
		}
	}

	template<typename RandomIt, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
	void heap_sort(RandomIt first, RandomIt last, Compare comp = Compare())
	{
		auto size = std::distance(first, last);
		if (size <= 1)
			return;

		for (auto i = size / 2 - 1; i >= 0; --i)
			detail::heapify(first, last, first + i, comp);

		for (auto i = size - 1; i > 0; --i)
		{
			std::swap(*first, *(first + i));
			detail::heapify(first, first + i, first, comp);
		}
	}

	// ========================================================================
	// INTROSORT (Introspective Sort)
	// ========================================================================
	namespace detail
	{
		template<typename RandomIt, typename Compare>
		void introsort_insertion(RandomIt first, RandomIt last, Compare comp)
		{
			if (first == last || std::distance(first, last) <= 1)
				return;

			for (auto i = first + 1; i != last; ++i)
			{
				auto key = *i;
				auto j = i;

				while (j != first && comp(key, *(j - 1)))
				{
					*j = *(j - 1);
					--j;
				}
				*j = key;
			}
		}

		template<typename RandomIt, typename Compare>
		RandomIt introsort_partition(RandomIt first, RandomIt last, Compare comp)
		{
			auto mid = first + std::distance(first, last) / 2;

			if (comp(*mid, *first))
				std::swap(*first, *mid);
			if (comp(*(last - 1), *first))
				std::swap(*first, *(last - 1));
			if (comp(*(last - 1), *mid))
				std::swap(*mid, *(last - 1));

			std::swap(*mid, *(last - 2));
			auto pivot = *(last - 2);

			auto i = first;
			for (auto j = first; j != last - 2; ++j)
			{
				if (comp(*j, pivot))
				{
					std::swap(*i, *j);
					++i;
				}
			}
			std::swap(*i, *(last - 2));
			return i;
		}

		template<typename RandomIt, typename Compare>
		void introsort_impl(RandomIt first, RandomIt last, int depth_limit, Compare comp)
		{
			const int THRESHOLD = 16;
			auto size = std::distance(first, last);

			if (size < THRESHOLD)
			{
				introsort_insertion(first, last, comp);
				return;
			}

			if (depth_limit == 0)
			{
				heap_sort(first, last, comp);
				return;
			}

			auto pivot_pos = introsort_partition(first, last, comp);

			introsort_impl(first, pivot_pos, depth_limit - 1, comp);
			introsort_impl(pivot_pos + 1, last, depth_limit - 1, comp);
		}
	}

	template<typename RandomIt, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
	void intro_sort(RandomIt first, RandomIt last, Compare comp = Compare())
	{
		if (std::distance(first, last) <= 1)
			return;

		auto size = std::distance(first, last);
		int depth_limit = 2 * static_cast<int>(std::log2(size));

		detail::introsort_impl(first, last, depth_limit, comp);
	}

	// ========================================================================
	// STRATEGY PATTERN
	// ========================================================================
	template <typename T, typename Compare = std::less<T>>
	class SortStrategy
	{
	public:
		virtual ~SortStrategy() = default;
		virtual void sort(T* begin, T* end, Compare comp = Compare()) = 0;
		virtual const char* getName() const = 0;
	};

	template <typename T, typename Compare = std::less<T>>
	class BubbleSort final : public SortStrategy<T, Compare>
	{
	public:
		BubbleSort() = default;
		~BubbleSort() override = default;
		void sort(T* begin, T* end, Compare comp = Compare()) override
		{
			bubble_sort(begin, end, comp);
		}
		const char* getName() const override
		{
			return "Bubble Sort";
		}
	};

	template <typename T, typename Compare = std::less<T>>
	class QuickSort final : public SortStrategy<T, Compare>
	{
	public:
		QuickSort() = default;
		~QuickSort() override = default;
		void sort(T* begin, T* end, Compare comp = Compare()) override
		{
			quick_sort(begin, end, comp);
		}
		const char* getName() const override
		{
			return "Quick Sort";
		}
	};

	template <typename T, typename Compare = std::less<T>>
	class InsertionSort final : public SortStrategy<T, Compare>
	{
	public:
		InsertionSort() = default;
		~InsertionSort() override = default;
		void sort(T* begin, T* end, Compare comp = Compare()) override
		{
			insertion_sort(begin, end, comp);
		}
		const char* getName() const override
		{
			return "Insertion Sort";
		}
	};

	template <typename T, typename Compare = std::less<T>>
	class SelectionSort final : public SortStrategy<T, Compare>
	{
	public:
		SelectionSort() = default;
		~SelectionSort() override = default;
		void sort(T* begin, T* end, Compare comp = Compare()) override
		{
			selection_sort(begin, end, comp);
		}
		const char* getName() const override
		{
			return "Selection Sort";
		}
	};

	template <typename T, typename Compare = std::less<T>>
	class MergeSort final : public SortStrategy<T, Compare>
	{
	public:
		MergeSort() = default;
		~MergeSort() override = default;
		void sort(T* begin, T* end, Compare comp = Compare()) override
		{
			merge_sort(begin, end, comp);
		}
		const char* getName() const override
		{
			return "Merge Sort";
		}
	};

	template <typename T, typename Compare = std::less<T>>
	class HeapSort final : public SortStrategy<T, Compare>
	{
	public:
		HeapSort() = default;
		~HeapSort() override = default;
		void sort(T* begin, T* end, Compare comp = Compare()) override
		{
			heap_sort(begin, end, comp);
		}
		const char* getName() const override
		{
			return "Heap Sort";
		}
	};

	template <typename T, typename Compare = std::less<T>>
	class IntroSort final : public SortStrategy<T, Compare>
	{
	public:
		IntroSort() = default;
		~IntroSort() override = default;
		void sort(T* begin, T* end, Compare comp = Compare()) override
		{
			intro_sort(begin, end, comp);
		}
		const char* getName() const override
		{
			return "Intro Sort";
		}
	};

	template <typename T, typename Compare = std::less<T>>
	class Algorithm
	{
	private:
		std::unique_ptr<SortStrategy<T, Compare>> _strategy;
	public:
		Algorithm() = default;
		explicit Algorithm(std::unique_ptr<SortStrategy<T, Compare>> strategy)
			: _strategy{ std::move(strategy) }
		{}
		void setStrategy(std::unique_ptr<SortStrategy<T, Compare>> strategy)
		{
			_strategy = std::move(strategy);
		}
		void sort(T* begin, T* end, Compare comp = Compare())
		{
			if (_strategy)
			{
				_strategy->sort(begin, end, comp);
			}
		}
		const char* getCurrentStrategyName() const
		{
			return _strategy ? _strategy->getName() : "No strategy set";
		}
	};
}