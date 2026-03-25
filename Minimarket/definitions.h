#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <iostream>
#include "dotGenerator.h"
using namespace std;
using namespace dotGenerator;
using namespace structures;

namespace data_structures{
    class WaitClient : private Queue<Client>{
private:
    int globalID = 1;
    string tittle;
    SubGraph graph;
     void printData(ClientNode* node) override{
       cout<<" - Cliente: "<< node->data->idClient << " en espera de un carrito..."<<endl;
    }

    void afterInsertAction(ClientNode* temp) override{ //el nombre siempre es tittle_id
        if(temp->next != nullptr){
            string nodeA = temp->nameNode;
            string nodeB = temp->next->nameNode;
            graph.simpleConnectNode(nodeA, nodeB);
        }
    }

    ClientNode* createNode(){
        ClientNode * new_node = new ClientNode;
        new_node->data->idClient = globalID++;

        string nameNode = tittle + "_" + to_string(new_node->data->idClient);
        graph.insertNode(nameNode, "Cliente_"+to_string(new_node->data->idClient));
        new_node->nameNode = nameNode;
        new_node->next = nullptr;
        return new_node;
    }

public:
    WaitClient(string tittle){
        graph.changeName(tittle);
        this->tittle = tittle;
    }

    SubGraph * getGraph() { return &graph;}

    void queueClient(){
        ClientNode* new_node = createNode();
        insert(new_node);
    }

    void print(){
        showList();
    }

    void add(ClientNode* client){
        if(client != nullptr)
       {
            if (client->next != nullptr) client->next = nullptr;

            insert(client);
            string nameNode = client->nameNode;

            graph.insertNode(nameNode , "Cliente_"+to_string(client->data->idClient));
       }
    }

    ClientNode* peekClient(){
        return peek();
    }

    ClientNode* dequeueClient(){
        ClientNode * last = dequeue();
        if (last != nullptr) {
            cout<<"borrando "<<last->nameNode<<endl;
            graph.removeNode(last->nameNode, "", "");
        }
        return last;
    }
};

// Stack CARRITOS
class StackCart : private Stack<Cart>{
private:
    string tittle;
    SubGraph graph;
    int globalID = 0;
    static int globalIDCounter; //static para que la compartan todas las instancias
    int idGroup;
    CartNode* createNode(){
        CartNode * new_node = new CartNode;
        new_node->data->idCart = globalID++;
        new_node->data->idGroup = idGroup;
        string nameNode = tittle + "_" +to_string(idGroup) + "_"+to_string(new_node->data->idCart);
        new_node->nameNode = nameNode;
        graph.insertNode(nameNode, "Carrito_"+to_string(idGroup)+":"+to_string(new_node->data->idCart));

        new_node->next = nullptr;

        return new_node;
    }

    void afterInsertAction(CartNode* temp) override{ //el nombre siempre es tittle_id
        if(temp->next != nullptr){
            string nodeA = temp->nameNode;
            string nodeB = temp->next->nameNode;
            graph.simpleConnectNode(nodeA, nodeB);
        }
    }

public:
    StackCart(string tittle){
        graph.changeName(tittle);
        this->tittle = tittle;
        globalIDCounter++;
        this->idGroup = globalIDCounter;
    }

    SubGraph * getGraph() { return &graph;}
    string getTittle() {return tittle;}

    void add(){
        CartNode* new_node = createNode();
        insert(new_node);
    }

    void add(CartNode* cart){
        if(cart != nullptr)
       {
            if (cart->next != nullptr)cart->next = nullptr;

            insert(cart);
            string nameNode = cart->nameNode;
            graph.insertNode(nameNode , nameNode);
       }
    }

    CartNode* peekCart(){
        return peek();
    }

    CartNode* popCart(){
        CartNode * last = pop();
        if (last != nullptr) {
            graph.removeNode(last->nameNode);
        }
        return last;
    }

};

// DOBLE ENLAZADA CAJAS REGISTRADORAS
class ListCashRegister : private DoubleLinkedList<CashRegister>{
private:
    int globalID = 0;
    SubGraph graph;
    string tittle;

    CashRegisterNode* createNode(){
        CashRegisterNode * new_node = new CashRegisterNode;
        new_node->data->idCashRegister = globalID++;
        new_node->next = nullptr;
        new_node->prev = nullptr;

        string nameNode = tittle + "_" +to_string(new_node->data->idCashRegister);
        string state = (new_node->data->state == 0) ? "Disponible" : "Ocupado por:\n" + to_string(new_node->data->client->idClient);
        new_node->nameNode = nameNode;
        graph.insertNode(nameNode, "{Caja_"+to_string(new_node->data->idCashRegister)+"|"+ state+"}");
        return new_node;
    }

    void afterInsertAction(CashRegisterNode* temp) override{ //el nombre siempre es tittle_id
        if(temp->next != nullptr){
            string nodeA = temp->nameNode;
            string nodeB = temp->next->nameNode;
            graph.simpleConnectNode(nodeA, nodeB);
            graph.simpleConnectNode(nodeB, nodeA);
        }
    }
public:
     SubGraph * getGraph() { return &graph;}

    ListCashRegister(string tittle){
        graph.changeName(tittle);
        this->tittle = tittle;
    }

    void add(){
        CashRegisterNode* new_node = createNode();
        insert(new_node);
    }

    CashRegisterNode* emptyCashRegister(){
        CashRegisterNode * temp = head;
        while(temp != nullptr){
            if (temp->data->state == 0) return temp;
            temp = temp->next;
        }
        return nullptr;
    }

//No dice en el docs como un cliente termina de pagar, asi que hice esto para que aumente el tiempo de servicio y cuando llegue a cierto numero, el cliente libere la caja
    void clockServiceTime(void (*updateGraph)(CashRegisterNode*, Client*)){
        CashRegisterNode * temp = head;
        while(temp != nullptr){
            if (temp->data->state == 1) {
                temp->data->timeService+=1;
                updateGraph(temp, temp->data->client); //usé lo que vimos en clase de punteros void

            }
            temp = temp->next;
        }
    }

    CashRegisterNode* endService(int duration){
        CashRegisterNode * temp = head;
        while(temp != nullptr){
            if (temp->data->state == 1) {
                if (temp->data->timeService >= duration) return temp;
            }
            temp = temp->next;
        }
        return nullptr;
    }


};

//CIRCULAR DOBLEMENTE ENLAZADA
class DCLBuying : private DoubleCircleList<Client>{
private:
    int globalID = 0;
    SubGraph graph;
    string tittle;

    void afterInsertAction(ClientBiNode* temp) override{ //el nombre siempre es tittle_id
        string name = "{Cliente_"+to_string(temp->data->idClient)+"| Carrito_"+ to_string(temp->data->cart->idGroup)+":"+to_string(temp->data->cart->idCart)+"}";
        graph.insertNode(temp->nameNode, name);
        recalculateConectionsGraph(temp);
    }


    ClientBiNode* createBiNode(ClientNode* client){
        ClientBiNode* new_node  = new ClientBiNode;
        new_node->id = client->id;
        new_node->data= client->data;
        new_node->nameNode = client->nameNode;
        delete client;
        return new_node;
    }

    ClientNode* createNode(ClientBiNode* client){
        ClientNode* new_node  = new ClientNode;
        new_node->id = client->id;
        new_node->data= client->data;
        new_node->nameNode = client->nameNode;
        delete client;
        return new_node;
    }

    ClientBiNode* findBiNode(int id){
        ClientBiNode* temp = getActual();
        do{
            if(temp->data->idClient == id) return temp;
            temp = temp->next;
        } while(temp != getActual());
        return nullptr;
    }

    void recalculateConectionsGraph(ClientBiNode* start){
        graph.removeAllConections();

        ClientBiNode* init = start;
        do{ //recalcular las conexiones (ironico usar un O(n) para la unica edd que simplifica el O(n) a O(1) xd)
            string nodeA = init->nameNode;
            string nodeNext = init->next->nameNode;
            string nodePrev = init->prev->nameNode;

            graph.simpleConnectNode(nodeA, nodeNext);
            graph.simpleConnectNode(nodeA, nodePrev);
            init = init->next;
        } while(init != start);
    }

public:
     SubGraph * getGraph() { return &graph;}

    DCLBuying(string tittle){
        graph.changeName(tittle);
        this->tittle = tittle;
    }

    void add(ClientNode* client){
        ClientBiNode* new_node = createBiNode(client);
        insert(new_node);
    }

    ClientNode* popClientNode(int id){
        ClientBiNode* node_remove = findBiNode(id);
        if(node_remove != nullptr){
            desligateNode(node_remove);
            graph.removeNode(node_remove->nameNode);
            recalculateConectionsGraph(head);
            return createNode(node_remove); // BiNode -> Node
        }
        cout<<"Sin remover"<<endl;
        return nullptr;
    }

    void moveNext(){
        modeForward();
    }

    ClientBiNode* actual(){
        return getActual();
    }

    int findID(int id){
        ClientBiNode* temp = actual();
        do{
            if (temp->data->idClient == id) return 0;
            temp = temp->next;
        }while(temp != actual());
        return -1;
    }

};


}

#endif
