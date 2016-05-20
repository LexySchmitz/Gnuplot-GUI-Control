#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gnuplot.h"
#include <QString>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_sendButton_clicked();

    void on_verticalSlider_actionTriggered(int action);

    void on_horizontalSlider_actionTriggered(int action);

    void on_zoomSlider_actionTriggered(int action);

private:
    Ui::MainWindow *ui;
    //Gnuplot* gnuplot;
};

#endif // MAINWINDOW_H
