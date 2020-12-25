#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAudio>
#include <QAudioBuffer>
#include "optionsmenu.h"
#include "QDebug"
#include <qwt_point_data.h>
#include <qwt_plot_curve.h>
#include <QAudioInput>
#include <QAudioRecorder>
#include <QAudioProbe>
#include <math.h>
#include <fftw3.h>
#include <QUrl>
#include "qwt_plot_spectrogram.h"
#include "qwt_matrix_raster_data.h"
#include <qwt_color_map.h>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_optionsButton_clicked();
    void processBuffer(const QAudioBuffer& buffer);
    void updateFFTPlot();
    void updateSTFTPlot();

signals:
    void fftFilled();
    void stftFilled();

private:
    Ui::MainWindow *ui;

    QVector<double> displayList;
    QVector<double> displayListIdx;
    QwtPlotCurve *curvePower = new QwtPlotCurve();
    QwtLinearColorMap *colorMap;

    void arrangePlots();

    double sampleRate = 48000;
    int fftLen = 1024;
    int fftCount = 0;
    int stftCount = 0;

    QString windowType = "Rectangular";
    QVector<double> fftList;
    QVector<double> fftListIdx;
    QVector<double> fftMag;
    QVector<double> stftMag;
    QVector<double> stftList;
    QwtMatrixRasterData *dataSpec = new QwtMatrixRasterData();

    QwtPlotCurve *curveFFT = new QwtPlotCurve();
    void convertDB(float &data);
    void arrangeFFTParams();

    QwtPlotSpectrogram *spec;
};

#endif // MAINWINDOW_H
