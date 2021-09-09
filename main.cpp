#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <utility>
#include <tuple>
#include <iomanip>
#include <stdlib.h>
#include <chrono>
#include "Graph.h"
#include "Node.h"

using namespace std;

Graph* leitura(ifstream& input_file, int directed, int weightedEdge, int weightedNode){

    //Variáveis para auxiliar na criação dos nós no Grafo
    int idNodeSource;
    int idNodeTarget;
    int order;

    //Pegando a ordem do grafo
    input_file >> order;

    //Criando objeto grafo
    Graph* graph = new Graph(order, directed, weightedEdge, weightedNode);

    //Leitura de arquivo

    if(!graph->getWeightedEdge() && !graph->getWeightedNode()){

        while(input_file >> idNodeSource >> idNodeTarget) {

            graph->insertEdge(idNodeSource, idNodeTarget, 0);

        }

    }else if(graph->getWeightedEdge() && !graph->getWeightedNode() ){

        float edgeWeight;

        while(input_file >> idNodeSource >> idNodeTarget >> edgeWeight) {

            graph->insertEdge(idNodeSource, idNodeTarget, edgeWeight);

        }

    }else if(graph->getWeightedNode() && !graph->getWeightedEdge()){

        float nodeSourceWeight, nodeTargetWeight;

        while(input_file >> idNodeSource >> nodeSourceWeight >> idNodeTarget >> nodeTargetWeight) {

            graph->insertEdge(idNodeSource, idNodeTarget, 0);
            graph->getNode(idNodeSource)->setWeight(nodeSourceWeight);
            graph->getNode(idNodeTarget)->setWeight(nodeTargetWeight);

        }

    }else if(graph->getWeightedNode() && graph->getWeightedEdge()){

        float nodeSourceWeight, nodeTargetWeight, edgeWeight;

        while(input_file >> idNodeSource >> nodeSourceWeight >> idNodeTarget >> nodeTargetWeight) {

            graph->insertEdge(idNodeSource, idNodeTarget, edgeWeight);
            graph->getNode(idNodeSource)->setWeight(nodeSourceWeight);
            graph->getNode(idNodeTarget)->setWeight(nodeTargetWeight);

        }

    }

    return graph;
}

Graph* leituraInstancia(ifstream& input_file){

    //Variáveis para auxiliar na criação dos nós no Grafo
    int order;
    int number_labels;

    //Pegando a ordem do grafo
    input_file >> order >> number_labels;

    //Criando objeto grafo
    Graph* graph = new Graph(order, number_labels);

    //Leitura de arquivo
    for(int i = 0; i < order; i++){
        for(int j = i + 1; j < order; j++){
            int label;
            input_file >> label;
            //cout << i << " i " << j << " j " << label << endl;
            if(label != number_labels){
                graph->insertEdgeLabel(i, j , label);
            }
        }
    }
    return graph;
}

int main(int argc, char const *argv[]) {

    //Verificação se todos os parâmetros do programa foram entrados
    if (argc < 2) {

        cout << "ERROR: Expecting: ./<program_name> <input_file> <output_file> <directed> <weighted_edge> <weighted_node> " << endl;
        return 1;

    }

    string program_name(argv[0]);
    string input_file_name(argv[1]);

    int method;

    float alpha;

    string instance;
    if(input_file_name.find(".") <= input_file_name.size()){
        string instance = input_file_name.substr(input_file_name.find("."));
        cout << "Rodando " << program_name << "com a instancia " << instance << " ... " << endl;
    }

    //Abrindo arquivo de entrada
    ifstream input_file;
    ofstream output_file;
    input_file.open(argv[1], ios::in);
    output_file.open(argv[2], ios::out | ios::trunc);

    Graph* graph;

    if(input_file.is_open()){

        graph = leituraInstancia(input_file);

    }else{
        cout << "Nao foi possivel abrir o arquivo " << argv[1];
        return 1;
    }

    cout << "Escolha um dos Algoritmos abaixo:" << endl <<
                "1 - Algoritmo Guloso" << endl <<
                "2 - Algoritmo Guloso Randomizado" << endl <<
                "3 - Algoritmo Guloso Randomizado Reativo" << endl <<
                "4 - Sair" << endl;
    cin >> method;

    while(method != 4){

        switch (method){
            case 1:  graph->GreedyAlgorithm();
            break;


        getchar();
        getchar();
        system("clear");

        cout << "Escolha um dos Algoritmos abaixo:" << endl <<
                "1 - Algoritmo Guloso" << endl <<
                "2 - Algoritmo Guloso Randomizado" << endl <<
                "3 - Algoritmo Guloso Randomizado Reativo" << endl <<
                "4 - Sair" << endl;

        cin >> method;
    }


    //Fechando arquivo de entrada
    input_file.close();

    //Fechando arquivo de saída
    output_file.close();

    return 0;
    }
}
