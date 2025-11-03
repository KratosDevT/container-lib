#pragma once

#include <functional>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace STDev
{
	template<typename K, typename Hash = std::hash<K>>
	class unordered_set
	{
	private:
		struct Node
		{
			K key;
			Node* next;

			Node(const K& k) : key(k), next(nullptr) {}
		};

		std::vector<Node*> buckets;
		size_t _size;
		Hash hash_function;
		static constexpr double MAX_LOAD_FACTOR = 0.75;

		size_t hash(const K& key) const
		{
			return hash_function(key) % buckets.size();
		}

		void rehash(size_t new_bucket_count)
		{
			std::vector<Node*> old_buckets = std::move(buckets);
			buckets = std::vector<Node*>(new_bucket_count, nullptr);
			_size = 0;

			for (Node* head : old_buckets)
			{
				while (head)
				{
					Node* next = head->next;
					insert(head->key);
					delete head;
					head = next;
				}
			}
		}

		void check_and_rehash()
		{
			if (buckets.empty() || load_factor() > MAX_LOAD_FACTOR)
			{
				size_t new_size = buckets.empty() ? 8 : buckets.size() * 2;
				rehash(new_size);
			}
		}

		Node* find_node(const K& key) const
		{
			if (buckets.empty())
				return nullptr;

			size_t idx = hash(key);
			Node* current = buckets[idx];

			while (current)
			{
				if (current->key == key)
					return current;
				current = current->next;
			}

			return nullptr;
		}

		void destroy_all()
		{
			for (Node* head : buckets)
			{
				while (head)
				{
					Node* next = head->next;
					delete head;
					head = next;
				}
			}
			buckets.clear();
			_size = 0;
		}

		void copy_from(const unordered_set& other)
		{
			buckets.resize(other.buckets.size(), nullptr);
			_size = 0;

			for (size_t i = 0; i < other.buckets.size(); ++i)
			{
				Node* current = other.buckets[i];
				while (current)
				{
					insert(current->key);
					current = current->next;
				}
			}
		}

	public:
		class iterator;
		class const_iterator;

		unordered_set() : _size(0)
		{
			buckets.resize(8, nullptr);
		}

		~unordered_set()
		{
			destroy_all();
		}

		// Copy constructor
		unordered_set(const unordered_set& other) : _size(0)
		{
			copy_from(other);
		}

		// Copy assignment
		unordered_set& operator=(const unordered_set& other)
		{
			if (this != &other)
			{
				destroy_all();
				copy_from(other);
			}
			return *this;
		}

		// Move constructor
		unordered_set(unordered_set&& other) noexcept
			: buckets(std::move(other.buckets)), _size(other._size)
		{
			other._size = 0;
		}

		// Move assignment
		unordered_set& operator=(unordered_set&& other) noexcept
		{
			if (this != &other)
			{
				destroy_all();
				buckets = std::move(other.buckets);
				_size = other._size;
				other._size = 0;
			}
			return *this;
		}

		bool insert(const K& key)
		{
			check_and_rehash();

			size_t idx = hash(key);
			Node* current = buckets[idx];

			// Check if key already exists
			while (current)
			{
				if (current->key == key)
					return false; // Key already exists
				current = current->next;
			}

			// Insert new node at front of bucket
			Node* new_node = new Node(key);
			new_node->next = buckets[idx];
			buckets[idx] = new_node;
			++_size;
			return true;
		}

		bool find(const K& key) const
		{
			return find_node(key) != nullptr;
		}

		bool contains(const K& key) const
		{
			return find(key);
		}

		bool erase(const K& key)
		{
			if (buckets.empty())
				return false;

			size_t idx = hash(key);
			Node* current = buckets[idx];
			Node* prev = nullptr;

			while (current)
			{
				if (current->key == key)
				{
					if (prev)
						prev->next = current->next;
					else
						buckets[idx] = current->next;

					delete current;
					--_size;
					return true;
				}
				prev = current;
				current = current->next;
			}

			return false;
		}

		void clear()
		{
			destroy_all();
			buckets.resize(8, nullptr);
		}

		size_t size() const { return _size; }
		bool empty() const { return _size == 0; }
		size_t bucket_count() const { return buckets.size(); }

		double load_factor() const
		{
			return buckets.empty() ? 0.0 : static_cast<double>(_size) / buckets.size();
		}

		void print_structure() const
		{
			std::cout << "\n=== UNORDERED_SET STRUCTURE ===" << std::endl;
			std::cout << "Size: " << _size << std::endl;
			std::cout << "Bucket count: " << buckets.size() << std::endl;
			std::cout << "Load factor: " << load_factor() << std::endl;
			std::cout << std::endl;

			for (size_t i = 0; i < buckets.size(); ++i)
			{
				std::cout << "Bucket[" << i << "]: ";
				Node* current = buckets[i];

				if (!current)
				{
					std::cout << "empty";
				}
				else
				{
					while (current)
					{
						std::cout << "[" << current->key << "]";
						if (current->next)
							std::cout << " -> ";
						current = current->next;
					}
				}
				std::cout << std::endl;
			}
			std::cout << "==============================\n" << std::endl;
		}

		// ==================== ITERATOR ====================
		class iterator
		{
			friend class unordered_set;

		private:
			unordered_set* container;
			size_t bucket_idx;
			Node* current;

			void advance_to_next()
			{
				if (current && current->next)
				{
					current = current->next;
					return;
				}

				current = nullptr;
				++bucket_idx;

				while (bucket_idx < container->buckets.size())
				{
					if (container->buckets[bucket_idx])
					{
						current = container->buckets[bucket_idx];
						return;
					}
					++bucket_idx;
				}
			}

			iterator(unordered_set* c, size_t idx, Node* node)
				: container(c), bucket_idx(idx), current(node)
			{}

		public:
			iterator() : container(nullptr), bucket_idx(0), current(nullptr) {}

			const K& operator*() const
			{
				return current->key;
			}

			const K* operator->() const
			{
				return &(current->key);
			}

			iterator& operator++()
			{
				advance_to_next();
				return *this;
			}

			iterator operator++(int)
			{
				iterator temp = *this;
				advance_to_next();
				return temp;
			}

			bool operator==(const iterator& other) const
			{
				return current == other.current;
			}

			bool operator!=(const iterator& other) const
			{
				return current != other.current;
			}
		};

		// ==================== CONST_ITERATOR ====================
		class const_iterator
		{
			friend class unordered_set;

		private:
			const unordered_set* container;
			size_t bucket_idx;
			const Node* current;

			void advance_to_next()
			{
				if (current && current->next)
				{
					current = current->next;
					return;
				}

				current = nullptr;
				++bucket_idx;

				while (bucket_idx < container->buckets.size())
				{
					if (container->buckets[bucket_idx])
					{
						current = container->buckets[bucket_idx];
						return;
					}
					++bucket_idx;
				}
			}

			const_iterator(const unordered_set* c, size_t idx, const Node* node)
				: container(c), bucket_idx(idx), current(node)
			{}

		public:
			const_iterator() : container(nullptr), bucket_idx(0), current(nullptr) {}

			const K& operator*() const
			{
				return current->key;
			}

			const K* operator->() const
			{
				return &(current->key);
			}

			const_iterator& operator++()
			{
				advance_to_next();
				return *this;
			}

			const_iterator operator++(int)
			{
				const_iterator temp = *this;
				advance_to_next();
				return temp;
			}

			bool operator==(const const_iterator& other) const
			{
				return current == other.current;
			}

			bool operator!=(const const_iterator& other) const
			{
				return current != other.current;
			}
		};

		iterator begin()
		{
			for (size_t i = 0; i < buckets.size(); ++i)
			{
				if (buckets[i])
					return iterator(this, i, buckets[i]);
			}
			return end();
		}

		iterator end()
		{
			return iterator(this, buckets.size(), nullptr);
		}

		const_iterator begin() const
		{
			for (size_t i = 0; i < buckets.size(); ++i)
			{
				if (buckets[i])
					return const_iterator(this, i, buckets[i]);
			}
			return end();
		}

		const_iterator end() const
		{
			return const_iterator(this, buckets.size(), nullptr);
		}

		const_iterator cbegin() const
		{
			return begin();
		}

		const_iterator cend() const
		{
			return end();
		}
	};
}