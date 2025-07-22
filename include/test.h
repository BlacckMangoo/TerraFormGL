

#include <iostream>

struct A {
    char c;
    int* ptr;
};

int main() {
    std::cout << "sizeof(char): " << sizeof(char) << "\n";
    std::cout << "sizeof(int*): " << sizeof(int*) << "\n";
    std::cout << "sizeof(A): " << sizeof(A) << "\n";
}
