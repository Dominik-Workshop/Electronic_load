#include "electronic_load_app.h"
#include "ui_electronic_load_app.h"
#include <QDoubleValidator>
#include <QVector>
#include <QFileDialog>
#include <QMessageBox>

Electronic_load_app::Electronic_load_app(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Electronic_load_app)
{
    ui->setupUi(this);

    this->setWindowIcon(QIcon(":/icon.png"));
    translator_PL.load(":/polish.qm");
    translator_DE.load(":/german.qm");

    // Fill combo box with available ports at startup
    updateAvailablePorts();

    // Set up timer to periodically check for available ports
    portCheckTimer = new QTimer(this);
    connect(portCheckTimer, &QTimer::timeout, this, &Electronic_load_app::updateAvailablePorts);
    portCheckTimer->start(1000); // Check every second

    ui->cmbSaveAs->addItem("jpg");
    ui->cmbSaveAs->addItem("csv");

    ui->capacity_mAh->setText(QString::number(measurements.capacity_mAh, 'f', 3));
    ui->capacity_Wh->setText(QString::number(measurements.capacity_Wh, 'f', 3));

    ui->VoltageAndCurrentPlot->xAxis->setLabel(QObject::tr("Time [s]"));
    ui->VoltageAndCurrentPlot->yAxis->setLabel(QObject::tr("Voltage [V]"));
    ui->VoltageAndCurrentPlot->yAxis2->setLabel(QObject::tr("Current [A]"));
    ui->VoltageAndCurrentPlot->yAxis2->setVisible(true);

    //ui->VoltagePlot->setInteraction(QCP::iRangeDrag, true);
    //ui->VoltagePlot->setInteraction(QCP::iRangeZoom, true);
    timer.start();

}

Electronic_load_app::~Electronic_load_app(){
    delete ui;
    if(serialPort != nullptr){
        serialPort->close();
        delete serialPort;
    }
}

void Electronic_load_app::connectTheFrickingSlots(){
    qDebug() << "connextsa?";
    connect(serialPort, SIGNAL(readyRead()), this, SLOT(readData()));
}

void Electronic_load_app::changeEvent(QEvent *event){
    if(event->type() == QEvent::LanguageChange){
        ui->retranslateUi(this);
    }
    QWidget::changeEvent(event);
}

void Electronic_load_app::updateAvailablePorts(){
    QStringList availablePorts;
    foreach (const QSerialPortInfo &port, QSerialPortInfo::availablePorts()) {
        availablePorts << port.portName();
    }

    if (availablePorts != currentPorts) {
        currentPorts = availablePorts;
        ui->cmbPorts->clear();
        ui->cmbPorts->addItems(currentPorts);
    }
}

void Electronic_load_app::plotVoltageAndCurrent(){
    QVector<double> x(measurements.readings.size()), y(measurements.readings.size());
    for (unsigned long i = 0; i < measurements.readings.size(); ++i) {
        x[i] = measurements.readings[i].time_s; // Assuming x-axis is index based
        y[i] = measurements.readings[i].voltage_V;
    }
    QVector<double> x2(measurements.readings.size()), y2(measurements.readings.size());
    for (unsigned long i = 0; i < measurements.readings.size(); ++i) {
        x2[i] = measurements.readings[i].time_s; // Assuming x-axis is index based
        y2[i] = measurements.readings[i].current_A;
    }

    // Clear existing graph and update with new data
    ui->VoltageAndCurrentPlot->clearGraphs();
    ui->VoltageAndCurrentPlot->legend->setVisible(true);
    ui->VoltageAndCurrentPlot->addGraph(ui->VoltageAndCurrentPlot->xAxis, ui->VoltageAndCurrentPlot->yAxis);
    ui->VoltageAndCurrentPlot->graph(0)->setPen(QPen(QColor(255, 100, 0)));
    ui->VoltageAndCurrentPlot->graph(0)->setData(x, y);
    ui->VoltageAndCurrentPlot->graph(0)->setName(QObject::tr("Voltage"));
    ui->VoltageAndCurrentPlot->addGraph(ui->VoltageAndCurrentPlot->xAxis2, ui->VoltageAndCurrentPlot->yAxis2);
    ui->VoltageAndCurrentPlot->graph(1)->setPen(QPen(QColor(0, 100, 255)));
    ui->VoltageAndCurrentPlot->graph(1)->setData(x2, y2);
    ui->VoltageAndCurrentPlot->graph(1)->setName(QObject::tr("Current"));

    ui->VoltageAndCurrentPlot->rescaleAxes();

    // Set axis ranges to start from 0
    //ui->VoltagePlot->yAxis->setRange(0, *std::max_element(y.constBegin(), y.constEnd()));
    //ui->VoltagePlot->yAxis2->setRange(0, *std::max_element(y2.constBegin(), y2.constEnd()));

    ui->VoltageAndCurrentPlot->replot();
    ui->VoltageAndCurrentPlot->update();
}

/**
 * @brief Read data from serial port
 */
void Electronic_load_app::readData(){
    if(serialPort->isOpen()){
        while(serialPort->bytesAvailable()){
            dataFromSerialPort += serialPort->readAll();
            if(dataFromSerialPort.at(dataFromSerialPort.length()-1) == char(10))
                isDataFromSerialPortReceived = true;
        }
        if(isDataFromSerialPortReceived){

            //qDebug() << "data from serial: " << Data_From_Serial_Port;
            processReceivedData();
            measurements.calculateCapacity();
            ui->capacity_mAh->setText(QString::number(measurements.capacity_mAh, 'f', 3));
            ui->capacity_Wh->setText(QString::number(measurements.capacity_Wh, 'f', 3));
            dataFromSerialPort = "";
            isDataFromSerialPortReceived = false;


            plotVoltageAndCurrent();

        }
    }
}

/**
 * @brief calculateCRC
 * @param data
 * @param length
 * @return
 */
uint32_t calculateCRC(const uint8_t* data, size_t length) {
  const uint32_t polynomial = 0xEDB88320;
  uint32_t crc = 0xFFFFFFFF;

  for (size_t i = 0; i < length; ++i) {
    crc ^= data[i];
    for (size_t j = 0; j < 8; ++j) {
      crc = (crc >> 1) ^ (-(int)(crc & 1) & polynomial);
    }
  }
  return crc ^ 0xFFFFFFFF;
}

/**
 * @brief Check CRC and process receided data
 *
 */
void Electronic_load_app::processReceivedData(){
    QStringList parts = QString(dataFromSerialPort).split(";");
    if (parts.size() != 9) {
        qDebug() << "Invalid data format. Expected 9 parts separated by semicolons.";
        return;
    }

    if (parts[Command] == 'm') {
        bool ok;
        qint16 receivedVoltage = parts[MeasuredVoltage].toInt(&ok);
        qint16 receivedCurrent = parts[MeasuredCurrent].toInt(&ok);
        qint16 receivedTemperature = parts[MeasuredTemperature].toInt(&ok);
        qint16 receivedCutoffVoltage = parts[SetCutofffVoltage].toInt(&ok);
        qint16 receivedDischargeCurrent = parts[SetCurret].toInt(&ok);
        qint16 receivedIsLoadOn = parts[IsLoadOn].toInt(&ok);
        qint32 receivedTime = parts[Time].toInt(&ok);
        uint32_t receivedCRC = parts[CRC].toUInt(&ok);

        if (!ok) {
            qDebug() << "Error converting received data";
            return;
        }

        // Prepare the data for CRC calculation
        uint8_t message[6 * sizeof(qint16) + sizeof(qint32)];
        size_t index = 0;
        memcpy(&message[index], &receivedVoltage, sizeof(qint16));
        index += sizeof(qint16);
        memcpy(&message[index], &receivedCurrent, sizeof(qint16));
        index += sizeof(qint16);
        memcpy(&message[index], &receivedTemperature, sizeof(qint16));
        index += sizeof(quint16);
        memcpy(&message[index], &receivedCutoffVoltage, sizeof(qint16));
        index += sizeof(qint16);
        memcpy(&message[index], &receivedDischargeCurrent, sizeof(qint16));
        index += sizeof(qint16);
        memcpy(&message[index], &receivedIsLoadOn, sizeof(qint16));
        index += sizeof(qint16);
        memcpy(&message[index], &receivedTime, sizeof(qint32));

        uint32_t calculatedCRC = calculateCRC(message, sizeof(message));

        //qDebug() << "Received CRC (Decimal):" << receivedCRC;
        //qDebug() << "Calculated CRC (Decimal):" << calculatedCRC;

        if (receivedCRC == calculatedCRC) {
            qDebug() << "CRC Matched!";
        } else {
            qDebug() << "!!!!!!!CRC Mismatch!!!!!!!";
            return;
        }

        // Update the set current and cutoff voltage if changed
        float setCurrent = parts[SetCurret].toFloat() / 1000;
        float setCutoffVoltage = parts[SetCutofffVoltage].toFloat() / 1000;

        if (prevCurrent != parts[SetCurret]) {
            ui->setCurrent->setText(QString::number(setCurrent, 'f', 3));
            prevCurrent = parts[SetCurret];
        }
        if (prevCutoff != parts[SetCutofffVoltage]) {
            ui->cutoffVoltage->setText(QString::number(setCutoffVoltage, 'f', 3));
            prevCutoff = parts[SetCutofffVoltage];
        }

        // Add reading to measurements and update UI
        float measuredVoltage = parts[MeasuredVoltage].toFloat() / 1000;
        float measuredCurrent = parts[MeasuredCurrent].toFloat() / 1000;
        int measuredTemperature = parts[MeasuredTemperature].toInt();
        float elapsedTime = timer.elapsed() / 1000.0;

        measurements.addReading(measuredVoltage, measuredCurrent, elapsedTime, measuredTemperature);

        ui->measuredVoltage->setText(QString::number(measuredVoltage, 'f', 3));
        ui->measuredCurrent->setText(QString::number(measuredCurrent, 'f', 3));

        if(parts[IsLoadOn]=="1"){
            ui->load_on_offfButton->setText(QObject::tr("Load ON"));
            ui->load_on_offfButton->setStyleSheet("* { background-color: rgb(0,255,0) }");
        }
        else {
            ui->load_on_offfButton->setStyleSheet("* { background-color: rgb(255,0,0) }");
            ui->load_on_offfButton->setText(QObject::tr("Load OFF"));
        }
    }

    if(measurements.nominalCapacity_mAh != 0){
        int capacityPercentage = 100 * measurements.capacity_mAh / measurements.nominalCapacity_mAh;
        ui->BatCapacityBar->setValue(capacityPercentage);
    }
}

void Electronic_load_app::on_load_on_offfButton_clicked(){
    if(serialPort != nullptr && serialPort->isOpen()){
        serialPort->write("o");
    }
}

void Electronic_load_app::on_resetMeas_clicked(){
    measurements.resetMeasurements();
    if(serialPort->isOpen()){
        serialPort->write("r");
        serialPort->write(ui->setCurrent->text().toLatin1()+ char(10));
    }
    timer.restart();
}

void Electronic_load_app::on_SaveButton_clicked(){
    QString selectedFilter = ui->cmbSaveAs->currentText();
    QString filter = selectedFilter == "csv" ? "CSV files (*.csv)" : "JPG files (*.jpg)";
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save file"), QDir::currentPath(), filter, &filter);

    if (fileName.isEmpty()) {
        return; // Do nothing if Cancel was pressed
    }

    if (selectedFilter == "jpg") {
        // Check if the file name ends with ".jpg". If not, append it.
        if (!fileName.endsWith(".jpg", Qt::CaseInsensitive)) {
            fileName += ".jpg";
        }
        QFile file(fileName);

        if (!file.open(QIODevice::WriteOnly))
        {
           qDebug() << file.errorString();
        } else {
           ui->VoltageAndCurrentPlot->saveJpg(fileName);
        }
    } else if (selectedFilter == "csv") {
        // Check if the file name ends with ".csv". If not, append it.
        if (!fileName.endsWith(".csv", Qt::CaseInsensitive)) {
            fileName += ".csv";
        }
        QFile file(fileName);

        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
           qDebug() << file.errorString();
        } else {
            QTextStream out(&file);
            out << "Time [s],Voltage [V],Current [A],Temperature [C]\n";
            for (const auto &reading : measurements.readings) {
                out << reading.time_s << "," << reading.voltage_V << "," << reading.current_A << "," << reading.temperature_C << "\n";
            }
        }
    }
}
/*
void Electronic_load_app::on_setCurrent_editingFinished(){
    if(COMPORT != nullptr && COMPORT->isOpen()){
        COMPORT->write("a");
        COMPORT->write(ui->setCurrent->text().toLatin1()+ char(10));
    }
}*/

void Electronic_load_app::on_setCurrent_editingFinished(){
    if (serialPort != nullptr && serialPort->isOpen()) {
        QString currentText = ui->setCurrent->text();
        if (!currentText.isEmpty()) {
            try {
                QByteArray currentData = currentText.toLatin1() + char(10);
                serialPort->write("a");
                serialPort->write(currentData);
                qDebug() << "Set current value sent: " << currentText;
            } catch (const std::exception &e) {
                qDebug() << "Exception occurred while writing to COMPORT: " << e.what();
            } catch (...) {
                qDebug() << "Unknown exception occurred while writing to COMPORT.";
            }
        } else {
            qDebug() << "Current text is empty.";
        }
    } else {
        qDebug() << "COMPORT is not initialized or not open.";
    }
}
/*
void Electronic_load_app::on_cutoffVoltage_editingFinished(){
    if(COMPORT->isOpen()){
        COMPORT->write("c");
        COMPORT->write(ui->cutoffVoltage->text().toLatin1()+ char(10));
    }
}*/

void Electronic_load_app::on_cutoffVoltage_editingFinished(){
    if (serialPort != nullptr && serialPort->isOpen()) {
        QString voltageText = ui->cutoffVoltage->text();
        if (!voltageText.isEmpty()) {
            try {
                QByteArray voltageData = voltageText.toLatin1() + char(10);
                serialPort->write("c");
                serialPort->write(voltageData);
                qDebug() << "Set cutoff voltage value sent: " << voltageText;
            } catch (const std::exception &e) {
                qDebug() << "Exception occurred while writing to COMPORT: " << e.what();
            } catch (...) {
                qDebug() << "Unknown exception occurred while writing to COMPORT.";
            }
        } else {
            qDebug() << "Cutoff voltage text is empty.";
        }
    } else {
        qDebug() << "COMPORT is not initialized or not open.";
    }
}

void Electronic_load_app::on_NominalCapacity_editingFinished(){
    measurements.nominalCapacity_mAh = ui->NominalCapacity->text().toInt();
}

void Electronic_load_app::on_portOpenButton_clicked(){
    if(serialPort != nullptr){
        serialPort->close();
        delete serialPort;
    }

    serialPort = new QSerialPort();
    serialPort->setPortName(ui->cmbPorts->currentText());
    serialPort->setBaudRate(QSerialPort::BaudRate::Baud9600);
    serialPort->setParity(QSerialPort::Parity::NoParity);
    serialPort->setDataBits(QSerialPort::DataBits::Data8);
    serialPort->setStopBits(QSerialPort::StopBits::OneStop);
    serialPort->setFlowControl(QSerialPort::FlowControl::NoFlowControl);
    serialPort->open(QIODevice::ReadWrite);

    if (serialPort->isOpen()) {
        QMessageBox msgBox;
        msgBox.setText("Port opened successfully");
        msgBox.setStyleSheet("QLabel{color: green;}"); // Change text color to green
        msgBox.setWindowTitle("Result");
        msgBox.exec();
    } else {
        QMessageBox msgBox;
        msgBox.setText(QObject::tr("Unable to open specified port"));
        msgBox.setStyleSheet("QLabel{color: red;}"); // Change text color to red
        msgBox.setWindowTitle(QObject::tr("Port error"));
        msgBox.exec();
    }

    connect(serialPort, SIGNAL(readyRead()), this, SLOT(readData()));
}

void Electronic_load_app::on_actionPL_triggered(){
    qApp->installTranslator(&translator_PL);
    ui->VoltageAndCurrentPlot->xAxis->setLabel(QObject::tr("Time [s]"));
    ui->VoltageAndCurrentPlot->yAxis->setLabel(QObject::tr("Voltage [V]"));
    ui->VoltageAndCurrentPlot->yAxis2->setLabel(QObject::tr("Current [A]"));
    ui->VoltageAndCurrentPlot->replot();
    ui->VoltageAndCurrentPlot->update();
}

void Electronic_load_app::on_actionEN_triggered(){
    qApp->removeTranslator(&translator_PL);
    qApp->removeTranslator(&translator_DE);
    ui->VoltageAndCurrentPlot->xAxis->setLabel(QObject::tr("Time [s]"));
    ui->VoltageAndCurrentPlot->yAxis->setLabel(QObject::tr("Voltage [V]"));
    ui->VoltageAndCurrentPlot->yAxis2->setLabel(QObject::tr("Current [A]"));
    ui->VoltageAndCurrentPlot->replot();
    ui->VoltageAndCurrentPlot->update();
}

void Electronic_load_app::on_actionDE_triggered(){
    qApp->installTranslator(&translator_DE);
    ui->VoltageAndCurrentPlot->xAxis->setLabel(QObject::tr("Time [s]"));
    ui->VoltageAndCurrentPlot->yAxis->setLabel(QObject::tr("Voltage [V]"));
    ui->VoltageAndCurrentPlot->yAxis2->setLabel(QObject::tr("Current [A]"));
    ui->VoltageAndCurrentPlot->replot();
    ui->VoltageAndCurrentPlot->update();
}
