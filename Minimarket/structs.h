#ifndef STRUCTS_H
#define STRUCTS_H

#include <iostream>
namespace structures{
    typedef struct Cart{
        int idCart = 0;
        int idGroup = 0;
    } Cart;

    typedef struct Client{
        int idClient = 0;
        Cart * cart;
    } Client;

    typedef struct CashRegister{
        int idCashRegister = 0;
        int timeService = 0.0;
        int state = 0;
        Client* client;
    } CashRegister;


    template <typename T>
    struct Node{
        int id = 0;
        T* data;
        std::string nameNode = "";
        struct Node* next;

        Node() : data(new T()) {}
        //~Node() { delete data; }
    };


    template <typename T>
    struct BiNode{
        int id;
        T* data;
        std::string nameNode = "";
        struct BiNode* next;
        struct BiNode* prev;

         BiNode() : data(new T()) {}
        //~BiNode() { delete data; }
    };

    //DECLARAR ESTRUCTURAS POR TYPEDEF
    typedef Node<Cart> CartNode;
    typedef Node<Client> ClientNode;
    typedef BiNode<Client> ClientBiNode;
    typedef BiNode<CashRegister> CashRegisterNode;

    template <typename T>
    class LinkedList{
    protected:
        struct Node<T>* head = nullptr;

        int checkList(){
            if (head == nullptr) return 1;
            return 0;
        }

        virtual void afterInsertAction(struct Node<T>* temp) {};

        int insert(struct Node<T>* new_node){
            if (checkList() == 1){
                head = new_node;
                head->next = nullptr;
                return 0;
            }
            else{
                struct Node<T>* temp = head;
                while(temp->next != nullptr){
                    temp = temp->next;
                }

                temp->next = new_node;
                afterInsertAction(temp);
                return 0;
            }
            return 1;
        }

        struct Node<T>* getByIndex(int index){
            if (checkList() == 0){
                struct Node<T>* temp = head;
                while(temp != nullptr){
                    if (temp->id == index){
                        return temp;
                    }
                    temp = temp->next;
                }
            }
            return nullptr;
        }

        struct Node<T>* getFirst(){
            if (checkList() ==0 ){
                return head;
            }
            return nullptr;
        };

        virtual void printData(struct Node<T>* node) {};

        void showList(){
            if (checkList() == 0){
                struct Node<T>* temp = head;
                while(temp != nullptr){
                    printData(temp);
                    temp = temp->next;
                }
            }
        }

        ~LinkedList(){
            struct Node<T>* temp;
            while (head) {
                temp = head;
                head = head->next;
                delete temp;
            }

        }

    };

    /*LISTA DOBLE ENLAZADA*/
    template <typename T>
    class DoubleLinkedList{
    protected:
        struct BiNode<T>* head = nullptr;

        int checkList(){
            if (head == nullptr) return 1;
            return 0;
        }

        virtual void afterInsertAction(struct BiNode<T>* temp) {};

        int insert(struct BiNode<T>* new_node){
            if (checkList() == 1){
                head = new_node;
                head->next = nullptr;
                head->prev = nullptr;
                return 0;
            }
            else{
                struct BiNode<T>* temp = head;
                while(temp->next != nullptr){
                    temp = temp->next;
                }
                temp->next = new_node;
                new_node->prev = temp;
                new_node->next = nullptr;
                afterInsertAction(temp);
                return 0;
            }
            return 1;
        }

        struct BiNode<T>* getByIndex(int index){
            if (checkList() == 0){
                struct Node<T>* temp = head;
                while(temp != nullptr){
                    if (temp->id == index){
                        return temp;
                    }
                    temp = temp->next;
                }
            }
            return nullptr;
        }

        struct BiNode<T>* getFirst(){
            if (checkList() ==0 ){
                return head;
            }
            return nullptr;
        };

        virtual void printData(struct BiNode<T>* node) {};

        void showList(){
            if (checkList() == 0){
                struct Node<T>* temp = head;
                while (temp != nullptr) {
                    printData(temp);
                    temp = temp->next;
                }
            }
        }

        //Lo del metodo desctructor lo investigue de: https://medium.com/@RobuRishabh/beginners-guide-to-linked-list-in-c-d8445ef906ab
        ~DoubleLinkedList(){
            struct BiNode<T>* temp;
            while (head) {
                temp = head;
                head = head->next;
                delete temp;
            }

        }
    };

    /*LISTA DOBLE CIRCULAR*/
    template <typename T>
    class DoubleCircleList{
    protected:
        struct BiNode<T>* head = nullptr;

        int checkList(){
            if (head == nullptr) return 1;
            return 0;
        }

        virtual void afterInsertAction(struct BiNode<T>* temp) {};

        int insert(struct BiNode<T>* new_node){
            if (checkList() == 1){
                head = new_node;
                head->next = new_node;
                head->prev = new_node;
            }
            else{
                struct BiNode<T> * last = head->prev;

                last->next = new_node;
                head->prev = new_node;

                new_node->next = head;
                new_node->prev = last;
            }
            afterInsertAction(new_node);
            return 0;
        }

        struct BiNode<T>* getActual(){
            return head;
        }

        void modeForward(){
            head = head->next;
        }

        //hacer la conversion de Node a BiNode
        struct BiNode<T>* removeActual(){
                if (checkList() == 1) return nullptr;

                struct BiNode<T>* temp = head;

                if (head->next == head) {
                    head = nullptr;
                } else {
                    struct BiNode<T>* last = head->prev;
                    head = temp->next;
                    last->next = head;
                    head->prev = last;
                }

                temp->next = nullptr;
                temp->prev = nullptr;
                return temp;
        }


        void desligateNode(struct BiNode<T> * node){ //Este metodo unicamente desliga el nodo de la lista circualr
            struct BiNode<T> *front_ = node->next;
            struct BiNode<T> *back_ = node->prev;
            if (back_ != NULL) {back_->next = front_;}
            if (front_ != NULL) {front_->prev = back_;}
            if (node == head) {
                if (node->next != NULL){
                    head = node->next;
                }else{
                    head = NULL;
                }
            }
            node->next = nullptr;
            node->prev = nullptr;

        }

        ~DoubleCircleList(){
            if (head == nullptr) return;

            struct BiNode<T>* temp = head;
            do{
                struct BiNode<T>* deleter = temp;
                temp = temp->next;
                delete deleter;
            } while(temp != head);
        }
    };

    //PILA
    template <typename T>
    class Stack : protected LinkedList<T>{
        using LinkedList<T>::head;

     public:
        struct Node<T>* peek(){
            if (head == nullptr) return nullptr;

            struct Node<T>* temp = head;
            while(temp->next != nullptr){
                temp = temp->next;
            }
            return temp;
        }

        struct Node<T>* pop(){
            //eliminar el ultimo
            if (head == nullptr) {
                std::cout<<"Ya nada"<<std::endl;
                return nullptr;

            }

            if (head->next == nullptr){
                struct Node<T>* temp = head;
                head = nullptr;
                std::cout<<"Es el ultimo"<<std::endl;
                return temp;
            }
            else{
                struct Node<T>* temp = head;
                struct Node<T>* prev = head;
                while(temp->next != nullptr){
                    prev = temp;
                    temp = temp->next;
                }
                prev->next = nullptr;
                temp->next = nullptr;
                return temp;
            }
        }

    };

    //COLA
    template <typename T>
    class Queue : protected LinkedList<T>{
        using LinkedList<T>::head;
        public:
            struct Node<T>* back(){
            struct Node<T>* temp = head;
            while(temp->next != nullptr){
                temp = temp->next;
            }
            return temp;
        }

        public:
            struct Node<T>* peek(){
            return head;
        }

        struct Node<T>* dequeue(){
            if (head == nullptr) return nullptr;

            struct Node<T>* temp = head;
            head = head->next;
            temp->next = nullptr;
            return temp;
        }
    };

}


#endif // STRUCTS_H
