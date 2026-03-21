#include <iostream>
#include "gui.h"
#include "structs.h"
#include <windows.h>
#include <thread>

using namespace std;
using namespace structures;
using namespace window;

class ClientManager : private LinkedList<Client> {
    private:
    int globalID = 1;

public:
     ClientNode* createNode(){
        ClientNode * new_node = new ClientNode;
        new_node->data.idClient = globalID++;
        new_node->next = nullptr;
        return new_node;
    }

    void queueClient(){
        ClientNode* new_node = createNode();
        insert(new_node);
    }

    void print(){
        showList();
    }

};

// COLA ESPERAR CARRITO (0)
class WaitClientCart : private Queue<Client>{
private:
    int globalID = 1;
     void printData(ClientNode* node) override{
       cout<<" - Cliente: "<< node->data.idClient << " en espera de un carrito..."<<endl;
    }

public:
     ClientNode* createNode(){
        ClientNode * new_node = new ClientNode;
        new_node->id = globalID++;
        new_node->next = nullptr;
        return new_node;
    }

    void queueClient(){
        ClientNode* new_node = createNode();
        insert(new_node);
    }

    void print(){
        showList();
    }

};

// COLA ESPERAR CAJA
class WaitClientCashRegister : private Queue<Client>{
private:
    int globalID = 1;
     void printData(ClientNode* node) override{
       cout<<" - Cliente: "<< node->data.idClient << " en espera de un carrito..."<<endl;
    }

public:
     ClientNode* createNode(){
        ClientNode * new_node = new ClientNode;
        new_node->id = globalID++;
        new_node->next = nullptr;
        return new_node;
    }

    void queueClient(){
        ClientNode* new_node = createNode();
        insert(new_node);
    }

    void print(){
        showList();
    }

};

// Stack
class StackCart : private Stack<Cart>{
private:
    int globalID = 0;
    CartNode* createNode(){
        CartNode * new_node = new CartNode;
        new_node->data.idCart = globalID++;
        new_node->next = nullptr;
        return new_node;
    }
public:
    void add(){
        CartNode* new_node = createNode();
        insert(new_node);
    }

};

// Doble enlazada
class ListCashRegister : private DoubleLinkedList<CashRegister>{
private:
    int globalID = 0;
    CashRegisterNode* createNode(){
        CashRegisterNode * new_node = new CashRegisterNode;
        new_node->data.idCashRegister = globalID++;
        new_node->next = nullptr;
        new_node->prev = nullptr;
        return new_node;
    }
public:
    void add(){
        CashRegisterNode* new_node = createNode();
        insert(new_node);
    }
};



int initializerValue(string type);
void createClients();
void createCarts();

/*VARIABLES EN EL SCOPE GLOBAL*/
ClientManager client_mg;
ListCashRegister cash_registers;

StackCart carts_stk_1;
StackCart carts_stk_2;

void simulacion(){
    createCarts();
    while(true){
      cout<<"Hola";
        Sleep(150);
    }
}

int main()
{
    Window win;

    // HILOS: https://www.geeksforgeeks.org/cpp/multithreading-in-cpp/
    thread console_thread(simulacion);
    console_thread.detach();

    return win.run();
}


// INICALIZACION DE CANTIDADES
void createClients(){
    int n = initializerValue("Clientes");
    for(int i = 0; i < n; i++){
        client_mg.queueClient();
    }
}

void createCarts(){
    int m = initializerValue("Carritos");
    for(int i = 0; i < m; i++){
        carts_stk_1.add();
        carts_stk_1.add();
    }
}

void createCashRegisters(){
    int m = initializerValue("Cajas de Pago");
    for(int i = 0; i < m; i++){
        cash_registers.add();
    }
}


/*UTILS (FUNCIONES QUE ME SIRVEN EN LA SIMULACION EN GENERAL, MAS QUE TODO ABSTRACCIONES)*/
int initializerValue(string type){
    int val;
    do{
        cout<<"--------------------------------------------------------------------------"<<endl;
        cout<<" > Ingresa la cantidad de " + type + " que va a haber en la simulacion: ";
        cin>>val;

        if(val > 0) {
            return val;
        }
        else {
            cout<<"\terror: la cantidad no es valida, intenta otra vez"<<endl<<endl;
        }
    } while(true);
}


