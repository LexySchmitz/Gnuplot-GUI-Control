#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <QString>

using namespace std;

class Graph
{

private:
    vector<vector<double*>*>* graph;
    int datasetDim;

    QString* title;


public:
    Graph(int dim);
    void addSegment();
    void addDataset(int segment, double* data);
    void setDatasetDim(int dim);
    double getValue(int segment, int dataset, int dim);
    int getDatasetDim();
    int getNumberOfDatasets(int segment);
    int getNumberOfSegments();
    void setTitle(QString* title);
    void setTitle(QString title);
    void setTitle(char* title);
    char* getTitle();

};

#endif // GRAPH_H
