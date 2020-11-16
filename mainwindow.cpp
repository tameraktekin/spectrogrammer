#include<iostream>
#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->powerPlot->enableAxis(0, false);
    ui->powerPlot->enableAxis(2, false);
    ui->powerPlot->setAxisScale(0, -120, 0);
    ui->powerPlot->setTitle("Power (dB)");

    for (int i = 0; i < 512; ++i) {
        displayList.push_back(0);
        displayListIdx.push_back(i);
    }

    curvePower->setPen( Qt::red, 1 ),
    curvePower->setRenderHint( QwtPlotItem::RenderAntialiased, true );

    ui->fftPlot->enableAxis(0, false);
    ui->fftPlot->enableAxis(2, false);

    ui->spectrogram->enableAxis(0, false);
    ui->spectrogram->enableAxis(2, false);

    ui->spectrogram3D->enableAxis(0, false);
    ui->spectrogram3D->enableAxis(2, false);

    QAudioProbe *probe;
    QAudioRecorder * audioRecorder = new QAudioRecorder(this);
            probe = new QAudioProbe;
            connect(probe, SIGNAL(audioBufferProbed(QAudioBuffer)), this, SLOT(processBuffer(QAudioBuffer)));
            probe->setSource(audioRecorder);
            audioRecorder->record();
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

}

void MainWindow::convertDB(float &data)
{
    data = 20 * log10(data * data);
}

void MainWindow::processBuffer(const QAudioBuffer& buffer)
{
    float soundData = *(buffer.constData<float>());

    convertDB(soundData);
    qDebug() << soundData;
    qDebug() << buffer.format();
    displayList.push_back(soundData);
    displayList.pop_front();

    QwtPointArrayData *data1=new QwtPointArrayData(displayListIdx, displayList);

    curvePower->setSamples(data1);
    curvePower->attach(ui->powerPlot );

    ui->powerPlot->replot();
    ui->powerPlot->show();
}
