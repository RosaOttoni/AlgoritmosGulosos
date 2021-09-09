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

Graph* leituraInstancia(ifstream& input_file, int directed, int weightedEdge, int weightedNode){

    //Variáveis para auxiliar na criação dos nós no Grafo
    int idNodeSource;
    int idNodeTarget;
    int order;
    int numEdges;

    //Pegando a ordem do grafo
    input_file >> order >> numEdges;

    //Criando objeto grafo
    Graph* graph = new Graph(order, directed, weightedEdge, weightedNode);

    //Leitura de arquivo
    while(input_file >> idNodeSource >> idNodeTarget) {

        graph->insertEdge(idNodeSource, idNodeTarget, 0);

    }

    return graph;
}

int menu(){

    int selecao;

    cout << "------------------------------------------------------" << endl;
    cout << "-------------------------MENU-------------------------" << endl;
    cout << "------------------------------------------------------" << endl;
    cout << "[1] Fecho Transitivo Direto" << endl;
    cout << "[2] Fecho Transitivo Indireto" << endl;
    cout << "[3] Caminho Mínimo entre dois vértices - Djkstra" << endl;
    cout << "[4] Caminho Mínimo entre dois vértices - Floyd" << endl;
    cout << "[5] Árvore Geradora Mínima de Prim" << endl;
    cout << "[6] Árvore Geradora Mínima de Kruskal" << endl;
    cout << "[7] Imprimir caminhamento em Profundidade" << endl;
    cout << "[8] Imprimir Ordenacao Topológica" << endl;
    cout << "[0] Sair" << endl;
    cout << "------------------------------------------------------" << endl;

    cout << "\nDigite a Opcao Desejada: ";
    cin >> selecao;

    return selecao;

}

void selecionar(int selecao, Graph* graph, ofstream& output_file){

    switch (selecao) {

        //Fecho Transitivo Direto
        case 1:{
            int id;
            cout << "Digite o ID do vertice: ";
            cin >> id;
            graph->directedTransitiveClosure(output_file, id);
            break;
        }
            //Fecho Transitivo Indireto
        case 2:{
            int id;
            cout << "Digite o ID do vertice: ";
            cin >> id;
            graph->indirectedTransitiveClosure(output_file, id);
            break;
        }

            //Caminho Mínimo entre dois vértices - Djkstra;
        case 3:
        {
            int idOrig, idDest;
            cout << "Digite o id do no inicial: ";
            cin >> idOrig;
            cout << "Digite o id do no de destino: ";
            cin >> idDest;

                if (!(idOrig < 0 || idOrig >= graph->getOrder()) || !(idDest < 0 || idDest >= graph->getOrder()))
                {
                    graph->dijkstra(idOrig, idDest, output_file);
                }
                else
                {
                    cout << endl
                        << "Id inexistente!" << endl;
                }
            break;
        }
            //Caminho Mínimo entre dois vértices - Floyd;
        case 4:{
            int idOrig, idDest;
            cout << "Digite o id do no inicial: ";
            cin >> idOrig;
            cout << "Digite o id do no de destino: ";
            cin >> idDest;

            if (!(idOrig < 0 || idOrig >= graph->getOrder()) || !(idDest < 0 || idDest >= graph->getOrder()))
            {
                graph->floyd(idOrig, idDest, output_file);
            }
            else
                cout << endl
                     << "Id inexistente!" << endl;
            break;
        }

            //Árvore Geradora Mínima de Prim;
        case 5:{

            graph->AGMPrim(graph->getVertInduz(),output_file);
            break;
        }


        case 6:{

            graph->AGMKruskal(graph->getVertInduz(), output_file);
            break;
        }
            //Imprimir caminhamento em Profundidade;
        case 7:{
            int id;
            cout << "Digite o ID do vertice: ";
            cin >> id;
            graph->depth(output_file, id);
            break;
        }

        //Imprimir Ordenacao Topológica;
        case 8:{
            graph->topologicalSorting(output_file);
            getchar();
            getchar();

            break;
        }

        default:
        {
            cout << "Erro!!!Opcao Invalida!!!" << endl;
            getchar();
            getchar();
        }

    }
}

int mainMenu(ofstream& output_file, Graph* graph){

    int selecao = menu();

    while(selecao != 0){

        if(output_file.is_open())
            selecionar(selecao, graph, output_file);

        else
            cout << "Unable to open the output_file" << endl;

        output_file << endl;

        system("clear");
        selecao = menu();
    }

    if(output_file.is_open()){
        output_file.close();
    }

    return 0;
}

int main(int argc, char const *argv[]) {

    //Verificação se todos os parâmetros do programa foram entrados
    if (argc != 6) {

        cout << "ERROR: Expecting: ./<program_name> <input_file> <output_file> <directed> <weighted_edge> <weighted_node> " << endl;
        return 1;

    }

    string program_name(argv[0]);
    string input_file_name(argv[1]);

    string instance;
    if(input_file_name.find("v") <= input_file_name.size()){
        string instance = input_file_name.substr(input_file_name.find("v"));
        cout << "Running " << program_name << " with instance " << instance << " ... " << endl;
    }

    //Abrindo arquivo de entrada
    ifstream input_file;
    ofstream output_file;
    input_file.open(argv[1], ios::in);
    output_file.open(argv[2], ios::out | ios::trunc);

    Graph* graph;

    if(input_file.is_open()){

        graph = leitura(input_file, atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));

    }else
        cout << "Unable to open " << argv[1];

    mainMenu(output_file, graph);

    //Fechando arquivo de entrada
    input_file.close();

    //Fechando arquivo de saída
    output_file.close();

    return 0;
}

