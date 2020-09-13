#include<iostream>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "optionsmenu.h"
#include "QDebug"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // @TODO: Make below lines more beautiful.
    ui->powerPlot->enableAxis(0, false);
    ui->powerPlot->enableAxis(2, false);

    ui->fftPlot->enableAxis(0, false);
    ui->fftPlot->enableAxis(2, false);

    ui->spectrogram->enableAxis(0, false);
    ui->spectrogram->enableAxis(2, false);

    ui->spectrogram3D->enableAxis(0, false);
    ui->spectrogram3D->enableAxis(2, false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_optionsButton_clicked()
{
    OptionsMenu optionsMenu;
    optionsMenu.setModal(true);
    optionsMenu.exec();

    this->fftLen = optionsMenu.getFftLen();
    this->windowType = optionsMenu.getWindowType();

    cout<<this->fftLen<<endl;
    qDebug()<<this->windowType<<Qt::endl;
}
