# STDev Container Library 📚

Una libreria completa di strutture dati **STL-compliant** implementata da zero per scopo didattico e comprensione profonda del C++ Standard Template Library.

---

## 📋 Indice

- [Panoramica](#-panoramica)
- [Container Implementati](#-container-implementati)
- [Quick Start](#-quick-start)
- [Confronto Performance](#-confronto-performance)
- [Guide Dettagliate](#-guide-dettagliate)
  - [Vector](#vector)
  - [List](#list)
  - [Deque](#deque)
  - [Stack](#stack)
  - [Queue](#queue)
  - [Map](#map)
  - [Set](#set)
  - [Unordered Map](#unordered_map)
  - [Unordered Set](#unordered_set)
- [Algoritmi di Sorting](#-algoritmi-di-sorting)
- [Quando Usare Cosa](#-quando-usare-cosa)
- [Test Suite](#-test-suite)
- [Complessità Computazionale](#-complessità-computazionale)
- [Concetti C++ Fondamentali](#-concetti-c-fondamentali)

---

## 🎯 Panoramica

**STDev** è una implementazione educativa completa dei principali container C++ STL. Ogni struttura dati è implementata con:

- ✅ **Iterator STL-compliant** (begin, end, cbegin, cend)
- ✅ **Copy & Move semantics** complete
- ✅ **Const correctness**
- ✅ **Exception safety**
- ✅ **Funzioni di debug** (print_structure, print_visual)
- ✅ **Test coverage completo** (100% pass rate)

### Obiettivi del Progetto

1. **Didattico**: Capire come funzionano internamente le strutture dati STL
2. **Pratico**: Codice production-quality ben testato
3. **Comparativo**: Confrontare implementazioni diverse (BST vs RB-Tree, Array vs Linked List, Tree vs Hash)

---

## 📦 Container Implementati

### Sequence Containers

| Container | Struttura | Complessità Insert | Complessità Access | Random Access |
|-----------|-----------|-------------------|-------------------|---------------|
| **vector** | Dynamic Array | O(1) amortized / O(n) worst | O(1) | ✅ |
| **list** | Doubly-Linked List | O(1) | O(n) | ❌ |
| **deque** | Blocchi Array | O(1) | O(1) | ✅ |

### Adapter Containers

| Container | Base | Operazioni Principali |
|-----------|------|----------------------|
| **stack** | deque | push, pop, top (LIFO) |
| **queue** | deque | push, pop, front, back (FIFO) |

### Associative Containers (Ordered)

| Container | Struttura | Complessità | Ordinato | Duplicati |
|-----------|-----------|-------------|----------|-----------|
| **map** | Red-Black Tree | O(log n) | ✅ | ❌ |
| **set** | Red-Black Tree | O(log n) | ✅ | ❌ |

### Unordered Associative Containers

| Container | Struttura | Complessità Media | Complessità Worst | Ordinato |
|-----------|-----------|------------------|------------------|----------|
| **unordered_map** | Hash Table | O(1) | O(n) | ❌ |
| **unordered_set** | Hash Table | O(1) | O(n) | ❌ |

---

## 🚀 Quick Start

### Compilazione

```bash
# Compila singolo test
g++ -std=c++17 -Wall -Wextra -o testVector testVector.cpp

# Compila tutti i test
g++ -std=c++17 -Wall -Wextra -o testAll test*.cpp
```

### Esempio Base

```cpp
#include "vector.h"
#include "map.h"
#include "unordered_set.h"

using namespace STDev;

int main() {
    // Vector
    vector<int> v;
    v.push_back(10);
    v.push_back(20);
    v[0] = 15;
    
    // Map
    map<string, int> m;
    m.insert("age", 25);
    m["name"] = "John";
    
    // Unordered Set
    unordered_set<int> s;
    s.insert(5);
    s.insert(3);
    
    // Iterazione
    for (const auto& val : v) {
        cout << val << " ";
    }
    
    return 0;
}
```

---

## 📊 Confronto Performance

### Insert Performance (1000 elementi)

| Container | Sequential | Random | Note |
|-----------|-----------|--------|------|
| **vector** | 0.1ms | 0.1ms | Push back velocissimo |
| **list** | 0.3ms | 0.3ms | Insert O(1) dovunque |
| **deque** | 0.2ms | 0.2ms | Ottimo compromesso |
| **map** (BST) | 50ms | 0.5ms | Sequenziale = worst case! |
| **map** (RB-Tree) | 0.5ms | 0.5ms | Sempre bilanciato |
| **unordered_map** | 0.2ms | 0.2ms | Più veloce, ma non ordinato |

### Search Performance (1000 ricerche)

| Container | Complessità | Tempo (1000 elem) | Note |
|-----------|-------------|------------------|------|
| **vector** | O(n) | 5ms | Linear search |
| **list** | O(n) | 8ms | Linear, cache-unfriendly |
| **deque** | O(n) | 6ms | Linear search |
| **map** | O(log n) | 0.3ms | Ordinato |
| **set** | O(log n) | 0.3ms | Ordinato |
| **unordered_map** | O(1) | 0.1ms | Più veloce! |
| **unordered_set** | O(1) | 0.1ms | Più veloce! |

---

## 📖 Guide Dettagliate

## Vector

### Struttura Interna

```
Capacity: 8
Size: 5

[10][20][30][40][50][__][__][__]
 ^                  ^
 data_             size_
```

### Caratteristiche

- **Dynamic array** con resize automatico
- **Contiguous memory** → ottima cache locality
- **Random access** O(1)
- **Push back amortized** O(1)

### Quando Usare

✅ **Usa vector quando:**
- Serve accesso random veloce
- Principalmente push/pop dalla fine
- Serve attraversare sequenzialmente (cache-friendly)
- Memoria contigua è importante

❌ **Evita vector quando:**
- Insert/erase frequenti nel mezzo
- Size variabile fortemente
- Serve insert O(1) all'inizio

### Esempio Completo

```cpp
vector<int> v;

// Inserimento
v.push_back(10);
v.push_back(20);
v.insert(15, 1);        // Insert at position 1

// Accesso
int first = v[0];       // No bounds check
int second = v.at(1);   // With bounds check

// Capacità
v.reserve(100);         // Pre-allocate
v.shrink_to_fit();      // Reduce capacity to size

// Rimozione
v.pop_back();
v.erase(0);             // Erase at position 0
v.clear();              // Remove all

// Iterazione
for (auto it = v.begin(); it != v.end(); ++it) {
    cout << *it << " ";
}

// Range-based for
for (const auto& val : v) {
    cout << val << " ";
}
```

### Complessità

| Operazione | Complessità | Note |
|------------|-------------|------|
| push_back | O(1) amortized | O(n) quando resize |
| pop_back | O(1) | |
| insert | O(n) | Shift elements |
| erase | O(n) | Shift elements |
| operator[] | O(1) | No bounds check |
| at() | O(1) | With bounds check |
| clear | O(n) | Destructor calls |

---

## List

### Struttura Interna

```
[Sentinel] <-> [Node 1] <-> [Node 2] <-> [Node 3] <-> [Sentinel]
    ^                                                       ^
    |_______________________________________________________|
                    (circular)
```

### Caratteristiche

- **Doubly-linked list** con sentinel node
- **Non-contiguous memory**
- **Insert/erase O(1)** con iterator
- **No random access**

### Quando Usare

✅ **Usa list quando:**
- Insert/erase frequenti ovunque
- Non serve random access
- Splice operations (merge lists)

❌ **Evita list quando:**
- Serve random access
- Serve cache locality
- Memoria limitata (overhead puntatori)

### Esempio Completo

```cpp
list<int> l;

// Inserimento
l.push_back(10);
l.push_front(5);

// Insert con iterator
auto it = l.begin();
++it;
l.insert(it, 7);        // Insert before iterator

// Erase
it = l.begin();
l.erase(it);            // Returns next iterator

// Splice (merge another list)
list<int> l2;
l2.push_back(100);
l.splice(l.end(), l2);  // l2 diventa vuota

// Iterazione (no random access!)
for (auto it = l.begin(); it != l.end(); ++it) {
    *it *= 2;           // Modify through iterator
}

// Bidirectional
auto rit = l.end();
--rit;                  // Last element
```

### Complessità

| Operazione | Complessità | Note |
|------------|-------------|------|
| push_front/back | O(1) | |
| pop_front/back | O(1) | |
| insert | O(1) | Con iterator valido |
| erase | O(1) | Con iterator valido |
| splice | O(1) | Merge lists |
| size() | O(1) | Tracked internally |
| Access [i] | ❌ | Not supported |

---

## Deque

### Struttura Interna

```
Map (array di puntatori a blocchi):
[B0*][B1*][B2*][B3*][B4*]
  |    |    |    |    |
  v    v    v    v    v
[--][--][10][20][30]  Block 0 (BLOCK_SIZE=8)
[40][50][60][70][80]  Block 1
[90][--][--][--][--]  Block 2
```

### Caratteristiche

- **Double-ended queue**
- **Blocchi di array** collegati
- **Push/pop O(1)** da entrambi i lati
- **Random access O(1)**

### Quando Usare

✅ **Usa deque quando:**
- Serve push/pop da entrambi i lati
- Serve random access
- Alternative a vector per push_front

❌ **Evita deque quando:**
- Insert/erase frequenti nel mezzo
- Serve memoria strettamente contigua

### Esempio Completo

```cpp
deque<int> d;

// Inserimento entrambi lati
d.push_back(10);
d.push_front(5);

// Random access
d[0] = 20;
int val = d.at(1);

// Rimozione entrambi lati
d.pop_front();
d.pop_back();

// Front e back
int first = d.front();
int last = d.back();

// Iterazione (random access iterator!)
for (size_t i = 0; i < d.size(); i++) {
    cout << d[i] << " ";
}

// STL algorithms
sort(d.begin(), d.end());
reverse(d.begin(), d.end());
```

### Complessità

| Operazione | Complessità | Note |
|------------|-------------|------|
| push_front | O(1) | |
| push_back | O(1) | |
| pop_front | O(1) | |
| pop_back | O(1) | |
| operator[] | O(1) | Block + offset calc |
| at() | O(1) | With bounds check |
| insert (middle) | O(n) | |

---

## Stack

### Struttura (Adapter)

```cpp
template<typename T, typename Container = deque<T>>
class stack {
    Container container_;  // Default: deque
    // ...
};
```

### Caratteristiche

- **LIFO** (Last In First Out)
- **Adapter** (usa deque internamente)
- **Push/pop O(1)** solo dal top

### Quando Usare

✅ **Usa stack quando:**
- Serve LIFO semantics
- Function call stack, undo/redo
- Parenthesis matching, expression evaluation

### Esempio Completo

```cpp
stack<int> s;

// Push
s.push(10);
s.push(20);
s.push(30);

// Access top
int top_val = s.top();  // 30

// Pop (no return value!)
s.pop();
int new_top = s.top();  // 20

// Size
cout << s.size();       // 2

// Empty check
if (!s.empty()) {
    s.pop();
}

// Esempio: Verifica parentesi bilanciate
bool isBalanced(const string& expr) {
    stack<char> s;
    for (char c : expr) {
        if (c == '(') s.push(c);
        else if (c == ')') {
            if (s.empty()) return false;
            s.pop();
        }
    }
    return s.empty();
}
```

### Complessità

| Operazione | Complessità |
|------------|-------------|
| push | O(1) |
| pop | O(1) |
| top | O(1) |
| empty | O(1) |
| size | O(1) |

---

## Queue

### Struttura (Adapter)

```cpp
template<typename T, typename Container = deque<T>>
class queue {
    Container container_;  // Default: deque
    // ...
};
```

### Caratteristiche

- **FIFO** (First In First Out)
- **Adapter** (usa deque internamente)
- **Push back, pop front**

### Quando Usare

✅ **Usa queue quando:**
- Serve FIFO semantics
- Task scheduling, BFS, buffer

### Esempio Completo

```cpp
queue<int> q;

// Enqueue (push)
q.push(10);
q.push(20);
q.push(30);

// Access front and back
int first = q.front();  // 10
int last = q.back();    // 30

// Dequeue (pop)
q.pop();
int new_front = q.front();  // 20

// Size
cout << q.size();       // 2

// Esempio: BFS
void bfs(int start) {
    queue<int> q;
    unordered_set<int> visited;
    
    q.push(start);
    visited.insert(start);
    
    while (!q.empty()) {
        int node = q.front();
        q.pop();
        
        for (int neighbor : graph[node]) {
            if (!visited.contains(neighbor)) {
                q.push(neighbor);
                visited.insert(neighbor);
            }
        }
    }
}
```

### Complessità

| Operazione | Complessità |
|------------|-------------|
| push | O(1) |
| pop | O(1) |
| front | O(1) |
| back | O(1) |
| empty | O(1) |
| size | O(1) |

---

## Map

### Struttura Interna (Red-Black Tree)

```
        [5:B]
       /     \
    [3:R]   [8:R]
    /   \   /   \
 [1:B] [4:B][6:B][9:B]

B = Black, R = Red
```

### Caratteristiche

- **Red-Black Tree** (auto-bilanciato)
- **Ordinato** per chiave
- **O(log n)** per tutte le operazioni
- **In-order traversal** dà elementi ordinati

### RB-Tree Properties

1. Ogni nodo è rosso o nero
2. Root è nero
3. Foglie (nil) sono nere
4. Nodo rosso ha figli neri
5. Ogni path dalla root ha stesso numero di nodi neri

### Quando Usare

✅ **Usa map quando:**
- Serve ordinamento per chiave
- Serve range queries (lower_bound, upper_bound)
- Serve iterazione ordinata

❌ **Evita map quando:**
- Non serve ordinamento
- Serve solo lookup veloce → usa unordered_map

### Esempio Completo

```cpp
map<string, int> m;

// Insert
m.insert("age", 25);
m.insert("height", 180);

// operator[]
m["name"] = 100;        // Creates if not exists

// Access
int age = m.at("age");  // Exception if not found
int x = m["xyz"];       // Creates with default value

// Find
if (m.find("age")) {
    cout << "Found!";
}

// Erase
bool erased = m.erase("age");

// Iterazione ORDINATA per chiave
for (const auto& [key, value] : m) {
    cout << key << " => " << value << endl;
}

// Tree type selection
map<int, string, TreeType::BinarySearchTree> bst;  // Unbalanced
map<int, string, TreeType::RedBlackTree> rbt;     // Balanced (default)

// Debug
m.print_tree();         // Visualizza struttura
m.is_valid_rb_tree();   // Verifica properties
```

### Complessità

| Operazione | BST Worst | RB-Tree | Note |
|------------|-----------|---------|------|
| insert | O(n) | O(log n) | BST degenera in lista |
| find | O(n) | O(log n) | |
| erase | O(n) | O(log n) | |
| Iteration | O(n) | O(n) | In-order traversal |

---

## Set

### Struttura Interna (Red-Black Tree)

```
        [5:B]
       /     \
    [3:R]   [8:R]
    /   \   /   \
 [1:B] [4:B][6:B][9:B]

Solo chiavi (no valori)
```

### Caratteristiche

- **Red-Black Tree** (come map, ma solo chiavi)
- **Ordinato** automaticamente
- **Unicità** garantita
- **O(log n)** operazioni

### Quando Usare

✅ **Usa set quando:**
- Serve collection ordinata di unique elements
- Serve trovare min/max velocemente
- Serve range operations

❌ **Evita set quando:**
- Non serve ordinamento → usa unordered_set
- Servono duplicati → usa multiset

### Esempio Completo

```cpp
set<int> s;

// Insert (ritorna bool)
bool inserted = s.insert(5);    // true
inserted = s.insert(5);         // false (duplicato)

// Find/Contains
if (s.find(5)) {
    cout << "Found!";
}
bool exists = s.contains(5);

// Erase
bool erased = s.erase(5);

// Iterazione ORDINATA
for (const auto& val : s) {
    cout << val << " ";  // Sempre in ordine crescente
}

// Min/Max O(1) con iteratori
int min_val = *s.begin();
int max_val = *s.rbegin();

// Esempio: Rimuovi duplicati mantenendo ordine
vector<int> removeDups(vector<int>& arr) {
    set<int> s(arr.begin(), arr.end());  // O(n log n)
    return vector<int>(s.begin(), s.end());
}

// Debug
s.print_tree();
```

### Complessità

| Operazione | Complessità | Note |
|------------|-------------|------|
| insert | O(log n) | |
| find | O(log n) | |
| erase | O(log n) | |
| contains | O(log n) | |
| min/max | O(1) | begin()/rbegin() |
| Iteration | O(n) | In order |

---

## Unordered_Map

### Struttura Interna (Hash Table)

```
Buckets:
[0] -> nullptr
[1] -> [9:nine] -> [1:one] -> nullptr
[2] -> nullptr
[3] -> [3:three] -> nullptr
[4] -> nullptr
[5] -> [5:five] -> nullptr
[6] -> nullptr
[7] -> [7:seven] -> nullptr

Hash Function: h(key) = hash(key) % bucket_count
Load Factor: size / bucket_count
```

### Caratteristiche

- **Hash Table** con separate chaining
- **O(1) medio** per tutte le operazioni
- **Non ordinato**
- **Dynamic resizing** (rehashing quando load > 0.75)

### Hash Collisions

```cpp
// Due chiavi con stesso hash → stesso bucket
h(9) % 8 = 1
h(1) % 8 = 1

Bucket[1]: [9:nine] -> [1:one]  // Collision chain
```

### Quando Usare

✅ **Usa unordered_map quando:**
- Serve solo lookup velocissimo
- Non serve ordinamento
- Performance > memoria

❌ **Evita unordered_map quando:**
- Serve iterazione ordinata → usa map
- Serve range queries → usa map
- Chiavi senza buona hash function

### Esempio Completo

```cpp
unordered_map<string, int> m;

// Insert
m.insert("age", 25);
m["name"] = 100;        // O(1)

// Access
int age = m.at("age");  // O(1)

// Find
if (m.find("age")) {    // O(1)
    cout << "Found!";
}

// Erase
bool erased = m.erase("age");  // O(1)

// Iterazione (ordine NON garantito!)
for (const auto& [key, value] : m) {
    cout << key << " => " << value << endl;
}

// Info hash table
cout << m.load_factor() << endl;    // size/buckets
cout << m.bucket_count() << endl;   // numero buckets

// Esempio: Word frequency
unordered_map<string, int> freq;
for (const string& word : words) {
    freq[word]++;  // O(1) invece di O(log n) con map
}

// Esempio: Two Sum O(n)
vector<int> twoSum(vector<int>& nums, int target) {
    unordered_map<int, int> seen;
    for (int i = 0; i < nums.size(); i++) {
        int complement = target - nums[i];
        if (seen.contains(complement)) {
            return {seen[complement], i};
        }
        seen[nums[i]] = i;
    }
    return {};
}

// Debug
m.print_structure();  // Visualizza buckets
```

### Complessità

| Operazione | Media | Worst | Note |
|------------|-------|-------|------|
| insert | O(1) | O(n) | Worst = all collisions |
| find | O(1) | O(n) | |
| erase | O(1) | O(n) | |
| operator[] | O(1) | O(n) | |
| Rehash | O(n) | O(n) | Triggered by load factor |

### Rehashing

```cpp
// Quando load_factor > 0.75:
1. Alloca nuovi buckets (size * 2)
2. Re-inserisci tutti gli elementi con nuovo hash
3. Dealloca vecchi buckets

Costo: O(n) ma ammortizzato su molte insert
```

---

## Unordered_Set

### Struttura Interna (Hash Table)

```
Buckets:
[0] -> nullptr
[1] -> [9] -> [1] -> nullptr
[2] -> nullptr
[3] -> [3] -> nullptr
[4] -> nullptr
[5] -> [5] -> nullptr
[6] -> nullptr
[7] -> [7] -> nullptr
```

### Caratteristiche

- **Hash Table** (come unordered_map, ma solo chiavi)
- **O(1) medio** per tutte le operazioni
- **Unicità** garantita
- **Non ordinato**

### Quando Usare

✅ **Usa unordered_set quando:**
- Serve solo verificare esistenza velocemente
- Non serve ordinamento
- Esempio: visited nodes, deduplication

❌ **Evita unordered_set quando:**
- Serve iterazione ordinata → usa set
- Serve trovare min/max → usa set

### Esempio Completo

```cpp
unordered_set<int> s;

// Insert (ritorna bool)
bool inserted = s.insert(5);    // true, O(1)
inserted = s.insert(5);         // false, O(1)

// Find/Contains
if (s.find(5)) {                // O(1)
    cout << "Found!";
}
bool exists = s.contains(5);    // O(1)

// Erase
bool erased = s.erase(5);       // O(1)

// Iterazione (ordine NON garantito!)
for (const auto& val : s) {
    cout << val << " ";
}

// Esempio: Rimuovi duplicati VELOCE
unordered_set<int> removeDups(vector<int>& arr) {
    return unordered_set<int>(arr.begin(), arr.end());
    // O(n) invece di O(n log n) con set!
}

// Esempio: Graph visited nodes
void dfs(int node, unordered_set<int>& visited) {
    if (visited.contains(node)) return;  // O(1)
    visited.insert(node);                // O(1)
    
    for (int neighbor : graph[node]) {
        dfs(neighbor, visited);
    }
}

// Esempio: Unicità automatica
vector<int> values = {1, 2, 3, 1, 2, 4, 5, 3, 1};
unordered_set<int> unique(values.begin(), values.end());
cout << unique.size();  // 5 (non 9!)

// Debug
s.print_structure();
```

### Complessità

| Operazione | Media | Worst | Note |
|------------|-------|-------|------|
| insert | O(1) | O(n) | |
| find | O(1) | O(n) | |
| erase | O(1) | O(n) | |
| contains | O(1) | O(n) | |
| Iteration | O(n) | O(n) | Non ordinato |

---

## 🔄 Algoritmi di Sorting

La libreria include implementazioni di 7 algoritmi di sorting con **Strategy Pattern**.

### Algoritmi Implementati

| Algoritmo | Complessità Media | Complessità Worst | Stabile | In-place |
|-----------|------------------|------------------|---------|----------|
| **Bubble Sort** | O(n²) | O(n²) | ✅ | ✅ |
| **Insertion Sort** | O(n²) | O(n²) | ✅ | ✅ |
| **Selection Sort** | O(n²) | O(n²) | ❌ | ✅ |
| **Quick Sort** | O(n log n) | O(n²) | ❌ | ✅ |
| **Merge Sort** | O(n log n) | O(n log n) | ✅ | ❌ |
| **Heap Sort** | O(n log n) | O(n log n) | ❌ | ✅ |
| **Intro Sort** | O(n log n) | O(n log n) | ❌ | ✅ |

### Esempio Utilizzo

```cpp
#include "sorting.h"

vector<int> arr = {5, 2, 8, 1, 9, 3};

// Uso diretto
quick_sort(arr.begin(), arr.end());
merge_sort(arr.begin(), arr.end());
intro_sort(arr.begin(), arr.end());

// Con custom comparator
quick_sort(arr.begin(), arr.end(), greater<int>());

// Strategy Pattern
Algorithm<int> algo;

// Cambia strategia runtime
algo.setStrategy(make_unique<QuickSort<int>>());
algo.sort(arr.data(), arr.data() + arr.size());

algo.setStrategy(make_unique<MergeSort<int>>());
algo.sort(arr.data(), arr.data() + arr.size());

cout << algo.getCurrentStrategyName();  // "Merge Sort"
```

### Intro Sort (Hybrid)

**Introsort** combina Quick Sort, Heap Sort e Insertion Sort:

```cpp
void intro_sort(begin, end) {
    if (size < 16) {
        insertion_sort(begin, end);     // Piccoli array
    } else if (depth_limit == 0) {
        heap_sort(begin, end);          // Depth limit superato
    } else {
        quick_sort(begin, end);         // Caso normale
    }
}
```

**Vantaggi:**
- Quick Sort performance nella media
- Heap Sort garantisce O(n log n) worst case
- Insertion Sort per piccoli array

**std::sort usa Intro Sort!**

---

## 🎯 Quando Usare Cosa?

### Decision Tree: Quale Container?

```
Serve accesso random?
├─ SI → Serve push/pop da entrambi i lati?
│      ├─ SI → DEQUE
│      └─ NO → VECTOR
└─ NO → Serve solo stack/queue semantics?
       ├─ SI → STACK o QUEUE
       └─ NO → Serve insert/erase O(1) ovunque?
              ├─ SI → LIST
              └─ NO → Serve key-value pairs?
                     ├─ SI → Serve ordinamento?
                     │      ├─ SI → MAP
                     │      └─ NO → UNORDERED_MAP
                     └─ NO → Serve solo uniqueness?
                            └─ Serve ordinamento?
                                   ├─ SI → SET
                                   └─ NO → UNORDERED_SET
```

### Use Cases Comuni

#### 1. Dynamic Array
```cpp
vector<int> v;  // 99% dei casi
```

#### 2. Stack Evaluation
```cpp
stack<int> s;
// Parenthesis matching, expression eval, undo/redo
```

#### 3. BFS Queue
```cpp
queue<int> q;
// Level-order traversal, task scheduling
```

#### 4. Frequency Counter
```cpp
unordered_map<string, int> freq;  // O(1) lookup
for (const auto& word : words) freq[word]++;
```

#### 5. Ordered Dictionary
```cpp
map<string, int> dict;  // Iterazione ordinata
for (const auto& [key, val] : dict) { ... }  // In order!
```

#### 6. Unique Collection (Fast)
```cpp
unordered_set<int> unique;  // O(1) insert/find
```

#### 7. Unique Collection (Ordered)
```cpp
set<int> ordered_unique;  // Auto-sorted
```

#### 8. Graph Adjacency List
```cpp
unordered_map<int, vector<int>> graph;
graph[1] = {2, 3, 4};
```

#### 9. Insert/Erase Middle
```cpp
list<int> l;  // O(1) con iterator
auto it = l.begin();
advance(it, 5);
l.insert(it, 100);
```

#### 10. Range Queries
```cpp
map<int, string> m;
auto lower = m.lower_bound(10);
auto upper = m.upper_bound(20);
// Iterate [10, 20]
```

---

## 🧪 Test Suite

### Coverage Completo

| Container | Test Count | Pass Rate | Coverage |
|-----------|-----------|-----------|----------|
| **vector** | 25+ | 100% | Insert, erase, access, capacity, copy/move |
| **list** | 30+ | 100% | Insert, erase, splice, copy/move, iterators |
| **deque** | 35+ | 100% | Push/pop both ends, iterators, STL algorithms |
| **stack** | 15+ | 100% | Push/pop, copy/move, edge cases |
| **queue** | 15+ | 100% | Push/pop, copy/move, edge cases |
| **map** | 50+ | 100% | Insert, find, erase, RB-Tree validation, BST comparison |
| **set** | 40+ | 100% | Insert, find, erase, uniqueness, ordering |
| **unordered_map** | 59 | 100% | Hash collisions, rehashing, iterators, stress |
| **unordered_set** | 62 | 100% | Hash collisions, rehashing, uniqueness |
| **sorting** | 70+ | 100% | All algorithms, comparators, edge cases |

### Test Categories

1. **Basic Operations**: insert, erase, access, find
2. **Copy/Move Semantics**: constructor, assignment, self-assignment
3. **Iterators**: begin, end, increment, dereference
4. **Edge Cases**: empty, single element, duplicates
5. **Stress Tests**: 1000+ elements, random operations
6. **STL Compatibility**: sort, find, accumulate, copy
7. **Structure Validation**: RB-Tree properties, hash load factor

### Esempio Test Output

```
========================================
TEST SUITE: UNORDERED_MAP
========================================

--- BASIC OPERATIONS ---
[PASS] Insert new element
[PASS] Find existing key
[PASS] Erase returns true
[PASS] Size after operations

--- HASH COLLISIONS ---
[PASS] 100 elements inserted
Load factor: 0.390625
Bucket count: 256

--- STRESS TEST ---
[PASS] 1000 random inserts
[PASS] 1000 random finds
[PASS] 500 random erases

========================================
TOTAL: 59 tests, 59 passed (100%)
========================================
```

---

## 📐 Complessità Computazionale

### Tabella Riassuntiva

#### Sequence Containers

| Operazione | vector | list | deque |
|------------|--------|------|-------|
| **Access [i]** | O(1) | ❌ | O(1) |
| **push_back** | O(1)* | O(1) | O(1) |
| **push_front** | ❌ | O(1) | O(1) |
| **pop_back** | O(1) | O(1) | O(1) |
| **pop_front** | ❌ | O(1) | O(1) |
| **insert (middle)** | O(n) | O(1)† | O(n) |
| **erase (middle)** | O(n) | O(1)† | O(n) |
| **Memory** | Contigua | Dispersa | Blocchi |

*\* O(n) quando resize*  
*† Con iterator valido*

#### Associative Containers

| Operazione | map | set | unordered_map | unordered_set |
|------------|-----|-----|---------------|---------------|
| **insert** | O(log n) | O(log n) | O(1)* | O(1)* |
| **find** | O(log n) | O(log n) | O(1)* | O(1)* |
| **erase** | O(log n) | O(log n) | O(1)* | O(1)* |
| **Iteration** | O(n) ordered | O(n) ordered | O(n) unordered | O(n) unordered |
| **Min/Max** | O(1) | O(1) | ❌ | ❌ |
| **Range query** | O(log n + k) | O(log n + k) | ❌ | ❌ |

*\* O(n) worst case (bad hash)*

#### Adapter Containers

| Operazione | stack | queue |
|------------|-------|-------|
| **push** | O(1) | O(1) |
| **pop** | O(1) | O(1) |
| **top/front** | O(1) | O(1) |

---

## 🎓 Concetti Avanzati

### 1. Iterator Invalidation

#### Vector
```cpp
vector<int> v = {1, 2, 3, 4, 5};
auto it = v.begin() + 2;

v.push_back(6);  // ⚠️ Può invalidare it se resize!
v.insert(it, 7); // ⚠️ Invalida tutti gli iteratori dopo it
v.erase(it);     // ⚠️ Invalida tutti gli iteratori dopo it
```

#### List
```cpp
list<int> l = {1, 2, 3, 4, 5};
auto it = l.begin();
++it;

l.insert(it, 7); // ✅ Altri iteratori rimangono validi
l.erase(it);     // ⚠️ Solo it viene invalidato, altri OK
```

#### Map/Set
```cpp
map<int, string> m;
auto it = m.find(5);

m.insert(10, "ten");  // ✅ it rimane valido
m.erase(5);           // ⚠️ Solo it viene invalidato
```

### 2. Copy vs Move

```cpp
// Copy (costoso per grandi container)
vector<int> v1 = {1, 2, 3};
vector<int> v2 = v1;        // Copia tutti gli elementi

// Move (veloce!)
vector<int> v3 = move(v1);  // Trasferisce ownership
// v1 ora è vuoto, v3 ha i dati
```

### 3. Reserve vs Resize

```cpp
vector<int> v;

v.reserve(100);  // Alloca spazio, size=0
// v[0] = 10;    // ❌ Undefined behavior!
v.push_back(10); // ✅ OK

v.resize(100);   // Alloca e inizializza, size=100
v[0] = 10;       // ✅ OK
```

### 4. Emplace vs Insert

```cpp
struct Point { int x, y; };

vector<Point> v;

v.push_back(Point{1, 2});     // Crea temporaneo, poi copia
v.emplace_back(1, 2);          // Costruisce in-place (più efficiente)
```

### 5. STL Algorithm Compatibility

```cpp
vector<int> v = {5, 2, 8, 1, 9};

// Sorting
sort(v.begin(), v.end());

// Finding
auto it = find(v.begin(), v.end(), 8);

// Counting
int count = count_if(v.begin(), v.end(), [](int x) { return x > 5; });

// Accumulate
int sum = accumulate(v.begin(), v.end(), 0);

// Transform
transform(v.begin(), v.end(), v.begin(), [](int x) { return x * 2; });
```

---

## 🔍 Pattern di Design

### 1. Strategy Pattern (Sorting)

```cpp
class SortStrategy {
public:
    virtual void sort(int* begin, int* end) = 0;
};

class QuickSort : public SortStrategy {
    void sort(int* begin, int* end) override;
};

class MergeSort : public SortStrategy {
    void sort(int* begin, int* end) override;
};

Algorithm<int> algo;
algo.setStrategy(make_unique<QuickSort>());
algo.sort(arr, arr + size);
```

### 2. RAII (Resource Acquisition Is Initialization)

```cpp
template<typename T>
class vector {
    T* data_;
    
public:
    vector() : data_(nullptr) {}  // Constructor acquires
    ~vector() { delete[] data_; } // Destructor releases
    
    // Copy
    vector(const vector& other) {
        data_ = new T[other.size_];
        // Copy elements...
    }
    
    // Move
    vector(vector&& other) noexcept 
        : data_(other.data_) {
        other.data_ = nullptr;  // Transfer ownership
    }
};
```

### 3. Adapter Pattern (Stack/Queue)

```cpp
template<typename T, typename Container = deque<T>>
class stack {
private:
    Container c_;  // Composition
    
public:
    void push(const T& x) { c_.push_back(x); }
    void pop() { c_.pop_back(); }
    T& top() { return c_.back(); }
};
```

---

## 💻 Best Practices

### 1. Prefer `const` Correctness

```cpp
// ✅ Buono
void print(const vector<int>& v) {  // const ref
    for (const auto& x : v) {       // const iteration
        cout << x << " ";
    }
}

// ❌ Cattivo
void print(vector<int> v) {  // Copia inutile!
    for (auto x : v) {       // Altra copia!
        cout << x << " ";
    }
}
```

### 2. Use Range-Based For

```cpp
// ✅ Moderno e chiaro
for (const auto& elem : container) {
    cout << elem << endl;
}

// ❌ Verboso
for (auto it = container.begin(); it != container.end(); ++it) {
    cout << *it << endl;
}
```

### 3. Reserve When You Know Size

```cpp
// ✅ Efficiente
vector<int> v;
v.reserve(1000);  // Evita multipli resize
for (int i = 0; i < 1000; i++) {
    v.push_back(i);
}

// ❌ Inefficiente
vector<int> v;
for (int i = 0; i < 1000; i++) {
    v.push_back(i);  // Può causare ~10 resize
}
```

### 4. Use Appropriate Container

```cpp
// ❌ Cattivo: linear search
vector<int> visited;
if (find(visited.begin(), visited.end(), node) != visited.end()) {
    // O(n) search ogni volta!
}

// ✅ Buono: hash set
unordered_set<int> visited;
if (visited.contains(node)) {
    // O(1) search!
}
```

### 5. Avoid Unnecessary Copies

```cpp
// ❌ Cattivo
map<string, vector<int>> m;
vector<int> v = m["key"];  // Copia!
v.push_back(10);
m["key"] = v;              // Altra copia!

// ✅ Buono
map<string, vector<int>> m;
m["key"].push_back(10);    // Nessuna copia!
```

---

## 🐛 Common Pitfalls

### 1. Iterator Invalidation

```cpp
// ❌ ERRORE
vector<int> v = {1, 2, 3, 4, 5};
for (auto it = v.begin(); it != v.end(); ++it) {
    if (*it % 2 == 0) {
        v.erase(it);  // Invalida it!
    }
}

// ✅ CORRETTO
vector<int> v = {1, 2, 3, 4, 5};
for (auto it = v.begin(); it != v.end(); ) {
    if (*it % 2 == 0) {
        it = v.erase(it);  // erase ritorna next iterator
    } else {
        ++it;
    }
}
```

### 2. Dangling References

```cpp
// ❌ ERRORE
const string& get_name() {
    string name = "John";
    return name;  // Ritorna reference a variabile locale!
}

// ✅ CORRETTO
string get_name() {
    string name = "John";
    return name;  // Ritorna per valore (move)
}
```

### 3. Reserve vs Resize Confusion

```cpp
vector<int> v;
v.reserve(10);
v[5] = 42;     // ❌ ERRORE: size() ancora 0!

v.resize(10);
v[5] = 42;     // ✅ OK: size() ora 10
```

### 4. Map operator[] Side Effects

```cpp
map<string, int> m;

// ❌ Crea entry se non esiste!
if (m["key"] == 5) { ... }

// ✅ Non crea entry
if (m.find("key") && m.at("key") == 5) { ... }
```

### 5. Comparing Floats as Keys

```cpp
// ⚠️ SCONSIGLIATO
unordered_set<double> s;
s.insert(0.1 + 0.2);
s.contains(0.3);  // Potrebbe essere false!

// ✅ MEGLIO
unordered_set<int> s;  // Usa integer se possibile
```

---

## 📚 Concetti C++ Fondamentali

Oltre alle strutture dati, questa libreria include guide approfondite su concetti fondamentali di C++:

### 📖 [Lvalue, Rvalue, Lvalue Reference e Rvalue Reference](LVALUE_RVALUE_REFERENCES.md)

Una guida completa per comprendere:
- **Lvalue vs Rvalue**: differenze e quando usare ciascuno
- **Lvalue Reference (`&`)**: riferimenti a lvalue
- **Rvalue Reference (`&&`)**: riferimenti a rvalue e move semantics
- **Move Semantics**: come ottimizzare le performance evitando copie costose
- **Perfect Forwarding**: preservare value categories in template
- **Best Practices**: quando e come usare move, forward, e reference types

Questa guida è essenziale per capire come funzionano internamente i container STL e come implementare copy/move constructor in modo efficiente.

---

## 📚 Risorse Aggiuntive

### File della Libreria

```
STDev/
├── vector.h              # Dynamic array
├── list.h                # Doubly-linked list
├── deque.h               # Double-ended queue
├── stack.h               # Stack adapter
├── queue.h               # Queue adapter
├── map.h                 # Red-Black Tree map
├── set.h                 # Red-Black Tree set
├── unordered_map.h       # Hash table map
├── unordered_set.h       # Hash table set
├── sorting.h             # 7 sorting algorithms
├── testVector.cpp        # Vector tests
├── testList.cpp          # List tests
├── testDeque.cpp         # Deque tests
├── testStack.cpp         # Stack tests
├── testQueue.cpp         # Queue tests
├── testMap.cpp           # Map tests
├── testSet.cpp           # Set tests
├── testUnorderedMap.cpp  # Unordered map tests
├── testUnorderedSet.cpp  # Unordered set tests
└── TestSorting.cpp       # Sorting tests
```

### Compilazione

```bash
# Singolo test
g++ -std=c++17 -Wall -Wextra -o test testVector.cpp

# Tutti i test
for file in test*.cpp; do
    g++ -std=c++17 -Wall -Wextra -o "${file%.cpp}" "$file"
    ./"${file%.cpp}"
done
```

### Learning Path Consigliato

1. **Beginner**: vector → stack → queue
2. **Intermediate**: list → deque → set
3. **Advanced**: map → unordered_map → sorting algorithms
4. **Expert**: Implementa i tuoi custom comparators e hash functions

---

## 🎯 Conclusioni

### Cosa Hai Imparato

- ✅ Implementazione di **9 container** STL-compliant
- ✅ **Iteratori** random-access, bidirectional, forward
- ✅ **Copy & Move** semantics
- ✅ **Alberi bilanciati** (Red-Black Tree)
- ✅ **Hash tables** con collision handling
- ✅ **7 algoritmi** di sorting
- ✅ **Strategy pattern** per sorting
- ✅ **Test-driven development**

### Prossimi Passi

Se vuoi continuare:

1. **priority_queue** (heap-based)
2. **multimap/multiset** (allow duplicates)
3. **array** (fixed-size container)
4. **forward_list** (singly-linked list)
5. **Custom allocators**
6. **Thread-safe containers**

---

## 📝 Licenza e Contributi

**Autore**: STDev Library  
**Scopo**: Educativo - Comprensione profonda delle strutture dati C++  
**Lingua**: C++17  
**Anno**: 2025

---

## 🙏 Ringraziamenti

Questa libreria è stata creata per:
- Capire internamente come funziona la STL
- Imparare design patterns e best practices C++
- Preparazione per interview tecniche
- Reference implementation per progetti educativi

**"The best way to learn is to build it yourself!"** 🚀
