#ifndef GRAPH_H
#define GRAPH_H

#include <vector>

using namespace std;

class Graph
{

private:
    vector<vector<double*>*>* graph;
    int datasetDim;

public:
    Graph(int dim);
    void addSegment();
    void addDataset(int segment, double* data);
    void setDatasetDim(int dim);
    double getValue(int segment, int dataset, int dim);
    int getDatasetDim();
    int getNumberOfDatasets(int segment);
    int getNumberOfSegments();

};

#endif // GRAPH_H
