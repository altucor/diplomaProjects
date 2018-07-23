#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_cursor = new QCursor;
    accelPlotInitialization();
    gyroPlotInitialization();
    packetParserInitialization();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::accelPlotInitialization()
{
    // aX graph
    ui->customPlotAccel->addGraph();
    ui->customPlotAccel->graph(0)->setPen(QPen(Qt::blue));
    // aY graph
    ui->customPlotAccel->addGraph();
    ui->customPlotAccel->graph(1)->setPen(QPen(Qt::red));
    // aZ graph
    ui->customPlotAccel->addGraph();
    ui->customPlotAccel->graph(2)->setPen(QPen(Qt::green));

    //ui->customPlotAccel->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20)));

    ui->customPlotAccel->xAxis->setLabel("Time");
    ui->customPlotAccel->yAxis->setLabel("Amplitude");

    // set axis ranges, so we see all data:
    ui->customPlotAccel->xAxis->setRange(0, m_xAxisMax);
    ui->customPlotAccel->yAxis->setRange(-1 * m_yAxisMax, m_yAxisMax);
}

void MainWindow::gyroPlotInitialization()
{
    // gX graph
    ui->customPlotGyro->addGraph();
    ui->customPlotGyro->graph(0)->setPen(QPen(Qt::magenta));
    // gY graph
    ui->customPlotGyro->addGraph();
    ui->customPlotGyro->graph(1)->setPen(QPen(Qt::black));
    // gZ graph
    ui->customPlotGyro->addGraph();
    ui->customPlotGyro->graph(2)->setPen(QPen(Qt::cyan));

    //ui->customPlotGyro->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20)));

    ui->customPlotGyro->xAxis->setLabel("Time");
    ui->customPlotGyro->yAxis->setLabel("Amplitude");

    // set axes ranges, so we see all data:
    ui->customPlotGyro->xAxis->setRange(0, m_xAxisMax);
    ui->customPlotGyro->yAxis->setRange(-1 * m_yAxisMax, m_yAxisMax);
}

void MainWindow::packetParserInitialization()
{
    m_zeroOffsetPacket.ax(115);
    m_zeroOffsetPacket.ay(-210);
    m_zeroOffsetPacket.az(-20);
    m_zeroOffsetPacket.gx(15);
    m_zeroOffsetPacket.gy(-20);
    m_zeroOffsetPacket.gz(-20);
    m_zeroOffsetPacket.temp(0);
    m_mpuPacketParser.setCalibrationPacket(m_zeroOffsetPacket);
}

void MainWindow::processNewPacket(QByteArray data)
{
    m_mpuPacketParser.setPacket(data);
    m_mpuPacketParser.setFilter(m_filterType);
    m_parsedPacket = m_mpuPacketParser.getPacket();
    ui->textEdit->append(m_parsedPacket.toString());
    updateUI();
    updateAccelPlot();
    updateGyroPlot();
    updateMousePos();
}

bool MainWindow::connectServer(UdpServerWrapper &serverPtr)
{
    return QObject::connect(&serverPtr, &UdpServerWrapper::processNewPacket, this, MainWindow::processNewPacket);
}

void MainWindow::updateUI()
{
    ui->progressBar_0->setValue(m_parsedPacket.ax()); // accel X
    ui->progressBar_1->setValue(m_parsedPacket.ay()); // accel Y
    ui->progressBar_2->setValue(m_parsedPacket.az()); // accel Z
    ui->progressBar_3->setValue(m_parsedPacket.gx()); // gyro X
    ui->progressBar_4->setValue(m_parsedPacket.gy()); // gyro Y
    ui->progressBar_5->setValue(m_parsedPacket.gz()); // gyro Z
    ui->label_0->setText("Temp: " + QString::number(m_parsedPacket.temp()));
}

void MainWindow::updateAccelPlot()
{
    ui->customPlotAccel->graph(0)->addData(m_xAxisCounter, m_parsedPacket.ax());
    ui->customPlotAccel->graph(1)->addData(m_xAxisCounter, m_parsedPacket.ay());
    ui->customPlotAccel->graph(2)->addData(m_xAxisCounter, m_parsedPacket.az());

    ui->customPlotAccel->xAxis->setRange(m_xAxisCounter, 512, Qt::AlignRight);
    ui->customPlotAccel->replot();

    ui->customPlotGyro->graph(0)->addData(m_xAxisCounter, m_parsedPacket.gx());
    ui->customPlotGyro->graph(1)->addData(m_xAxisCounter, m_parsedPacket.gy());
    ui->customPlotGyro->graph(2)->addData(m_xAxisCounter, m_parsedPacket.gz());

    ui->customPlotGyro->xAxis->setRange(m_xAxisCounter, 512, Qt::AlignRight);
    ui->customPlotGyro->replot();

    m_xAxisCounter++;

    //if(m_xAxisCounter > m_xAxisMax)
    //    m_xAxisCounter = 0;
}

void MainWindow::updateGyroPlot()
{
    //
}

void MainWindow::updateMousePos()
{
    if(m_movingMouse == false) return;
    m_mousePoint = m_cursor->pos();
    /* 0.5 - offset for zero position mouse
     * if mouse value little bit lower than 0, than mouse moves
     * if value in range beetween 0.0 - 0.5 than it stay on zero poisiton
     */
    double x = 0.5 + m_mousePoint.x() + m_parsedPacket.ax() / 200 * -1;
    double y = 0.5 + m_mousePoint.y() + m_parsedPacket.ay() / 200;

    m_mousePoint.setX(x);
    m_mousePoint.setY(y);
    m_cursor->setPos(m_mousePoint.x(), m_mousePoint.y());


    qDebug() << "Mouse X: " << x << " Mouse Y: " << y;
}

void MainWindow::on_pushButton_3_clicked()
{
    // make lock for object when we clearing data
    ui->textEdit->clear();
    m_xAxisCounter = 0;
    //ui->customPlotAccel->clearItems();
    //ui->customPlotAccel = new QcustomPlotAccel();
    ui->customPlotAccel->replot();
}

void MainWindow::on_checkBox_0_stateChanged(int arg1)
{
    if(ui->checkBox_0->isChecked())
        m_movingMouse = true;
    else
        m_movingMouse = false;
}

void MainWindow::on_checkBox_1_stateChanged(int arg1)
{
    if(ui->checkBox_1->isChecked())
        m_filterType = KALMAN;
    else
        m_filterType = NONE;
}
