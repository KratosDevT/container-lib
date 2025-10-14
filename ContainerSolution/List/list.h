#pragma once
#include <string>

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

		list() : _size(0)
		{
			nodeSentinel = new Node<T>();
		}

		~list()
		{
			clear();
			delete nodeSentinel;
		}

		void push_front(T&& value)
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

			// Visualizzazione lineare
			std::cout << "[Sentinel]";
			Node<T>* current = nodeSentinel->nextNode;
			while (current != nodeSentinel)
			{
				std::cout << " <-> [" << current->value << "]";
				current = current->nextNode;
			}
			std::cout << " <-> [Sentinel] (circular)\n" << std::endl;

			// Stampa dettagliata usando print_node
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


	public:

		class iterator // bidirectional iterator
		{

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
				return temp; //return the previous copy
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
		};//end class iterator

		iterator begin()
		{
			return iterator(nodeSentinel->nextNode);
		}
		iterator end()
		{
			return iterator(nodeSentinel);
		}

	};// end class list
}