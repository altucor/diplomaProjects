#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "qcustomplot.h"
#include "packet.h"
#include "packetparser.h"
#include "kalmanpacketfilter.h"
#include "highpassfilter.h"
#include "complementarypacketfilter.h"

enum plotterType{
    ACCELEROMETER = 0,
    GYROSCOPE
};

enum plotterPair{
    ORIGINAL = 0,
    PROCESSED
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setOffsetPacket();
    void initializeAll();
    void initializeProgressBars(qint64 min, qint64 max, qint64 startValue);
    void resetOutPlotters();
    void initializePLotter(QCustomPlot *plotter, int colorScheme);
    void loadFromFile(QString fileName);
    quint64 getPacketsCountFromFile(QFile &fd);
    void updateUI(Packet packet);
    void updatePlot(QCustomPlot *plot, double x, double y, double z);
    void updateAccelAndGyro(QCustomPlot *accelPlot, QCustomPlot *gyroPlot, Packet &dataPacket);
    void updateSlider();
    void setPlotRange(QCustomPlot *plot);
    void setScrollString(quint64 min, quint64 current, quint64 max);
    void updateOffsetPacketFromUI();
    void resetCounters();
    void redrawDataFromBuffer(bool useOffsetPacket = false);
    void showStatusBar(QString message);
    void showPacketsProgress(qint64 current, qint64 maximum);


private slots:
    void on_pushButton_clicked();
    void on_horizontalSlider_valueChanged(int value);

    void on_pushButton_2_clicked();

    void on_pushButton_kalman_apply_clicked();

    void on_pushButton_complementary_apply_clicked();

private:
    Ui::MainWindow *ui;
    quint64 m_xAxisCounter = 0;
    quint64 m_xAxisMax = 1024 * 2;
    double m_yAxisMax = 3100.0;
    quint64 m_plotWidth = 512;
    Packet m_parsedPacket;
    Packet m_zeroOffsetPacket;
    QVector<Packet> m_loadedPackets;
    QVector<Packet> m_loadedPacketsProcessed;
    PacketParser m_mpuPacketParser;
    quint64 m_sliderPosition = 0;
    KalmanPacketFilter m_KalmanPacketFilter;
    ComplementaryPacketFilter m_ComplementaryPacketFilter;
    quint64 m_xAxisCounterProcessedPair = 0;
};

#endif // MAINWINDOW_H
