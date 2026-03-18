#include <iostream>
#include "Headers/structs.h"

using namespace std;
using namespace structures;

class waitClientCart : protected LinkedList<Client>{
private:
     void printData(ClientNode* node) override{
       cout<<" - Cliente: "<< node->data.idClient << " en espera de un carrito..."<<endl;
    }

public:
     ClientNode* createNode(){
        ClientNode * new_node = new ClientNode;
        new_node->next = nullptr;
        return new_node;
    }

    void add(){
        ClientNode* new_node = createNode();
        insert(new_node);
        //delete new_node;
    }

    void print(){
        showList();
    }

};




int main()
{
    waitClientCart list_;
    list_.add();
    list_.print();
    return 0;
}



