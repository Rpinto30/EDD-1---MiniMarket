#include <iostream>
#include "Headers/structs.h"

using namespace std;
using namespace structures;

class waitClientCart : private LinkedList<Client>{
private:
    int globalID = 1;
     void printData(ClientNode* node) override{
       cout<<" - Cliente: "<< node->data.idClient << " en espera de un carrito..."<<endl;
    }

public:
     ClientNode* createNode(){
        ClientNode * new_node = new ClientNode;
        new_node->data.idClient = globalID++;
        new_node->next = nullptr;
        return new_node;
    }

    void add(){
        ClientNode* new_node = createNode();
        insert(new_node);
    }

    void print(){
        showList();
    }

};

class stackCart : private Stack<Cart>{
private:
    int globalID = 0;
    CartNode* createNode(){
        CartNode * new_node = new CartNode;
        new_node->data.idCart = globalID++;
        new_node->next = nullptr;
        return new_node;
    }

    void add(){
        CartNode* new_node = createNode();
        insert(new_node);
    }
};



int main()
{
    /*
    waitClientCart list_;
    list_.add();
    list_.print();*/
    return 0;
}



