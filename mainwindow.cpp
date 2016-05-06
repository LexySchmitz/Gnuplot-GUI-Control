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

}



void MainWindow::on_pushButton_2_clicked()
{
    ui->gnuplot->addCommand(new QString("plot sin(1/x)"));
}

void MainWindow::on_sendButton_clicked()
{
    ui->gnuplot->plot();
}


