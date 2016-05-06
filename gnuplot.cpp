#include "gnuplot.h"
#include <dirent.h>
#include <string>
#include <sstream>
#include <string.h>
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <fcntl.h>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>



#define READ_PORT 0
#define WRITE_PORT 1

using namespace std;
using namespace cv;



Gnuplot::Gnuplot(QWidget* parent) : QLabel(parent)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerEvent()));

    state = INITIALIZING;
    timer->start(50);
}



Gnuplot::~Gnuplot()
{
    fprintf(this->gnuplotInput, "exit\n");
    fflush(this->gnuplotInput);

    pclose(this->gnuplotInput);
    pclose(this->gnuplotOutput);

    delete this->timer;

    free(this->buffer);
}



bool Gnuplot::initialize()
{
    cout << "initialisieren..." << endl;

    char wd[255];
    getcwd(wd, 254);

    int inputPipe[2];
    int outputPipe[2];

    pipe(inputPipe);
    pipe(outputPipe);

    this->process = fork();

    if (this->process == 0)
    {
        dup2(inputPipe[READ_PORT], STDIN_FILENO);
        dup2(outputPipe[WRITE_PORT], STDOUT_FILENO);
        execlp("/usr/bin/gnuplot", "gnuplot", NULL);
    }
    else
    {
        //close(inputPipe[READ_PORT]);
        //close(outputPipe[WRITE_PORT]);
        this->gnuplotInput = fdopen(inputPipe[WRITE_PORT], "w");
        this->gnuplotOutput = fdopen(outputPipe[READ_PORT], "r");
        fcntl(fileno(this->gnuplotOutput), F_SETFL, O_NONBLOCK);
    }

    this->buffer = (unsigned char*) calloc(10000, sizeof(char));

    this->failed = 0;

    this->commandList = new QList<QString*>();

    this->title = new QString("Titel");

    this->xRange[0] = -1;
    this->xRange[1] = 1;
    this->yRange[0] = -1;
    this->yRange[1] = 1;

    this->xLabel = new QString("xLabel");
    this->yLabel = new QString("yLabel");

    this->datasetMode = false;
    this->dataset = new QList<QList<double>*>;
    this->datasetDim = 2;

    return true;
}



bool Gnuplot::sendMessage()
{
    cout << "senden..." << endl;

    fprintf(this->gnuplotInput, "set terminal png size %d,%d\n", this->width(), this->height());

    fprintf(this->gnuplotInput, "set title '%s'\n", this->title->toLatin1().data());
    fprintf(this->gnuplotInput, "set xrange [%d:%d]\n", this->xRange[0], this->xRange[1]);
    fprintf(this->gnuplotInput, "set yrange [%d:%d]\n", this->yRange[0], this->yRange[1]);
    fprintf(this->gnuplotInput, "set xlabel '%s'\n", this->xLabel->toLatin1().data());
    fprintf(this->gnuplotInput, "set ylabel '%s'\n", this->yLabel->toLatin1().data());

    for (int i = 0; i < this->commandList->size(); i++)
    {
        fprintf(this->gnuplotInput, (char*) this->commandList->at(i)->toLatin1().data());
    }

    if (datasetMode)
    {
        fprintf(this->gnuplotInput, "plot '-' u 1:2 w l title 'Punkte'\n");
        for (int i = 0; i < this->dataset->size(); i++)
        {
            for (int j = 0; j < this->dataset->at(i)->size(); j++)
            {
                fprintf(this->gnuplotInput, "%f ", this->dataset->at(i)->at(j));
            }
            fprintf(this->gnuplotInput, "\n");
        }
        fprintf(this->gnuplotInput, "e\n");
    }

    //fprintf(this->gnuplotInput, "plot sin(x)\n");
    fflush(this->gnuplotInput);

    return true;
}



bool Gnuplot::receiveMessage()
{
    cout << "empfangen..." << endl;

    bool success;

    int size = read(fileno(this->gnuplotOutput), this->buffer, 10000);

    if (size == -1)
    {
        success = false;
        this->failed++;

        if (this->failed > 20)
        {
            cout << "Fehler!!!" << endl;
            this->timer->stop();
        }
    }
    else
    {

        int i = 0;
        vector<uchar> pngbytes(size);

        while (i < size)
        {
            //cout << i << " " << buffer[i] << endl;
            pngbytes[i] = this->buffer[i];
            i++;
        }

        Mat a = imdecode(pngbytes, CV_LOAD_IMAGE_COLOR);

        this->setPixmap(QPixmap(QPixmap::fromImage(QImage((unsigned char*) a.data, a.cols, a.rows, a.cols * 3, QImage::Format_RGB888))));

        success = true;
        this->failed = 0;
    }

    return success;
}



void Gnuplot::timerEvent()
{
    switch (state)
    {
        case INITIALIZING:
            if (initialize())
            {
                cout << "Initialisierung abgeschlossen" << endl;
                state = READY;
            }
            break;

        case READY:
            this->timer->stop();
            break;

        case SENDING:
            if (sendMessage())
            {
                cout << "Sendevorgang abgeschlossen" << endl;
                state = RECEIVING;
            }
            break;

        case RECEIVING:
            if (receiveMessage())
            {
                cout << "Empfangsvorgang abgeschlossen" << endl;
                state = READY;
            }
            break;
    }
}



int Gnuplot::getProcess()
{
    return this->process;
}



void Gnuplot::plot()
{
    state = SENDING;
    this->timer->start(50);
}



void Gnuplot::addCommand(QString* command)
{
    command->append("\n");
    this->commandList->push_back(command);
}

void Gnuplot::setXLabel(QString *xLabel)
{
    this->xLabel = xLabel;
}

void Gnuplot::setYLabel(QString *yLabel)
{
    this->yLabel = yLabel;
}

void Gnuplot::setXRange(int lowerBound, int upperBound)
{
    this->xRange[0] = lowerBound;
    this->xRange[1] = upperBound;
}

void Gnuplot::setYRange(int lowerBound, int upperBound)
{
    this->yRange[0] = lowerBound;
    this->yRange[1] = upperBound;
}

void Gnuplot::setTitle(QString *title)
{
    this->title = title;
}

void Gnuplot::setDatasetMode(bool mode)
{
    this->datasetMode = mode;
}

void Gnuplot::setDatasetDim(int dim)
{
    if (dim <= 3 && dim >= 0)
    {
        this->datasetDim = dim;
    }
}

void Gnuplot::addDataset(QList<double>* data)
{
    this->dataset->append(data);
}


