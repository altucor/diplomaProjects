#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "qcustomplot.h"
#include "packet.h"
#include "packetparser.h"

enum plotterType{
    ACCELEROMETER = 0,
    GYROSCOPE
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
    void initializePLotter(QCustomPlot *plotter, int colorScheme);
    void loadFromFile(QString fileName);
    void updateUI();
    void updatePlot(QCustomPlot *plot, double x, double y, double z);
    void updateAccelAndGyro(QCustomPlot *accelPlot, QCustomPlot *gyroPlot, Packet &dataPacket);
    void updateSlider();
    void setPlotRange(QCustomPlot *plot);
    void setScrollString(quint64 min, quint64 current, quint64 max);


private slots:
    void on_pushButton_clicked();
    void on_horizontalSlider_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    quint64 m_xAxisCounter = 0;
    quint64 m_xAxisMax = 1024 * 2;
    double m_yAxisMax = 3100;
    quint64 m_plotWidth = 512;
    Packet m_parsedPacket;
    Packet m_zeroOffsetPacket;
    QVector<Packet> m_loadedPackets;
    PacketParser m_mpuPacketParser;
    quint64 m_sliderPosition = 0;
};

#endif // MAINWINDOW_H
