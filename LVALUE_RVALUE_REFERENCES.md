# Lvalue, Rvalue, Lvalue Reference e Rvalue Reference in C++ 📚

Una guida completa per comprendere i concetti fondamentali di value categories e reference types in C++.

---

## 📋 Indice

- [Introduzione](#-introduzione)
- [Lvalue vs Rvalue](#-lvalue-vs-rvalue)
- [Lvalue Reference](#-lvalue-reference)
- [Rvalue Reference](#-rvalue-reference)
- [Move Semantics](#-move-semantics)
- [Perfect Forwarding](#-perfect-forwarding)
- [Quando Usare Cosa](#-quando-usare-cosa)
- [Best Practices](#-best-practices)
- [Common Pitfalls](#-common-pitfalls)

---

## 🎯 Introduzione

In C++, ogni espressione appartiene a una delle **value categories**:
- **lvalue** (locator value): espressione che identifica un oggetto in memoria
- **rvalue** (read value): espressione temporanea che non persiste oltre l'espressione

Le **reference** sono un modo per riferirsi a oggetti esistenti:
- **lvalue reference** (`&`): riferimento a un lvalue
- **rvalue reference** (`&&`): riferimento a un rvalue (C++11)

---

## 📖 Lvalue vs Rvalue

### Cos'è un Lvalue?

Un **lvalue** è un'espressione che:
- Ha un **indirizzo in memoria** accessibile
- **Persiste** oltre la singola espressione
- Può apparire a **sinistra** di un assegnamento

```cpp
int x = 10;        // x è un lvalue
int* ptr = &x;     // Posso prendere l'indirizzo di x

x = 20;            // x può essere a sinistra dell'assegnamento
```

#### Esempi di Lvalue

```cpp
int x = 5;                 // x è lvalue
int arr[10];               // arr è lvalue
std::string str = "hello"; // str è lvalue

x;                         // lvalue
arr[3];                    // lvalue
str;                       // lvalue
*ptr;                      // lvalue (dereferenziazione)
++x;                       // lvalue (pre-incremento)
```

### Cos'è un Rvalue?

Un **rvalue** è un'espressione che:
- **Non** ha un indirizzo accessibile (o è temporaneo)
- **Non persiste** oltre l'espressione
- Può apparire solo a **destra** di un assegnamento

```cpp
10;                // rvalue (letterale)
x + y;             // rvalue (risultato temporaneo)
std::string("hi"); // rvalue (temporaneo)
```

#### Esempi di Rvalue

```cpp
42;                        // rvalue (letterale)
x + y;                     // rvalue (risultato temporaneo)
x++;                       // rvalue (post-incremento)
std::string("hello");      // rvalue (oggetto temporaneo)
getValue();                // rvalue (valore di ritorno)
std::move(x);              // rvalue (cast esplicito)
```

### Distinzione Visiva

```cpp
int x = 5;              // x è lvalue, 5 è rvalue

int y = x;              // y è lvalue, x è lvalue (copiato)
int z = x + 5;          // z è lvalue, (x + 5) è rvalue

int* p1 = &x;           // ✅ OK: x è lvalue
int* p2 = &5;           // ❌ ERRORE: 5 è rvalue, non ha indirizzo
int* p3 = &(x + 5);     // ❌ ERRORE: (x + 5) è rvalue temporaneo
```

### Regola Semplice

**Se puoi prendere l'indirizzo con `&`, è un lvalue. Altrimenti è un rvalue.**

```cpp
&x;              // ✅ OK: x è lvalue
&42;             // ❌ ERRORE: 42 è rvalue
&(x + y);        // ❌ ERRORE: (x + y) è rvalue
&(++x);          // ✅ OK: ++x ritorna lvalue
&(x++);          // ❌ ERRORE: x++ ritorna rvalue
```

---

## 🔗 Lvalue Reference

### Definizione

Una **lvalue reference** è un alias (riferimento) a un lvalue esistente.
Si dichiara con l'operatore `&`.

```cpp
int x = 10;
int& ref = x;    // ref è una lvalue reference a x

ref = 20;        // Modifica x attraverso ref
std::cout << x;  // Stampa: 20
```

### Caratteristiche

1. **Deve essere inizializzata** al momento della dichiarazione
2. **Non può essere riassegnata** (è come un alias permanente)
3. **Si lega solo a lvalue**, non a rvalue

```cpp
int x = 10;
int& ref1 = x;        // ✅ OK: x è lvalue

int& ref2 = 42;       // ❌ ERRORE: 42 è rvalue
int& ref3 = x + 5;    // ❌ ERRORE: x + 5 è rvalue
```

### Const Lvalue Reference

Una **const lvalue reference** può legarsi anche a rvalue (temporanei):

```cpp
const int& ref1 = 42;           // ✅ OK: estende la vita del temporaneo
const int& ref2 = x + 5;        // ✅ OK: estende la vita del risultato
const std::string& ref3 = std::string("hello");  // ✅ OK
```

**Perché funziona?**
Il temporaneo viene esteso fino alla fine dello scope della reference.

```cpp
const std::string& ref = std::string("hello");
std::cout << ref;    // ✅ OK: temporaneo ancora vivo
// Qui il temporaneo viene distrutto
```

### Uso Comune: Parametri di Funzione

```cpp
// ❌ Cattivo: copia inutile
void print(std::string str) {
    std::cout << str << std::endl;
}

// ✅ Buono: passa per const reference
void print(const std::string& str) {
    std::cout << str << std::endl;
}

// Uso
std::string s = "hello";
print(s);              // Nessuna copia!
print("world");        // ✅ OK: const ref accetta temporaneo
```

---

## ⚡ Rvalue Reference

### Definizione

Una **rvalue reference** è un riferimento che si lega a un rvalue (temporaneo).
Si dichiara con l'operatore `&&` (C++11).

```cpp
int&& ref = 42;              // ✅ OK: 42 è rvalue
int&& ref2 = x + 5;          // ✅ OK: x + 5 è rvalue
std::string&& ref3 = std::string("hello");  // ✅ OK
```

### Caratteristiche

1. **Si lega solo a rvalue** (temporanei)
2. **Permette di modificare** il temporaneo
3. **Fondamentale per move semantics**

```cpp
int x = 10;
int&& ref1 = x;        // ❌ ERRORE: x è lvalue
int&& ref2 = 42;       // ✅ OK: 42 è rvalue

// std::move converte lvalue in rvalue
int&& ref3 = std::move(x);  // ✅ OK: std::move(x) è rvalue
```

### Confronto: Lvalue Reference vs Rvalue Reference

```cpp
int x = 10;

// Lvalue reference
int& lref = x;           // ✅ OK
int& lref2 = 42;         // ❌ ERRORE
const int& clref = 42;   // ✅ OK (special case)

// Rvalue reference
int&& rref = x;          // ❌ ERRORE
int&& rref2 = 42;        // ✅ OK
int&& rref3 = std::move(x);  // ✅ OK
```

### Tabella Riassuntiva

| Tipo Reference | Si lega a Lvalue | Si lega a Rvalue | Note |
|----------------|------------------|------------------|------|
| `T&` (lvalue ref) | ✅ | ❌ | Solo lvalue |
| `const T&` (const lvalue ref) | ✅ | ✅ | Accetta tutto! |
| `T&&` (rvalue ref) | ❌ | ✅ | Solo rvalue |

---

## 🚀 Move Semantics

### Il Problema della Copia

```cpp
std::vector<int> create_vector() {
    std::vector<int> v(1000000);
    // ... riempi il vector
    return v;  // ❌ Copia costosa! (C++03)
}

std::vector<int> vec = create_vector();  // Copia!
```

### La Soluzione: Move

Con **move semantics** (C++11), possiamo "rubare" le risorse invece di copiarle:

```cpp
std::vector<int> create_vector() {
    std::vector<int> v(1000000);
    return v;  // ✅ Move automatico! (C++11)
}

std::vector<int> vec = create_vector();  // Move, non copia!
```

### Move Constructor e Move Assignment

```cpp
class MyString {
    char* data_;
    size_t size_;

public:
    // Copy constructor (costoso)
    MyString(const MyString& other) 
        : size_(other.size_) {
        data_ = new char[size_];
        std::copy(other.data_, other.data_ + size_, data_);
    }

    // Move constructor (veloce!)
    MyString(MyString&& other) noexcept 
        : data_(other.data_), size_(other.size_) {
        other.data_ = nullptr;  // "Ruba" le risorse
        other.size_ = 0;
    }

    // Copy assignment
    MyString& operator=(const MyString& other) {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = new char[size_];
            std::copy(other.data_, other.data_ + size_, data_);
        }
        return *this;
    }

    // Move assignment
    MyString& operator=(MyString&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = other.data_;      // "Ruba" le risorse
            size_ = other.size_;
            other.data_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }

    ~MyString() {
        delete[] data_;
    }
};
```

### std::move

`std::move` converte un lvalue in rvalue, permettendo il move:

```cpp
std::string str1 = "hello";
std::string str2 = str1;             // Copy
std::string str3 = std::move(str1);  // Move!

// Attenzione: str1 ora è in stato "moved-from" (valido ma non specificato)
std::cout << str1;  // ⚠️ Probabilmente vuoto
```

### Esempio Pratico

```cpp
std::vector<MyString> vec;

MyString s1("hello");
vec.push_back(s1);             // Copy: s1 è lvalue
vec.push_back(MyString("hi")); // Move: temporaneo è rvalue
vec.push_back(std::move(s1));  // Move: std::move rende s1 un rvalue

// s1 ora è in stato "moved-from"
```

### Vantaggi del Move

```cpp
// Senza move (C++03)
std::vector<std::string> v1;
for (int i = 0; i < 1000; i++) {
    std::string temp = get_string();
    v1.push_back(temp);  // Copia ogni volta!
}

// Con move (C++11)
std::vector<std::string> v2;
for (int i = 0; i < 1000; i++) {
    v2.push_back(get_string());  // Move automatico del temporaneo!
}

// Move esplicito
std::vector<std::string> v3;
for (int i = 0; i < 1000; i++) {
    std::string temp = get_string();
    v3.push_back(std::move(temp));  // Move esplicito
    // temp ora è "moved-from"
}
```

---

## 🎯 Perfect Forwarding

### Il Problema

Come scrivere una funzione template che preserva la "lvalue-ness" o "rvalue-ness" dell'argomento?

```cpp
template<typename T>
void wrapper(T arg) {
    process(arg);  // ❌ arg è sempre lvalue, anche se passi rvalue!
}

std::string s = "hello";
wrapper(s);                  // arg è lvalue ✅
wrapper(std::string("hi"));  // arg è lvalue ❌ (dovrebbe essere rvalue!)
```

### La Soluzione: Universal Reference + std::forward

```cpp
template<typename T>
void wrapper(T&& arg) {  // Universal reference (forwarding reference)
    process(std::forward<T>(arg));  // Preserva lvalue/rvalue
}

std::string s = "hello";
wrapper(s);                  // T = std::string&, arg è lvalue
wrapper(std::string("hi"));  // T = std::string, arg è rvalue
```

### Universal Reference

`T&&` in un template context è una **universal reference** (o forwarding reference):

```cpp
template<typename T>
void func(T&& param);  // Universal reference

// Quando chiami:
int x = 10;
func(x);        // T dedotto come int&, param è int& (lvalue ref)
func(10);       // T dedotto come int, param è int&& (rvalue ref)
```

### Reference Collapsing Rules

```cpp
// Regole di collapsing:
T&  &   →  T&      // lvalue ref + lvalue ref = lvalue ref
T&  &&  →  T&      // lvalue ref + rvalue ref = lvalue ref
T&& &   →  T&      // rvalue ref + lvalue ref = lvalue ref
T&& &&  →  T&&     // rvalue ref + rvalue ref = rvalue ref
```

### Esempio Completo

```cpp
#include <utility>  // std::forward

template<typename T>
void process(T&& value) {
    std::cout << "Rvalue reference version\n";
}

void process(const int& value) {
    std::cout << "Lvalue reference version\n";
}

template<typename T>
void wrapper(T&& arg) {
    process(std::forward<T>(arg));  // Perfect forwarding
}

int main() {
    int x = 10;
    wrapper(x);        // Chiama lvalue version
    wrapper(10);       // Chiama rvalue version
    wrapper(std::move(x));  // Chiama rvalue version
}
```

---

## 📊 Quando Usare Cosa?

### Decision Tree

```
Devi passare un parametro?
├─ Non modifichi l'argomento?
│  ├─ Tipo piccolo (int, char, pointer)?
│  │  └─ Passa per VALORE: void func(int x)
│  └─ Tipo grande (string, vector)?
│     └─ Passa per CONST LVALUE REF: void func(const T& x)
│
└─ Modifichi l'argomento?
   ├─ Vuoi modificare l'originale?
   │  └─ Passa per LVALUE REF: void func(T& x)
   └─ Vuoi "rubare" le risorse?
      └─ Passa per RVALUE REF: void func(T&& x)
```

### Use Cases

#### 1. Lettura (senza modifica)

```cpp
// ✅ Tipo piccolo: per valore
void print(int x) {
    std::cout << x << std::endl;
}

// ✅ Tipo grande: const lvalue ref
void print(const std::string& str) {
    std::cout << str << std::endl;
}
```

#### 2. Modifica dell'originale

```cpp
// ✅ Lvalue reference
void increment(int& x) {
    x++;
}

int val = 5;
increment(val);  // val diventa 6
```

#### 3. Sink Functions (prendere ownership)

```cpp
class Container {
    std::vector<int> data_;

public:
    // ✅ Rvalue reference per move
    void setData(std::vector<int>&& vec) {
        data_ = std::move(vec);  // Move, non copia
    }

    // Oppure: overload per copy e move
    void setData(const std::vector<int>& vec) {  // Copy
        data_ = vec;
    }
    void setData(std::vector<int>&& vec) {       // Move
        data_ = std::move(vec);
    }
};

Container c;
std::vector<int> v1 = {1, 2, 3};
c.setData(v1);                        // Copy
c.setData(std::vector<int>{4, 5, 6}); // Move
c.setData(std::move(v1));             // Move
```

#### 4. Perfect Forwarding

```cpp
template<typename T>
void wrapper(T&& arg) {
    process(std::forward<T>(arg));
}
```

#### 5. Return by Value (RVO/NRVO)

```cpp
// ✅ Return by value: il compilatore ottimizza (RVO)
std::vector<int> create_vector() {
    std::vector<int> v;
    v.push_back(1);
    return v;  // Nessuna copia! (RVO o move)
}
```

---

## 💡 Best Practices

### 1. Preferisci const lvalue reference per parametri

```cpp
// ✅ Buono
void func(const std::string& str);

// ❌ Cattivo (copia inutile)
void func(std::string str);
```

### 2. Usa std::move quando appropriato

```cpp
std::vector<std::string> source;
// ... riempi source

// ✅ Move
std::vector<std::string> dest = std::move(source);
// source ora è vuoto

// ❌ Non usare source dopo move (eccetto riassegnamento)
std::cout << source.size();  // ⚠️ Probabilmente 0, ma non garantito
```

### 3. Implementa move constructor e assignment

```cpp
class MyClass {
public:
    // Move constructor
    MyClass(MyClass&& other) noexcept;
    
    // Move assignment
    MyClass& operator=(MyClass&& other) noexcept;
};
```

### 4. Usa noexcept per move operations

```cpp
// ✅ noexcept permette ottimizzazioni (es. vector reallocation)
MyClass(MyClass&& other) noexcept {
    // ...
}

// ❌ Senza noexcept, std::vector usa copy invece di move
MyClass(MyClass&& other) {
    // ...
}
```

### 5. Return by value, non per reference

```cpp
// ✅ Buono: RVO + move
std::vector<int> create() {
    std::vector<int> v;
    return v;
}

// ❌ Cattivo: dangling reference
std::vector<int>& create() {
    std::vector<int> v;
    return v;  // ❌ ERRORE: ritorna reference a variabile locale
}
```

### 6. Usa auto&& per universal reference

```cpp
// ✅ auto&& preserva lvalue/rvalue
auto&& ref = get_value();

// In range-based for con modifica
for (auto&& elem : container) {
    elem.modify();
}
```

---

## ⚠️ Common Pitfalls

### 1. Usare oggetto dopo std::move

```cpp
std::string str = "hello";
std::string str2 = std::move(str);

// ❌ ERRORE: str è in stato "moved-from"
std::cout << str;  // ⚠️ Probabilmente vuoto, ma non garantito

// ✅ OK: riassegnamento
str = "world";
std::cout << str;  // ✅ "world"
```

### 2. Rvalue reference è un lvalue!

```cpp
void process(std::string&& str) {
    // str è una rvalue reference, MA è un lvalue!
    // (ha un nome e un indirizzo)
    
    foo(str);              // ❌ Passa lvalue
    foo(std::move(str));   // ✅ Passa rvalue
}
```

### 3. Const rvalue reference è inutile

```cpp
// ❌ Inutile: non puoi "rubare" da const
void func(const std::string&& str);

// ✅ Usa const lvalue reference invece
void func(const std::string& str);
```

### 4. Universal reference vs Rvalue reference

```cpp
template<typename T>
void func(T&& param);     // ✅ Universal reference (in template)

void func(int&& param);   // ✅ Rvalue reference (tipo concreto)

class MyClass {
    template<typename T>
    void method(T&& param);  // ❌ Non è universal reference!
                             // (T dipende dalla classe, non dalla funzione)
};
```

### 5. std::move non move, solo cast

```cpp
std::string str = "hello";
std::move(str);  // ❌ Non fa nulla! È solo un cast

// ✅ Usa std::move solo in assegnamento/costruzione
std::string str2 = std::move(str);  // OK
```

### 6. Return std::move è pessimizzazione

```cpp
// ❌ Cattivo: disabilita RVO!
std::vector<int> func() {
    std::vector<int> v;
    return std::move(v);  // ❌ Peggio che senza move!
}

// ✅ Buono: permette RVO
std::vector<int> func() {
    std::vector<int> v;
    return v;  // RVO o move automatico
}
```

---

## 🎓 Esempi Pratici

### Esempio 1: Ottimizzazione Vector

```cpp
std::vector<std::string> vec;

// ❌ Lento: copia
std::string s1 = "hello";
vec.push_back(s1);  // Copy constructor

// ✅ Veloce: move
vec.push_back(std::string("world"));  // Move constructor (temporaneo)
vec.push_back(std::move(s1));         // Move constructor (esplicito)

// ✅ Più veloce: emplace
vec.emplace_back("foo");  // Costruisce in-place
```

### Esempio 2: Swap Efficiente

```cpp
template<typename T>
void swap(T& a, T& b) {
    T temp = std::move(a);  // Move, non copy
    a = std::move(b);       // Move, non copy
    b = std::move(temp);    // Move, non copy
}

// std::swap fa esattamente questo!
std::string s1 = "hello";
std::string s2 = "world";
std::swap(s1, s2);  // 3 move, 0 copy
```

### Esempio 3: Factory Pattern

```cpp
class Widget {
    std::vector<int> data_;
    std::string name_;

public:
    // Factory con move
    static Widget create(std::string name) {
        Widget w;
        w.name_ = std::move(name);  // Move del parametro
        w.data_.resize(1000);
        return w;  // RVO o move
    }
};

// Uso
Widget w = Widget::create("MyWidget");  // Efficiente!
```

### Esempio 4: Copy-and-Swap Idiom

```cpp
class MyClass {
    int* data_;

public:
    // Copy constructor
    MyClass(const MyClass& other) {
        data_ = new int(*other.data_);
    }

    // Move constructor
    MyClass(MyClass&& other) noexcept : data_(other.data_) {
        other.data_ = nullptr;
    }

    // Unified assignment (usando copy-and-swap)
    MyClass& operator=(MyClass other) {  // Per valore: copia o move
        std::swap(data_, other.data_);
        return *this;
        // Distruzione automatica di other (con vecchio data_)
    }

    ~MyClass() {
        delete data_;
    }
};

MyClass a, b, c;
b = a;                // Copy: a è lvalue
c = std::move(a);     // Move: std::move(a) è rvalue
```

---

## 📚 Riassunto

### Concetti Chiave

| Concetto | Descrizione | Esempio |
|----------|-------------|---------|
| **Lvalue** | Ha indirizzo, persiste | `int x = 5;` |
| **Rvalue** | Temporaneo, non persiste | `42`, `x + y` |
| **Lvalue Reference** (`&`) | Riferimento a lvalue | `int& ref = x;` |
| **Rvalue Reference** (`&&`) | Riferimento a rvalue | `int&& ref = 42;` |
| **std::move** | Cast lvalue → rvalue | `std::move(x)` |
| **std::forward** | Perfect forwarding | `std::forward<T>(arg)` |

### Quando Usare

- **`const T&`**: Parametri read-only (più comune)
- **`T&`**: Parametri da modificare (out parameters)
- **`T&&`**: Sink functions, move semantics
- **`T&& + std::forward`**: Perfect forwarding (template)

### Benefici del Move

- ⚡ **Performance**: evita copie costose
- 📦 **Ownership transfer**: trasferimento esplicito di risorse
- 🔄 **Ottimizzazioni**: RVO, NRVO automatici dal compilatore

---

## 🎯 Conclusioni

### Cosa Hai Imparato

- ✅ Differenza tra **lvalue** e **rvalue**
- ✅ Come funzionano **lvalue reference** (`&`) e **rvalue reference** (`&&`)
- ✅ **Move semantics**: come evitare copie costose
- ✅ **Perfect forwarding**: preservare value categories
- ✅ **Best practices** e **common pitfalls**

### Prossimi Passi

Per approfondire:
1. Implementa move constructor/assignment nei tuoi container
2. Usa `std::move` e `std::forward` appropriatamente
3. Studia RVO (Return Value Optimization) e NRVO
4. Esplora `std::unique_ptr` e `std::shared_ptr` (move-only types)

---

**"Move semantics è una delle feature più importanti di C++11!"** 🚀
