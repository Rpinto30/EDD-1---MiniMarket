#include <iostream>

using namespace std;

typedef struct Cart{
    int id;
} Cart;

typedef struct CashRegister{
    int id;
} CashRegister;

template <typename T>
struct Node{
    T data;
    struct Node* next;
    struct Node* prev;
};

typedef Node<Cart> CartNode;


int main()
{
    CartNode{ Cart{1} };
    return 0;
}
