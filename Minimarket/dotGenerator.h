#ifndef DOT_GENERATOR
#define DOT_GENERATOR

#include <iostream>
#include <fstream>

namespace dotGenerator{
    using namespace std;

    class DotFile{
        private:
          void generate(const string filename) {
            ofstream outFile(filename);
            if (!outFile) return;

            outFile << "digraph MiniMarket {\n";
            outFile << "subgraph cluster_1{";
            outFile << "nodeA [label=\"Hello\"];\n";
            outFile << "nodeB [label=\"World\"];\n";
            outFile << "nodeA -> nodeB [label=\"edge\"];\n";
            outFile << "}\n";
            outFile << "}\n";
            outFile.close();
        } //generate("graph.dot")
    };

    class SubGraph{

        string context; //aca voy a colocar todo en un texto en conjunto, luego lo meto todo al dotfile
        SubGraph(string name){

            this->context = "subgraph" + name +"{\n";
        }

        public:
            void insertNode(string name){
                this->context += name + "[label=\"" + name +"\"];\n";
            }

            void simpleConnectNode(string nodeA, string nodeB){
                this->context += nodeA + "->" + nodeB +";\n";
            }
    };


}

#endif // DOT_GENERATOR
