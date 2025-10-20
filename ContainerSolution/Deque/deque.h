#pragma once
#include <cstddef>
#include <stdexcept>
#include <iostream>

namespace STDev
{
	template<typename T>
	class deque
	{
	private:
		static const size_t BLOCK_SIZE = 8;

		T** map;
		size_t map_size;
		size_t first_block;
		size_t last_block;
		size_t first_elem;
		size_t last_elem;
		size_t count;

		void initialize_map(size_t initial_size)
		{
			map_size = initial_size;
			map = new T * [map_size];
			for (size_t i = 0; i < map_size; i++)
			{
				map[i] = nullptr;
			}
			first_block = map_size / 2;
			last_block = first_block;
		}

		void deallocate_map()
		{
			if (map)
			{
				delete[] map;
				map = nullptr;
			}
		}

		T* allocate_block()
		{
			return new T[BLOCK_SIZE];
		}

		void reallocate_map()
		{
			size_t new_map_size = map_size * 2;
			T** new_map = new T * [new_map_size];

			for (size_t i = 0; i < new_map_size; i++)
			{
				new_map[i] = nullptr;
			}

			size_t offset = (new_map_size - map_size) / 2;
			for (size_t i = 0; i < map_size; i++)
			{
				new_map[offset + i] = map[i];
			}

			delete[] map;
			map = new_map;
			first_block += offset;
			last_block += offset;
			map_size = new_map_size;
		}

	public:
		class iterator;
		class const_iterator;

		deque()
			: map(nullptr), map_size(0), first_block(0),
			last_block(0), first_elem(0), last_elem(0), count(0)
		{
			initialize_map(2);
		}

		deque(const deque& other)
			: map(nullptr), map_size(0), first_block(0),
			last_block(0), first_elem(0), last_elem(0), count(0)
		{
			initialize_map(other.map_size);
			first_block = other.first_block;
			last_block = other.last_block;
			first_elem = other.first_elem;
			last_elem = other.last_elem;
			count = other.count;

			for (size_t i = first_block; i <= last_block && count > 0; i++)
			{
				if (other.map[i])
				{
					map[i] = allocate_block();
					for (size_t j = 0; j < BLOCK_SIZE; j++)
					{
						map[i][j] = other.map[i][j];
					}
				}
			}
		}

		deque& operator=(const deque& other)
		{
			if (this != &other)
			{
				clear();
				deallocate_map();

				initialize_map(other.map_size);
				first_block = other.first_block;
				last_block = other.last_block;
				first_elem = other.first_elem;
				last_elem = other.last_elem;
				count = other.count;

				for (size_t i = first_block; i <= last_block && count > 0; i++)
				{
					if (other.map[i])
					{
						map[i] = allocate_block();
						for (size_t j = 0; j < BLOCK_SIZE; j++)
						{
							map[i][j] = other.map[i][j];
						}
					}
				}
			}
			return *this;
		}

		deque(deque&& other) noexcept
			: map(other.map), map_size(other.map_size),
			first_block(other.first_block), last_block(other.last_block),
			first_elem(other.first_elem), last_elem(other.last_elem),
			count(other.count)
		{
			other.map = nullptr;
			other.map_size = 0;
			other.first_block = 0;
			other.last_block = 0;
			other.first_elem = 0;
			other.last_elem = 0;
			other.count = 0;
		}

		deque& operator=(deque&& other) noexcept
		{
			if (this != &other)
			{
				clear();
				deallocate_map();

				map = other.map;
				map_size = other.map_size;
				first_block = other.first_block;
				last_block = other.last_block;
				first_elem = other.first_elem;
				last_elem = other.last_elem;
				count = other.count;

				other.map = nullptr;
				other.map_size = 0;
				other.first_block = 0;
				other.last_block = 0;
				other.first_elem = 0;
				other.last_elem = 0;
				other.count = 0;
			}
			return *this;
		}

		~deque()
		{
			clear();
			deallocate_map();
		}

		void push_back(const T& value)
		{
			if (count == 0)
			{
				map[first_block] = allocate_block();
				map[first_block][first_elem] = value;
				last_block = first_block;
				last_elem = first_elem;
			}
			else
			{
				last_elem++;
				if (last_elem >= BLOCK_SIZE)
				{
					last_block++;
					last_elem = 0;
					if (last_block >= map_size)
					{
						reallocate_map();
					}
					map[last_block] = allocate_block();
				}
				map[last_block][last_elem] = value;
			}
			count++;
		}

		void push_front(const T& value)
		{
			if (count == 0)
			{
				map[first_block] = allocate_block();
				map[first_block][first_elem] = value;
				last_block = first_block;
				last_elem = first_elem;
			}
			else
			{
				if (first_elem == 0)
				{
					if (first_block == 0)
					{
						reallocate_map();
					}
					first_block--;
					first_elem = BLOCK_SIZE - 1;
					map[first_block] = allocate_block();
				}
				else
				{
					first_elem--;
				}
				map[first_block][first_elem] = value;
			}
			count++;
		}

		void pop_back()
		{
			if (count == 0)
			{
				throw std::out_of_range("pop_back on empty deque");
			}

			if (last_elem == 0)
			{
				delete[] map[last_block];
				map[last_block] = nullptr;
				last_block--;
				last_elem = BLOCK_SIZE - 1;
			}
			else
			{
				last_elem--;
			}
			count--;

			if (count == 0)
			{
				first_elem = 0;
				last_elem = 0;
			}
		}

		void pop_front()
		{
			if (count == 0)
			{
				throw std::out_of_range("pop_front on empty deque");
			}

			if (first_elem == BLOCK_SIZE - 1)
			{
				delete[] map[first_block];
				map[first_block] = nullptr;
				first_block++;
				first_elem = 0;
			}
			else
			{
				first_elem++;
			}
			count--;

			if (count == 0)
			{
				first_elem = 0;
				last_elem = 0;
			}
		}

		T& operator[](size_t index)
		{
			size_t global_idx = first_elem + index;
			size_t block_idx = first_block + global_idx / BLOCK_SIZE;
			size_t elem_idx = global_idx % BLOCK_SIZE;
			return map[block_idx][elem_idx];
		}

		const T& operator[](size_t index) const
		{
			size_t global_idx = first_elem + index;
			size_t block_idx = first_block + global_idx / BLOCK_SIZE;
			size_t elem_idx = global_idx % BLOCK_SIZE;
			return map[block_idx][elem_idx];
		}

		T& at(size_t index)
		{
			if (index >= count)
			{
				throw std::out_of_range("Index out of range");
			}
			return (*this)[index];
		}

		const T& at(size_t index) const
		{
			if (index >= count)
			{
				throw std::out_of_range("Index out of range");
			}
			return (*this)[index];
		}

		T& front()
		{
			if (count == 0)
			{
				throw std::out_of_range("front on empty deque");
			}
			return map[first_block][first_elem];
		}

		const T& front() const
		{
			if (count == 0)
			{
				throw std::out_of_range("front on empty deque");
			}
			return map[first_block][first_elem];
		}

		T& back()
		{
			if (count == 0)
			{
				throw std::out_of_range("back on empty deque");
			}
			return map[last_block][last_elem];
		}

		const T& back() const
		{
			if (count == 0)
			{
				throw std::out_of_range("back on empty deque");
			}
			return map[last_block][last_elem];
		}

		size_t size() const { return count; }
		bool empty() const { return count == 0; }

		void clear()
		{
			for (size_t i = first_block; i <= last_block && count > 0; i++)
			{
				if (map[i])
				{
					delete[] map[i];
					map[i] = nullptr;
				}
			}
			count = 0;
			first_elem = 0;
			last_elem = 0;
			first_block = map_size / 2;
			last_block = first_block;
		}

		void print_structure() const
		{
			std::cout << "\n=== STRUTTURA DEQUE ===\n";
			std::cout << "Map size: " << map_size << "\n";
			std::cout << "Total elements: " << count << "\n";
			std::cout << "first_block: " << first_block << ", first_elem: " << first_elem << "\n";
			std::cout << "last_block: " << last_block << ", last_elem: " << last_elem << "\n\n";

			for (size_t i = 0; i < map_size; i++)
			{
				std::cout << "map[" << i << "] = ";

				if (map[i] == nullptr)
				{
					std::cout << "nullptr";
				}
				else
				{
					std::cout << "Block @ " << (void*)map[i] << " -> [";

					for (size_t j = 0; j < BLOCK_SIZE; j++)
					{
						bool is_valid = false;
						if (i == first_block && i == last_block)
						{
							is_valid = (j >= first_elem && j <= last_elem);
						}
						else if (i == first_block)
						{
							is_valid = (j >= first_elem);
						}
						else if (i == last_block)
						{
							is_valid = (j <= last_elem);
						}
						else if (i > first_block && i < last_block)
						{
							is_valid = true;
						}

						if (is_valid)
						{
							std::cout << map[i][j];
						}
						else
						{
							std::cout << "_";
						}

						if (j < BLOCK_SIZE - 1) std::cout << ",";
					}
					std::cout << "]";
				}

				if (i == first_block && i == last_block)
				{
					std::cout << "  <- FIRST & LAST BLOCK";
				}
				else if (i == first_block)
				{
					std::cout << "  <- FIRST BLOCK";
				}
				else if (i == last_block)
				{
					std::cout << "  <- LAST BLOCK";
				}

				std::cout << "\n";
			}
			std::cout << "=====================\n\n";
		}

		// Iterator methods
		iterator begin();
		iterator end();
		const_iterator begin() const;
		const_iterator end() const;
		const_iterator cbegin() const;
		const_iterator cend() const;

		// ============ ITERATOR CLASS ============
		class iterator
		{
			friend class deque;

		private:
			deque* container;
			size_t index;

			iterator(deque* c, size_t idx) : container(c), index(idx) {}

		public:
			// Iterator traits
			using iterator_category = std::random_access_iterator_tag;
			using value_type = T;
			using difference_type = ptrdiff_t;
			using pointer = T*;
			using reference = T&;

			iterator() : container(nullptr), index(0) {}

			// Dereference
			reference operator*() const
			{
				return (*container)[index];
			}

			pointer operator->() const
			{
				return &((*container)[index]);
			}

			// Pre-increment
			iterator& operator++()
			{
				++index;
				return *this;
			}

			// Post-increment
			iterator operator++(int)
			{
				iterator temp = *this;
				++index;
				return temp;
			}

			// Pre-decrement
			iterator& operator--()
			{
				--index;
				return *this;
			}

			// Post-decrement
			iterator operator--(int)
			{
				iterator temp = *this;
				--index;
				return temp;
			}

			// Arithmetic operators
			iterator& operator+=(difference_type n)
			{
				index += n;
				return *this;
			}

			iterator& operator-=(difference_type n)
			{
				index -= n;
				return *this;
			}

			iterator operator+(difference_type n) const
			{
				return iterator(container, index + n);
			}

			iterator operator-(difference_type n) const
			{
				return iterator(container, index - n);
			}

			difference_type operator-(const iterator& other) const
			{
				return static_cast<difference_type>(index) - static_cast<difference_type>(other.index);
			}

			// Subscript operator
			reference operator[](difference_type n) const
			{
				return (*container)[index + n];
			}

			// Comparison operators
			bool operator==(const iterator& other) const
			{
				return container == other.container && index == other.index;
			}

			bool operator!=(const iterator& other) const
			{
				return !(*this == other);
			}

			bool operator<(const iterator& other) const
			{
				return index < other.index;
			}

			bool operator>(const iterator& other) const
			{
				return index > other.index;
			}

			bool operator<=(const iterator& other) const
			{
				return index <= other.index;
			}

			bool operator>=(const iterator& other) const
			{
				return index >= other.index;
			}
		};

		// ============ CONST_ITERATOR CLASS ============
		class const_iterator
		{
			friend class deque;

		private:
			const deque* container;
			size_t index;

			const_iterator(const deque* c, size_t idx) : container(c), index(idx) {}

		public:
			// Iterator traits
			using iterator_category = std::random_access_iterator_tag;
			using value_type = T;
			using difference_type = ptrdiff_t;
			using pointer = const T*;
			using reference = const T&;

			const_iterator() : container(nullptr), index(0) {}

			// Conversion from iterator to const_iterator
			const_iterator(const iterator& it) : container(it.container), index(it.index) {}

			reference operator*() const
			{
				return (*container)[index];
			}

			pointer operator->() const
			{
				return &((*container)[index]);
			}

			const_iterator& operator++()
			{
				++index;
				return *this;
			}

			const_iterator operator++(int)
			{
				const_iterator temp = *this;
				++index;
				return temp;
			}

			const_iterator& operator--()
			{
				--index;
				return *this;
			}

			const_iterator operator--(int)
			{
				const_iterator temp = *this;
				--index;
				return temp;
			}

			const_iterator& operator+=(difference_type n)
			{
				index += n;
				return *this;
			}

			const_iterator& operator-=(difference_type n)
			{
				index -= n;
				return *this;
			}

			const_iterator operator+(difference_type n) const
			{
				return const_iterator(container, index + n);
			}

			const_iterator operator-(difference_type n) const
			{
				return const_iterator(container, index - n);
			}

			difference_type operator-(const const_iterator& other) const
			{
				return static_cast<difference_type>(index) - static_cast<difference_type>(other.index);
			}

			reference operator[](difference_type n) const
			{
				return (*container)[index + n];
			}

			bool operator==(const const_iterator& other) const
			{
				return container == other.container && index == other.index;
			}

			bool operator!=(const const_iterator& other) const
			{
				return !(*this == other);
			}

			bool operator<(const const_iterator& other) const
			{
				return index < other.index;
			}

			bool operator>(const const_iterator& other) const
			{
				return index > other.index;
			}

			bool operator<=(const const_iterator& other) const
			{
				return index <= other.index;
			}

			bool operator>=(const const_iterator& other) const
			{
				return index >= other.index;
			}
		};
	};

	// ============ ITERATOR METHOD IMPLEMENTATIONS ============

	template<typename T>
	typename deque<T>::iterator deque<T>::begin()
	{
		return iterator(this, 0);
	}

	template<typename T>
	typename deque<T>::iterator deque<T>::end()
	{
		return iterator(this, count);
	}

	template<typename T>
	typename deque<T>::const_iterator deque<T>::begin() const
	{
		return const_iterator(this, 0);
	}

	template<typename T>
	typename deque<T>::const_iterator deque<T>::end() const
	{
		return const_iterator(this, count);
	}

	template<typename T>
	typename deque<T>::const_iterator deque<T>::cbegin() const
	{
		return const_iterator(this, 0);
	}

	template<typename T>
	typename deque<T>::const_iterator deque<T>::cend() const
	{
		return const_iterator(this, count);
	}

	// Global operator+ for iterator arithmetic (n + iterator)
	template<typename T>
	typename deque<T>::iterator operator+(
		typename deque<T>::iterator::difference_type n,
		const typename deque<T>::iterator& it)
	{
		return it + n;
	}

	template<typename T>
	typename deque<T>::const_iterator operator+(
		typename deque<T>::const_iterator::difference_type n,
		const typename deque<T>::const_iterator& it)
	{
		return it + n;
	}
}