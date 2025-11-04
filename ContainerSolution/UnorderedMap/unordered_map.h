#pragma once

#include <utility>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <vector>

#ifndef STDEV_MACROS
#define STDEV_MACROS

#define STDEV_BEGIN \
namespace STDev \
{

#define STDEV_END \
}// STDEV_END

#endif // !STDEV_MACROS

STDEV_BEGIN

constexpr size_t FNV1A_OFFSET = 14695981039346656037ULL; //64bit
constexpr size_t FNV1A_PRIME = 1099511628211ULL; //64bit


size_t fnv1a_append_bytes(size_t base, const unsigned char* const data,
	const size_t byteNums)
{
	for (size_t i = 0; i < byteNums; ++i) // ACCUMULATE HASH ON base
	{
		base ^= static_cast<size_t>(data[i]); //CAST ON 8 BYTE AND XOR
		base *= FNV1A_PRIME; // MULTIPLY
	}
	return base;
}

template<typename T>
size_t hash_function(const T& data)
{
	const unsigned char* bytes = reinterpret_cast<const unsigned char*>(&data);
	return fnv1a_append_bytes(FNV1A_OFFSET, bytes, sizeof(T));
}

//specializzazione string
template<>
size_t hash_function<std::string>(const std::string& data)
{
	const unsigned char* bytes = reinterpret_cast<const unsigned char*>(data.c_str());
	return fnv1a_append_bytes(FNV1A_OFFSET, bytes, data.size());//data size cycles
}

template<typename K, typename V/*, typename Hash = std::hash<K>*/>
class unordered_map
{

private:

	static constexpr double MAX_LOAD_FACTOR = 0.75;
	static constexpr int MIN_BUCKETS = 8;

	struct Node
	{
		std::pair<const K, V> data;
		Node* next;

		Node(const K& key, const V& value)
			: data(key, value), next(nullptr)
		{}
	};

	std::vector<Node*> buckets;
	size_t _size;
	//Hash hash_function; implemented custom

	size_t hash(const K& key) const
	{
		return hash_function<K>(key) % buckets.size();
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
				insert(head->data.first, head->data.second);
				delete head;
				head = next;
			}
		}
	}

	void check_and_rehash()
	{
		if (buckets.empty() || load_factor() > MAX_LOAD_FACTOR)
		{
			size_t new_size = buckets.empty() ? MIN_BUCKETS : buckets.size() * 2;
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
			if (current->data.first == key)
				return current; //return when found!
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

	void copy_from(const unordered_map& other)
	{
		buckets.resize(other.buckets.size(), nullptr);
		_size = 0;

		for (size_t i = 0; i < other.buckets.size(); ++i)
		{
			Node* current = other.buckets[i];
			while (current)
			{
				insert(current->data.first, current->data.second);
				current = current->next;
			}
		}
	}

public:
	class iterator;
	class const_iterator;

	unordered_map() : _size(0)
	{
		buckets.resize(MIN_BUCKETS, nullptr);
	}

	~unordered_map()
	{
		destroy_all();
	}

	// Copy constructor
	unordered_map(const unordered_map& other) : _size(0)
	{
		copy_from(other);
	}

	// Move constructor
	unordered_map(unordered_map&& other) noexcept
		: buckets(std::move(other.buckets)), _size(other._size)
	{
		other._size = 0;
	}

	// Copy assignment
	unordered_map& operator=(const unordered_map& other)
	{
		if (this != &other)
		{
			destroy_all();
			copy_from(other);
		}
		return *this;
	}

	// Move assignment
	unordered_map& operator=(unordered_map&& other) noexcept //auto&& universal reference
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

	void insert(const K& key, const V& value)
	{
		check_and_rehash();

		size_t idx = hash(key);
		Node* current = buckets[idx];

		//update node value if same key
		while (current)
		{
			if (current->data.first == key)
			{
				current->data.second = value; // Update
				return;
			}
			current = current->next;
		}

		// Insert new node at front of bucket
		Node* new_node = new Node(key, value);
		new_node->next = buckets[idx]; // ci metto quello che c'era prima
		buckets[idx] = new_node;
		++_size;
	}

	V& operator[](const K& key)
	{
		Node* node = find_node(key);
		if (!node)//insert default constructor value if key not found
		{
			insert(key, V());
			node = find_node(key);
		}
		return node->data.second;
	}

	V& at(const K& key) // with bound check
	{
		Node* node = find_node(key);
		if (!node)
			throw std::out_of_range("STDev::unordered_map::at: key not found");
		return node->data.second;
	}

	const V& at(const K& key) const // with bound check
	{
		Node* node = find_node(key);
		if (!node)
			throw std::out_of_range("unordered_map::at: key not found");
		return node->data.second;
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
			if (current->data.first == key)
			{
				if (prev)
					prev->next = current->next; // link prev with next
				else
					buckets[idx] = current->next; // or set bucket head

				delete current; // delete when found!
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
		buckets.resize(MIN_BUCKETS, nullptr);
	}

	double load_factor() const // calculate actual load factor
	{
		return buckets.empty() ? 0.0 : static_cast<double>(_size) / buckets.size();
	}

	size_t size() const { return _size; }
	bool empty() const { return _size == 0; }
	size_t bucket_count() const { return buckets.size(); }

	void print_structure() const
	{
		std::cout << "\n=== UNORDERED_MAP STRUCTURE ===" << std::endl;
		std::cout << "Size: " << _size << std::endl;
		std::cout << "Bucket count: " << buckets.size() << std::endl;
		//std::cout << "Load factor: " << load_factor() << std::endl;
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
					std::cout << "[" << current->data.first << ":" << current->data.second << "]";
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
		friend class unordered_map;

	private:
		unordered_map* container;
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

		iterator(unordered_map* c, size_t idx, Node* node)
			: container(c), bucket_idx(idx), current(node)
		{}

	public:
		iterator() : container(nullptr), bucket_idx(0), current(nullptr) {}

		std::pair<const K, V>& operator*()
		{
			return current->data;
		}

		std::pair<const K, V>* operator->()
		{
			return &(current->data);
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
		friend class unordered_map;

	private:
		const unordered_map* container;
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

		const_iterator(const unordered_map* c, size_t idx, const Node* node)
			: container(c), bucket_idx(idx), current(node)
		{}

	public:
		const_iterator() : container(nullptr), bucket_idx(0), current(nullptr) {}

		const std::pair<const K, V>& operator*() const
		{
			return current->data;
		}

		const std::pair<const K, V>* operator->() const
		{
			return &(current->data);
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

STDEV_END