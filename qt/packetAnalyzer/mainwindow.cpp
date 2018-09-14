#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("packetAnalyzer");
    // Initialize previous states of layers checkboxes
    for(int i=0; i<6; i++)
        m_checkedLayersPreviousState.push_back(true);

    initializeAll();
    m_fftForm = std::unique_ptr<form_fft>( new form_fft());
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

void MainWindow::resetInPlotters()
{
    ui->customPlotAccelerometerIn->clearPlottables();
    ui->customPlotGyroscopeIn->clearPlottables();

    initializePLotter(ui->customPlotAccelerometerIn, ACCELEROMETER);
    initializePLotter(ui->customPlotGyroscopeIn, GYROSCOPE);
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

double MainWindow::accelToAngle(double value)
{
    // 16g - 2048 = 1G
    double angle = 0.0;
    double gValue = value / m_1G;

    if(gValue > 1.0){
        gValue = 1.0;
    } else if(gValue < -1.0) {
        gValue = -1.0;
    }

    if(gValue >= 0){
        angle = 90 - m_toDeg * acos(gValue);
    }else{
        angle = m_toDeg * acos(-gValue) - 90;
    }

    return angle;
}

void MainWindow::processAccelAngles()
{
    m_xAccelAngle = accelToAngle(m_parsedPacket.ax());
    m_yAccelAngle = accelToAngle(m_parsedPacket.ay());
    m_zAccelAngle = accelToAngle(m_parsedPacket.az());

    ui->label_accel_angle->setText("Accel Angle: X=" + QString::number(m_xAccelAngle) + "; Y=" + QString::number(m_yAccelAngle) + "; Z=" + QString::number(m_zAccelAngle));
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
    processAccelAngles();
}

void MainWindow::updatePlot(QCustomPlot *plot, double x, double y, double z)
{
    plot->graph(0)->addData(m_xAxisCounter, x);
    plot->graph(1)->addData(m_xAxisCounter, y);
    plot->graph(2)->addData(m_xAxisCounter, z);

    plot->xAxis->setRange(m_xAxisCounter, m_plotWidth, Qt::AlignRight);
    plot->replot();
}

void MainWindow::updateMaxYAxis(QCustomPlot *accelPlot, QCustomPlot *gyroPlot, Packet & dataPacket)
{
    //m_yAxisMax
    if(abs(dataPacket.ax()) > m_yAxisMax)
        m_yAxisMax = abs(dataPacket.ax()) + 100;
    if(abs(dataPacket.ay()) > m_yAxisMax)
        m_yAxisMax = abs(dataPacket.ay()) + 100;
    if(abs(dataPacket.az()) > m_yAxisMax)
        m_yAxisMax = abs(dataPacket.az()) + 100;

    if(abs(dataPacket.gx()) > m_yAxisMax)
        m_yAxisMax = abs(dataPacket.gx()) + 100;
    if(abs(dataPacket.gy()) > m_yAxisMax)
        m_yAxisMax = abs(dataPacket.gy()) + 100;
    if(abs(dataPacket.gz()) > m_yAxisMax)
        m_yAxisMax = abs(dataPacket.gz()) + 100;

    accelPlot->yAxis->setRange(-1 * m_yAxisMax, m_yAxisMax);
    gyroPlot->yAxis->setRange(-1 * m_yAxisMax, m_yAxisMax);
}

void MainWindow::updateAccelAndGyro(QCustomPlot *accelPlot, QCustomPlot *gyroPlot, Packet &dataPacket)
{
    updateMaxYAxis(accelPlot, gyroPlot, dataPacket);
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
        //ui->textEdit->verticalScrollBar()->setValue(m_sliderPosition);
        QTextCursor cursor(ui->textEdit->document()->findBlockByLineNumber(m_sliderPosition-1));
        ui->textEdit->setTextCursor(cursor);
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

void MainWindow::replotAllViewsWithCustomLayers()
{
    Packet bufferPacketIn;
    Packet bufferPacketOut;
    m_xAxisCounter = 0;

    resetInPlotters();
    resetOutPlotters();

    m_loadedPacketsProcessed.resize(m_loadedPackets.size());

    for(int i=0; i<m_loadedPackets.size(); i++){
        bufferPacketIn = m_loadedPackets[i];
        bufferPacketOut = m_loadedPacketsProcessed[i];

        if( !m_checkedLayersPreviousState[0] ){
            bufferPacketIn.ax(0);
            bufferPacketOut.ax(0);
        }
        if( !m_checkedLayersPreviousState[1] ){
            bufferPacketIn.ay(0);
            bufferPacketOut.ay(0);
        }
        if( !m_checkedLayersPreviousState[2] ){
            bufferPacketIn.az(0);
            bufferPacketOut.az(0);
        }

        if( !m_checkedLayersPreviousState[3] ){
            bufferPacketIn.gx(0);
            bufferPacketOut.gx(0);
        }
        if( !m_checkedLayersPreviousState[4] ){
            bufferPacketIn.gy(0);
            bufferPacketOut.gy(0);
        }
        if( !m_checkedLayersPreviousState[5] ){
            bufferPacketIn.gz(0);
            bufferPacketOut.gz(0);
        }

        updateAccelAndGyro(ui->customPlotAccelerometerIn, ui->customPlotGyroscopeIn, bufferPacketIn);
        updateAccelAndGyro(ui->customPlotAccelerometerOut, ui->customPlotGyroscopeOut, bufferPacketOut);
        showPacketsProgress(i, (m_loadedPackets.size() - 1));
        m_xAxisCounter++;
    }
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

void MainWindow::on_pushButton_3_clicked()
{
    int changes = 0;

    if(ui->checkBox_draw_ax->isChecked() != m_checkedLayersPreviousState[0]){
        m_checkedLayersPreviousState[0] = ui->checkBox_draw_ax->isChecked();
        changes++;
    }
    if(ui->checkBox_draw_ay->isChecked() != m_checkedLayersPreviousState[1]){
        m_checkedLayersPreviousState[1] = ui->checkBox_draw_ay->isChecked();
        changes++;
    }
    if(ui->checkBox_draw_az->isChecked() != m_checkedLayersPreviousState[2]){
        m_checkedLayersPreviousState[2] = ui->checkBox_draw_az->isChecked();
        changes++;
    }

    if(ui->checkBox_draw_gx->isChecked() != m_checkedLayersPreviousState[3]){
        m_checkedLayersPreviousState[3] = ui->checkBox_draw_gx->isChecked();
        changes++;
    }
    if(ui->checkBox_draw_gy->isChecked() != m_checkedLayersPreviousState[4]){
        m_checkedLayersPreviousState[4] = ui->checkBox_draw_gy->isChecked();
        changes++;
    }
    if(ui->checkBox_draw_gz->isChecked() != m_checkedLayersPreviousState[5]){
        m_checkedLayersPreviousState[5] = ui->checkBox_draw_gz->isChecked();
        changes++;
    }

    //if(changes > 0){
    replotAllViewsWithCustomLayers();
}

void MainWindow::on_pushButton_4_clicked()
{
    // Show FFT Form
    m_fftForm->show();
}
