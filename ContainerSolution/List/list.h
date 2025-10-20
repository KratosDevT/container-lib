#pragma once
#include <string>
#include <iostream>

namespace STDev
{
	template<typename K>
	struct Node
	{
		Node* nextNode; // successor node, or first element if head
		Node* previousNode; // predecessor node, or last element if head
		K value;

		Node() : nextNode(this), previousNode(this), value()
		{}

		Node(const K& val) : nextNode(nullptr), previousNode(nullptr), value(val)
		{}
	};

	template<typename T>
	class list
	{
	private:
		Node<T>* nodeSentinel;
		size_t _size;

	public:
		class iterator;

		list() : _size(0)
		{
			nodeSentinel = new Node<T>();
		}

		~list()
		{
			clear();
			delete nodeSentinel;
		}

		// Copy constructor
		list(const list& other) : _size(0)
		{
			nodeSentinel = new Node<T>();

			Node<T>* current = other.nodeSentinel->nextNode;
			while (current != other.nodeSentinel)
			{
				push_back(current->value);
				current = current->nextNode;
			}
		}

		// Copy assignment
		list& operator=(const list& other)
		{
			if (this != &other)
			{
				clear();

				Node<T>* current = other.nodeSentinel->nextNode;
				while (current != other.nodeSentinel)
				{
					push_back(current->value);
					current = current->nextNode;
				}
			}
			return *this;
		}

		// Move constructor
		list(list&& other) noexcept
			: nodeSentinel(other.nodeSentinel), _size(other._size)
		{
			other.nodeSentinel = new Node<T>();
			other._size = 0;
		}

		// Move assignment
		list& operator=(list&& other) noexcept
		{
			if (this != &other)
			{
				clear();
				delete nodeSentinel;

				nodeSentinel = other.nodeSentinel;
				_size = other._size;

				other.nodeSentinel = new Node<T>();
				other._size = 0;
			}
			return *this;
		}

		void push_front(const T& value)
		{
			Node<T>* newNode = new Node<T>(value);

			newNode->nextNode = nodeSentinel->nextNode;
			newNode->previousNode = nodeSentinel;

			nodeSentinel->nextNode->previousNode = newNode;
			nodeSentinel->nextNode = newNode;

			++_size;
		}

		void push_back(const T& value)
		{
			Node<T>* newNode = new Node<T>(value);

			newNode->previousNode = nodeSentinel->previousNode;
			newNode->nextNode = nodeSentinel;

			nodeSentinel->previousNode->nextNode = newNode;
			nodeSentinel->previousNode = newNode;

			++_size;
		}

		void pop_front()
		{
			if (empty())
			{
				throw std::out_of_range("pop_front on empty list");
			}

			Node<T>* toDelete = nodeSentinel->nextNode;

			nodeSentinel->nextNode = toDelete->nextNode;
			toDelete->nextNode->previousNode = nodeSentinel;

			delete toDelete;
			--_size;
		}

		void pop_back()
		{
			if (empty())
			{
				throw std::out_of_range("pop_back on empty list");
			}

			Node<T>* toDelete = nodeSentinel->previousNode;

			nodeSentinel->previousNode = toDelete->previousNode;
			toDelete->previousNode->nextNode = nodeSentinel;

			delete toDelete;
			--_size;
		}

		iterator insert(iterator pos, const T& value)
		{
			Node<T>* posNode = pos.current;
			Node<T>* newNode = new Node<T>(value);

			newNode->previousNode = posNode->previousNode;
			newNode->nextNode = posNode;

			posNode->previousNode->nextNode = newNode;
			posNode->previousNode = newNode;

			++_size;

			return iterator(newNode);
		}

		iterator erase(iterator pos)
		{
			if (pos.current == nodeSentinel)
			{
				throw std::out_of_range("Cannot erase sentinel");
			}

			Node<T>* toDelete = pos.current;
			Node<T>* nextNode = toDelete->nextNode;

			toDelete->previousNode->nextNode = toDelete->nextNode;
			toDelete->nextNode->previousNode = toDelete->previousNode;

			delete toDelete;
			--_size;

			return iterator(nextNode);
		}

		void splice(iterator pos, list& other)
		{
			if (other.empty() || this == &other)
			{
				return;
			}

			Node<T>* posNode = pos.current;
			Node<T>* otherFirst = other.nodeSentinel->nextNode;
			Node<T>* otherLast = other.nodeSentinel->previousNode;

			other.nodeSentinel->nextNode = other.nodeSentinel;
			other.nodeSentinel->previousNode = other.nodeSentinel;

			otherFirst->previousNode = posNode->previousNode;
			otherLast->nextNode = posNode;

			posNode->previousNode->nextNode = otherFirst;
			posNode->previousNode = otherLast;

			_size += other._size;
			other._size = 0;
		}

		T& front()
		{
			if (empty())
			{
				throw std::out_of_range("front on empty list");
			}
			return nodeSentinel->nextNode->value;
		}

		const T& front() const
		{
			if (empty())
			{
				throw std::out_of_range("front on empty list");
			}
			return nodeSentinel->nextNode->value;
		}

		T& back()
		{
			if (empty())
			{
				throw std::out_of_range("back on empty list");
			}
			return nodeSentinel->previousNode->value;
		}

		const T& back() const
		{
			if (empty())
			{
				throw std::out_of_range("back on empty list");
			}
			return nodeSentinel->previousNode->value;
		}

		void clear()
		{
			Node<T>* current = nodeSentinel->nextNode;
			while (current != nodeSentinel)
			{
				Node<T>* toDelete = current;
				current = current->nextNode;
				delete toDelete;
			}

			nodeSentinel->nextNode = nodeSentinel;
			nodeSentinel->previousNode = nodeSentinel;
			_size = 0;
		}

		size_t size() const
		{
			return _size;
		}

		bool empty() const
		{
			return _size == 0;
		}

		void print_visual() const
		{
			std::cout << "\n=== CIRCULAR LIST ===" << std::endl;
			std::cout << "Size: " << _size << "\n" << std::endl;

			if (empty())
			{
				std::cout << "Empty list:" << std::endl;
				print_node(nodeSentinel, "Sentinel (empty)");
				std::cout << "=====================\n" << std::endl;
				return;
			}

			std::cout << "[Sentinel]";
			Node<T>* current = nodeSentinel->nextNode;
			while (current != nodeSentinel)
			{
				std::cout << " <-> [" << current->value << "]";
				current = current->nextNode;
			}
			std::cout << " <-> [Sentinel] (circular)\n" << std::endl;

			std::cout << "Detailed nodes:" << std::endl;
			print_node(nodeSentinel, "Sentinel");

			std::cout << "     |" << std::endl;
			std::cout << "     v" << std::endl;

			current = nodeSentinel->nextNode;
			int index = 0;
			while (current != nodeSentinel)
			{
				print_node(current, "Node " + std::to_string(index));

				if (current->nextNode != nodeSentinel)
				{
					std::cout << "     |" << std::endl;
					std::cout << "     v" << std::endl;
				}

				current = current->nextNode;
				++index;
			}

			std::cout << "     |" << std::endl;
			std::cout << "     v" << std::endl;
			std::cout << "[Back to Sentinel]" << std::endl;
			std::cout << "=====================\n" << std::endl;
		}

		void print_node(const Node<T>* node, const std::string& label) const
		{
			std::cout << "[" << label << " @" << node << "]" << std::endl;
			std::cout << "  value: " << node->value << std::endl;
			std::cout << "  prev: " << node->previousNode << std::endl;
			std::cout << "  next: " << node->nextNode << std::endl;
		}



		iterator begin();
		iterator end();
		const iterator begin() const;
		const iterator end() const;

	public:
		class iterator // bidirectional iterator
		{
			friend class list;
			Node<T>* current;

		public:
			iterator(Node<T>* ptr) : current(ptr)
			{}

			iterator() : current{ nullptr }
			{}

			iterator& operator++() // ++it
			{
				current = current->nextNode;
				return *this;
			}

			iterator operator++(int) // it++
			{
				iterator temp = *this;
				current = current->nextNode;
				return temp;
			}

			iterator& operator--() // --it
			{
				current = current->previousNode;
				return *this;
			}

			iterator operator--(int) // it--
			{
				iterator temp = *this;
				current = current->previousNode;
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

			T& operator*() const
			{
				return current->value;
			}

			T* operator->() const
			{
				return &(current->value);
			}
		};
	};

	template<typename T>
	typename list<T>::iterator list<T>::begin()
	{
		return iterator(nodeSentinel->nextNode);
	}

	template<typename T>
	typename list<T>::iterator list<T>::end()
	{
		return iterator(nodeSentinel);
	}

	template<typename T>
	const typename list<T>::iterator list<T>::begin() const
	{
		return iterator(nodeSentinel->nextNode);
	}

	template<typename T>
	const typename list<T>::iterator list<T>::end() const
	{
		return iterator(nodeSentinel);
	}
}