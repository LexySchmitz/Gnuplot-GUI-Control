#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gnuplot.h"
#include <string>
#include <sstream>
#include <string.h>
#include <cstdio>
#include <iostream>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    ui->gnuplot->setDatasetMode(true);
    ui->gnuplot->setDatasetDim(3);

    Graph* newGraph;
    double data[3];

    cout << "test" << endl;

    newGraph = new Graph(3);
    newGraph->addSegment();
    data[0] = -2.5;
    data[1] = 3.5;
    data[2] = 1.0;
    newGraph->addDataset(0, data);
    data[0] = 1.5;
    data[1] = 5.5;
    data[2] = 2.0;
    newGraph->addDataset(0, data);
    data[0] = 4.5;
    data[1] = -7.5;
    data[2] = -1.0;
    newGraph->addDataset(0, data);
    data[0] = 2.5;
    data[1] = 3.2;
    data[2] = -2.0;
    newGraph->addDataset(0, data);
    data[0] = -7.5;
    data[1] = -9.2;
    data[2] = -3.0;
    newGraph->addDataset(0, data);

    newGraph->addSegment();
    data[0] = 2.5;
    data[1] = -3.5;
    data[2] = -1.0;
    newGraph->addDataset(1, data);
    data[0] = -1.5;
    data[1] = -5.5;
    data[2] = -2.0;
    newGraph->addDataset(1, data);
    data[0] = -4.5;
    data[1] = 7.5;
    data[2] = 1.0;
    newGraph->addDataset(1, data);
    data[0] = -2.5;
    data[1] = -3.2;
    data[2] = 2.0;
    newGraph->addDataset(1, data);
    data[0] = 7.5;
    data[1] = 9.2;
    data[2] = 3.0;
    newGraph->addDataset(1, data);

    ui->gnuplot->addGraph(newGraph);

    newGraph = new Graph(3);
    newGraph->addSegment();
    data[0] = 3.5;
    data[1] = 2.5;
    data[2] = 1.0;
    newGraph->addDataset(0, data);
    data[0] = 2.3;
    data[1] = 1.2;
    data[2] = 2.7;
    newGraph->addDataset(0, data);
    data[0] = 7.5;
    data[1] = -2.3;
    data[2] = -1.2;
    newGraph->addDataset(0, data);
    data[0] = 1.3;
    data[1] = 3.0;
    data[2] = -2.5;
    newGraph->addDataset(0, data);
    data[0] = -9.2;
    data[1] = -3.2;
    data[2] = -8.0;
    newGraph->addDataset(0, data);

    newGraph->addSegment();
    data[0] = -3.5;
    data[1] = -2.5;
    data[2] = -1.0;
    newGraph->addDataset(1, data);
    data[0] = -2.3;
    data[1] = -1.2;
    data[2] = -2.7;
    newGraph->addDataset(1, data);
    data[0] = -7.5;
    data[1] = 2.3;
    data[2] = 1.2;
    newGraph->addDataset(1, data);
    data[0] = -1.3;
    data[1] = -3.0;
    data[2] = 2.5;
    newGraph->addDataset(1, data);
    data[0] = 9.2;
    data[1] = 3.2;
    data[2] = 8.0;
    newGraph->addDataset(1, data);

    ui->gnuplot->addGraph(newGraph);
}



void MainWindow::on_pushButton_2_clicked()
{
    ui->gnuplot->addCommand(new QString("plot sin(1/x)"));
}

void MainWindow::on_sendButton_clicked()
{
    ui->gnuplot->plot();
}



void MainWindow::on_verticalSlider_actionTriggered(int action)
{
    ui->gnuplot->plot(ui->horizontalSlider->value(), ui->verticalSlider->value());
}

void MainWindow::on_horizontalSlider_actionTriggered(int action)
{
    ui->gnuplot->plot(ui->horizontalSlider->value(), ui->verticalSlider->value());
}

void MainWindow::on_zoomSlider_actionTriggered(int action)
{
    double zoomLevel = ui->zoomSlider->value() > 0.0
                    ? ui->zoomSlider->value() + 1.0
                    : 1.0 / abs(ui->zoomSlider->value() + 1.0);
    ui->gnuplot->plot(zoomLevel);
}
