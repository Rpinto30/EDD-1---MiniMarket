#include <iostream>
#include <windows.h>
#include <thread>
#include <string>
#include <cstdlib>
#include <ctime> //para los numeros aleatorios

#include "structs.h"
#include "dotGenerator.h"
#include "definitions.h"

/*NAMESPACES*/
using namespace std;
using namespace structures;
using namespace dotGenerator;
using namespace data_structures;

/*INICIALIZAR LAS FUCNIONES DE ABAJO*/
int initializerValue(string type);
int randomBetween(int min_value, int max_value);
int generateDot(bool debugMessage);

void createClients();
void createClientsCashRegister();
void createClientsBuy();
void createCarts();
void createCashRegisters();
void graphIntoDotFile();

//PARA LA SIMULACION
bool endSimulation();
ClientNode* getCart(bool message);
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

void simulation(){
    int attepps = 0;

    while(true){
        if (endSimulation()) { //TERMINAR LA SIMULACION
            cout<<endl<<" - El minimarket ha cerrado!"<<endl;
            break;
        }

        cout<<"************ PASO "<<attepps<<"************"<<endl;
        ClientNode* cl = getCart(true);
        if (cl != nullptr){
            cout<<"   - Cliente: "<<cl->data->idClient<<", toma el carrito "<<to_string(cl->data->cart->idGroup)+":"+to_string(cl->data->cart->idCart)<<endl;
            insertBuyClient(cl);
        }

        //si no le gusta al inge xd  randomBetween(0,100)
        int ran_1 = randomBetween(0,wait_client_cart.getGlobalID());
        if (buying_clients.findID(ran_1) == 0)
        {
            ClientNode* c = buying_clients.popClientNode(ran_1);
            if (c != nullptr){
                cout<<"   - El cliente "<<c->data->idClient<<" pasa a esperar una caja registradora"<<endl;
                wait_client_cashRegister.add(c);
            }
        }else{
            cout<<"   ! Todos los clientes anteriores siguen comprando"<<endl;
        }

        moveClientToPay();
        removeClientCashRegister();
        Sleep(0);
        if (generateDot(false) != 0) {
            cout<<endl<<"  ! Ningun cambio hasta el momento!"<<endl;
            Sleep(1000);
        }
        attepps++;


    }
}

void set_conditions(){
    createCarts();
    createCashRegisters();
    createClients();
    createClientsCashRegister();
    createClientsBuy();
    generateDot(true);
}


/*------------------------------------------------MAIN--------------------------------------------------*/
int main()
{
    graphIntoDotFile();
    cout<<"---------------------------------SIMULACION DE MINIMARKET---------------------------------"<<endl;
    set_conditions();

    string init_str;
    cout<<endl<<"Condiciones iniciales han sido agregadas!"<<endl;
    while(init_str != "Y"){
        cout<<"Quieres iniciar la simulacion? [Y/N]: ";
        cin>>init_str;
    }
    cout<<endl<<endl<<endl<<endl<<endl<<endl<<"Iniciando.."<<endl;
    simulation();

    return 0;
}
/*------------------------------------------------MAIN--------------------------------------------------*/

bool endSimulation(){ //ya no hay clientes
    return (cash_registers.allCashIsEmpty() == 0 && wait_client_cart.peekClient() == nullptr && buying_clients.actual() == nullptr) ? true : false;
}

// INICALIZACION DE CANTIDADES
void createCarts(){
    int m = initializerValue("CARRITOS");
    for(int i = 0; i < m; i++){
        carts_stk_1.add();
        carts_stk_2.add();
    }
}

void createCashRegisters(){
    int m = initializerValue("CAJAS REGISTRADORAS");
    for(int i = 0; i < m; i++){
        cash_registers.add();
    }
}

void createClients(){
    int m = initializerValue("CLIENTES GENERALES");
    for(int i = 0; i < m; i++){
        wait_client_cart.queueClient();
    }
}

// COMO ESTOY REUTILIZNDO CLIENTES, AGREGO EXCEPCIONES PARA EVITAR QUE SE AÑADAN MAS CLIENTES DE LOS QUE HAY A LAS OTRAS LISTAS
void createClientsCashRegister(){

    int m = initializerValue("CLIENTES EN ESPERA DE UNA CAJA REGISTRADORAS");
    for(int i = 0; i < m; i++){
        wait_client_cart.queueClient();
        ClientNode* cl = getCart(false);
        if (cl != nullptr){
            wait_client_cashRegister.add(cl);
        }
    }
}

void createClientsBuy(){
    int m = initializerValue("CLIENTES COMPRANDO");



    for(int i = 0; i < m; i++){
        wait_client_cart.queueClient();
        ClientNode* cl = getCart(false);
        if (cl != nullptr){
            insertBuyClient(cl);
        }
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

StackCart* selectCart(bool message = false){ // Para seleccionar los carritos de los clientes
    if(carts_stk_1.peekCart() == nullptr && carts_stk_2.peekCart() == nullptr){
        if(message) cout<<"   ! Hay "+ to_string(wait_client_cart.lenght()) +" clientes esperando una carreta!"<<endl;
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

ClientNode* getCart(bool message){ // Para sacar un cliente de la pila de espera
    StackCart* stack_use = selectCart(message);

    if (stack_use != nullptr) {
        ClientNode* client = wait_client_cart.dequeueClient();
        if(client != nullptr){
            CartNode* cart = stack_use->popCart();
            client->data->cart = cart->data;
            delete cart;
            return client;
        }
        return nullptr;
    } else{
        return nullptr;
    }
}

void insertBuyClient(ClientNode* client){ //añadir a cola de compras
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
            cout<<"   - El cliente: "+ to_string(empty_cash_register->data->client->idClient)+ " esta siendo atendido en la caja: "<< to_string(empty_cash_register->data->idCashRegister)<<endl;
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

        int stack_number = 0;
        if (rand == 1) {
            new_node->nameNode = carts_stk_1.getTittle() + "_" +to_string(cart->idGroup) + "_"+to_string(cart->idCart);
            carts_stk_1.add(new_node);
            stack_number = 0;
        }
        else {
            new_node->nameNode = carts_stk_2.getTittle() + "_" +to_string(cart->idGroup) + "_"+to_string(cart->idCart);
            carts_stk_2.add(new_node);
            stack_number = 1;
        }

        cout<<"   - El cliente: "<< to_string(client->idClient) <<" ha salido del minimarket"<<endl;
        cout<<"   - La carreta: "<< to_string(cart->idGroup) +":"+to_string(cart->idCart) << " ha sido devuleta a la pila: "<<to_string(stack_number)<<endl;
        delete client;
    }
}

/*UTILS (FUNCIONES QUE ME SIRVEN EN LA SIMULACION EN GENERAL, MAS QUE TODO ABSTRACCIONES)*/
int generateDot(bool debugMessage = false){
    int t = dot_file.generateNewFiles();
    if (t == 0){
        if(debugMessage) cout<<" $ .dot generador con exito"<<endl;
        return 0;
    } else if (t == 2) {
         if(debugMessage) cout<<" $ .ya generado"<<endl;
         return 1;
    } else{
        if(debugMessage) cout<<" x Error al crear el .dot"<<endl;
        return -1;
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



