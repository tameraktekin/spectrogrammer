#include<iostream>
#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->powerPlot->enableAxis(0, true);
    ui->powerPlot->enableAxis(2, true);
    ui->powerPlot->setAxisScale(ui->powerPlot->yLeft, -120, 0);
    ui->powerPlot->setAxisScale(ui->powerPlot->xBottom, 0, 100);
    ui->powerPlot->setTitle("Power (dB)");
    ui->powerPlot->setAxisTitle(ui->powerPlot->xBottom, "Time (ms)");

    for (int i = 0; i < 10000; ++i) {
        displayList.push_back(0);
        displayListIdx.push_back((i / sampleRate) * 1000);
    }

    curvePower->setPen( Qt::red, 1 ),
    curvePower->setRenderHint( QwtPlotItem::RenderAntialiased, true );

    ui->fftPlot->enableAxis(0, true);
    ui->fftPlot->enableAxis(2, true);
    ui->fftPlot->setAxisScale(ui->fftPlot->yLeft, 0, 1);
    ui->fftPlot->setAxisScale(ui->fftPlot->xBottom, 0, sampleRate / 2);
    ui->fftPlot->setTitle("FFT Magnitude");
    ui->fftPlot->setAxisTitle(ui->fftPlot->xBottom, "Frequency (Hz)");

    curveFFT->setPen( Qt::red, 1 ),
    curveFFT->setRenderHint( QwtPlotItem::RenderAntialiased, true );

    ui->spectrogram->enableAxis(0, true);
    ui->spectrogram->enableAxis(2, true);

    ui->spectrogram3D->enableAxis(0, true);
    ui->spectrogram3D->enableAxis(2, true);

    fftLen = 1024;
    windowType = "Rectangular";

    arrangeFFTParams();

    connect(this, SIGNAL(fftFilled()), this, SLOT(updateFFTPlot()));

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

    fftLen = optionsMenu.getFftLen();
    windowType = optionsMenu.getWindowType();

    arrangeFFTParams();
}

void MainWindow::convertDB(float &data)
{
    data = 20 * log10(data * data);
}

void MainWindow::processBuffer(const QAudioBuffer& buffer)
{
    const float *soundData = buffer.constData<float>();
    float data;

    for (int i = 0; i < buffer.frameCount(); i++){
        data = soundData[i];

        fftList.push_back(data);
        fftList.pop_front();

        fftCount++;
        if (fftCount == fftLen){
            emit fftFilled();
            fftCount = 0;
        }

        convertDB(data);
        displayList.push_back(data);
        displayList.pop_front();
    }

    QwtPointArrayData *data1=new QwtPointArrayData(displayListIdx, displayList);

    curvePower->setSamples(data1);
    curvePower->attach(ui->powerPlot);

    ui->powerPlot->replot();
    ui->powerPlot->show();
}

void MainWindow::arrangeFFTParams(){
    fftList.clear();

    fftListIdx.clear();
    fftMag.clear();

    for (int i = 0; i < fftLen; ++i) {
        fftList.push_back(0);
        if (i < fftLen / 2) fftListIdx.push_back((i * sampleRate) / fftLen);
        fftMag.push_back(0);
    }
}

void MainWindow::updateFFTPlot(){
    fftw_plan p;
    fftw_complex *out;

    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * fftLen);
    p = fftw_plan_dft_r2c_1d(fftLen, fftList.data(), out, FFTW_ESTIMATE);

    fftw_execute(p);

    for (int i = 0; i < fftLen / 2; i++){
        fftMag.push_back(sqrt(out[i][0] * out[i][0] + out[i][1] * out[i][1]));
        fftMag.pop_front();
    }

    QwtPointArrayData *data1=new QwtPointArrayData(fftListIdx, fftMag);

    curveFFT->setSamples(data1);
    curveFFT->attach(ui->fftPlot);

    ui->fftPlot->replot();
    ui->fftPlot->show();
}
