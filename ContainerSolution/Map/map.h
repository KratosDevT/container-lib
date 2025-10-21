#pragma once

#include <utility>
#include <iostream>

namespace STDev
{
	enum class TreeType
	{
		//BST(Binary Search Tree)
		//Nessun bilanciamento
		//Caso peggiore : O(n) diventa una lista!
		//Caso migliore : O(log n)
		BinarySearchTree,

		//Red - Black Tree
		//Bilanciamento "rilassato"
		//Altezza <= 2log2(n + 1)
		//Insert / Delete: max 3 rotazioni
		//Ricerca : leggermente più lenta di AdelsonVelskyLandisTree
		//Ottimo per insert / delete frequenti(std::map usa questo!)
		RedBlackTree,

		//AdelsonVelskyLandisTree
		//Bilanciamento "rigido"
		//Altezza <= 1.44log2(n + 2)
		//Insert / Delete: possono servire O(log n) rotazioni
		//Ricerca : più veloce di RedBlackTree
		//Ottimo per ricerche frequenti, insert / delete rari
		AdelsonVelskyLandisTree
	};

	template<typename K, typename T, TreeType Type = TreeType::RedBlackTree>
	class map
	{
	private:

		enum NodeColor { NONE, RED, BLACK };

		typedef std::pair<const K, T> value_type;

		struct Node
		{
			value_type data;
			Node* left;
			Node* right;
			Node* parent;
			NodeColor color;

			Node(const K& key, const T& value)
				: data{ key, value }, left{ nullptr }, right{ nullptr }, parent{ nullptr }, color{ NodeColor::NONE }
			{}
		};

		Node* root;
		size_t _size; //numero di nodi

		Node* insert_node(const K& key, const T& value)
		{
			Node* parentNode = nullptr;
			Node* currentNode = root;

			//serve a trovare il parent corretto, se nullptr è devo costruire la root e non ciclo
			while (currentNode != nullptr)
			{
				parentNode = currentNode;
				if (key < currentNode->data.first)
				{
					currentNode = currentNode->left;
				}
				else if (key > currentNode->data.first)
				{
					currentNode = currentNode->right;
				}
				else //per sovrascrivere l'attuale nodo con un nuovo valore, non creo un nuovo nodo
				{
					currentNode->data.second = value;
					return currentNode;
				}
			}

			Node* newNode = new Node(key, value);

			newNode->parent = parentNode;
			if (parentNode == nullptr)
			{
				root = newNode;
			}
			else if (newNode->data.first < parentNode->data.first)
			{
				parentNode->left = newNode;
			}
			else
			{
				parentNode->right = newNode;
			}
			++_size;

			return newNode;
		}

		void insert_fixup(Node* newInsertedNode)
		{
			newInsertedNode->color = NodeColor::RED;
			while (newInsertedNode->parent != nullptr && newInsertedNode->parent->color == NodeColor::RED) //continuo il ciclo solo se il parent è rosso
			{
				if (newInsertedNode->parent == newInsertedNode->parent->parent->left) //accesso sicuro perchè parent è rosso
				{
					Node* uncleNode = newInsertedNode->parent->parent->right;
					if (uncleNode != nullptr && uncleNode->color == NodeColor::RED)
					{
						newInsertedNode->parent->color = NodeColor::BLACK;
						uncleNode->color = NodeColor::BLACK;
						newInsertedNode->parent->parent->color = NodeColor::RED;
						newInsertedNode = newInsertedNode->parent->parent;
					}
					else
					{
						if (newInsertedNode == newInsertedNode->parent->right)
						{
							newInsertedNode = newInsertedNode->parent;
							rotate_left(newInsertedNode);
						}
						newInsertedNode->parent->color = NodeColor::BLACK;
						newInsertedNode->parent->parent->color = NodeColor::RED;
						rotate_right(newInsertedNode->parent->parent);
					}
				}
				else
				{
					Node* y = newInsertedNode->parent->parent->left;
					if (y != nullptr && y->color == NodeColor::RED)
					{
						newInsertedNode->parent->color = NodeColor::BLACK;
						y->color = NodeColor::BLACK;
						newInsertedNode->parent->parent->color = NodeColor::RED;
						newInsertedNode = newInsertedNode->parent->parent;
					}
					else
					{
						if (newInsertedNode == newInsertedNode->parent->left)
						{
							newInsertedNode = newInsertedNode->parent;
							rotate_right(newInsertedNode);
						}
						newInsertedNode->parent->color = NodeColor::BLACK;
						newInsertedNode->parent->parent->color = NodeColor::RED;
						rotate_left(newInsertedNode->parent->parent);
					}
				}
			}
			root->color = NodeColor::BLACK;
		}

		void rotate_left(Node* node)
		{
			Node* childDx = node->right;
			node->right = childDx->left;

			if (childDx->left != nullptr)
			{
				childDx->left->parent = node;
			}

			childDx->parent = node->parent;

			if (node->parent == nullptr)
			{
				root = childDx;
			}
			else if (node == node->parent->left)
			{
				node->parent->left = childDx;
			}
			else
			{
				node->parent->right = childDx;
			}

			childDx->left = node;
			node->parent = childDx;
		}

		void rotate_right(Node* node)
		{
			Node* childSx = node->left;
			node->left = childSx->right;
			if (childSx->right != nullptr)
			{
				childSx->right->parent = node;
			}

			childSx->parent = node->parent;

			if (node->parent == nullptr)
			{
				root = childSx;
			}
			else if (node == node->parent->left)
			{
				node->parent->left = childSx;
			}
			else
			{
				node->parent->right = childSx;
			}

			childSx->right = node;
			node->parent = childSx;
		}


		Node* find_helper(Node* node, const K& key) const
		{
			while (node != nullptr)
			{
				if (key < node->data.first)
				{
					node = node->left;
				}
				else if (key > node->data.first)
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

		Node* copy_helper(Node* node)
		{
			if (!node)
				return nullptr;

			Node* new_node = new Node(node->data.first, node->data.second);

			if constexpr (Type == TreeType::RedBlackTree)
			{
				new_node->color = node->color;
			}

			new_node->left = copy_helper(node->left);
			new_node->right = copy_helper(node->right);

			if (new_node->left)
			{
				new_node->left->parent = new_node;
			}

			if (new_node->right)
			{
				new_node->right->parent = new_node;
			}

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

		void transplant(Node* u, Node* v)
		{
			if (u->parent == nullptr)
				root = v;
			else if (u == u->parent->left)
				u->parent->left = v;
			else
				u->parent->right = v;

			if (v != nullptr)
				v->parent = u->parent;
		}

		Node* delete_node_rb(const K& key, bool& erased)
		{
			Node* z = find_helper(root, key);
			if (!z)
			{
				erased = false;
				return root;
			}

			erased = true;
			_size--;

			Node* y = z;
			Node* x;
			Node* x_parent;
			NodeColor y_original_color = y->color;

			if (z->left == nullptr)
			{
				x = z->right;
				x_parent = z->parent;
				transplant(z, z->right);
			}
			else if (z->right == nullptr)
			{
				x = z->left;
				x_parent = z->parent;
				transplant(z, z->left);
			}
			else
			{
				y = find_min(z->right);
				y_original_color = y->color;
				x = y->right;

				if (y->parent == z)
				{
					x_parent = y;
				}
				else
				{
					x_parent = y->parent;
					transplant(y, y->right);
					y->right = z->right;
					y->right->parent = y;
				}

				transplant(z, y);
				y->left = z->left;
				y->left->parent = y;
				y->color = z->color;
			}

			delete z;

			if (y_original_color == BLACK)
				delete_fixup(x, x_parent);

			return root;
		}

		void delete_fixup(Node* x, Node* x_parent)
		{
			while (x != root && (x == nullptr || x->color == BLACK))
			{
				if (x == x_parent->left)
				{
					Node* w = x_parent->right; // fratello

					if (w->color == RED)
					{
						// Caso 1: fratello rosso
						w->color = BLACK;
						x_parent->color = RED;
						rotate_left(x_parent);
						w = x_parent->right;
					}

					if ((w->left == nullptr || w->left->color == BLACK) &&
						(w->right == nullptr || w->right->color == BLACK))
					{
						// Caso 2: fratello nero con figli neri
						w->color = RED;
						x = x_parent;
						x_parent = x->parent;
					}
					else
					{
						if (w->right == nullptr || w->right->color == BLACK)
						{
							// Caso 3: fratello nero, figlio sinistro rosso, destro nero
							if (w->left != nullptr)
								w->left->color = BLACK;
							w->color = RED;
							rotate_right(w);
							w = x_parent->right;
						}
						// Caso 4: fratello nero, figlio destro rosso
						w->color = x_parent->color;
						x_parent->color = BLACK;
						if (w->right != nullptr)
							w->right->color = BLACK;
						rotate_left(x_parent);
						x = root;
					}
				}
				else // Simmetrico
				{
					Node* w = x_parent->left;

					if (w->color == RED)
					{
						w->color = BLACK;
						x_parent->color = RED;
						rotate_right(x_parent);
						w = x_parent->left;
					}

					if ((w->right == nullptr || w->right->color == BLACK) &&
						(w->left == nullptr || w->left->color == BLACK))
					{
						w->color = RED;
						x = x_parent;
						x_parent = x->parent;
					}
					else
					{
						if (w->left == nullptr || w->left->color == BLACK)
						{
							if (w->right != nullptr)
								w->right->color = BLACK;
							w->color = RED;
							rotate_left(w);
							w = x_parent->left;
						}
						w->color = x_parent->color;
						x_parent->color = BLACK;
						if (w->left != nullptr)
							w->left->color = BLACK;
						rotate_right(x_parent);
						x = root;
					}
				}
			}

			if (x != nullptr)
				x->color = BLACK;
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

				if constexpr (Type == TreeType::RedBlackTree)
				{
					if (node->color == NodeColor::RED)
						std::cout << "(R)";
					else if (node->color == NodeColor::BLACK)
						std::cout << "(B)";
				}

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
			if (node == nullptr)
				return;

			Node* stack[10000];
			int stack_size = 0;

			stack[stack_size++] = node;

			while (stack_size > 0)
			{
				Node* current = stack[--stack_size];

				if (current->left != nullptr)
					stack[stack_size++] = current->left;

				if (current->right != nullptr)
					stack[stack_size++] = current->right;

				delete current;
			}
		}

		bool check_rb_properties(Node* node, int current_black_count, int& expected_black_height) const
		{
			// Caso base: foglia (nullptr)
			if (node == nullptr)
			{
				// Regola 3: le foglie sono nere (nullptr conta come nero)
				if (expected_black_height == -1)
				{
					// Prima volta che raggiungiamo una foglia: salva la black-height
					expected_black_height = current_black_count;
				}
				else
				{
					// Regola 5: tutti i percorsi devono avere la stessa black-height
					if (current_black_count != expected_black_height)
					{
						std::cout << "ERRORE: Black-height diversa! Trovato "
							<< current_black_count << " invece di "
							<< expected_black_height << std::endl;
						return false;
					}
				}
				return true;
			}

			// Regola 4: nodo rosso non può avere figli rossi
			if (node->color == RED)
			{
				if ((node->left && node->left->color == RED) ||
					(node->right && node->right->color == RED))
				{
					std::cout << "ERRORE: Nodo rosso [" << node->data.first
						<< "] ha un figlio rosso!" << std::endl;
					return false;
				}
			}

			// Conta i nodi neri
			int black_count = current_black_count;
			if (node->color == BLACK)
			{
				black_count++;
			}

			// Controlla ricorsivamente i sottoalberi
			if (!check_rb_properties(node->left, black_count, expected_black_height))
				return false;

			if (!check_rb_properties(node->right, black_count, expected_black_height))
				return false;

			return true;
		}

		int compute_height(Node* node) const
		{
			if (!node)
				return 0;

			int left_height = compute_height(node->left);
			int right_height = compute_height(node->right);

			return 1 + std::max(left_height, right_height);
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

		map(map&& other) noexcept
			: root(other.root), _size(other._size)
		{
			other.root = nullptr;
			other._size = 0;
		}

		map& operator=(map&& other) noexcept
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

		void insert(const K& key, const T& value)
		{
			Node* inserted = insert_node(key, value); // for all tree

			if constexpr (Type == TreeType::RedBlackTree) // fixup only for RBT
			{
				insert_fixup(inserted);
			}
		}

		T& operator[](const K& key)
		{
			Node* node = find_helper(root, key);
			if (!node)
			{
				insert(key, T());
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

			if constexpr (Type == TreeType::RedBlackTree)
			{
				root = delete_node_rb(key, erased);
			}
			else
			{
				root = erase_helper(root, key, erased);
			}

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

		bool is_valid_rb_tree() const
		{
			if constexpr (Type == TreeType::RedBlackTree)
			{
				if (root != nullptr && root->color != BLACK)
				{
					std::cout << "ERRORE: La radice non è nera!" << std::endl;
					return false;
				}

				int expected_black_height = -1;
				bool valid = check_rb_properties(root, 0, expected_black_height);

				if (valid)
				{
					std::cout << "RB-Tree valido! Black-height = "
						<< expected_black_height << std::endl;
				}

				return valid;
			}

			return true;
		}

		void print_tree_info() const
		{
			if constexpr (Type == TreeType::RedBlackTree)
			{
				std::cout << "\n=== RB-Tree Info ===" << std::endl;
				std::cout << "Nodes: " << _size << std::endl;

				if (root)
				{
					int height = compute_height(root);
					int black_height = -1;
					check_rb_properties(root, 0, black_height);

					std::cout << "Total height: " << height << std::endl;
					std::cout << "Black height: " << black_height << std::endl;
					std::cout << "Max theoretical height: " << 2 * black_height << std::endl;
					std::cout << "Root color: " << (root->color == RED ? "RED" : "BLACK") << std::endl;
				}

				std::cout << "===================" << std::endl;
			}
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
					<< root->data.second << "]";

				if constexpr (Type == TreeType::RedBlackTree)
				{
					std::cout << (root->color == NodeColor::RED ? "(R)" : "(B)");
				}

				std::cout << " @" << root << std::endl;

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