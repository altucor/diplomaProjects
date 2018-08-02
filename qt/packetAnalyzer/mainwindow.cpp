#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initializeAll();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setOffsetPacket()
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

void MainWindow::initializeAll()
{
    initializePLotter(ui->customPlotAccelerometerIn, ACCELEROMETER);
    initializePLotter(ui->customPlotAccelerometerOut, ACCELEROMETER);
    initializePLotter(ui->customPlotGyroscopeIn, GYROSCOPE);
    initializePLotter(ui->customPlotGyroscopeOut, GYROSCOPE);
}

void MainWindow::initializePLotter(QCustomPlot *plotter, int colorScheme)
{
    QPen penX = QPen(Qt::blue);
    QPen penY = QPen(Qt::red);
    QPen penZ = QPen(Qt::green);

    if(colorScheme == GYROSCOPE){
        penX = QPen(Qt::magenta);
        penY = QPen(Qt::black);
        penZ = QPen(Qt::cyan);
    }

    // aX graph
    plotter->addGraph();
    plotter->graph(0)->setPen(penX);
    // aY graph
    plotter->addGraph();
    plotter->graph(1)->setPen(penY);
    // aZ graph
    plotter->addGraph();
    plotter->graph(2)->setPen(penZ);

    plotter->xAxis->setLabel("Time");
    plotter->yAxis->setLabel("Amplitude");

    // set axis ranges, so we see all data:
    plotter->xAxis->setRange(0, m_xAxisMax);
    plotter->yAxis->setRange(-1 * m_yAxisMax, m_yAxisMax);
}

void MainWindow::on_pushButton_clicked()
{
    // Load from file to memory
    QString fileName = QFileDialog::getOpenFileName(this, "Select file for reading data", QDir::currentPath(), "*.dat");
    loadFromFile(fileName);
}

void MainWindow::loadFromFile(QString fileName)
{
    ui->horizontalSlider->setEnabled(false);

    QFile fd(fileName);
    if( !fd.open(QIODevice::ReadOnly) ) return;

    QTextStream fStream(&fd);

    ui->textEdit->clear();

    m_xAxisCounter = 0;

    while( !fStream.atEnd() ){
        QString packetLine = fStream.readLine();
        ui->textEdit->append(packetLine);
        m_mpuPacketParser.setPacket(packetLine);
        m_parsedPacket = m_mpuPacketParser.getPacket();
        updateUI();
        updateAccelAndGyro(ui->customPlotAccelerometerIn, ui->customPlotGyroscopeIn, m_parsedPacket);
        m_loadedPackets.push_back(m_mpuPacketParser.getPacket());
        m_xAxisCounter++;
    }

    updateSlider();
    ui->horizontalSlider->setEnabled(true);
}

void MainWindow::updateUI()
{
    ui->progressBar_0->setValue(m_parsedPacket.ax()); // accel X
    ui->progressBar_1->setValue(m_parsedPacket.ay()); // accel Y
    ui->progressBar_2->setValue(m_parsedPacket.az()); // accel Z
    ui->progressBar_3->setValue(m_parsedPacket.gx()); // gyro X
    ui->progressBar_4->setValue(m_parsedPacket.gy()); // gyro Y
    ui->progressBar_5->setValue(m_parsedPacket.gz()); // gyro Z
    ui->label_13->setText("Temperature: " + QString::number(m_parsedPacket.temp()));
}

void MainWindow::updatePlot(QCustomPlot *plot, double x, double y, double z)
{
    plot->graph(0)->addData(m_xAxisCounter, x);
    plot->graph(1)->addData(m_xAxisCounter, y);
    plot->graph(2)->addData(m_xAxisCounter, z);

    plot->xAxis->setRange(m_xAxisCounter, m_plotWidth, Qt::AlignRight);
    plot->replot();
}

void MainWindow::updateAccelAndGyro(QCustomPlot *accelPlot, QCustomPlot *gyroPlot, Packet &dataPacket)
{
    updatePlot(accelPlot, dataPacket.ax(), dataPacket.ay(), dataPacket.az());
    updatePlot(gyroPlot, dataPacket.gx(), dataPacket.gy(), dataPacket.gz());
}

void MainWindow::setPlotRange(QCustomPlot *plot)
{
    plot->xAxis->setRange(m_sliderPosition, m_plotWidth, Qt::AlignRight);
    plot->replot();
}

void MainWindow::setScrollString(quint64 min, quint64 current, quint64 max)
{
    ui->label_14->setText("Min = " + QString::number(min) + "; Current = " + QString::number(current) + "; Max = " + QString::number(max) + ";");

}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    m_sliderPosition = ui->horizontalSlider->value();

    //ui->label_14->setText("Min = 0; Current = " + QString::number(m_sliderPosition) + "; Max = " + QString::number(m_xAxisCounter) + ";");
    setScrollString(0, m_sliderPosition, m_xAxisCounter);


    setPlotRange(ui->customPlotAccelerometerIn);
    setPlotRange(ui->customPlotGyroscopeIn);
    setPlotRange(ui->customPlotAccelerometerOut);
    setPlotRange(ui->customPlotGyroscopeOut);

    if( m_loadedPackets.size() > m_plotWidth &&
        m_loadedPackets.size() > m_sliderPosition
        ){
            m_parsedPacket = m_loadedPackets[m_sliderPosition];
            updateUI();
    }
}

void MainWindow::updateSlider()
{
    ui->horizontalSlider->setMaximum(m_xAxisCounter);
    ui->horizontalSlider->setValue(m_xAxisCounter);
    //ui->label_14->setText("Min = 0; Current = " + QString::number(m_xAxisCounter) + "; Max = " + QString::number(m_xAxisCounter) + ";");
    setScrollString(0, m_xAxisCounter, m_xAxisCounter);
}
