#include "Graph.h"
#include "Node.h"
#include "Edge.h"
#include <iostream>
#include <fstream>
#include <stack>
#include <queue>
#include <list>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <float.h>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <chrono>

#define INFINITO std::numeric_limits<int>::max()

using namespace std;

/**************************************************************************************************
 * Defining the Graph's methods
**************************************************************************************************/

// Constructor
Graph::Graph(int order, bool directed, bool weighted_edge, bool weighted_node)
{

    this->order = order;
    this->directed = directed;
    this->weighted_edge = weighted_edge;
    this->weighted_node = weighted_node;
    this->first_node = this->last_node = nullptr;
    this->number_edges = 0;
}

Graph::Graph (int order, int number_labels){

    this->order = order;
    this->directed = false;
    this->weighted_edge = false;
    this->weighted_node = false;
    this->first_node = this->last_node = nullptr;
    this->number_edges = 0;
    this->number_labels = number_labels;
}

// Destructor
Graph::~Graph()
{

    Node *next_node = this->first_node;

    while (next_node != nullptr)
    {

        next_node->removeAllEdges();
        Node *aux_node = next_node->getNextNode();
        delete next_node;
        next_node = aux_node;
    }
}

// Getters
int Graph::getOrder()
{

    return this->order;
}
int Graph::getNumberEdges()
{

    return this->number_edges;
}

int Graph::getNumberLabels(){
    return this->number_labels;
 }
//Function that verifies if the graph is directed
bool Graph::getDirected()
{

    return this->directed;
}
//Function that verifies if the graph is weighted at the edges
bool Graph::getWeightedEdge()
{

    return this->weighted_edge;
}

//Function that verifies if the graph is weighted at the nodes
bool Graph::getWeightedNode()
{

    return this->weighted_node;
}

Node *Graph::getFirstNode()
{

    return this->first_node;
}

Node *Graph::getLastNode()
{

    return this->last_node;
}

void Graph::setFirstNode(Node *node)
{
    this->first_node = node;
}

void Graph::setLastNode(Node *node)
{
    this->last_node = node;
}

// Other methods
/*
    The outdegree attribute of nodes is used as a counter for the number of edges in the graph.
    This allows the correct updating of the numbers of edges in the graph being directed or not.
*/
void Graph::insertNode(int id)
{
    if (this->getFirstNode() == nullptr)
    {
        Node *node = new Node(id);
        this->setFirstNode(node);
        this->setLastNode(node);
    }
    else
    {
        if (!this->searchNode(id))
        {
            Node *node = new Node(id);
            this->getLastNode()->setNextNode(node);
            this->setLastNode(node);
        }
        else
        {
            cerr << "Error: Node " << id << " already exists" << endl;
        }
    }
}

void Graph::insertEdge(int id, int target_id, float weight)
{
    Node *node = this->getNode(id);
    Node *target_node = this->getNode(target_id);

    if (node == nullptr)
    {
        this->insertNode(id);
        node = this->getNode(id);
    }
    if (target_node == nullptr)
    {
        this->insertNode(target_id);
        target_node = this->getNode(target_id);
    }
    if (!node->searchEdge(target_id))
    {
        this->number_edges++;

        if (this->getDirected())
        {
            node->insertEdge(target_id, weight);
            node->incrementOutDegree();
            target_node->incrementInDegree();
        }
        else
        {
            node->insertEdge(target_id, weight);
            target_node->insertEdge(id, weight);
        }
    }
}

void Graph::removeNode(int id)
{
    if (this->searchNode(id))
    {
        Node *node_current = this->getFirstNode();
        Node *node_previous = nullptr;

        while (node_current->getId() != id)
        {
            node_previous = node_current;
            node_current = node_current->getNextNode();
        }
        if (node_previous == nullptr)
        {
            this->setFirstNode(node_current->getNextNode());
        }
        else
        {
            node_previous->setNextNode(node_current->getNextNode());
        }
        if (node_current->getNextNode() == nullptr)
        {
            this->setLastNode(node_previous);
        }
        node_current->removeAllEdges();
        delete node_current;
    }
    else
    {
        cerr << "Error: Node " << id << "does not exist" << endl;
    }
}

bool Graph::searchNode(int id)
{
    if (this->first_node != nullptr)
    {
        // Searching for a specific node of a id equal to target id
        for (Node *aux = this->first_node; aux != nullptr; aux = aux->getNextNode())
        {
            if (aux->getId() == id)
            {
                return true;
            }
        }
    }

    return false;
}

Node *Graph::getNode(int id)
{
    if (this->first_node != nullptr)
    {
        // Searching for a specific node of a id equal to target id
        for (Node *aux = this->first_node; aux != nullptr; aux = aux->getNextNode())
        {
            if (aux->getId() == id)
            {
                return aux;
            }
        }
    }
    return nullptr;
}
void Graph::printGraph()
{
    cout << "Impressao Grafo: " << endl;
    for (Node *aux = this->getFirstNode(); aux != nullptr; aux = aux->getNextNode())
    {
        cout << aux->getId();
        for (Edge *adj = aux->getFirstEdge(); adj != nullptr; adj = adj->getNextEdge())
        {
            cout << " -  " << adj->getTargetId() << " (" << adj->getLabel() << ")";
        }
        cout << endl;
    }
}

void Graph::insertEdgeLabel(int id, int target_id, int label)
{
    Node *node = this->getNode(id);
    Node *target_node = this->getNode(target_id);

    if (node == nullptr)
    {
        this->insertNode(id);
        node = this->getNode(id);
    }
    if (target_node == nullptr)
    {
        this->insertNode(target_id);
        target_node = this->getNode(target_id);
    }
    if (!node->searchEdge(target_id))
    {
        this->number_edges++;

        if (this->getDirected())
        {
            node->insertEdgeLabel(target_id, label);
            node->incrementOutDegree();
            target_node->incrementInDegree();
        }
        else
        {
            node->insertEdgeLabel(target_id, label);
            target_node->insertEdgeLabel(id, label);
        }
    }
}

//Fecho Transitivo Direto Recursivo
list <int> Graph::directedTransitiveClosureRec(list <int> &closureD, int id){

    closureD.push_back(id);
    Node *node = getNode(id);

    //Percorre os vertices adjacentes
    for(Edge *adj = node->getFirstEdge(); adj != nullptr; adj = adj->getNextEdge())
    {
        int targetId = adj->getTargetId();

        //Verifica se o vertice ja esta no fecho
        if(find(closureD.begin(),closureD.end(),targetId)== closureD.end())
        {
            directedTransitiveClosureRec(closureD,targetId);
        }
    }

    return closureD;
}

//Fecho Transitivo Indireto Recursivo
list <int> Graph::indirectedTransitiveClosureRec(list <int> &closureI, int id){

    closureI.push_back(id);

    //Percorre a lista de nos
    for(Node *aux = this->getFirstNode();aux != nullptr; aux = aux->getNextNode())
    {
        int nodeId = aux->getId();

        //Percorre a lista adjacente a procura de arestas em que o no e destino
        for(Edge *adj = aux->getFirstEdge(); adj != nullptr; adj = adj->getNextEdge())
        {
            int targetId = adj->getTargetId();

            //Verifica se o no e o destino
            if((targetId == id) && (find(closureI.begin(),closureI.end(),nodeId)==closureI.end()))
            {
                indirectedTransitiveClosureRec(closureI,nodeId);
                break;
            }
        }
    }
    return closureI;
}

//Impressao do Fecho Transitivo Direto
void Graph::directedTransitiveClosure(ofstream &output_file, int id){

    list <int> closureD;

    closureD = directedTransitiveClosureRec(closureD,id);

    list<int>::iterator it;

    cout << "---------Fecho Transitivo Direto--------------" << endl;
    cout << "----------------------------------------------" << endl;

    //Imprime o fecho transitivo direto na tela
    for(it = closureD.begin(); it != closureD.end(); it++){
        cout << *it << " ";
    }
    cout << endl << "----------------------------------------------" << endl;

    //saida .dot
    output_file << "graph {" << endl;
    for(it = closureD.begin(); it != closureD.end(); it++){
        output_file << *it << endl;
    }
    output_file << "}";
    getchar();
    getchar();
}

//Impressao do Fecho Transitivo Indireto
void Graph::indirectedTransitiveClosure(ofstream &output_file, int id){

    list <int> closureI;

    indirectedTransitiveClosureRec(closureI,id);

    list<int>::iterator it;

    cout << "---------Fecho Transitivo Indireto------------" << endl;
    cout << "----------------------------------------------" << endl;

    //Imprime o fecho transitivo indireto na tela
    for(it = closureI.begin(); it != closureI.end(); it++){
        cout << *it << " ";
    }
    cout << endl << "----------------------------------------------" << endl;

    //saida .dot
    output_file << "graph {" << endl;
    for(it = closureI.begin(); it != closureI.end(); it++){
        output_file << *it << endl;
    }
    output_file << "}";
    getchar();
    getchar();
}

//Funcao para marcar os nos do grafo como nao visited
void Graph::cleanVisited()
{

    //Ponteiro para percorrer entre os nos
    Node *n = this->getFirstNode();

    while (n != nullptr)
    {
        //Seta o no como nao visitado
        n->setVisited(false);
        //Ponteiro passa a apontar para o proximo no do grafo.
        n = n->getNextNode();
    }
}

//Ordenacao Topologica
void Graph::topologicalSorting(ofstream &output_file)
{
    //Lista de vertices fontes
    list<Node*> sources;

    //Lista com a ordenacao topologica
    list<int> sorting;

    // Percorre a lista de vertice e encontra os vertices que s??o fontes
    for(Node *aux = this->getFirstNode();aux != nullptr; aux = aux->getNextNode()){
        //Testa o grau de entrada
        if(aux->getInDegree() == 0){
            //Insere os vertices fontes na lista
            sources.push_back(aux);
        }
    }
    //Enquanto tiver algum vertice fonte percorre a lista
    while (!sources.empty()){
        //Retira o primeiro vertice da lista de fonte
        Node *s = sources.front();
        sources.pop_front();
        //Insere o vertice na ordenacao topologica
        sorting.push_back(s->getId());

        //Percorre os adjacentes de s
        for(Edge *adj = s->getFirstEdge(); adj != nullptr; adj = adj->getNextEdge()){
            //Busca o vertice adjacente na lista de vertices
            Node *node = this->getNode(adj->getTargetId());
            //Reduz o grau de entrada do vertice
            node->decrementInDegree();
            //Se for fonte insere na lista de fontes
            if(node->getInDegree() == 0){
                sources.push_back(node);
            }
        }
    }
    //Impressao da ordenacao topologica
    if(sorting.size() == this->getOrder()){
        list<int>::iterator it;

        cout << "---------Ordenacao Topologica-----------------" << endl;
        cout << "----------------------------------------------" << endl;

        for(it = sorting.begin(); it != sorting.end(); it++){
            cout << (*it) << " ";
        }
        cout << endl <<"----------------------------------------------" << endl;
    }
    else{
        cout << "Nao e um grafo aciclico direcionado" << endl;
    }
    getchar();
    getchar();
}
// Funcao para gerar um subgraph Vertice Induzido
Graph *Graph::getVertInduz()
{
    cout << "\nDigite os IDs dos vertices do subgraph separados por ponto-virgula" << endl;
    cout << "Ex: 2;7;3 " << endl;

    // Lendo os vertices do subgraph
    string aux;
    cout << "Vertices: ";
    cin >> aux;

    // Vector para armazenar os ids dos vertices do subgraph
    vector<int> idvertices;
    idvertices.clear();

    // Separando a string
    stringstream ss(aux);
    while (getline(ss, aux, ';'))
    {
        if (this->searchNode(stoi(aux)))
            idvertices.push_back(stoi(aux));
        else
            cout << "Vertice invalido, " << aux << " nao se encontra no grafo original" << endl;
    }

    // Criar o subgraph vertice induzido
    Graph *subgraph = new Graph(idvertices.size(), this->getDirected(), this->getWeightedEdge(), this->getWeightedNode());

    // Inserindo as arestas correspondentes no subgraph
    this->cleanVisited();
    for (int i = 0; i < idvertices.size(); i++)
    {
        for (int j = i + 1; j < idvertices.size(); j++)

            // Verificar se a aresta realmente existe no grafo original
            if ((!this->getNode(idvertices[j])->getVisited()) && this->getNode(idvertices[i])->searchEdge(idvertices[j]))
            {
                Edge *aux = this->getNode(idvertices[i])->getEdge(idvertices[j]);
                subgraph->insertEdge(idvertices[i], idvertices[j], aux->getWeight());
            }
        this->getNode(idvertices[i])->setVisited(true);
    }

    cout << "\nO subgraph X feito com sucesso! ";
    cout << "(Ordem = " << subgraph->getOrder() << " e Numero de Arestas = " << subgraph->getNumberEdges() << ")" << endl;

    return subgraph;
}

bool checkNode(myEdge nos_visited[], int id_node, int id_destiny, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (nos_visited[i].origin == id_destiny && nos_visited[i].destiny == id_node)
        {
            return true;
        }
    }
    return false;
}

//Uma funcao de utilidade para trocar dois elementos
void swap(myEdge *a, myEdge *b)
{
    myEdge t = *a;
    *a = *b;
    *b = t;
}

/* Esta funcao pega o ultimo elemento como pivo, coloca o elemento pivo em sua posicao correta
*  na matriz classificada e coloca todos os elementos menores (menores do que o pivo) a esquerda
*  do pivo e todos os elementos maiores ?? direita do pivo
*/
int partition(myEdge arr[], int low, int high)
{
    //pivo
    int pivot = arr[high].weight;

    //indice do elemento menor
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++)
    {
        //Se o elemento current for menor que ou
        //igual ao pivo
        if (arr[j].weight <= pivot)
        {
            //incrementa o indice do elemento menor
            i++;
            //e troca o indice menor i com o current j
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSort(myEdge arr[], int low, int high)
{
    if (low < high)
    {
        //pi eh o indice de particionamento
        int pi = partition(arr, low, high);

        //Classificamos os elementos separadamente antes
        //particao e depois da particao
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void Graph::printKruskal(ofstream &output_file, myEdge solution[], int numberSolution, int Ordem)
{
    cout << "-------------------------------" << endl;
    int summationWeights = 0;
    cout << "---------AGM KRUSKAL-----------" << endl;
    cout << "[No_origem -- No_destino] - Peso" << endl;
    cout << "-------------------------------" << endl;
    cout << "Ordem:" << Ordem << endl;
    for (int i = 0; i < numberSolution; i++)
    {
        cout << "[" << solution[i].origin << "-> " << solution[i].destiny << "] - " << solution[i].weight << endl;

        summationWeights += solution[i].weight;
    }
    cout << "Somatorio dos peso: " << summationWeights << endl;
    cout << "Quantidade de arestas: " << numberSolution << endl;
    cout << "-------------------------------" << endl;

    //Saida dot
    output_file << "graph {" << endl;
    for (int i = 0; i < numberSolution; i++)
    {
        output_file << "  " << solution[i].origin << " -- " << solution[i].destiny;
        output_file << " [label = " << solution[i].weight << "]" << endl;
    }
    output_file << "}" << endl;
    cout << "Impressao Finalizada!" << endl;
}

/**
 * Arvore geradora minima gerado pelo algoritmo de Kruskal
 * @param subgraph subgraph induzido com subconjunto X de vertices
 * @param output_file arquivo de saida de dados
 */
void Graph::AGMKruskal(Graph *subgraph, ofstream &output_file)
{

    //Numero de Arestas
    int numberEdges = subgraph->getNumberEdges();

    //Numero de Nos
    int V = subgraph->getOrder();

    //Criar uma lista L com as arestas
    myEdge *list_edges = new myEdge[numberEdges];

    //Ponteiro para andar entre os nos
    Node *p = subgraph->getFirstNode();

    //Ponteiro para andar entre as arestas
    Edge *a = p->getFirstEdge();

    //variavel auxiliar para armazenar weight
    int auxweight;

    //contador
    int i = 0;

    //percorre o this induzido preenchendo as informa????es das arestas

    //Percorre no subgraph
    while (p != nullptr)
    {
        a = p->getFirstEdge();

        while (a != nullptr)
        {
            //verifica se no ja foi visitado
            if (!checkNode(list_edges, p->getId(), a->getTargetId(), i))
            {
                list_edges[i].origin = p->getId();
                list_edges[i].destiny = a->getTargetId();
                list_edges[i].weight = a->getWeight();
                i++;
            }
            a = a->getNextEdge();
        }
        p = p->getNextNode();
    }

    quickSort(list_edges, 0, numberEdges - 1);

    //vetor com as arestas da solution
    myEdge *solution = new myEdge[numberEdges];

    //contador de numero de arestas na solution
    int numberSolution = 0;

    //indice para solution
    int current = 0;

    //vetor com tamanho de Arestas
    int tree[numberEdges];

    //inicia o vetor tree de 0 a numberEdges
    for (int i = 0; i < numberEdges; i++)
    {
        tree[i] = i;
    }

    //percorre as arestas adicionando-as no vetor tree e na solucao
    int c = 0;
    while (c < numberEdges)
    {
        int u = list_edges[c].origin;
        int v = list_edges[c].destiny;

        if (tree[u] != tree[v])
        {
            //Coloca a aresta na solucao
            solution[current] = list_edges[c];

            //Incrementa indice de solution e numberSolution
            current++;
            numberSolution++;

            //O vertice antigo recebe a origem
            int oldvertex = tree[u];
            //O vertice novo recebe o destino
            int newvertex = tree[v];

            //Atualiza o vetor de "arvore" para o indice do vertice novo (id destino)
            for (int j = 0; j < numberEdges; j++)
            {
                if (tree[j] == oldvertex)
                {
                    tree[j] = newvertex;
                }
            }
        }
        c++;
    }
    //Imprime Solucao
    printKruskal(output_file, solution, numberSolution, subgraph->getOrder());

    //desaloca
    delete[] list_edges;
    delete[] solution;

    getchar();
    getchar();
}


//so imprime o final
void Graph::auxprintPrim(ofstream &output_file, int weight, int currentEdge)
{
    cout << "--------------------------" << endl;
    cout << "Peso Total: " << weight << endl;
    cout << "Numero de arestas: " << currentEdge << endl;
    cout << "--------------------------" << endl;

    output_file << "}" << endl;
    cout << "Impressao Finalizada!" << endl;
}
void Graph::printPrim(ofstream &output_file, int previousvertex, int indice, int i, int min)
{
    //para imprimir apenas na 1a vez
    if (i == 0)
    {
        cout << "---------AGM Prim---------" << endl;
        cout << "No -- No  |  Peso" << endl;
        cout << "--------------------------" << endl;
        output_file << "graph {" << endl;
    }

    cout << previousvertex << " -- " << indice << " | " << min << endl;

    //Imprime dot
    output_file << "  " << previousvertex << " -- " << indice;
    output_file << " [label = " << min << "]" << endl;
}

void Graph::AGMPrim(Graph *subgraph, ofstream &output_file)
{

    //guarda os nos com (ids) e os weights
    int **graph;

    //guarda os menores custos
    int *mincost = new int[this->getOrder()];

    //preenche com os vertices adjacentes
    int *moreNext = new int[this->getOrder()];

    //para marcar os vertices visitados
    bool *visited = new bool[this->getOrder()];

    //aloca a matriz
    graph = new int *[this->getOrder()];


    //preenche a matriz graph e mincost com o valor INFINITO
    for (int i = 0; i < this->getOrder(); i++)
    {
        graph[i] = new int[this->getOrder()];
        mincost[i] = INFINITO;
        for (int j = 0; j < this->getOrder(); j++)
        {
            graph[i][j] = INFINITO;
        }
    }


    //preenche a matriz de adjac??ncias com o peso do subgrafo
    for (Node *p = subgraph->getFirstNode(); p != nullptr; p = p->getNextNode())
    {
        for (Edge *a = p->getFirstEdge(); a != nullptr; a = a->getNextEdge())
        {
            graph[p->getId()][a->getTargetId()] = a->getWeight();
            graph[a->getTargetId()][p->getId()] = a->getWeight();
        }
    }

    Node *firstnode = subgraph->getFirstNode();


    //preenche o vetor de menores custos com os menores custos da matriz de adjac??ncias
    for (int i = 0; i < this->getOrder(); i++)
    {
        mincost[i] = graph[firstnode->getId()][i];
        moreNext[i] = firstnode->getId();
        visited[i] = false;
    }

    //marca o primeiro vertice como visitado
    visited[firstnode->getId()] = true;

    //variaveis auxiliares
    int weight = 0;
    int currentEdge = 0;
    int previousvertex = firstnode->getId();

    //para o tamanho do grafo
    for (int i = 0; i < this->getOrder(); i++)
    {
        int min = INFINITO;
        int indice = 1;

        // percorre o vetor de menores custos currentizando-o de acordo com os nos ja visitados
        for (int j = 0; j < this->getOrder(); j++)
        {
            if (mincost[j] < min && !visited[j])
            {
                min = mincost[j];
                indice = j;
            }
        }

        //caso o indice seja 1 e o minimo sej?? INFINITO, nao salvo no contador de peso
        if (indice == 1 && min == INFINITO)
        {
            break;
        }
        else
        {
            weight += min;
        }

        //Imprime vertice anterior indice e peso
        printPrim(output_file, previousvertex, indice, i, min);

        //Atualiza os vertices
        previousvertex = indice;
        visited[indice] = true;
        currentEdge++;

        //Atualiza os custos e o proximo vertice
        for (int j = 0; j < this->getOrder(); j++)
        {

            if ((graph[indice][j] < mincost[j]) && (!visited[j]))
            {
                //mincost representa o peso minimo requerido para ir de um ponto a outro em uma "arvore"
                mincost[j] = graph[indice][j];
                moreNext[j] = indice;
            }
        }
    }

    auxprintPrim(output_file, weight, currentEdge);

    //Desalocacao
    for (int i = 0; i < this->getOrder(); i++)
    {
        delete[] graph[i];
    }
    delete[] graph;
    delete[] mincost;
    delete[] moreNext;
    delete[] visited;

    getchar();
    getchar();
}

//Caminhamento em Profundidade
void Graph::depth(ofstream &output_file, int id){
    //Listas de arestas da arvore e de retorno
    list<myEdge> treeEdge, returnEdge;

    //Tempo de descoberta do vertice
    int *timeFind = new int[this->getOrder()];
    //Tempo de finalizacao da descoberta de todos os vertices adjacentes
    int *timeFinish = new int[this->getOrder()];
    //Inicializacao do tempo
    int time = 0;
    //Vetor que guarda o pai de cada vertice na arvore
    int *father = new int[this->getOrder()];

    //Inicializacao dos vetores
    for(int i = 0; i < this->getOrder(); i++){
        timeFind[i] = 0;
        timeFinish[i] = 0;
        father[i] = -1;
    }

    //Chamada da funcao depht recursiva
    depthRecursive(id, treeEdge, returnEdge, father, time, timeFind, timeFinish);
    //Chamada da funcao depth que gera a saida .dot
    outputdepth(output_file, treeEdge, returnEdge);

    cout << "---------Caminhamento em Profundidade---------" << endl;
    cout << "----------------------------------------------" << endl;

    list<myEdge>::iterator it;
    cout << "Arestas da Arvore: " << endl;
    for(it = treeEdge.begin(); it != treeEdge.end(); it++){
        cout << "[" << (*it).origin << " -- "<< (*it).destiny << "]" << endl;
    }
    cout << "Arestas de Retorno: " << endl;
    for(it = returnEdge.begin(); it != returnEdge.end(); it++){
        cout << "[" << (*it).origin << " -- "<< (*it).destiny << "]" << endl;
    }
    cout << "----------------------------------------------" << endl;

    delete[] timeFind;
    delete[] timeFinish;
    delete[] father;

    getchar();
    getchar();
}

//Saida da Profundidade no formato .dot
void Graph::outputdepth(ofstream &output_file, list<myEdge> treeEdge, list<myEdge> returnEdge){
    //saida .dot
    output_file.clear();

    output_file << "graph {" << endl;

    list<myEdge>::iterator it;

    for(it = treeEdge.begin(); it != treeEdge.end(); it++){
        output_file << (*it).origin << "--"<< (*it).destiny << endl;
    }
    for(it = returnEdge.begin(); it != returnEdge.end(); it++){
        // A aresta de retorno tem o estilo pontilhado
        output_file << (*it).origin << "--"<< (*it).destiny <<  "[style=dotted]" << endl;
    }
    output_file << "}"<< endl;
}
//Profundidade Recursivo
void Graph::depthRecursive(int id, list<myEdge> &treeEdge, list<myEdge> &returnEdge, int *father, int time, int *timeFind, int *timeFinish){

   time++;
   timeFind[id] = time;

    Node *node = getNode(id);

    //Percorre a lista de adjacente do vertice
    for(Edge *adj = node->getFirstEdge(); adj != nullptr; adj = adj->getNextEdge()){
        myEdge edge = {id, adj->getTargetId(), 0};

        //Se o vertice nao foi descoberto
        if(timeFind[adj->getTargetId()] == 0){
            //Adiciona a aresta na arvore
            treeEdge.push_back(edge);

            //Define o vertice pai
            father[adj->getTargetId()] = id;

            //Chamada recursiva
            depthRecursive(adj->getTargetId(), treeEdge, returnEdge, father, time, timeFind, timeFinish);
        }
        else{
            //Se o vertice nao foi finalizado e o pai for diferente do vertice atual, adiciona aresta de retorno
            if((timeFinish[adj->getTargetId()] == 0) && (father[id] != adj->getTargetId())){

                returnEdge.push_back(edge);
            }
        }
    }
    //Define o tempo de finalizacao
    timeFinish[id] = time;
}
//Algoritmo de Floyd

void Graph::floyd(int idOrig, int idDest, ofstream &output_file)
{ //
    // cost[] e matrizAdjac[] armazenam o menor caminho
    int **cost, **path;
    int **matrizAdjac;
    path = new int *[this->getOrder()];
    cost = new int *[this->getOrder()];
    matrizAdjac = new int *[this->getOrder()];

    //inicializa as matrizes
    for (int i = 0; i < this->getOrder(); i++)
    {
        path[i] = new int[this->getOrder()];
        cost[i] = new int[this->getOrder()];
        matrizAdjac[i] = new int[this->getOrder()];
        for (int j = 0; j < this->getOrder(); j++)
        {
            matrizAdjac[i][j] = INFINITO;
            if (i == j)
            {
                matrizAdjac[i][j] = 0;
            }
        }
    }

    Node *n = this->getFirstNode();
    Edge *e;
    while (n != nullptr)
        {
            e = n->getFirstEdge();

            while (e != nullptr)
            {
                    matrizAdjac[n->getId()][e->getTargetId()] = e->getWeight();

                e = e->getNextEdge();
            }
            n = n->getNextNode();
        }

    // Preenche cost[] e path[]
    for (int v = 0; v < this->getOrder(); v++)
    {
        for (int u = 0; u < this->getOrder(); u++)
        {
            // inicialmente o cost sera igual ao peso da aresta
            cost[v][u] = matrizAdjac[v][u];
            if (v == u)
                path[v][u] = 0;
            else if (cost[v][u] != INFINITO)
                path[v][u] = v;
            else
                path[v][u] = -1;
        }
    }

    //algoritmo de Floyd
    //como a complexidade deste algoritmo ?? O(n^3), para instancias muito grandes ele demora a executar
    for (int k = 0; k < this->getOrder(); k++)
    {
        for (int v = 0; v < this->getOrder(); v++)
        {
            for (int u = 0; u < this->getOrder(); u++)
            {
                // caso o vertice k esteja no menor path de v para u, ent??o o valor da posi????o cost[v][u] ?? atualizado
                if (cost[v][k] != INFINITO && cost[k][u] != INFINITO && cost[v][k] + cost[k][u] < cost[v][u])
                {
                    cost[v][u] = cost[v][k] + cost[k][u];
                    path[v][u] = path[k][u];
                }
            }

            // se os elementos da diagonal se tornam negativos, o grafo atua em um ciclo de peso negativo
            if (cost[v][v] < 0)
            {
                cout << "** Ciclo de peso negativo!! **"<<endl;
                return;
            }
        }
    }

    //Plotando o grafo no .dot
    dotFloyd(path, cost, idOrig, idDest, directed, output_file);

    //Resultado em tela
    printFloyd(path, cost, idOrig, idDest, directed, output_file);

    for (int i = 0; i < this->getOrder(); i++)
    {
        delete[] path[i];
        delete[] cost[i];
        delete[] matrizAdjac[i];
    }
    delete[] path;
    delete[] cost;
    delete[] matrizAdjac;
    cout << "-------------------------------------------------------------------" << endl;

    getchar();
    getchar();
}

//Fun????o para imprimir em tela o resultado

void Graph::printFloyd(int **path, int **cost, int idOrig, int idDest, int directed, ofstream &output_file)
{
    cout << "-------------------------------FLOYD-------------------------------" << endl;
    cout << "[caminho minimo entre os nos " << idOrig << " e " << idDest << " ]" << " - custo do caminho minimo" << endl;
    cout << "-------------------------------------------------------------------" << endl;

    if(cost[idOrig][idDest]==INFINITO){
        cout<< "[ "<<idOrig << ", "<< idDest<<"]"<< " - " <<"Nao existe caminho"<<endl;
    }
    else if(idOrig == idDest)
    {
        cout<< "[ " << idOrig << ", "<<idDest<< "] "<< " - "<< "0"<< endl;
    }
    else{
        cout << "[" << idOrig << ", ";
        printPathFloyd(path, idOrig, idDest, output_file);
        cout << idDest << "] - ";
        cout<< cost[idOrig][idDest] << endl;
    }
}


//Auxiliar para imprimir caminho minimo no algoritimo de Floyd

void Graph::printPathFloyd(int **path, int idOrig, int idDest, ofstream &output_file)
{
        if (path[idOrig][idDest] == idOrig)
        return;

        printPathFloyd(path, idOrig, path[idOrig][idDest], output_file);
        cout << path[idOrig][idDest] << ", ";

}

//Funcao para montar o arquivo .dot no algoritimo de Floyd

void Graph::dotFloyd(int **path, int **cost, int idOrig, int idDest, int directed, ofstream &output_file)
{
    Node *n = this->getFirstNode();
    Edge *e;

    if (directed==1)
    {
        output_file << "strict digraph {" << endl;

        while (n != nullptr)
        {
            e = n->getFirstEdge();

            while (e != nullptr)
            {
                output_file << n->getId() <<"->";
                output_file << e->getTargetId() <<" [";
                output_file << "label=" << e->getWeight() <<"];"<<endl;

                e = e->getNextEdge();
            }
            n = n->getNextNode();
        }

        if(cost[idOrig][idDest]==INFINITO){
            output_file <<"}";
        }
        else if(idOrig == idDest){
            output_file << idOrig <<" [color=red];"<<endl;
            output_file <<"}";
        }
        else{
            output_file << idOrig << "->";
            auxDotPathFloyd(path, idOrig, idDest, directed, output_file);
            output_file << idDest << " [color=red];"<<endl;
            output_file <<"}";
        }

    }

    else
    {
    output_file << "strict graph {" << endl;

        while (n != nullptr)
        {
            e = n->getFirstEdge();

            while (e != nullptr)
            {
                output_file << n->getId() <<"--";
                output_file << e->getTargetId() <<" [";
                output_file << "label=" << e->getWeight() <<"];"<<endl;

                e = e->getNextEdge();
            }
            n = n->getNextNode();
        }

        if(cost[idOrig][idDest]==INFINITO){
            output_file <<"}";
        }
        else if(idOrig == idDest){ //(cost[idOrig][idDest]==0)
            output_file << idOrig <<" [color=red];"<<endl;
            output_file <<"}";
        }
        else{
            output_file << idOrig << "--";
            auxDotPathFloyd(path, idOrig, idDest, directed, output_file);
            output_file << idDest << " [color=red];"<<endl;
            output_file <<"}";
        }

    }

}

//Auxiliar para plotar o path minimo no arquivo.dot de Floyd

void Graph::auxDotPathFloyd(int **path, int idOrig, int idDest, int directed, ofstream &output_file)
{
    if(directed==1)
    {
        if (path[idOrig][idDest] == idOrig)
        return;

        auxDotPathFloyd(path, idOrig, path[idOrig][idDest], directed, output_file);
        output_file << path[idOrig][idDest] << "->";
    }
    else
    {
        if (path[idOrig][idDest] == idOrig)
        return;

        auxDotPathFloyd(path, idOrig, path[idOrig][idDest], directed, output_file);
        output_file << path[idOrig][idDest] << "--";
    }

}


//Algoritmo de Dijkstra

void Graph::dijkstra(int idOrig, int idDest, ofstream &output_file)
{
    float *dist = new float[this->getOrder()];
    int *map = new int[this->getOrder()];
    int *aPercorrer = new int[this->getOrder()];
    int *noAnterior = new int[this->getOrder()];

    //preenche os vetores map, a dist e aPercorrer com os valores 0, 1 ou -1
    for (int i = 0; i < this->getOrder(); i++)
    {
        map[i] = i;
        if (i == idOrig)
        {
            dist[i] = 0;
            aPercorrer[i] = 0;
        }
        else
        {
            dist[i] = -1;
            aPercorrer[i] = 1;
        }
        noAnterior[i] = -1;
    }

    auxDijkstra(dist, aPercorrer, noAnterior, map, idOrig);

    dotDijkstra(map, noAnterior, dist, idOrig, idDest, directed, output_file);

    printDijkstra(map, noAnterior, dist, idOrig, idDest, directed, output_file);

    delete[] aPercorrer;
    delete[] noAnterior;
    delete[] dist;
    delete[] map;

    getchar();
    getchar();
}

//Auxiliar para o algoritmo de Dijkstra

void Graph::auxDijkstra(float *dist, int *aPercorrer, int *noAnterior, int *map, int atual)
{
    int indiceAtual = mapeamento(map, atual);
    int indiceAresta;
    Edge *adj;
    Node *aux = this->getFirstNode();

    //enquanto a aresta n??o ?? nula preenche os vetores de dist??ncia e noAnterior
    while (aux != nullptr)
    {
        if(atual == aux->getId())
        {
            while (aux != nullptr)
            {
                adj = aux->getFirstEdge();
                while(adj != nullptr)
                {
                    indiceAresta = mapeamento(map, adj->getTargetId());
                    //caso o indicie atual da aresta n??o seja -1
                    if (dist[indiceAresta] != -1)
                    {
                        if (dist[indiceAresta] > dist[indiceAtual] + adj->getWeight())
                        {
                            dist[indiceAresta] = dist[indiceAtual] + adj->getWeight();
                            noAnterior[indiceAresta] = atual;
                        }
                    }
                     else
                    {
                        dist[indiceAresta] = dist[indiceAtual] + adj->getWeight();
                        noAnterior[indiceAresta] = atual;
                    }
                    adj = adj->getNextEdge();
                }
                aux = nullptr;
            }
        }
        else{
            aux = aux->getNextNode();
            }
    }
    int menor = -1;
    //percorre o vetor de n??s aPercorrer
    for (int i = 0; i < this->getOrder() && menor == -1; i++)
    {
        if (aPercorrer[i])
        {
            if (dist[i] != -1)
            {
                menor = dist[i];
                atual = map[i];
            }
        }
    }
    if (menor != -1)
    {
        for (int i = 0; i < this->getOrder(); i++)
        {
            if (aPercorrer[i])
                if (dist[i] != -1)
                    if (dist[i] < menor)
                    {
                        menor = dist[i];
                        atual = map[i];
                    }
        }
        aPercorrer[indiceAtual] = 0;
        auxDijkstra(dist, aPercorrer, noAnterior, map, atual);
    }
}

//mapeia as arestas de retorno para o algoritmo de busca

int Graph::mapeamento(int *map, int id)
{
    for (int i = 0; i < this->getOrder(); i++)
    {
        if (map[i] == id)
            return i;
    }
    return -1;
}

//Funcao que monta o arquivo .dot do algoritimo de Dijkstra

void Graph::dotDijkstra(int *map, int *noAnterior, float *dist, int idOrig, int idDest, int directed, ofstream &output_file)
{
    Node *n = this->getFirstNode();
    Edge *e;

    if (directed==1)
    {
        output_file << "strict digraph {" << endl;

        while (n != nullptr)
        {
            e = n->getFirstEdge();

            while (e != nullptr)
            {
                output_file << n->getId() <<"->";
                output_file << e->getTargetId() <<" [";
                output_file << "label=" << e->getWeight() <<"];"<<endl;

                e = e->getNextEdge();
            }
            n = n->getNextNode();
        }

        auxPathDotDijkstra(map, noAnterior, dist, idOrig, idDest, directed, output_file);
        output_file <<"}";

    }

    else
    {
        output_file << "strict graph {" << endl;

        while (n != nullptr)
        {
            e = n->getFirstEdge();

            while (e != nullptr)
            {
                output_file << n->getId() <<"--";
                output_file << e->getTargetId() <<" [";
                output_file << "label=" << e->getWeight() <<"];"<<endl;

                e = e->getNextEdge();
            }
            n = n->getNextNode();
        }

        auxPathDotDijkstra(map, noAnterior, dist, idOrig, idDest, directed, output_file);
        output_file <<"}";
    }

}

void Graph::auxPathDotDijkstra(int *map, int *noAnterior, float *dist, int idOrig, int idDest, int directed, ofstream &output_file)
{
 if (dist[mapeamento(map, idDest)] != -1)
    {
        int *guarda = new int[this->getOrder()];
        int i=0;
        guarda[i]=idDest;
        i++;

        int path = noAnterior[mapeamento(map, idDest)];
        //percorre o vetor de n??s a percorrer at?? que encontre uma posi????o igual a -1
        while (path != -1)
        {
            guarda[i]=path;

            path = noAnterior[mapeamento(map, path)];
            i++;
        }
        i--;

        while(i>=0)
        {
            if(directed==1){
                if(i!=0){
                    output_file << guarda[i]<<"->";
                }
                else{
                    output_file << guarda[i];
                }
            }
            else{
                if(i!=0){
                    output_file << guarda[i]<<"--";
                }
                else{
                    output_file << guarda[i];
                }
            }
            i--;
        }
        output_file << " [color=red]; "<< endl;
    }
}

void Graph::printDijkstra(int *map, int *noAnterior, float *dist, int idOrig, int idDest, int directed, ofstream &output_file)
{
    cout << "------------------------------DIJKSTRA------------------------------" << endl;

    cout << "[caminho minimo entre os nos " << idOrig << " e " << idDest << " ]" << " - custo do caminho minimo" << endl;

    cout << "--------------------------------------------------------------------" << endl;

    if (dist[mapeamento(map, idDest)] != -1)
    {
        int *guarda = new int[this->getOrder()];
        int i=0;
        guarda[i]=idDest;
        i++;

        int path = noAnterior[mapeamento(map, idDest)];
        //percorre o vetor de n??s a percorrer at?? que encontre uma posi????o igual a -1
        while (path != -1)
        {
            guarda[i]=path;

            path = noAnterior[mapeamento(map, path)];
            i++;
        }
        i--;
        cout << "[";
        while(i>=0)
        {
            if(i!=0){
                cout << guarda[i]<<", ";
            }
            else{
                cout << guarda[i];
            }
            i--;
        }
        cout << "] - "<< dist[mapeamento(map, idDest)] << endl;
    }
    else
    {
        cout << "[" << idOrig << ", " << idDest << "] - -1 Nao ha caminho";
    }
    cout << "--------------------------------------------------------------------" << endl;
}

void swapLabel(myLabel *a, myLabel *b)
{
    myLabel t = *a;
    *a = *b;
    *b = t;
}

/* Esta funcao pega o ultimo elemento como pivo, coloca o elemento pivo em sua posicao correta
*  na matriz classificada e coloca todos os elementos menores (menores do que o pivo) a esquerda
*  do pivo e todos os elementos maiores ?? direita do pivo
*/
int partitionLabel(vector <myLabel> &arr, int low, int high)
{
    //pivo
    int pivot = arr[high].frequency;

    //indice do elemento menor
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++)
    {
        //Se o elemento current for maior que ou
        //igual ao pivo
        if (arr[j].frequency >= pivot)
        {
            //incrementa o indice do elemento menor
            i++;
            //e troca o indice menor i com o current j
            swapLabel(&arr[i], &arr[j]);
        }
    }
    swapLabel(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSortLabel(vector <myLabel> &arr, int low, int high)
{
    if (low < high)
    {
        //pi eh o indice de particionamento
        int pi = partitionLabel(arr, low, high);

        //Classificamos os elementos separadamente antes
        //particao e depois da particao
        quickSortLabel(arr, low, pi - 1);
        quickSortLabel(arr, pi + 1, high);
    }
}

//Fun????o que conta a frequ??ncia de r??tulos
void Graph::FrequencyLabels(vector<myLabel> &labels){
    //Inicializa a frequ??ncia com 0
    for(int i = 0; i < this->getNumberLabels(); i++){
        myLabel label;
        label.label = i;
        label.frequency = 0;
        labels.push_back(label);
    }
    //Os dois for percorre para preencher a frequ??ncia dos r??tulos
    for (Node *aux = this->getFirstNode(); aux != nullptr; aux = aux->getNextNode())
    {
        for (Edge *adj = aux->getFirstEdge(); adj != nullptr; adj = adj->getNextEdge())
        {
            labels[adj->getLabel()].frequency++;
        }
    }
    //Dividimos por dois, pois o grafo conta cada aresta duas vezes
    for(int i = 0; i < this->getNumberLabels(); i++){
        labels[i].frequency /= 2;
    }
}

//Fun????o que adiciona as arestas de um r??tulo a solu????o
void Graph::AddEdgesLabel(Graph *graph, int label){

    for (Node *aux = this->getFirstNode(); aux != nullptr; aux = aux->getNextNode()){

        for (Edge *adj = aux->getFirstEdge(); adj != nullptr; adj = adj->getNextEdge()){

            if(adj->getLabel() == label){

                graph->insertEdgeLabel(aux->getId(), adj->getTargetId(), label);
            }
        }
    }
}

//Fun????o que testa a solu????o est?? completa atrav??s do n??mero de componentes conexas
bool Graph::IsComplete(Graph *solution){

    int id = 0;
    int number_vertex = solution->getOrder();
    int *component = new int [number_vertex];

    //Inicializa a componente de cada v??rtice com -1
    for(int i = 0; i < number_vertex; i++){
        component[i] = -1;
    }
    for(int i = 0; i < number_vertex; i++){
        if(component[i] == -1){
            AuxComponent(solution, component, i, id++);
        }
    }

    //Se o grafo tiver mais de uma componente conexa ele n??o ?? conexo
    for(int i = 0; i < number_vertex; i++){
        if(component[i] != 0){
            return false;
        }
    }

    return true;
}

//Busca em profundidade para encontrar as componentes conexas
void Graph::AuxComponent(Graph *solution, int component[], int vertex, int id){
    component[vertex] = id;
     Node *node = solution->getNode(vertex);

    //Percorre a lista de adjacente do vertice, caso ele tenha vertices adjacentes
    if(node != nullptr){

        for(Edge *adj = node->getFirstEdge(); adj != nullptr; adj = adj->getNextEdge()){
            //Verifica de se o id j?? foi visitado
            if(component[adj->getTargetId()] == -1){
                AuxComponent(solution, component, adj->getTargetId(), id);
            }
        }
    }
}

//Busca em profundidade para gerar a ??rvore a partir das arestas dos r??tulos escolhidos
void treeSolutionRec(Graph *solution, list<myEdge> &treeSolution, bool visited[], int id){
    visited[id] = true;
    Node *node = solution->getNode(id);

    //Percorre a lista de adjac??ncia do n??
    for(Edge *adj = node->getFirstEdge(); adj != nullptr; adj = adj->getNextEdge()){
        if(!visited[adj->getTargetId()]){
            myEdge edge;
            edge.origin = id;
            edge.destiny = adj->getTargetId();
            edge.label = adj->getLabel();
            treeSolution.push_back(edge);
            treeSolutionRec(solution, treeSolution, visited, adj->getTargetId());
        }
    }
}

//Gera a ??rvore a partir do grafo formado pelas arestas dos r??tulos escolhidos
list<myEdge> getTreeSolution(Graph *solution){
    bool *visited = new bool[solution->getOrder()];
    list<myEdge> treeSolution;

    //Inicializa o vetor de visitados
    for(int i = 0; i < solution->getOrder(); i++){
        visited[i] = false;
    }

    treeSolutionRec(solution, treeSolution, visited, 0);

    return treeSolution;
}

// Algoritmo Guloso
void Graph::GreedyAlgorithm (string output_file){
    //In??cio da contagem de tempo da execu????o do algoritmo
    auto timeStart = std::chrono::high_resolution_clock::now();

    //N??mero de r??tulos da solu????o
    int solutionSize = 0;

    //Inicializa a solu????o somente com os v??rtices de G
    Graph *solution = new Graph(this->getOrder(), false, false, false);

    //Cria o vetor de labels
    vector<myLabel> labels;
    //myLabel *labels = new myLabel[this->getNumberLabels()];

    FrequencyLabels(labels);

   /* for(int i = 0; i < this->getNumberLabels(); i++){
        cout << labels[i].frequency << endl;
    }*/

    quickSortLabel(labels, 0, this->getNumberLabels() - 1);

    /*for(int i = 0; i < this->getNumberLabels(); i++){
        cout << labels[i].label << " " << labels[i].frequency << endl;
    }*/

    //Percorre a lista de r??tulos ordenados de acordo com a heur??stica
    for(int i = 0; i < this->getNumberLabels(); i++){
        //incrementa o tamanho da solu????o
        solutionSize++;
        AddEdgesLabel(solution, labels[i].label);

        //solution->printGraph();

        //Testa se a solu????o parcial ?? um grafo conexo
        if(IsComplete(solution)){
            break;
        }
    }

    list<myEdge> treeSolution = getTreeSolution(solution);

    //Diferen??a de tempo
    auto diff = std::chrono::high_resolution_clock::now() - timeStart;

    //Convers??o para microsegundos
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(diff);

    cout << "Solucao Guloso = " << solutionSize << " rotulos"<< endl;

    cout << "Tempo de execucao = " << time.count() << " milissegundos"<< endl;

    //Imprime sa??da no arquivo .dot
    cout << "Saida impressa no arquivo " << output_file << endl;

    outputTree(treeSolution, output_file);

    //Utilizado para gerar os resultados do relat??rio
    /*FILE* file = fopen(output_file.c_str(), "a+");
    stringstream ss;
    ss << solutionSize << ";" << time.count()<<"\n";
    fputs(ss.str().c_str(), file);

    fclose(file);*/
}

//Algoritmo Guloso Randomizado
void Graph::GreedyAlgorithmRandomized(float alpha, int iterations, string output_file){
    //In??cio da contagem de tempo da execu????o do algoritmo
    auto timeStart = std::chrono::high_resolution_clock::now();

    //Custo da melhor solu????o e custo da solu????o atual
    int bestCost = this->getNumberLabels(), costSolution;

    vector <myLabel> labels, auxLabels;

    list<myEdge> treeSolution;

    //Inicializa a semente do random
    srand(time(NULL));

    //Lendo as frequencias de cada r??tulo do grafo
    FrequencyLabels(labels);

    quickSortLabel(labels, 0, this->getNumberLabels() - 1);

    for(int i = 0; i < iterations; i++){
        //Inicializa a solu????o somente com os v??rtices de G
        Graph *solution = new Graph(this->getOrder(), false, false, false);

        costSolution = 0;

        auxLabels = labels;

        //Encontra uma solu????o
        do{
            //seleciona randomicamente um r??tulo de acordo com o alpha
            int random = rand() % ((int)(alpha * (auxLabels.size() - 1)));
            //incrementa o tamanho da solu????o
            costSolution++;

            //adiciona as arestas do r??tulo selecionado
            AddEdgesLabel(solution, auxLabels[random].label);
            //remove a aresta do vetor
            auxLabels.erase(auxLabels.begin() + random);

        }while(!IsComplete(solution));

        if (costSolution < bestCost){

            bestCost = costSolution;

            treeSolution = getTreeSolution(solution);
        }
        //delete solution;
    }

    //Diferen??a de tempo
    auto diff = std::chrono::high_resolution_clock::now() - timeStart;

    //Convers??o para microsegundos
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(diff);

    cout << "Solucao Guloso Randomizado = " << bestCost << " rotulos"<< endl;

    cout << "Tempo de execucao = " << time.count() << " milissegundos"<< endl;

    //Imprime sa??da no arquivo .dot
    cout << "Saida impressa no arquivo " << output_file << endl;

    outputTree(treeSolution, output_file);

    //Utilizado para gerar os resultados do relat??rio
    /*FILE* file = fopen(output_file.c_str(), "a+");
    stringstream ss;
    ss << bestCost << ";" << time.count()<<"\n";
    fputs(ss.str().c_str(), file);

    fclose(file);*/

}

//Inicializa os vetores de probabilidades e m??dias
void initializeVectors(vector <float> &probabilities, vector <myAverage> &averages, int tam){
    for(int i = 0; i < tam; i++){
        myAverage aux;

        aux.quantity = 0;

        aux.average = INFINITO;

        probabilities.push_back(1.0/tam);

        averages.push_back(aux);
    }
}

void updateProbabilities(vector <float> &probabilities, vector <myAverage> averages, int solBest){
    int sigma = 10;

    vector <float> q;

    float sum = 0;
    //Inicializa o vetor q e o somat??rio de qi
    for(int i = 0; i < probabilities.size(); i++){

        float qi = pow((solBest / averages[i].average), sigma);

        sum += qi;

        q.push_back(qi);
    }
    //Inicializa as probabilidades
    for(int i = 0; i < probabilities.size(); i++){

        probabilities[i] = q[i] / sum;
    }
}

// Escolhe o Alpha
int chooseAlpha (vector <float> probabilities){

    //vetor que acumula as probabilidades
    vector <float> accumulatedProbabilities;
    //acumulador para preencher o vetor
    float accumulator = 0, random;
    int indexAlpha;

    for(int i = 0;  i < probabilities.size(); i++){
        accumulator += probabilities[i];
        accumulatedProbabilities.push_back(accumulator);
    }
    //gera um n??mero aleat??rio entre 0 e 1
    random = ((float)rand()) / RAND_MAX;

    for(int i = 0; i < accumulatedProbabilities.size(); i++){
        if(random < accumulatedProbabilities[i]){
             indexAlpha = i;
             break;
        }
    }

    return indexAlpha;
}

void uptadeAverages (vector <myAverage> &averages, int costSolution, int indexAlpha){
    //Testa se ?? a primeira solu????o com esse alpha
    if(averages[indexAlpha].quantity == 0){

        averages[indexAlpha].quantity = 1;

        averages[indexAlpha].average = costSolution;
    }
    else{
        //calcula a nova m??dia
        averages[indexAlpha].average = (averages[indexAlpha].average * averages[indexAlpha].quantity + costSolution) / (averages[indexAlpha].quantity + 1);
        //incrementa a quantidade
        averages[indexAlpha].quantity++;
    }
}

// Algoritmo Guloso Randomizado Reativo
void Graph::GreedyAlgorithmRandomizedReactive(vector <float> alphas, int block, int iterations, string output_file){
    //In??cio da contagem de tempo da execu????o do algoritmo
    auto timeStart = std::chrono::high_resolution_clock::now();

    //Custo da melhor solu????o, custo da solu????o atual e o index do alpha
    int bestCost = INFINITO, costSolution, indexAlpha;

    vector <float> probabilities;

    vector <myLabel> labels, auxLabels;

    vector <myAverage> averages;

    list <myEdge> treeSolution;

    //Inicializa a semente do random
    srand(time(NULL));

    //Lendo as frequencias de cada r??tulo do grafo
    FrequencyLabels(labels);

    quickSortLabel(labels, 0, this->getNumberLabels() - 1);

    initializeVectors(probabilities, averages, alphas.size());

    for(int i = 0; i < iterations; i++){
        if(i % block == 0){
            updateProbabilities(probabilities, averages, bestCost);
        }

        indexAlpha = chooseAlpha(probabilities);
        //Inicializa a solu????o somente com os v??rtices de G
        Graph *solution = new Graph(this->getOrder(), false, false, false);

        costSolution = 0;

        auxLabels = labels;

        //Encontra uma solu????o
        do{
            //seleciona randomicamente um r??tulo de acordo com o alpha
            int random = rand() % ((int)(alphas[indexAlpha] * (auxLabels.size() - 1)));

            //incrementa o tamanho da solu????o
            costSolution++;

            //adiciona as arestas do r??tulo selecionado
            AddEdgesLabel(solution, auxLabels[random].label);

            //remove a aresta do vetor
            auxLabels.erase(auxLabels.begin() + random);

        }while(!IsComplete(solution));

        uptadeAverages(averages, costSolution, indexAlpha);

        if (costSolution < bestCost){

            bestCost = costSolution;

            treeSolution = getTreeSolution(solution);
        }
        //delete solution;
    }

       //Diferen??a de tempo
    auto diff = std::chrono::high_resolution_clock::now() - timeStart;

    //Convers??o para microsegundos
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(diff);

    cout << "Solucao Guloso Randomizado Reativo = " << bestCost << " rotulos"<< endl;

    cout << "Tempo de execucao = " << time.count() << " milissegundos"<< endl;

    //Imprime sa??da no arquivo .dot
    cout << "Saida impressa no arquivo " << output_file << endl;

    outputTree(treeSolution, output_file);

    //Utilizado para gerar os resultados do relat??rio
    /*FILE* file = fopen(output_file.c_str(), "a+");
    stringstream ss;
    ss << bestCost << ";" << time.count()<<"\n";
    fputs(ss.str().c_str(), file);

    fclose(file);*/
}

void Graph::outputTree(list<myEdge> treeSolution, string outputFileName){

    ofstream output_file;

    output_file.open(outputFileName.c_str(), ios::out | ios::trunc);

    output_file << "graph {" << endl;

    list<myEdge>::iterator it;

    for(it = treeSolution.begin(); it != treeSolution.end(); it++){
        // A aresta de retorno tem o estilo pontilhado
        output_file << (*it).origin << "--"<< (*it).destiny <<  "[label="<< (*it).label << "]" << endl;
    }
    output_file << "}"<< endl;

    output_file.close();
}
