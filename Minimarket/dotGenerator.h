#ifndef DOT_GENERATOR
#define DOT_GENERATOR

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "structs.h"



namespace dotGenerator{
    using namespace std;
    using namespace structures;

    class SubGraph{
        private:
        string tittle;
        string context; //aca voy a colocar todo en un texto en conjunto, luego lo meto todo al dotfile

        public:
            string getTittle() const {return tittle;}
            string getContext() const { return context;}

            void changeName(string new_name){
                if (context == ""){
                    context = "subgraph cluster_" + new_name +"{\nlabel=\""+ new_name +"\"\n";
                } else{
                    int pos = context.find("{");
                    string context_wth_name = context.substr(pos+1);
                    context = "subgraph cluster_" + new_name +"{\nlabel=\""+ new_name +"\"\n" + context_wth_name;
                }
                tittle = new_name;

            }

            void insertNode(string name){
                if (context == ""){
                    changeName("na");
                }
                context += tittle + "_" + name + "[label=\"" + name +"\"];\n";
            }

            void insertNode(int name){
                if (context == ""){
                    changeName("na");
                }
                string name_ = to_string(name);
                context +=  tittle + "_" + name_ + "[label=\"" + name_ +"\"];\n";
            }

            void simpleConnectNode(string nodeA, string nodeB){
                context += nodeA + " -> " + nodeB +";\n";
            }
    };


    // voy a hacer una linkedList de subgraphs, para poder tener una lista de los subgraphs de mi dotFile (está el diagrama que hice)
    typedef Node<SubGraph> SubGraphNode;

    class SubgraphQueue : private LinkedList<SubGraph>{
        using LinkedList::head;

        private:
        SubGraphNode* createNode(SubGraph* subgraph){
            SubGraphNode* new_node = new SubGraphNode;
            new_node->data = *subgraph;
            new_node->next = nullptr;
            return new_node;
        }

        public:
        void add(SubGraph* subgraph){
            SubGraphNode* new_node = createNode(subgraph);
            insert(new_node);
        }

        string getContexts(){
            string data = "";
            if (head != nullptr){
                SubGraphNode* temp = head;
                while(temp != nullptr){
                    data += temp->data.getContext() + "\n}\n\n";
                    temp = temp->next;
                }
            }
            return data;
        }

    };

    class DotFile{
        private:
            string tittle;
            string context;
            SubgraphQueue queue_subgraph;

            void constructFile(const string filename) {
                ofstream outFile(filename);

                if (!outFile) return;

                outFile << context;
                outFile.close();
            }

        public:

            SubgraphQueue* getQueue() {return &queue_subgraph;}

            void updateSubGraphs(){
                context = "digraph MiniMarket {\n" + queue_subgraph.getContexts() +"\n}";

            }

            int generateFile(){
                constructFile("graph.dot");
                int result = system("dot -Tpng graph.dot -o output.png");
                return result;
            }

    };


}

#endif // DOT_GENERATOR
