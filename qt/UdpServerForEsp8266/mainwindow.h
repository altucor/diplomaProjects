#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "udpserverwrapper.h"
#include "qcustomplot.h"
#include "packetparser.h"
#include "packet.h"

namespace Ui {
class MainWindow;
}

enum STATE{
    PLAY = 0,
    PAUSE
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void initializeProgressBars(qint64 min, qint64 max, qint64 startValue);
    bool connectServer(UdpServerWrapper &serverPtr);
    void updateUI();
    void updateAccelPlot();
    void updateGyroPlot();
    void updateMousePos();
    void resetMaxYAxis();
    void updateMaxYAxis(QCustomPlot *accelPlot, QCustomPlot *gyroPlot, Packet & dataPacket);
    void updateSlider();
    void loadFromFile(QString fileName);
    double accelToAngle(double value);
    void processAccelAngles();
    void gyroToSpeed(double value, double &speed);
    void processGyroSpeeds();
    void gyroToDistance(double value, double &speedValue, double &distance);
    void processDistance();

private:
    void accelPlotInitialization();
    void gyroPlotInitialization();
    void packetParserInitialization();

private:
    Ui::MainWindow *ui;
    QCursor *m_cursor;
    QPoint m_mousePoint;
    quint64 m_xAxisCounter = 0;
    quint64 m_xAxisMax = 1024 * 2;
    double m_yAxisMax = 100.0;
    PacketParser m_mpuPacketParser;
    bool m_movingMouse = false;
    Packet m_parsedPacket;
    Packet m_zeroOffsetPacket;
    FilterType m_filterType = RAW;
    bool m_state = PLAY;
    int m_sliderPosition = 0;
    quint64 m_plotWidth = 512;
    QVector<Packet> m_loadedPackets;
    uint64_t m_dt = 5; // 5 ms polling
    double m_1G = 2048.0;
    double m_toDeg = 57.29577951308232087679815481410517033f;
    double m_xAccelAngle = 0.0;
    double m_yAccelAngle = 0.0;
    double m_zAccelAngle = 0.0;
    double m_xSpeed = 0.0;
    double m_ySpeed = 0.0;
    double m_zSpeed = 0.0;
    double m_xDistance = 0.0;
    double m_yDistance = 0.0;
    double m_zDistance = 0.0;


public slots:
    void processNewPacket(QByteArray data);
private slots:
    void on_pushButton_3_clicked();
    void on_checkBox_0_stateChanged(int arg1);
    void on_checkBox_1_stateChanged(int arg1);
    void on_pushButton_4_clicked();
    void on_pushButton_clicked();
    void on_horizontalSlider_valueChanged(int value);
    void on_pushButton_2_clicked();
    void on_radioButton_clicked();
    void on_radioButton_2_clicked();
    void on_pushButton_5_clicked();
    void on_radioButton_4_clicked();
};

#endif // MAINWINDOW_H
