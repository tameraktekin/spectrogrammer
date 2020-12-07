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

signals:
    void fftFilled();

private:
    Ui::MainWindow *ui;

    QVector<double> displayList;
    QVector<double> displayListIdx;
    QwtPlotCurve *curvePower = new QwtPlotCurve();

    double sampleRate = 48000;
    int fftLen;
    int fftCount = 0;

    QString windowType;
    QVector<double> fftList;
    QVector<double> fftListIdx;
    QVector<double> fftMag;

    QwtPlotCurve *curveFFT = new QwtPlotCurve();
    void convertDB(float &data);
    void arrangeFFTParams();
};

#endif // MAINWINDOW_H
