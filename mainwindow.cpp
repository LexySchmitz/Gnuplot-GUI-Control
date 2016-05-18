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

    ui->gnuplot->addSet();

    cout << "test" << endl;

    double data[3];
    data[0] = -2.5;
    data[1] = 3.5;
    data[2] = 1.0;
    ui->gnuplot->addDataset(0, data);
    data[0] = 1.5;
    data[1] = 5.5;
    data[2] = 2.0;
    ui->gnuplot->addDataset(0, data);
    data[0] = 4.5;
    data[1] = -7.5;
    data[2] = -1.0;
    ui->gnuplot->addDataset(0, data);
    data[0] = 2.5;
    data[1] = 3.2;
    data[2] = -2.0;
    ui->gnuplot->addDataset(0, data);
    data[0] = -7.5;
    data[1] = -9.2;
    data[2] = -3.0;
    ui->gnuplot->addDataset(0, data);

    ui->gnuplot->addSet();
    data[0] = -1.5;
    data[1] = 2.5;
    data[2] = -3.0;
    ui->gnuplot->addDataset(1, data);
    data[0] = 0.5;
    data[1] = 3.5;
    data[2] = -2.0;
    ui->gnuplot->addDataset(1, data);
    data[0] = 5.5;
    data[1] = -8.5;
    data[2] = -1.0;
    ui->gnuplot->addDataset(1, data);
    data[0] = 3.5;
    data[1] = 2.2;
    data[2] = 0.0;
    ui->gnuplot->addDataset(1, data);
    data[0] = -9.5;
    data[1] = -4.2;
    data[2] = 1.0;
    ui->gnuplot->addDataset(1, data);
}



void MainWindow::on_pushButton_2_clicked()
{
    ui->gnuplot->addCommand(new QString("plot sin(1/x)"));
}

void MainWindow::on_sendButton_clicked()
{
    ui->gnuplot->plot();
}


