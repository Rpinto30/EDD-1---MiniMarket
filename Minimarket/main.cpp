#include <iostream>
#include <windows.h>
#include <thread>
#include <string>

#include "gui.h"
#include "structs.h"
#include "dotGenerator.h"

/*VARIABLES EN EL SCOPE GLOBAL*/
//DotFile dot;


using namespace std;
using namespace structures;
using namespace window;
using namespace dotGenerator;

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

    void addClient(){
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

    WaitClientCart(){

    }

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
    string tittle;
    SubGraph graph;
    int globalID = 0;
    CartNode* createNode(){
        CartNode * new_node = new CartNode;
        new_node->data.idCart = globalID++;
        new_node->next = nullptr;
        graph.insertNode(new_node->data.idCart);
        return new_node;
    }

    void afterInsertAction(CartNode* temp) override{ //el nombre siempre es tittle_id
        if(temp->next != nullptr){
            string nodeA = tittle +"_"+ to_string(temp->data.idCart);
            string nodeB = tittle +"_"+ to_string(temp->next->data.idCart);
            graph.simpleConnectNode(nodeA, nodeB);
        }
    }

public:
    StackCart(string tittle){
        graph.changeName(tittle);
        this->tittle = tittle;
    }

    SubGraph * getGraph() { return &graph;}

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
void graphIntoDotFile();

//ESTRUCTURAS DIRECTAS DEL PROYECTO
ListCashRegister cash_registers;

StackCart carts_stk_1("Pila_1");
StackCart carts_stk_2("Pila_2");

//ESTRUCTURAS ADICIONALES, PERO IGUAL USAN MIS EDD
ClientManager client_mg;
DotFile dot_file;

/*------------------------------------------------MAIN--------------------------------------------------*/

// Este meotdo lo uso para hacer la simulacion de manera async con la ventana (modulo visible)
void simulacion()
{
    createCarts();
    graphIntoDotFile();
    dot_file.updateSubGraphs();
    int r = dot_file.generateFile();
    if (r == 0) {

        cout<<" $ .dot generado correctamente"<<endl;
    } else{
        cout<<" $ .dot ha falldo al generarse"<<endl;
    }


    while(true){

        cout<<"LO LOGREEEEEE  ";
        Sleep(1500);
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
/*------------------------------------------------MAIN--------------------------------------------------*/

// INICALIZACION DE CANTIDADES
void createClients(){
    int n = initializerValue("Clientes");
    for(int i = 0; i < n; i++){
        client_mg.addClient();
    }
}

void createCarts(){
    int m = initializerValue("Carritos");
    for(int i = 0; i < m; i++){
        carts_stk_1.add();
        carts_stk_2.add();
    }
}

void createCashRegisters(){
    int m = initializerValue("Cajas de Pago");
    for(int i = 0; i < m; i++){
        cash_registers.add();
    }
}

void graphIntoDotFile(){ // Este metodo me ayuda a insertar todos los subgraphs de las EDD que voy a querer visualizar
    SubgraphQueue* dot_queue = dot_file.getQueue();
    dot_queue->add(carts_stk_1.getGraph());
    dot_queue->add(carts_stk_2.getGraph());
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



