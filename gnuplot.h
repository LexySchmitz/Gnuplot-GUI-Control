#ifndef GNUPLOT_H
#define GNUPLOT_H

#include <QMainWindow>
#include <QPicture>
#include <QTimer>
#include <QLabel>
#include <QList>
#include <cstdio>
#include <unistd.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

#include "graph.h"

using namespace std;
using namespace cv;

typedef enum
{
    INITIALIZING, READY, SENDING, WAITING, RECEIVING, CLOSING
} STATE;


class Gnuplot : public QLabel
{
    Q_OBJECT

private:
    QTimer* timer;
    STATE state;

    int process;
    FILE* gnuplotInput;
    FILE* gnuplotOutput;

    unsigned char* buffer;
    int failed;

    QList<QString*>* commandList;

    QString* title;
    int xRange[2];
    int yRange[2];
    int zRange[2];
    QString* xLabel;
    QString* yLabel;
    QString* zLabel;

    int hAngle;
    int vAngle;
    int zoomLevel;

    bool datasetMode;
    int datasetDim;
    vector<Graph*>* graphset;

private:
    bool initialize();
    bool sendMessage();
    bool receiveMessage();


private slots:
    void timerEvent();


public:
    explicit Gnuplot(QWidget* parent = 0);
    ~Gnuplot();
    int getProcess();
    void plot();
    void plot(int hAngle, int vAngle);
    void plot(double zoomLevel);
    void addCommand(QString* command);
    void setTitle(QString* title);
    void setXRange(int lowerBound, int upperBound);
    void setYRange(int lowerBound, int upperBound);
    void setZRange(int lowerBound, int upperBound);
    void setXLabel(QString* xLabel);
    void setYLabel(QString* yLabel);
    void setZLabel(QString* zLabel);
    void setDatasetMode(bool mode);
    void setDatasetDim(int dim);
    void addGraph(Graph* graph);
};

#endif // GNUPLOT_H
