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
    if (argc < 4) {

        cout << "ERRO: Esperado: ./nome_programa <arquivo_entrada> <arquivo_saida> <metodo> <iteracoes> <alpha>" << endl;
        return 1;

    }

    int method = atoi(argv[3]);

    int iterations;

    float alpha;

    //Abrindo arquivo de entrada
    ifstream input_file;
    ofstream output_file;
    input_file.open(argv[1], ios::in);

    Graph* graph;

    if(input_file.is_open()){

        graph = leituraInstancia(input_file);

    }else{
        cout << "Nao foi possivel abrir o arquivo " << argv[1];
        return 1;
    }

    cout << "Rodando com a instancia " << argv[1] << " ... " << endl << endl;

    switch (method){
        case 1:  graph->GreedyAlgorithm(argv[2]);
        break;

        case 2:

            if(argc < 6){
                cout << "ERRO: Esperado: ./nome_programa <arquivo_entrada> <arquivo_saida> <metodo> <iteracoes> <alpha>" << endl;
                return 1;
            }
            else{
                iterations = atoi(argv[4]);
                alpha = atof(argv[5]);
                graph->GreedyAlgorithmRandomized(alpha, iterations, argv[2]);
            }

        break;

        case 3:

            if(argc < 5){
                cout << "ERRO: Esperado: ./nome_programa <arquivo_entrada> <arquivo_saida> <metodo> <iteracoes>" << endl;
                return 1;
            }
            else{
                iterations = atoi(argv[4]);
                vector <float> alphas;
                alphas.push_back(0.05);
                alphas.push_back(0.10);
                alphas.push_back(0.15);
                alphas.push_back(0.30);
                alphas.push_back(0.50);

                graph->GreedyAlgorithmRandomizedReactive(alphas, 100, iterations, argv[2]);
            }
        break;
    }


    //Fechando arquivo de entrada
    input_file.close();

    //Fechando arquivo de saída
    output_file.close();

    return 0;
}
