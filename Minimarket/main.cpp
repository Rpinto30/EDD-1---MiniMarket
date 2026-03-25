#include <iostream>
#include <windows.h>
#include <thread>
#include <string>
#include <cstdlib>
#include <ctime> //para los numeros aleatorios

#include "gui.h"
#include "structs.h"
#include "dotGenerator.h"
#include "definitions.h"

/*NAMESPACES*/
using namespace std;
using namespace structures;
using namespace window;
using namespace dotGenerator;
using namespace data_structures;

/*INICIALIZAR LAS FUCNIONES DE ABAJO*/
int initializerValue(string type);
int randomBetween(int min_value, int max_value);
void generateDot();

void createClients();
void createCarts();
void createCashRegisters();
void graphIntoDotFile();

//PARA LA SIMULACION
ClientNode* getCart();
void insertBuyClient(ClientNode* client);
void moveClientToPay();
void removeClientCashRegister();

/*VARIABLES EN EL SCOPE GLOBAL*/
//ESTRUCTURAS DIRECTAS DEL PROYECTO
ListCashRegister cash_registers("Cajas_Registradoras");
DCLBuying buying_clients("Clientes_Comprando");

int StackCart::globalIDCounter = 0; //variable static
StackCart carts_stk_1("Pila_1");
StackCart carts_stk_2("Pila_2");
WaitClient wait_client_cart("Cola_Espera_Carritos");
WaitClient wait_client_cashRegister("Cola_Espera_Cajas_Registradoras");
//la otra cola de espera es solo otra copia

DotFile dot_file;


/*------------------------------------------------MAIN--------------------------------------------------*/
int main()
{
    graphIntoDotFile();

    createCarts();
    createCashRegisters();
    createClients();
    generateDot();
    int af = 0;

    while(af < 20){
        int ran = randomBetween(0,1);
        if (ran == 1) wait_client_cart.queueClient();

        cout<<"  - SIGUIENTE PASO "<<endl;
        ClientNode* cl = getCart();

        if (cl != nullptr){
            cout<<"Cliente: "<<cl->data->idClient<<", toma carrito: "<<to_string(cl->data->cart->idGroup)+":"+to_string(cl->data->cart->idCart)<<endl;
            insertBuyClient(cl);
        }

        int ran_1 = randomBetween(1,10);
        if (buying_clients.findID(ran_1) == 0)
        {
            ClientNode* c = buying_clients.popClientNode(ran_1);
            cout<<"salio de comprar"<<c->nameNode<<endl;
            wait_client_cashRegister.add(c);
        }
        moveClientToPay();
        removeClientCashRegister();
        Sleep(1500);
        generateDot();
        af++;
    }



    return 0;
    //Window win;
    // HILOS: https://www.geeksforgeeks.org/cpp/multithreading-in-cpp/
    //thread console_thread(simulacion);
    //console_thread.detach();

    //return win.run();
}
/*------------------------------------------------MAIN--------------------------------------------------*/

// INICALIZACION DE CANTIDADES
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

void createClients(){
    int m = initializerValue("Clientes");
    for(int i = 0; i < m; i++){
        wait_client_cart.queueClient();
    }
}

void graphIntoDotFile(){ // Este metodo me ayuda a insertar todos los subgraphs de las EDD que voy a querer visualizar
    SubgraphQueue* dot_queue = dot_file.getQueue();
    dot_queue->add(wait_client_cart.getGraph());
    dot_queue->add(carts_stk_1.getGraph());
    dot_queue->add(carts_stk_2.getGraph());
    dot_queue->add(buying_clients.getGraph());
    dot_queue->add(cash_registers.getGraph());
    dot_queue->add(wait_client_cashRegister.getGraph());


}

StackCart* selectCart(){
    if(carts_stk_1.peekCart() == nullptr && carts_stk_2.peekCart() == nullptr){
        cout<<"Sin carritos disponibles!!"<<endl;
        return nullptr;
    }

    if (carts_stk_1.peekCart() == nullptr){
        return &carts_stk_2;
    }else if (carts_stk_2.peekCart() == nullptr){
        return &carts_stk_1;
    }else{
        int rand_cart = randomBetween(0,1);
        switch(rand_cart){
            case 0: return &carts_stk_1; break;
            case 1: return &carts_stk_2; break;
            default: return nullptr; break;
        }
    }


}

ClientNode* getCart(){ // Para sacar un cliente de la pila de espera
    StackCart* stack_use = selectCart();

    if (stack_use != nullptr) {
        ClientNode* client = wait_client_cart.dequeueClient();
        CartNode* cart = stack_use->popCart();
        client->data->cart = cart->data;
        delete cart;
        return client;
    } else{
        return nullptr;
    }
}

void insertBuyClient(ClientNode* client){
    buying_clients.add(client);
}

void updateTextCashRegister(CashRegisterNode* node, Client* client){
    if (client != nullptr){
         string state = "Ocupado por el cliente: " + to_string(client->idClient);
        string cart = "Usando el carrito: " + to_string(client->cart->idGroup) + ":" + to_string(client->cart->idCart);
        string time = "Tiempo de servicio: " + to_string(node->data->timeService) + "s";
        cash_registers.getGraph()->updateLabelNode(node->nameNode, "{Caja_"+ to_string(node->data->idCashRegister) +"|"+ state+"|"+ cart+ "|"+ time+"}");
    }else{
        cash_registers.getGraph()->updateLabelNode(node->nameNode, "{Caja_"+ to_string(node->data->idCashRegister) +"| Disponible}");
    }

}

//PAGO DE CLIENTES
void moveClientToPay(){
    ClientNode* client = wait_client_cashRegister.peekClient();
    if(client != nullptr){
        CashRegisterNode* empty_cash_register = cash_registers.emptyCashRegister();
        if(empty_cash_register != nullptr){
            client = wait_client_cashRegister.dequeueClient();
            empty_cash_register->data->state = 1;
            empty_cash_register->data->client = client->data;
            updateTextCashRegister(empty_cash_register, client->data);
            delete client;
            cout<<endl<<"Cliente: "+ to_string(empty_cash_register->data->client->idClient)+ " está siendo atendido en la caja: "<< to_string(empty_cash_register->data->idCashRegister)<<endl;
        }
    }
}

//REGRESAR CARRETA Y SALIR DEL SISTEMA

void removeClientCashRegister(){
    cash_registers.clockServiceTime(updateTextCashRegister);
    int random_time =  randomBetween(3,7);
    CashRegisterNode* full_cash_register = cash_registers.endService(random_time);

    if(full_cash_register != nullptr){
        Client* client = full_cash_register->data->client;
        Cart* cart = client->cart;

        full_cash_register->data->state = 0;
        full_cash_register->data->client = nullptr;
        full_cash_register->data->timeService = 0;
        updateTextCashRegister(full_cash_register, nullptr);
        int rand = randomBetween(0,1);

        CartNode* new_node = new CartNode; //Ya es muy tarde para cambiar la TDA x'd
        new_node->data = cart;
        new_node->next = nullptr;


        if (rand == 1) {
            new_node->nameNode = carts_stk_1.getTittle() + "_" +to_string(cart->idGroup) + "_"+to_string(cart->idCart);
            carts_stk_1.add(new_node);
        }
        else {
            new_node->nameNode = carts_stk_2.getTittle() + "_" +to_string(cart->idGroup) + "_"+to_string(cart->idCart);
            carts_stk_2.add(new_node);
        }

        cout<<"El cliente: "<< to_string(client->idClient) <<" ha salido del minimarket"<<endl;
        cout<<"La carreta: "<< to_string(cart->idCart) << "ha sido devuleta a la pila: "<<0<<endl;
        delete client;
    }
}

/*UTILS (FUNCIONES QUE ME SIRVEN EN LA SIMULACION EN GENERAL, MAS QUE TODO ABSTRACCIONES)*/
void generateDot(){
    int t = dot_file.generateNewFiles();
    if (t == 0){
        cout<<" $ .dot generador con exito"<<endl;
    } else{
        cout<<" x Error al crear el .dot"<<endl;
    }

}

int randomBetween(int min_value, int max_value){
    srand(time(0));
    int random = min_value + (rand() % (max_value+1));
    return random;
}

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



