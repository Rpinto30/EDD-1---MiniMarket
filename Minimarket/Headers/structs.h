#ifndef STRUCTS_H
#define STRUCTS_H

namespace structures{
    typedef struct Cart{
        int id;
    } Cart;

    typedef struct CashRegister{
        int idCart;
    } CashRegister;

    typedef struct Client{
        int idClient;
    } Client;


    template <typename T>
    struct Node{
        T data;
        struct Node* next;
        struct Node* prev;
    };


    //DECLARAR ESTRUCTURAS POR TYPEDEF
    typedef Node<Cart> CartNode;
    typedef Node<Client> ClientNode;

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

    };
     /*CREAR PILAS Y COLAS (L.D.E)*/
}


#endif // STRUCTS_H
