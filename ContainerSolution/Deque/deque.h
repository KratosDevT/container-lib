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
		static const size_t BLOCK_SIZE = 8;  // Elementi per blocco

		T** map;              // Array di puntatori ai blocchi
		size_t map_size;      // Quanti puntatori nel map
		size_t first_block;   // Indice del primo blocco usato
		size_t last_block;    // Indice dell'ultimo blocco usato
		size_t first_elem;    // Indice nel primo blocco
		size_t last_elem;     // Indice nell'ultimo blocco
		size_t count;         // Numero totale di elementi

	public:
		// Costruttore di default
		deque()
			: map(nullptr), map_size(0), first_block(0),
			last_block(0), first_elem(0), last_elem(0), count(0)
		{
			initialize_map(2);
		}

		// Copy constructor
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

			// Copia i blocchi allocati
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

		// Copy assignment
		deque& operator=(const deque& other)
		{
			if (this != &other)
			{
				// Pulisci risorse esistenti
				clear();
				deallocate_map();

				// Copia dalla sorgente
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

		// Move constructor
		deque(deque&& other) noexcept
			: map(other.map), map_size(other.map_size),
			first_block(other.first_block), last_block(other.last_block),
			first_elem(other.first_elem), last_elem(other.last_elem),
			count(other.count)
		{
			// Lascia other in stato valido ma vuoto
			other.map = nullptr;
			other.map_size = 0;
			other.first_block = 0;
			other.last_block = 0;
			other.first_elem = 0;
			other.last_elem = 0;
			other.count = 0;
		}

		// Move assignment
		deque& operator=(deque&& other) noexcept
		{
			if (this != &other)
			{
				// Pulisci risorse esistenti
				clear();
				deallocate_map();

				// Ruba risorse da other
				map = other.map;
				map_size = other.map_size;
				first_block = other.first_block;
				last_block = other.last_block;
				first_elem = other.first_elem;
				last_elem = other.last_elem;
				count = other.count;

				// Lascia other in stato valido
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

		// Distruttore
		~deque()
		{
			clear();
			deallocate_map();
		}

		// Push operations
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

		// Pop operations
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

		// Access operations
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

		// Capacity operations
		size_t size() const { return count; }
		bool empty() const { return count == 0; }

		// Utility
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

	private:
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
	};
}