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
    bool connectServer(UdpServerWrapper &serverPtr);
    void updateUI();
    void updateAccelPlot();
    void updateGyroPlot();
    void updateMousePos();
    void updateSlider();
    void loadFromFile(QString fileName);

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
    double m_yAxisMax = 3100.0;
    PacketParser m_mpuPacketParser;
    bool m_movingMouse = false;
    Packet m_parsedPacket;
    Packet m_zeroOffsetPacket;
    int m_filterType = NONE;
    bool m_state = PLAY;
    int m_sliderPosition = 0;
    int m_plotWidth = 512;
    QVector<Packet> m_loadedPackets;

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
};

#endif // MAINWINDOW_H
