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

private:
    Ui::MainWindow *ui;
    int fftLen;
    QString windowType;
    QVector<double> displayList;
    QVector<double> displayListIdx;
    QwtPlotCurve *curvePower = new QwtPlotCurve();
    void convertDB(float &data);
};

#endif // MAINWINDOW_H
