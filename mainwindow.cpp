#include<iostream>
#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    for (int i = 0; i < 10000; ++i) {
        displayList.push_back(0);
        displayListIdx.push_back((i / sampleRate) * 1000);
    }

    arrangePlots();
    arrangeFFTParams();

    connect(this, SIGNAL(fftFilled()), this, SLOT(updateFFTPlot()));
    connect(this, SIGNAL(stftFilled()), this, SLOT(updateSTFTPlot()));

    QAudioProbe *probe;
    QAudioRecorder * audioRecorder = new QAudioRecorder(this);
            probe = new QAudioProbe;
            connect(probe, SIGNAL(audioBufferProbed(QAudioBuffer)), this, SLOT(processBuffer(QAudioBuffer)));
            probe->setSource(audioRecorder);
            audioRecorder->setOutputLocation(QUrl::fromLocalFile("tmp"));
            audioRecorder->record();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::arrangePlots()
{
    ui->powerPlot->enableAxis(0, true);
    ui->powerPlot->enableAxis(2, true);
    ui->powerPlot->setAxisScale(ui->powerPlot->yLeft, -120, 0);
    ui->powerPlot->setAxisScale(ui->powerPlot->xBottom, 0, 100);
    ui->powerPlot->setTitle("Power (dB)");
    ui->powerPlot->setAxisTitle(ui->powerPlot->xBottom, "Time (ms)");

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

    spec = new QwtPlotSpectrogram();

    ui->spectrogram->enableAxis(0, true);
    ui->spectrogram->enableAxis(2, true);
    ui->spectrogram->setAxisScale(ui->spectrogram->xBottom, 0, 1000 * (16 * fftLen) / sampleRate);
    ui->spectrogram->setTitle("STFT");
    ui->spectrogram->setAxisTitle(ui->spectrogram->yLeft, "Frequency (Hz)");
    ui->spectrogram->setAxisTitle(ui->spectrogram->xBottom, "Time (ms)");

    colorMap = new QwtLinearColorMap(Qt::darkCyan, Qt::red);
    colorMap->addColorStop(0.1, Qt::cyan);
    colorMap->addColorStop(0.6, Qt::green);
    colorMap->addColorStop(0.95, Qt::yellow);

    spec->setColorMap(colorMap);

    dataSpec->setInterval(Qt::XAxis, QwtInterval( 0, 1000 * (16 * fftLen) / sampleRate));
    dataSpec->setInterval(Qt::YAxis, QwtInterval( 0, (sampleRate / 2) ));
    dataSpec->setInterval(Qt::ZAxis, QwtInterval( 0, 2 ));
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

        stftList.push_back(data);
        stftCount++;
        if (stftCount == 8 * fftLen){
            emit stftFilled();
            stftCount = 0;
        }
        convertDB(data);
        displayList.push_back(data);
        displayList.pop_front();
    }

    dataPower = new QwtPointArrayData(displayListIdx, displayList);

    curvePower->setSamples(dataPower);
    curvePower->attach(ui->powerPlot);

    ui->powerPlot->replot();
    ui->powerPlot->show();
}

void MainWindow::arrangeFFTParams(){
    fftList.clear();

    fftListIdx.clear();
    fftMag.clear();
    stftMag.clear();

    for (int i = 0; i < fftLen; ++i) {
        fftList.push_back(0);
        if (i < fftLen / 2) fftListIdx.push_back((i * sampleRate) / fftLen);
        fftMag.push_back(0);
    }

    for (int i = 0; i < (16 * fftLen); i++)
        stftMag.push_back(0);
    dataSpec->setValueMatrix(stftMag, 32);
}

void MainWindow::updateFFTPlot(){
    fftw_plan p;
    fftw_complex *out;
    double res;

    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * fftLen);
    p = fftw_plan_dft_r2c_1d(fftLen, fftList.data(), out, FFTW_ESTIMATE);

    fftw_execute(p);

    for (int i = 0; i < fftLen / 2; i++){
        res = sqrt(out[i][0] * out[i][0] + out[i][1] * out[i][1]);
        fftMag.push_back(res);
        fftMag.pop_front();
    }

    dataFFT = new QwtPointArrayData(fftListIdx, fftMag);

    curveFFT->setSamples(dataFFT);
    curveFFT->attach(ui->fftPlot);

    ui->fftPlot->replot();
    ui->fftPlot->show();

    fftw_destroy_plan(p);
}

void MainWindow::updateSTFTPlot(){
    fftw_plan p;
    fftw_complex *out;
    double res;
    int count = 0;
    double stftBlock[fftLen];

    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * fftLen);

    for (int j = 0; j < stftList.length(); j = j + (fftLen / 2 )){
        for (int k = 0; k < fftLen; k++){
            stftBlock[k] = stftList[k + j];

            if (windowType == "Hann"){
                stftBlock[k] = stftBlock[k] * 0.5 * (1 - cos(2 * M_PI * k / fftLen));
            }
        }

        p = fftw_plan_dft_r2c_1d(fftLen, stftBlock, out, FFTW_ESTIMATE);

        fftw_execute(p);

        for (int i = 0; i < fftLen / 2; i++){
            res = sqrt(out[i][0] * out[i][0] + out[i][1] * out[i][1]);

            dataSpec->setValue(i, (count / (fftLen / 2)), res);
            count++;

            if (count >= (32 * fftLen)){
                spec->setData(dataSpec);
                spec->attach(ui->spectrogram);

                ui->spectrogram->replot();
                ui->spectrogram->show();

                count = 0;
                stftList.clear();
            }
        }
    }
    fftw_destroy_plan(p);
}
