// COLA ESPERAR CAJA
class WaitClientCashRegister : private Queue<Client>{
private:
    string tittle;
    int globalID = 1;
    SubGraph graph;
     void printData(ClientNode* node) override{
       cout<<" - Cliente: "<< node->data->idClient << " en espera de un carrito..."<<endl;
    }

public:
    WaitClientCashRegister(string tittle){
        this->tittle = tittle;
    }

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

    void add(ClientNode* client){
        if(client != nullptr)
       {
            if (client->next != nullptr) client->next = nullptr;

            insert(client);
            string nameNode = client->nameNode;
            graph.insertNode(nameNode , nameNode);
       }
    }
    ClientNode* dequeueCart(){
        ClientNode * last = dequeue();
        if (last != nullptr) {
            cout<<"borrando"<<last->nameNode<<endl;
            graph.removeNode(last->nameNode);
        }
        return last;
    }

};