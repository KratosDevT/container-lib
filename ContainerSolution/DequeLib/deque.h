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
        deque()
            : map(nullptr), map_size(0), first_block(0),
            last_block(0), first_elem(0), last_elem(0), count(0)
        {
            initialize_map(2);  // Inizia con 2 blocchi
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

        size_t size() const { return count; }
        bool empty() const { return count == 0; }

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

                    // Mostra il contenuto del blocco
                    for (size_t j = 0; j < BLOCK_SIZE; j++)
                    {
                        // Evidenzia gli elementi validi
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

                // Indicatori
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