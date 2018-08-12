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
    ui->pushButton_2->setEnabled(false);

    initializePLotter(ui->customPlotAccelerometerIn, ACCELEROMETER);
    initializePLotter(ui->customPlotGyroscopeIn, GYROSCOPE);

    initializePLotter(ui->customPlotAccelerometerOut, ACCELEROMETER);
    initializePLotter(ui->customPlotGyroscopeOut, GYROSCOPE);

    initializeProgressBars(m_yAxisMax * (-1), m_yAxisMax, 0);
}

void MainWindow::initializeProgressBars(qint64 min, qint64 max, qint64 startValue)
{
    ui->progressBar_0->setMinimum(min);
    ui->progressBar_0->setMaximum(max);
    ui->progressBar_0->setValue(startValue);

    ui->progressBar_1->setMinimum(min);
    ui->progressBar_1->setMaximum(max);
    ui->progressBar_1->setValue(startValue);

    ui->progressBar_2->setMinimum(min);
    ui->progressBar_2->setMaximum(max);
    ui->progressBar_2->setValue(startValue);

    ui->progressBar_3->setMinimum(min);
    ui->progressBar_3->setMaximum(max);
    ui->progressBar_3->setValue(startValue);

    ui->progressBar_4->setMinimum(min);
    ui->progressBar_4->setMaximum(max);
    ui->progressBar_4->setValue(startValue);

    ui->progressBar_5->setMinimum(min);
    ui->progressBar_5->setMaximum(max);
    ui->progressBar_5->setValue(startValue);
}

void MainWindow::resetOutPlotters()
{
    ui->customPlotAccelerometerOut->clearPlottables();
    ui->customPlotGyroscopeOut->clearPlottables();

    initializePLotter(ui->customPlotAccelerometerOut, ACCELEROMETER);
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

    plotter->xAxis->setLabel("Packet number");
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

    if(m_loadedPackets.size() > m_plotWidth)
        ui->pushButton_2->setEnabled(true);
}

void MainWindow::loadFromFile(QString fileName)
{
    ui->horizontalSlider->setEnabled(false);

    QFile fd(fileName);
    if( !fd.open(QIODevice::ReadOnly) ) return;

    quint64 packetsCount = getPacketsCountFromFile(fd);

    QTextStream fStream(&fd);

    ui->textEdit->clear();

    m_xAxisCounter = 0;

    while( !fStream.atEnd() ){
        QString packetLine = fStream.readLine();
        ui->textEdit->append(packetLine);
        m_mpuPacketParser.setPacket(packetLine);
        m_parsedPacket = m_mpuPacketParser.getPacket();
        updateUI(m_parsedPacket);
        updateAccelAndGyro(ui->customPlotAccelerometerIn, ui->customPlotGyroscopeIn, m_parsedPacket);
        m_loadedPackets.push_back(m_mpuPacketParser.getPacket());
        showPacketsProgress(m_xAxisCounter, packetsCount);
        m_xAxisCounter++;
    }

    updateSlider();
    ui->horizontalSlider->setEnabled(true);
}

quint64 MainWindow::getPacketsCountFromFile(QFile &fd)
{
    quint64 packetsCount = 0;
    fd.seek(0);
    QTextStream fStream(&fd);
    while( !fStream.atEnd() ){
        fStream.readLine();
        packetsCount++;
    }
    fd.seek(0);
    return packetsCount - 1;
}

void MainWindow::updateUI(Packet packet)
{
    ui->progressBar_0->setValue(packet.ax()); // accel X
    ui->progressBar_1->setValue(packet.ay()); // accel Y
    ui->progressBar_2->setValue(packet.az()); // accel Z
    ui->progressBar_3->setValue(packet.gx()); // gyro X
    ui->progressBar_4->setValue(packet.gy()); // gyro Y
    ui->progressBar_5->setValue(packet.gz()); // gyro Z
    ui->label_13->setText("Temperature: " + QString::number(packet.temp()));
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

    setScrollString(0, m_sliderPosition, m_xAxisCounter);

    setPlotRange(ui->customPlotAccelerometerIn);
    setPlotRange(ui->customPlotGyroscopeIn);
    setPlotRange(ui->customPlotAccelerometerOut);
    setPlotRange(ui->customPlotGyroscopeOut);

    if( m_loadedPackets.size() > m_plotWidth &&
        m_loadedPackets.size() > m_sliderPosition)
    {
        m_parsedPacket = m_loadedPackets[m_sliderPosition];
        updateUI(m_parsedPacket);
    }
}

void MainWindow::updateSlider()
{
    ui->horizontalSlider->setMaximum(m_xAxisCounter);
    ui->horizontalSlider->setValue(m_xAxisCounter);
    setScrollString(0, m_xAxisCounter, m_xAxisCounter);
}

void MainWindow::resetCounters()
{
    m_xAxisCounter = 0;
    m_xAxisMax = 0;
}

void MainWindow::redrawDataFromBuffer(bool useOffsetPacket)
{
    //m_loadedPackets
    ui->horizontalSlider->setEnabled(false);

    resetCounters();

    for(int i=0; i<m_loadedPackets.size(); i++){
        m_parsedPacket = m_loadedPackets[i];

        if(useOffsetPacket)
            m_parsedPacket += m_zeroOffsetPacket;

        updateUI(m_parsedPacket);
        updateAccelAndGyro(ui->customPlotAccelerometerIn, ui->customPlotGyroscopeIn, m_parsedPacket);
        m_xAxisCounter++;
    }

    updateSlider();
    ui->horizontalSlider->setEnabled(true);
}

void MainWindow::updateOffsetPacketFromUI()
{
    m_zeroOffsetPacket.ax(ui->doubleSpinBox_AcXOff->value());
    m_zeroOffsetPacket.ay(ui->doubleSpinBox_AcYOff->value());
    m_zeroOffsetPacket.az(ui->doubleSpinBox_AcZOff->value());

    m_zeroOffsetPacket.gx(ui->doubleSpinBox_GyXOff->value());
    m_zeroOffsetPacket.gx(ui->doubleSpinBox_GyYOff->value());
    m_zeroOffsetPacket.gx(ui->doubleSpinBox_GyZOff->value());

    m_zeroOffsetPacket.temp(0);

    m_mpuPacketParser.setCalibrationPacket(m_zeroOffsetPacket);
}

void MainWindow::on_pushButton_2_clicked()
{
    // Apply offset packet
    updateOffsetPacketFromUI();
    redrawDataFromBuffer(true);
}

void MainWindow::on_pushButton_kalman_apply_clicked()
{
    Packet processedPacket;
    m_xAxisCounter = 0;
    m_KalmanPacketFilter.setKalmanCoefficient(ui->doubleSpinBox_kalman_coefficient->value());

    resetOutPlotters();

    m_loadedPacketsProcessed.resize(m_loadedPackets.size());

    for(int i=0; i<m_loadedPackets.size(); i++){
        processedPacket = m_KalmanPacketFilter.processPacket(m_loadedPackets[i]);
        m_loadedPacketsProcessed[i] = processedPacket;
        updateAccelAndGyro(ui->customPlotAccelerometerOut, ui->customPlotGyroscopeOut, processedPacket);
        showPacketsProgress(i, (m_loadedPackets.size() - 1));
        m_xAxisCounter++;
    }
}

void MainWindow::showStatusBar(QString message)
{
    ui->statusBar->showMessage(message, 3000);
}

void MainWindow::showPacketsProgress(qint64 current, qint64 maximum)
{
    QString msg = "Processing: " + QString::number(current) + " of " + QString::number(maximum) + " packets";
    showStatusBar(msg);
}

void MainWindow::on_pushButton_complementary_apply_clicked()
{
    // Apply complemetary filter
    Packet processedPacket;
    m_xAxisCounter = 0;
    m_ComplementaryPacketFilter.setCoefficient(ui->doubleSpinBox_complementary_coefficient->value());

    resetOutPlotters();

    m_loadedPacketsProcessed.resize(m_loadedPackets.size());

    for(int i=0; i<m_loadedPackets.size(); i++){
        processedPacket = m_ComplementaryPacketFilter.processPacket(m_loadedPackets[i]);
        m_loadedPacketsProcessed[i] = processedPacket;
        updateAccelAndGyro(ui->customPlotAccelerometerOut, ui->customPlotGyroscopeOut, processedPacket);
        showPacketsProgress(i, (m_loadedPackets.size() - 1));
        m_xAxisCounter++;
    }
}
