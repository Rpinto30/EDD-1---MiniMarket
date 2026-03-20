#ifndef STRUCTS_H
#define STRUCTS_H

namespace structures{

    typedef struct Cart{
        int idCart;
    } Cart;

    typedef struct CashRegister{
        int idCart;
        float timeService;
        int state;
        int idClient;
    } CashRegister;

    typedef struct Client{
        int idClient;
    } Client;

    template <typename T>
    struct Node{
        int id;
        T data;
        struct Node* next;
    };


    template <typename T>
    struct BiNode{
        int id;
        T data;
        struct BiNode* next;
        struct BiNode* prev;
    };


    //DECLARAR ESTRUCTURAS POR TYPEDEF
    typedef Node<Cart> CartNode;
    typedef Node<Client> ClientNode;
    typedef Node<Client> ClientBuying;

    template <typename T>
    class LinkedList{
    protected:
        struct Node<T>* head = nullptr;

        int checkList(){
            if (head == nullptr) return 1;
            return 0;
        }

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
                return 0;
            }
            return 1;
        }

        struct Node<T>* getByIndex(int index){
            if (checkList() == 0){
                struct Node<T>* temp = head;
                while(temp->next != nullptr){
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
                return 0;
            }
            return 1;
        }

        struct Node<T>* getByIndex(int index){
            if (checkList() == 0){
                struct Node<T>* temp = head;
                while(temp->next != nullptr){
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
                do {
                    printData(temp);
                    temp = temp->next;
                }while (temp != head);

            }
        }

        //Lo del metodo desctructor lo investigue de: https://medium.com/@RobuRishabh/beginners-guide-to-linked-list-in-c-d8445ef906ab

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

        int insert(struct BiNode<T>* new_node){
            if (checkList() == 1){
                head = new_node;
                head->next = new_node;
                head->prev = new_node;
                return 0;
            }
            else{
                struct BiNode<T> * last = head->prev;

                last->next = new_node;
                head->prev = new_node;

                new_node->next = head;
                new_node->prev = last;
                return 0;
            }
            return 1;
        }
    };

    //PILA
    template <typename T>
    class Stack : protected LinkedList<T>{
        using LinkedList<T>::head;

     public:
        struct Node<T>* peek(){
            struct Node<T>* temp = head;
            while(temp->next != nullptr){
                temp = temp->next;
            }
            return temp;
        }

        void pop(){
            //eliminar el ultimo
            if (head == nullptr) return;

            if (head->next == nullptr){
                delete head;
                head = nullptr;
            }
            else{
                struct Node<T>* temp = head;
                struct Node<T>* prev = head;
                while(temp->next != nullptr){
                    prev = temp;
                    temp = temp->next;
                }

                prev->next = nullptr;
                delete temp;
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
            struct Node<T>* temp = head;
            temp->next = nullptr;
            head = head->next;
            return temp;
        }

    };

}


#endif // STRUCTS_H
