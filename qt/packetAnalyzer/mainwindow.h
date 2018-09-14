#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>

#include "form_fft.h"

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
    void resetInPlotters();
    void resetOutPlotters();
    void initializePLotter(QCustomPlot *plotter, int colorScheme);
    void loadFromFile(QString fileName);
    quint64 getPacketsCountFromFile(QFile &fd);
    double accelToAngle(double value);
    void processAccelAngles();
    void updateUI(Packet packet);
    void updatePlot(QCustomPlot *plot, double x, double y, double z);
    void updateMaxYAxis(QCustomPlot *accelPlot, QCustomPlot *gyroPlot, Packet & dataPacket);
    void updateAccelAndGyro(QCustomPlot *accelPlot, QCustomPlot *gyroPlot, Packet &dataPacket);
    void updateSlider();
    void setPlotRange(QCustomPlot *plot);
    void setScrollString(quint64 min, quint64 current, quint64 max);
    void updateOffsetPacketFromUI();
    void resetCounters();
    void redrawDataFromBuffer(bool useOffsetPacket = false);
    void showStatusBar(QString message);
    void showPacketsProgress(qint64 current, qint64 maximum);
    void replotAllViewsWithCustomLayers();


private slots:
    void on_pushButton_clicked();
    void on_horizontalSlider_valueChanged(int value);
    void on_pushButton_2_clicked();
    void on_pushButton_kalman_apply_clicked();
    void on_pushButton_complementary_apply_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();

private:
    std::unique_ptr<form_fft> m_fftForm;
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
    QVector<bool> m_checkedLayersPreviousState;
    uint64_t m_dt = 5; // 5 ms polling
    double m_1G = 2048.0;
    double m_toDeg = 57.29577951308232087679815481410517033f;
    double m_xAccelAngle = 0.0;
    double m_yAccelAngle = 0.0;
    double m_zAccelAngle = 0.0;
    double m_xGyroSpeed = 0.0;
    double m_yGyroSpeed = 0.0;
    double m_zGyroSpeed = 0.0;
    double m_xGyroDistance = 0.0;
    double m_yGyroDistance = 0.0;
    double m_zGyroDistance = 0.0;

};

#endif // MAINWINDOW_H
