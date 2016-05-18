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

    this->xRange[0] = -10;
    this->xRange[1] = 10;
    this->yRange[0] = -10;
    this->yRange[1] = 10;
    this->zRange[0] = -10;
    this->zRange[1] = 10;

    this->xLabel = new QString("xLabel");
    this->yLabel = new QString("yLabel");
    this->zLabel = new QString("zLabel");

    this->datasetMode = false;
    this->datasetDim = 2;
    this->dataset = new vector<vector<vector<double>*>*>();

    return true;
}



bool Gnuplot::sendMessage()
{
    cout << "senden..." << endl;

    fprintf(this->gnuplotInput, "set terminal png size %d, %d\n", this->width(), this->height());

    fprintf(this->gnuplotInput, "set title '%s'\n", this->title->toLatin1().data());
    fprintf(this->gnuplotInput, "set xrange [%d:%d]\n", this->xRange[0], this->xRange[1]);
    fprintf(this->gnuplotInput, "set yrange [%d:%d]\n", this->yRange[0], this->yRange[1]);
    fprintf(this->gnuplotInput, "set zrange [%d:%d]\n", this->zRange[0], this->zRange[1]);
    fprintf(this->gnuplotInput, "set xlabel '%s'\n", this->xLabel->toLatin1().data());
    fprintf(this->gnuplotInput, "set ylabel '%s'\n", this->yLabel->toLatin1().data());
    fprintf(this->gnuplotInput, "set zlabel '%s'\n", this->zLabel->toLatin1().data());

    fprintf(this->gnuplotInput, "set view 20,50\n");


    for (int i = 0; i < this->commandList->size(); i++)
    {
        fprintf(this->gnuplotInput, (char*) this->commandList->at(i)->toLatin1().data());
    }


    if (datasetMode)
    {
        if (datasetDim == 2)
        {
            fprintf(this->gnuplotInput, "plot ");
            printf("plot ");
        }
        else if (datasetDim == 3)
        {
            fprintf(this->gnuplotInput, "splot ");
            printf("splot ");
        }

        for (int k = 0; k < this->dataset->size(); k++)
        {
            fprintf(this->gnuplotInput, "'-' using ");
            printf("'-' using ");

            if (datasetDim == 2)
            {
                fprintf(this->gnuplotInput, "1:2 ");
                printf("1:2 ");
            }
            else if (datasetDim == 3)
            {
                fprintf(this->gnuplotInput, "1:2:3 ");
                printf("1:2:3 ");
            }

            fprintf(this->gnuplotInput, " with lines title 'Punkte %d'", k);
            printf(" with lines title 'Punkte %d'", k);

            if (k < this->dataset->size() - 1){
                fprintf(this->gnuplotInput, ", ");
                printf(", ");
            }
        }
        fprintf(this->gnuplotInput, "\n");
        printf("\n");

        for (int k = 0; k < this->dataset->size(); k++)
        {
            for(int i = 0; i < this->dataset->at(k)->size(); i++)
            {
                for (int j = 0; j < this->dataset->at(k)->at(i)->size(); j++)
                {
                    fprintf(this->gnuplotInput, "%f\t", this->dataset->at(k)->at(i)->at(j));
                    printf("%f\t", this->dataset->at(k)->at(i)->at(j));
                }
                fprintf(this->gnuplotInput, "\n");
                printf("\n");
            }
            fprintf(this->gnuplotInput, "e\n");
            printf("e\n");
        }
    }

    fflush(this->gnuplotInput);
    fflush(stdout);

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

void Gnuplot::setXLabel(QString* xLabel)
{
    this->xLabel = xLabel;
}

void Gnuplot::setYLabel(QString* yLabel)
{
    this->yLabel = yLabel;
}

void Gnuplot::setZLabel(QString* zLabel)
{
    this->zLabel = zLabel;
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

void Gnuplot::setZRange(int lowerBound, int upperBound)
{
    this->zRange[0] = lowerBound;
    this->zRange[1] = upperBound;
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
    if (dim <= 3 && dim >= 2)
    {
        this->datasetDim = dim;
    }
}

void Gnuplot::addSet()
{
    vector<vector<double>*>* newSet = new vector<vector<double>*>();
    this->dataset->push_back(newSet);
}

void Gnuplot::addDataset(int setNumber, double* data)
{
    vector<double>* newSet = new vector<double>();
    for (int i = 0; i < this->datasetDim; i++)
    {
        newSet->push_back(data[i]);
    }
    this->dataset->at(setNumber)->push_back(newSet);
}




