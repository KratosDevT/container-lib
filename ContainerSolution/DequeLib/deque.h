#pragma once

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
		deque()
			: map(nullptr), map_size(0), first_block(0),
			last_block(0), first_elem(0), last_elem(0), count(0)
		{
			initialize_map(2);  // Inizia con 2 blocchi perchè fin da subito voglio espendare a dx e sx
		}

		~deque()
		{}

		void push_back(const T& value)
		{}

		void push_front(const T& value)
		{}

		T& operator[](size_t index)
		{}

		const T& operator[](size_t index) const
		{}

		size_t size();
		bool empty();
		void clear();

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

	};
}