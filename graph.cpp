#include "graph.h"
#include "stdlib.h"

Graph::Graph(int dim)
{
    this->graph = new vector<vector<double*>*>();
    this->datasetDim = dim;
}

void Graph::addSegment()
{
    vector<double*>* newSegment = new vector<double*>();
    this->graph->push_back(newSegment);
}

void Graph::addDataset(int segment, double* data)
{
     double* newDataset = (double*) malloc(3 * sizeof(double));
     for (int i = 0; i < this->datasetDim; i++)
     {
        newDataset[i] = data[i];
     }
     this->graph->at(segment)->push_back(newDataset);
}

void Graph::setDatasetDim(int dim)
{
    if (dim <= 3 && dim >= 2)
    {
        this->datasetDim = dim;
    }
}

int Graph::getNumberOfSegments()
{
    return this->graph->size();
}

int Graph::getNumberOfDatasets(int segment)
{
    return this->graph->at(segment)->size();
}

int Graph::getDatasetDim()
{
    return this->datasetDim;
}

double Graph::getValue(int segment, int dataset, int dim)
{
    return this->graph->at(segment)->at(dataset)[dim];
}




