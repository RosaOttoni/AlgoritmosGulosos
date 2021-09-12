/**************************************************************************************************
 * Implementation of the TAD Graph
**************************************************************************************************/

#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED
#include "Node.h"
#include <fstream>
#include <stack>
#include <list>
#include <vector>

using namespace std;

//Criar uma struct origem, destino, peso
/**
 * Classe que define uma aresta simples auxiliar ao algoritmo de Kruskal
 */
typedef struct
{
    int origin;
    int destiny;
    int weight;
} myEdge;

typedef struct
{
    int label;
    int frequency;
} myLabel;

typedef struct
{
    int quantity;
    float average;
}myAverage;

class Graph{

    //Atributes
    private:

        int order;

        int number_edges;

        int number_labels;

        bool directed;

        bool weighted_edge;

        bool weighted_node;

        Node* first_node;

        Node* last_node;

        list <int> directedTransitiveClosureRec(list <int> &closureD, int id);

        list <int> indirectedTransitiveClosureRec(list <int> &closureI, int id);

        void depthRecursive(int id, list<myEdge> &treeEdge, list<myEdge> &returnEdge, int *father, int time, int *timeFind, int *timeFinish);

        void outputdepth(ofstream &output_file, list<myEdge> treeEdge, list<myEdge> returnEdge);

        void printFloyd(int **path, int **cost, int idOrig, int idDest, int directed, ofstream &output_file);

        void printPathFloyd(int **path, int vertice1, int vertice2, ofstream &output_file);

        void dotFloyd(int **path, int **cost, int idOrig, int idDest, int directed, ofstream &output_file);

        void auxDotPathFloyd(int **path, int idOrig, int idDest, int directed, ofstream &output_file);

        void auxDijkstra(float *dist, int *aPercorrer, int *Nonterior, int *map, int atual);

        int mapeamento(int *map, int id);

        void dotDijkstra(int *map, int *noAnterior, float *dist, int idOrig, int idDest, int directed, ofstream &output_file);

        void auxPathDotDijkstra(int *map, int *noAnterior, float *dist, int idOrig, int idDest, int directed, ofstream &output_file);

        void printDijkstra(int *map, int *noAnterior, float *dist, int idOrig, int idDest, int directed, ofstream &output_file);

        void FrequencyLabels(vector<myLabel> &labels);

        void AddEdgesLabel(Graph *graph, int label);

        bool IsComplete(Graph *solution);

        void AuxComponent(Graph *solution, int component[], int vertex, int id);

    public:

        //Constructor
        Graph(int order, bool directed, bool weighted_edge, bool weighted_node);

        Graph(int order, int number_labels);

        //Destructor
        ~Graph();

        //Getters
        int getOrder();

        int getNumberEdges();

        bool getDirected();

        bool getWeightedEdge();

        bool getWeightedNode();

        Node* getFirstNode();

        Node* getLastNode();

        int getNumberLabels();

        void Heuristic();

        // Setters
        void setFirstNode(Node* node);

        void setLastNode(Node* node);

        //Other methods
        void insertNode(int id);

        void insertEdge(int id, int target_id, float weight);

        void removeNode(int id);

        bool searchNode(int id);

        Node* getNode(int id);

        void cleanVisited();

        void printGraph();

        void insertEdgeLabel(int id, int target_id, int label);

        //methods phase1
        void directedTransitiveClosure(ofstream &output_file, int id);

        void indirectedTransitiveClosure(ofstream &output_file, int id);

        void topologicalSorting(ofstream &output_file);

	    void breadthFirstSearch(ofstream &output_file);

        Graph* getVertexInduced(int* listIdNodes);

        void printKruskal(ofstream &output_file, myEdge solution[], int numberSolution, int Ordem);

        void AGMKruskal(Graph *subgrafo, ofstream &output_file);

	    void auxprintPrim(ofstream &output_file, int peso, int atualArestas);

        void printPrim(ofstream &output_file, int previousvertex, int indice, int i, int min);

        Node* arestaMenorPeso();

        int arestaPesoDoisNos(Node *n, Node *m);

        Graph *getVertInduz();

        void AGMPrim(Graph *subgrafo, ofstream &output_file);

        float floydMarshall(int idSource, int idTarget);

        void floyd(int idOrig, int idDest, ofstream &output_file);

        void dijkstra(int idOrig, int idDest, ofstream &output_file);

        void depth(ofstream &output_file, int id);

        void GreedyAlgorithm ();

        void GreedyAlgorithmRandomized(float alpha, int iterations);

        void GreedyAlgorithmRandomizedReactive(vector <float> alphas, int block, int iterations);
};

#endif // GRAPH_H_INCLUDED
