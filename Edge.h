/**************************************************************************************************
 * Implementation of the TAD Edge
**************************************************************************************************/

#ifndef EDGE_H_INCLUDED
#define EDGE_H_INCLUDED

using namespace std;

// Definition of the Edge class
class Edge{

    // Attributes
    private:
        int target_id;
        Edge* next_edge;
        float weight;
        int label;

    public:
        // Constructor
        Edge();
        Edge(int target_id);
        // Destructor
        ~Edge();
        // Getters
        int getTargetId();
        Edge* getNextEdge();
        float getWeight();
        int getLabel();
        // Setters
        void setNextEdge(Edge* edge);
        void setWeight(float weight);
        void setLabel(int label);

};

#endif // EDGE_H_INCLUDED
