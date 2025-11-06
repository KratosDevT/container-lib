/**
 * Esempi Pratici: Lvalue, Rvalue, Reference Types e Move Semantics
 * 
 * Questo file contiene esempi pratici che dimostrano:
 * - Differenza tra lvalue e rvalue
 * - Uso di lvalue reference e rvalue reference
 * - Move semantics in azione
 * - Perfect forwarding
 * 
 * Per compilare:
 * g++ -std=c++17 -Wall -Wextra -o examples examples_lvalue_rvalue.cpp
 */

#include <iostream>
#include <string>
#include <vector>
#include <utility>  // std::move, std::forward
#include <cstring>  // strlen, strcpy

// ==================== ESEMPIO 1: Lvalue vs Rvalue ====================

void esempio_lvalue_rvalue() {
    std::cout << "\n=== ESEMPIO 1: Lvalue vs Rvalue ===" << std::endl;
    
    int x = 10;              // x è lvalue
    int y = 20;              // y è lvalue
    
    // Posso prendere l'indirizzo di lvalue
    int* ptr1 = &x;          // ✅ OK
    std::cout << "Indirizzo di x: " << ptr1 << std::endl;
    
    // NON posso prendere l'indirizzo di rvalue
    // int* ptr2 = &42;      // ❌ ERRORE
    // int* ptr3 = &(x + y); // ❌ ERRORE
    
    // x + y è un rvalue (temporaneo)
    int z = x + y;           // z è lvalue, (x + y) è rvalue
    std::cout << "z = " << z << std::endl;
}

// ==================== ESEMPIO 2: Lvalue Reference ====================

void increment(int& x) {     // Lvalue reference
    x++;
}

void esempio_lvalue_reference() {
    std::cout << "\n=== ESEMPIO 2: Lvalue Reference ===" << std::endl;
    
    int val = 5;
    std::cout << "Prima: val = " << val << std::endl;
    
    increment(val);          // ✅ OK: val è lvalue
    std::cout << "Dopo increment: val = " << val << std::endl;
    
    // increment(10);        // ❌ ERRORE: 10 è rvalue, non può legarsi a int&
    
    // Const lvalue reference accetta anche rvalue
    const int& ref = 42;     // ✅ OK: estende la vita del temporaneo
    std::cout << "const ref = " << ref << std::endl;
}

// ==================== ESEMPIO 3: Rvalue Reference ====================

void print_rvalue(int&& x) {  // Rvalue reference
    std::cout << "Rvalue: " << x << std::endl;
}

void esempio_rvalue_reference() {
    std::cout << "\n=== ESEMPIO 3: Rvalue Reference ===" << std::endl;
    
    int x = 10;
    
    // print_rvalue(x);      // ❌ ERRORE: x è lvalue
    print_rvalue(42);        // ✅ OK: 42 è rvalue
    print_rvalue(x + 5);     // ✅ OK: x + 5 è rvalue
    print_rvalue(std::move(x)); // ✅ OK: std::move converte lvalue in rvalue
}

// ==================== ESEMPIO 4: Move Semantics ====================

class MyString {
private:
    char* data_;
    size_t size_;

public:
    // Constructor
    MyString(const char* str = "") {
        size_ = strlen(str);
        data_ = new char[size_ + 1];
        strcpy(data_, str);
        std::cout << "  Constructor: \"" << data_ << "\"" << std::endl;
    }

    // Copy constructor (costoso)
    MyString(const MyString& other) : size_(other.size_) {
        data_ = new char[size_ + 1];
        strcpy(data_, other.data_);
        std::cout << "  Copy Constructor: \"" << data_ << "\"" << std::endl;
    }

    // Move constructor (veloce!)
    MyString(MyString&& other) noexcept 
        : data_(other.data_), size_(other.size_) {
        other.data_ = nullptr;  // "Ruba" le risorse
        other.size_ = 0;
        std::cout << "  Move Constructor (efficiente!)" << std::endl;
    }

    // Copy assignment
    MyString& operator=(const MyString& other) {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = new char[size_ + 1];
            strcpy(data_, other.data_);
            std::cout << "  Copy Assignment: \"" << data_ << "\"" << std::endl;
        }
        return *this;
    }

    // Move assignment
    MyString& operator=(MyString&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = other.data_;
            size_ = other.size_;
            other.data_ = nullptr;
            other.size_ = 0;
            std::cout << "  Move Assignment (efficiente!)" << std::endl;
        }
        return *this;
    }

    ~MyString() {
        if (data_) {
            std::cout << "  Destructor: \"" << data_ << "\"" << std::endl;
        } else {
            std::cout << "  Destructor: (moved-from)" << std::endl;
        }
        delete[] data_;
    }

    const char* c_str() const { return data_ ? data_ : ""; }
};

void esempio_move_semantics() {
    std::cout << "\n=== ESEMPIO 4: Move Semantics ===" << std::endl;
    
    std::cout << "\n1. Copy vs Move:" << std::endl;
    MyString s1("Hello");
    
    std::cout << "\nCopia:" << std::endl;
    MyString s2 = s1;        // Copy constructor
    
    std::cout << "\nMove:" << std::endl;
    MyString s3 = std::move(s1);  // Move constructor
    
    std::cout << "\ns1 dopo move: \"" << s1.c_str() << "\"" << std::endl;
    std::cout << "s3 dopo move: \"" << s3.c_str() << "\"" << std::endl;
    
    std::cout << "\n2. Vector con Move:" << std::endl;
    std::vector<MyString> vec;
    
    MyString temp("World");
    vec.push_back(temp);              // Copy
    vec.push_back(MyString("Temp"));  // Move (temporaneo)
    vec.push_back(std::move(temp));   // Move (esplicito)
}

// ==================== ESEMPIO 5: Perfect Forwarding ====================

void process_lvalue(int& x) {
    std::cout << "  process_lvalue chiamato: " << x << std::endl;
}

void process_rvalue(int&& x) {
    std::cout << "  process_rvalue chiamato: " << x << std::endl;
}

// Senza perfect forwarding (SBAGLIATO)
template<typename T>
void wrapper_cattivo(T arg) {
    process_lvalue(arg);  // arg è sempre lvalue!
}

// Con perfect forwarding (CORRETTO)
template<typename T>
void wrapper_buono(T&& arg) {  // Universal reference
    if constexpr (std::is_lvalue_reference_v<T>) {
        process_lvalue(arg);
    } else {
        process_rvalue(std::forward<T>(arg));
    }
}

void esempio_perfect_forwarding() {
    std::cout << "\n=== ESEMPIO 5: Perfect Forwarding ===" << std::endl;
    
    int x = 42;
    
    std::cout << "\nWrapper cattivo:" << std::endl;
    wrapper_cattivo(x);    // Passa lvalue
    wrapper_cattivo(10);   // Passa rvalue, ma viene trattato come lvalue!
    
    std::cout << "\nWrapper buono:" << std::endl;
    wrapper_buono(x);      // Preserva lvalue
    wrapper_buono(10);     // Preserva rvalue
}

// ==================== ESEMPIO 6: Use Cases Pratici ====================

// Factory function
std::vector<int> crea_vettore_grande() {
    std::vector<int> v(1000000);
    for (int i = 0; i < 1000000; i++) {
        v[i] = i;
    }
    return v;  // RVO o move automatico
}

void esempio_use_cases() {
    std::cout << "\n=== ESEMPIO 6: Use Cases Pratici ===" << std::endl;
    
    std::cout << "\n1. Return by value (RVO/Move):" << std::endl;
    std::vector<int> vec = crea_vettore_grande();
    std::cout << "  Vettore creato con " << vec.size() << " elementi (no copia!)" << std::endl;
    
    std::cout << "\n2. Swap efficiente:" << std::endl;
    std::string a = "prima";
    std::string b = "seconda";
    std::cout << "  Prima: a=\"" << a << "\", b=\"" << b << "\"" << std::endl;
    std::swap(a, b);  // Usa move internamente
    std::cout << "  Dopo swap: a=\"" << a << "\", b=\"" << b << "\"" << std::endl;
    
    std::cout << "\n3. Emplace vs Push:" << std::endl;
    std::vector<std::string> vec_str;
    
    std::string s1 = "hello";
    vec_str.push_back(s1);                 // Copy
    vec_str.push_back(std::string("hi"));  // Move
    vec_str.emplace_back("world");         // Costruisce in-place (più efficiente)
    
    std::cout << "  Vector size: " << vec_str.size() << std::endl;
}

// ==================== MAIN ====================

int main() {
    std::cout << "======================================" << std::endl;
    std::cout << "ESEMPI: Lvalue, Rvalue e Move Semantics" << std::endl;
    std::cout << "======================================" << std::endl;
    
    esempio_lvalue_rvalue();
    esempio_lvalue_reference();
    esempio_rvalue_reference();
    esempio_move_semantics();
    esempio_perfect_forwarding();
    esempio_use_cases();
    
    std::cout << "\n======================================" << std::endl;
    std::cout << "Fine esempi" << std::endl;
    std::cout << "======================================" << std::endl;
    
    return 0;
}
