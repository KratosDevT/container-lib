#pragma once
#include <utility>
#include <iostream>
#include <stdexcept>

namespace STDev
{
	enum class SetTreeType
	{
		BinarySearchTree,
		RedBlackTree
	};

	template<typename K, SetTreeType Type = SetTreeType::RedBlackTree>
	class set
	{
	private:
		enum NodeColor { NONE, RED, BLACK };

		struct Node
		{
			K key;
			Node* left;
			Node* right;
			Node* parent;
			NodeColor color;

			Node(const K& k)
				: key(k), left(nullptr), right(nullptr),
				parent(nullptr), color(NONE)
			{}
		};

		Node* root;
		size_t _size;

		// Inserimento nodo (BST standard)
		Node* insert_node(const K& key, bool& inserted)
		{
			Node* parentNode = nullptr;
			Node* currentNode = root;

			while (currentNode != nullptr)
			{
				parentNode = currentNode;
				if (key < currentNode->key)
				{
					currentNode = currentNode->left;
				}
				else if (key > currentNode->key)
				{
					currentNode = currentNode->right;
				}
				else // Chiave già esistente
				{
					inserted = false;
					return currentNode;
				}
			}

			Node* newNode = new Node(key);
			newNode->parent = parentNode;

			if (parentNode == nullptr)
			{
				root = newNode;
			}
			else if (newNode->key < parentNode->key)
			{
				parentNode->left = newNode;
			}
			else
			{
				parentNode->right = newNode;
			}

			++_size;
			inserted = true;
			return newNode;
		}

		// Fixup per Red-Black Tree dopo inserimento
		void insert_fixup(Node* node)
		{
			node->color = RED;

			while (node->parent != nullptr && node->parent->color == RED)
			{
				if (node->parent == node->parent->parent->left)
				{
					Node* uncle = node->parent->parent->right;
					if (uncle != nullptr && uncle->color == RED)
					{
						// Caso 1: zio rosso
						node->parent->color = BLACK;
						uncle->color = BLACK;
						node->parent->parent->color = RED;
						node = node->parent->parent;
					}
					else
					{
						// Caso 2: zio nero
						if (node == node->parent->right)
						{
							node = node->parent;
							rotate_left(node);
						}
						// Caso 3
						node->parent->color = BLACK;
						node->parent->parent->color = RED;
						rotate_right(node->parent->parent);
					}
				}
				else
				{
					// Simmetrico
					Node* uncle = node->parent->parent->left;
					if (uncle != nullptr && uncle->color == RED)
					{
						node->parent->color = BLACK;
						uncle->color = BLACK;
						node->parent->parent->color = RED;
						node = node->parent->parent;
					}
					else
					{
						if (node == node->parent->left)
						{
							node = node->parent;
							rotate_right(node);
						}
						node->parent->color = BLACK;
						node->parent->parent->color = RED;
						rotate_left(node->parent->parent);
					}
				}
			}
			root->color = BLACK;
		}

		// Rotazioni
		void rotate_left(Node* node)
		{
			Node* rightChild = node->right;
			node->right = rightChild->left;

			if (rightChild->left != nullptr)
			{
				rightChild->left->parent = node;
			}

			rightChild->parent = node->parent;

			if (node->parent == nullptr)
			{
				root = rightChild;
			}
			else if (node == node->parent->left)
			{
				node->parent->left = rightChild;
			}
			else
			{
				node->parent->right = rightChild;
			}

			rightChild->left = node;
			node->parent = rightChild;
		}

		void rotate_right(Node* node)
		{
			Node* leftChild = node->left;
			node->left = leftChild->right;

			if (leftChild->right != nullptr)
			{
				leftChild->right->parent = node;
			}

			leftChild->parent = node->parent;

			if (node->parent == nullptr)
			{
				root = leftChild;
			}
			else if (node == node->parent->left)
			{
				node->parent->left = leftChild;
			}
			else
			{
				node->parent->right = leftChild;
			}

			leftChild->right = node;
			node->parent = leftChild;
		}

		// Ricerca
		Node* find_helper(Node* node, const K& key) const
		{
			while (node != nullptr)
			{
				if (key < node->key)
				{
					node = node->left;
				}
				else if (key > node->key)
				{
					node = node->right;
				}
				else
				{
					return node;
				}
			}
			return nullptr;
		}

		// Trova minimo
		Node* find_min(Node* node) const
		{
			while (node && node->left)
				node = node->left;
			return node;
		}

		// Copia profonda
		Node* copy_helper(Node* node)
		{
			if (!node)
				return nullptr;

			Node* new_node = new Node(node->key);

			if constexpr (Type == SetTreeType::RedBlackTree)
			{
				new_node->color = node->color;
			}

			new_node->left = copy_helper(node->left);
			new_node->right = copy_helper(node->right);

			if (new_node->left)
				new_node->left->parent = new_node;

			if (new_node->right)
				new_node->right->parent = new_node;

			return new_node;
		}

		// Cancellazione (BST semplice)
		Node* erase_helper(Node* node, const K& key, bool& erased)
		{
			if (!node)
				return nullptr;

			if (key < node->key)
			{
				node->left = erase_helper(node->left, key, erased);
			}
			else if (key > node->key)
			{
				node->right = erase_helper(node->right, key, erased);
			}
			else
			{
				erased = true;
				_size--;

				// Nodo foglia
				if (!node->left && !node->right)
				{
					delete node;
					return nullptr;
				}
				// Un solo figlio destro
				else if (!node->left)
				{
					Node* temp = node->right;
					delete node;
					return temp;
				}
				// Un solo figlio sinistro
				else if (!node->right)
				{
					Node* temp = node->left;
					delete node;
					return temp;
				}
				// Due figli
				else
				{
					Node* successor = find_min(node->right);
					const_cast<K&>(node->key) = successor->key;
					node->right = erase_helper(node->right, successor->key, erased);
					_size++; // Compenso perché verrà decrementato di nuovo
				}
			}
			return node;
		}

		// Distruzione ricorsiva
		void destroy_helper(Node* node)
		{
			if (!node)
				return;

			destroy_helper(node->left);
			destroy_helper(node->right);
			delete node;
		}

		// Stampa struttura
		void print_helper(Node* node, const std::string& prefix, bool isLeft) const
		{
			if (node != nullptr)
			{
				std::cout << prefix;
				std::cout << (isLeft ? "|-- " : "`-- ");
				std::cout << "[" << node->key << "]";

				if constexpr (Type == SetTreeType::RedBlackTree)
				{
					std::cout << (node->color == RED ? "(R)" : "(B)");
				}

				std::cout << std::endl;

				if (node->left || node->right)
				{
					if (node->left)
						print_helper(node->left, prefix + (isLeft ? "|   " : "    "), true);
					else
						std::cout << prefix << (isLeft ? "|   " : "    ") << "|-- NULL\n";

					if (node->right)
						print_helper(node->right, prefix + (isLeft ? "|   " : "    "), false);
					else
						std::cout << prefix << (isLeft ? "|   " : "    ") << "`-- NULL\n";
				}
			}
		}

	public:
		// Costruttore di default
		set() : root(nullptr), _size(0) {}

		// Distruttore
		~set()
		{
			destroy_helper(root);
		}

		// Copy constructor
		set(const set& other) : root(nullptr), _size(0)
		{
			root = copy_helper(other.root);
			_size = other._size;
		}

		// Copy assignment
		set& operator=(const set& other)
		{
			if (this != &other)
			{
				destroy_helper(root);
				root = copy_helper(other.root);
				_size = other._size;
			}
			return *this;
		}

		// Move constructor
		set(set&& other) noexcept
			: root(other.root), _size(other._size)
		{
			other.root = nullptr;
			other._size = 0;
		}

		// Move assignment
		set& operator=(set&& other) noexcept
		{
			if (this != &other)
			{
				destroy_helper(root);
				root = other.root;
				_size = other._size;
				other.root = nullptr;
				other._size = 0;
			}
			return *this;
		}

		// Inserimento
		bool insert(const K& key)
		{
			bool inserted = false;
			Node* node = insert_node(key, inserted);

			if (inserted && Type == SetTreeType::RedBlackTree)
			{
				insert_fixup(node);
			}

			return inserted;
		}

		// Cancellazione
		bool erase(const K& key)
		{
			bool erased = false;
			root = erase_helper(root, key, erased);
			return erased;
		}

		// Ricerca
		bool find(const K& key) const
		{
			return find_helper(root, key) != nullptr;
		}

		bool contains(const K& key) const
		{
			return find(key);
		}

		// Capacità
		size_t size() const { return _size; }
		bool empty() const { return _size == 0; }

		// Clear
		void clear()
		{
			destroy_helper(root);
			root = nullptr;
			_size = 0;
		}

		// Stampa
		void print_tree() const
		{
			std::cout << "=== Set Structure ===" << std::endl;
			std::cout << "Size: " << _size << std::endl;
			std::cout << std::endl;

			if (root == nullptr)
			{
				std::cout << "Empty set" << std::endl;
			}
			else
			{
				std::cout << "ROOT [" << root->key << "]";
				if constexpr (Type == SetTreeType::RedBlackTree)
				{
					std::cout << (root->color == RED ? "(R)" : "(B)");
				}
				std::cout << std::endl;

				if (root->left || root->right)
				{
					if (root->left)
						print_helper(root->left, "", true);
					else
						std::cout << "|-- NULL" << std::endl;

					if (root->right)
						print_helper(root->right, "", false);
					else
						std::cout << "`-- NULL" << std::endl;
				}
			}
			std::cout << std::endl;
		}

		// Iteratore (in-order traversal)
		class iterator
		{
		private:
			Node* current;
			Node* root_node;
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
			iterator(Node* node, Node* r) : current(nullptr), root_node(r), stack_size(0)
			{
				if (node == nullptr && root_node != nullptr)
				{
					push_left(root_node);
					if (stack_size > 0)
						current = stack[--stack_size];
				}
				else
				{
					current = node;
				}
			}

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
		};

		iterator begin()
		{
			return iterator(nullptr, root);
		}

		iterator end()
		{
			return iterator(nullptr, nullptr);
		}

		iterator begin() const
		{
			return iterator(nullptr, root);
		}

		iterator end() const
		{
			return iterator(nullptr, nullptr);
		}
	};
}