#pragma once

#include <utility>
#include <iostream>

namespace STDev
{

	template<typename K, typename T>
	class map
	{
	private:
		typedef std::pair<const K, T> value_type;

		struct Node
		{
			value_type data;
			Node* left;
			Node* right;

			Node(const K& key, const T& value)
				: data{ key, value }, left{ nullptr }, right{ nullptr }
			{}
		};

		Node* root;
		size_t _size; //numero di nodi

		Node* insert_helper(Node* node, const K& key, const T& value)
		{
			if (!node)
			{
				_size++;
				return new Node(key, value);
			}

			if (key < node->data.first)
				node->left = insert_helper(node->left, key, value);
			else if (key > node->data.first)
				node->right = insert_helper(node->right, key, value);
			else
				node->data.second = value;

			return node;
		}

		Node* find_helper(Node* node, const K& key) const
		{
			if (!node)
				return nullptr;

			if (key < node->data.first)
				return find_helper(node->left, key);
			else if (key > node->data.first)
				return find_helper(node->right, key);
			else
				return node;
		}

		Node* copy_helper(Node* node)
		{
			if (!node)
				return nullptr;

			Node* new_node = new Node(node->data.first, node->data.second);
			new_node->left = copy_helper(node->left);
			new_node->right = copy_helper(node->right);
			return new_node;
		}

		Node* erase_helper(Node* node, const K& key, bool& erased)
		{
			if (!node)
				return nullptr;

			if (key < node->data.first)
			{
				node->left = erase_helper(node->left, key, erased);
			}
			else if (key > node->data.first)
			{
				node->right = erase_helper(node->right, key, erased);
			}
			else
			{
				// Nodo trovato
				erased = true;
				_size--;

				// Caso 1: Nodo foglia (senza figli)
				if (!node->left && !node->right)
				{
					delete node;
					return nullptr;
				}
				// Caso 2: Nodo con un solo figlio (destro)
				else if (!node->left)
				{
					Node* temp = node->right;
					delete node;
					return temp;
				}
				// Caso 3: Nodo con un solo figlio (sinistro)
				else if (!node->right)
				{
					Node* temp = node->left;
					delete node;
					return temp;
				}
				// Caso 4: Nodo con due figli
				else
				{
					// Trova il successore in-order (minimo del sottoalbero destro)
					Node* successor = find_min(node->right);

					// Copia i dati del successore nel nodo corrente
					// Nota: dobbiamo fare un cast per modificare la chiave const
					const_cast<K&>(node->data.first) = successor->data.first;
					node->data.second = successor->data.second;

					// Elimina il successore
					node->right = erase_helper(node->right, successor->data.first, erased);
					_size++; // Compensiamo perché verrà decrementato di nuovo
				}
			}
			return node;
		}

		Node* find_min(Node* node) const
		{
			while (node && node->left)
				node = node->left;
			return node;
		}

		void print_helper(Node* node, const std::string& prefix, bool isLeft) const
		{
			if (node != nullptr)
			{
				std::cout << prefix;
				std::cout << (isLeft ? "|-- " : "`-- ");
				std::cout << "[" << node->data.first << ":" << node->data.second << "]";
				std::cout << " @" << node << std::endl;

				if (node->left != nullptr || node->right != nullptr)
				{
					if (node->left != nullptr)
						print_helper(node->left, prefix + (isLeft ? "|   " : "    "), true);
					else
					{
						std::cout << prefix << (isLeft ? "|   " : "    ") << "|-- NULL" << std::endl;
					}

					if (node->right != nullptr)
						print_helper(node->right, prefix + (isLeft ? "|   " : "    "), false);
					else
					{
						std::cout << prefix << (isLeft ? "|   " : "    ") << "`-- NULL" << std::endl;
					}
				}
			}
		}

		void print_details_helper(Node* node) const
		{
			if (node != nullptr)
			{
				print_details_helper(node->left);

				std::cout << "Node @" << node << ":" << std::endl;
				std::cout << "  Key: " << node->data.first << std::endl;
				std::cout << "  Value: " << node->data.second << std::endl;
				std::cout << "  Left: " << (node->left ? "Yes" : "NULL")
					<< " @" << node->left << std::endl;
				std::cout << "  Right: " << (node->right ? "Yes" : "NULL")
					<< " @" << node->right << std::endl;
				std::cout << std::endl;

				print_details_helper(node->right);
			}
		}

		void destroy_helper(Node* node)
		{
			if (node)
			{
				destroy_helper(node->left);
				destroy_helper(node->right);
				delete node;
			}
		}

	public:

		map() : root(nullptr), _size(0)
		{}

		~map()
		{
			destroy_helper(root);
		}

		map(const map& other) : root(nullptr), _size(0)
		{
			root = copy_helper(other.root);
			_size = other._size;
		}

		map& operator=(const map& other)
		{
			if (this != &other)
			{
				destroy_helper(root);
				root = copy_helper(other.root);
				_size = other._size;
			}
			return *this;
		}

		void insert(const K& key, const T& value)
		{
			root = insert_helper(root, key, value);
		}

		T& operator[](const K& key)
		{
			Node* node = find_helper(root, key);
			if (!node)
			{
				root = insert_helper(root, key, T());
				node = find_helper(root, key);
			}
			return node->data.second;
		}

		bool find(const K& key) const
		{
			return find_helper(root, key) != nullptr;
		}

		void clear()
		{
			destroy_helper(root);
			root = nullptr;
			_size = 0;
		}

		bool erase(const K& key)
		{
			bool erased = false;
			root = erase_helper(root, key, erased);
			return erased;
		}

		T& at(const K& key)
		{
			Node* node = find_helper(root, key);
			if (!node)
				throw std::out_of_range("map::at: key not found");
			return node->data.second;
		}

		const T& at(const K& key) const
		{
			Node* node = find_helper(root, key);
			if (!node)
				throw std::out_of_range("map::at: key not found");
			return node->data.second;
		}

		void print_tree() const
		{
			std::cout << "=== Tree Structure ===" << std::endl;
			std::cout << "Size: " << _size << std::endl;
			std::cout << "Root: @" << root << std::endl;
			std::cout << std::endl;

			if (root == nullptr)
			{
				std::cout << "Empty tree" << std::endl;
			}
			else
			{
				std::cout << "ROOT [" << root->data.first << ":"
					<< root->data.second << "] @" << root << std::endl;

				if (root->left != nullptr || root->right != nullptr)
				{
					if (root->left != nullptr)
						print_helper(root->left, "", true);
					else
						std::cout << "|-- NULL" << std::endl;

					if (root->right != nullptr)
						print_helper(root->right, "", false);
					else
						std::cout << "`-- NULL" << std::endl;
				}
			}
			std::cout << std::endl;
		}

		void print_details() const
		{
			std::cout << "=== Node Details (In-Order) ===" << std::endl;
			std::cout << "Total nodes: " << _size << std::endl;
			std::cout << std::endl;

			if (root == nullptr)
			{
				std::cout << "Empty map" << std::endl;
			}
			else
			{
				print_details_helper(root);
			}
		}

		void print_map() const
		{
			print_tree();
			print_details();
		}

		size_t size() const
		{
			return _size;
		}

		bool empty() const
		{
			return _size == 0;
		}

		class iterator
		{
		private:
			Node* current;
			Node* root;

			Node* stack[1000];
			int stack_size;

			void push_left(Node* node)
			{
				while (node)
				{
					stack[stack_size++] = node;
					node = node->left;
				}
			}

		public:
			iterator(Node* node, Node* r) : current(nullptr), root(r), stack_size(0)
			{
				if (node == nullptr && root != nullptr)
				{
					push_left(root);
					if (stack_size > 0)
						current = stack[--stack_size];
				}
				else
				{
					current = node;
				}
			}

			value_type& operator*()
			{
				return current->data;
			}

			value_type* operator->()
			{
				return &(current->data);
			}

			iterator& operator++()
			{
				if (!current)
					return *this;

				if (current->right)
				{
					push_left(current->right);
					if (stack_size > 0)
						current = stack[--stack_size];
				}
				else if (stack_size > 0)
				{
					current = stack[--stack_size];
				}
				else
				{
					current = nullptr;
				}

				return *this;
			}

			iterator operator++(int)
			{
				iterator temp = *this;
				++(*this);
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

			friend class map;
		};//end iterator class

		class const_iterator
		{
		private:
			const Node* current;
			const Node* root;

			const Node* stack[1000];
			int stack_size;

			void push_left(const Node* node)
			{
				while (node)
				{
					stack[stack_size++] = node;
					node = node->left;
				}
			}

		public:
			const_iterator(const Node* node, const Node* r) : current(nullptr), root(r), stack_size(0)
			{
				if (node == nullptr && root != nullptr)
				{
					push_left(root);
					if (stack_size > 0)
						current = stack[--stack_size];
				}
				else
				{
					current = node;
				}
			}

			const value_type& operator*() const
			{
				return current->data;
			}

			const value_type* operator->() const
			{
				return &(current->data);
			}

			const_iterator& operator++()
			{
				if (!current)
					return *this;

				if (current->right)
				{
					push_left(current->right);
					if (stack_size > 0)
						current = stack[--stack_size];
				}
				else if (stack_size > 0)
				{
					current = stack[--stack_size];
				}
				else
				{
					current = nullptr;
				}

				return *this;
			}

			const_iterator operator++(int)
			{
				const_iterator temp = *this;
				++(*this);
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

			friend class map;
		};//end const_iterator class

		iterator begin()
		{
			return iterator(nullptr, root);
		}

		iterator end()
		{
			return iterator(nullptr, nullptr);
		}

		const_iterator begin() const
		{
			return const_iterator(nullptr, root);
		}

		const_iterator end() const
		{
			return const_iterator(nullptr, nullptr);
		}

		const_iterator cbegin() const
		{
			return const_iterator(nullptr, root);
		}

		const_iterator cend() const
		{
			return const_iterator(nullptr, nullptr);
		}
	};
}