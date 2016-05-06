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
    QString* xLabel;
    QString* yLabel;

    bool datasetMode;
    int datasetDim;
    QList<QList<double>*>* dataset;


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
    void addCommand(QString* command);
    void setTitle(QString* title);
    void setXRange(int lowerBound, int upperBound);
    void setYRange(int lowerBound, int upperBound);
    void setXLabel(QString* xLabel);
    void setYLabel(QString* yLabel);
    void setDatasetMode(bool mode);
    void setDatasetDim(int dim);
    void addDataset(QList<double>* data);
};

#endif // GNUPLOT_H
