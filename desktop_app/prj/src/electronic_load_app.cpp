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
    translator.load(":/polish.qm");
    // Fill combo box with available ports at startup
    updateAvailablePorts();

    // Set up timer to periodically check for available ports
    portCheckTimer = new QTimer(this);
    connect(portCheckTimer, &QTimer::timeout, this, &Electronic_load_app::updateAvailablePorts);
    portCheckTimer->start(1000); // Check every second

    /*
    foreach (auto &port, QSerialPortInfo::availablePorts()) {
        qDebug() << port.portName();
        ui->cmbPorts->addItem(port.portName());
    }

    COMPORT = new QSerialPort();
    COMPORT->setPortName("ttyUSB0");
    COMPORT->setBaudRate(QSerialPort::BaudRate::Baud9600);
    COMPORT->setParity(QSerialPort::Parity::NoParity);
    COMPORT->setDataBits(QSerialPort::DataBits::Data8);
    COMPORT->setStopBits(QSerialPort::StopBits::OneStop);
    COMPORT->setFlowControl(QSerialPort::FlowControl::NoFlowControl);
    COMPORT->open(QIODevice::ReadWrite);

    if(COMPORT->isOpen())
        qDebug() << "Serial connnected";
    else{
        qDebug() << "Serial failed to connnect";
        qDebug() << COMPORT->error();
    }*/

    //connect(COMPORT, SIGNAL(readyRead()), this, SLOT(readData()));

    ui->cmbSaveAs->addItem("jpg");
    ui->cmbSaveAs->addItem("csv");

    ui->capacity_mAh->setText(QString::number(measurements.mAhCapacity, 'f', 3));
    ui->capacity_Wh->setText(QString::number(measurements.WhCapacity, 'f', 3));

    ui->VoltagePlot->xAxis->setLabel("Time [s]");
    ui->VoltagePlot->yAxis->setLabel("Voltage [V]");
    ui->VoltagePlot->yAxis2->setLabel("Current [A]");
    ui->VoltagePlot->yAxis2->setVisible(true);
    ui->VoltagePlot->legend->setVisible(true);
    //ui->VoltagePlot->setInteraction(QCP::iRangeDrag, true);
    //ui->VoltagePlot->setInteraction(QCP::iRangeZoom, true);
    timer.start();

}

Electronic_load_app::~Electronic_load_app(){
    delete ui;
    if(COMPORT != nullptr){
        COMPORT->close();
        delete COMPORT;
    }
}


void Electronic_load_app::changeEvent(QEvent *event){
    if(event->type() == QEvent::LanguageChange){
        ui->retranslateUi(this);
    }
    //this->setWindowTitle("multilang widget");
    QWidget::changeEvent(event);
}

void Electronic_load_app::updateAvailablePorts()
{
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

/**
 * @brief Read data from serial port
 */
void Electronic_load_app::readData(){
    if(COMPORT->isOpen()){
        while(COMPORT->bytesAvailable()){
            Data_From_Serial_Port += COMPORT->readAll();
            if(Data_From_Serial_Port.at(Data_From_Serial_Port.length()-1) == char(10))
                Is_data_received = true;
        }
        if(Is_data_received){

            qDebug() << "data from serial: " << Data_From_Serial_Port;
            processReceivedData();
            measurements.calculateCapacity();
            ui->capacity_mAh->setText(QString::number(measurements.mAhCapacity, 'f', 3));
            ui->capacity_Wh->setText(QString::number(measurements.WhCapacity, 'f', 3));
            Data_From_Serial_Port = "";
            Is_data_received = false;


            QVector<double> x(measurements.numberOfReadings), y(measurements.numberOfReadings);
            for (int i = 0; i < measurements.numberOfReadings; ++i) {
                x[i] = measurements.time[i]; // Assuming x-axis is index based
                y[i] = measurements.voltageReadings[i];
            }
            QVector<double> x2(measurements.numberOfReadings), y2(measurements.numberOfReadings);
            for (int i = 0; i < measurements.numberOfReadings; ++i) {
                x2[i] = measurements.time[i]; // Assuming x-axis is index based
                y2[i] = measurements.currentReadings[i];
            }


            // Clear existing graph and update with new data
            ui->VoltagePlot->clearGraphs();
            ui->VoltagePlot->addGraph(ui->VoltagePlot->xAxis, ui->VoltagePlot->yAxis);
            ui->VoltagePlot->graph(0)->setPen(QPen(QColor(255, 100, 0)));
            ui->VoltagePlot->graph(0)->setData(x, y);
            ui->VoltagePlot->graph(0)->setName("Voltage");
            ui->VoltagePlot->addGraph(ui->VoltagePlot->xAxis2, ui->VoltagePlot->yAxis2);
            ui->VoltagePlot->graph(1)->setPen(QPen(QColor(0, 100, 255)));
            ui->VoltagePlot->graph(1)->setData(x2, y2);
            ui->VoltagePlot->graph(1)->setName("Current");

            ui->VoltagePlot->rescaleAxes();
            ui->VoltagePlot->replot();
            ui->VoltagePlot->update();

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
    QStringList parts = Data_From_Serial_Port.split(";");
    QStringList parts2 = Data_From_Serial_Port.split(";");

    // Extract the received CRC checksum from the last part of the received data
    uint32_t receivedCRC = parts2.back().toUInt();

    // Remove the CRC part from the received data
    parts2.removeLast();

    // Remove the 'm' character from the beginning of the data
    QByteArray data = parts2.join(";").toUtf8();
    data.remove(0, 1);

    // Ensure there are no leading semicolons after removing 'm'
    if (data.startsWith(';')) {
        data.remove(0, 1);
    }

    // Print the received data and extracted CRC checksum for debugging
    //qDebug() << "Received data: " << data;
    //qDebug() << "Received CRC: " << receivedCRC;

    // Calculate CRC checksum over the received data
    //qDebug() << (data.constData());
    uint32_t calculatedCRC = calculateCRC(reinterpret_cast<const uint8_t*>(data.constData()), data.size());

    // Print the calculated CRC for debugging
    //qDebug() << "Calculated CRC: " << calculatedCRC;

    // Compare received CRC with calculated CRC
    if (receivedCRC == calculatedCRC) {
        qDebug() << "CRC checksum matches. Data integrity verified.";
        // Now you can proceed with processing the received data
    } else {
        qDebug() << "CRC checksum does not match. Data may be corrupted.";
    }




    if(parts[Command] == 'm'){
        if (parts.size() != 9) {
            qDebug() << "Invalid data format. Expected 9 parts separated by semicolons.";
            return;
        }
        if(prevCurrent != parts[SetCurret]){
            ui->setCurrent->setText(parts[SetCurret]);
            prevCurrent = QString(parts[SetCurret]);
        }
        if(prevCutoff != parts[SetCutofffVoltage]){
            ui->cutoffVoltage->setText(parts[SetCutofffVoltage]);
            prevCutoff = QString(parts[SetCutofffVoltage]);
        }

        measurements.addReadings(parts[MeasuredVoltage].toFloat(), parts[MeasuredCurrent].toFloat(), timer.elapsed()/1000.0);
        ui->measuredVoltage->setText(parts[MeasuredVoltage]);
        ui->measuredCurrent->setText(parts[MeasuredCurrent]);

        if(parts[IsLoadOn]=="1"){
            ui->load_on_offfButton->setText("Load ON");
            ui->load_on_offfButton->setStyleSheet("* { background-color: rgb(0,255,0) }");
        }
        else {
            ui->load_on_offfButton->setStyleSheet("* { background-color: rgb(255,0,0) }");
            ui->load_on_offfButton->setText("Load OFF");
        }
    }
    if(measurements.mAhNominalCapacity != 0){
        ui->BatCapacityBar->setValue(100 * measurements.mAhCapacity/measurements.mAhNominalCapacity);
    }


}

void Electronic_load_app::on_load_on_offfButton_clicked(){
    if(COMPORT->isOpen()){
        COMPORT->write("o");
    }
}

void Electronic_load_app::on_resetMeas_clicked(){
    measurements.resetMeasurements();
    if(COMPORT->isOpen()){
        COMPORT->write("r");
        COMPORT->write(ui->setCurrent->text().toLatin1()+ char(10));
    }
    timer.restart();
}

void Electronic_load_app::on_SaveButton_clicked(){
    //for(int i = 0; i < measurements.numberOfReadings; ++i){
    //      qDebug()<< measurements.voltageReadings[i] << ", " << measurements.currentReadings[i];
    //}
    QString filter = "JPG files (*.jpg)";
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save file"), QDir::currentPath(), filter, &filter);
    // Check if the file name ends with ".png". If not, append it.
    if (!fileName.endsWith(".jpg", Qt::CaseInsensitive)) {
        fileName += ".jpg";
    }
    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly))
    {
       qDebug() << file.errorString();
    } else {
       ui->VoltagePlot->saveJpg(fileName);
    }
}

void Electronic_load_app::on_setCurrent_editingFinished(){
    if(COMPORT->isOpen()){
        COMPORT->write("a");
        COMPORT->write(ui->setCurrent->text().toLatin1()+ char(10));
    }
}

void Electronic_load_app::on_cutoffVoltage_editingFinished(){
    if(COMPORT->isOpen()){
        COMPORT->write("c");
        COMPORT->write(ui->cutoffVoltage->text().toLatin1()+ char(10));
    }
}

void Electronic_load_app::on_NominalCapacity_editingFinished(){
    measurements.mAhNominalCapacity = ui->NominalCapacity->text().toInt();
}

void Electronic_load_app::on_capacity_mAh_editingFinished()
{
    measurements.mAhCapacity = ui->capacity_mAh->text().toInt();
}

void Electronic_load_app::on_portOpenButton_clicked(){
    if(COMPORT != nullptr){
        COMPORT->close();
        delete COMPORT;
    }

    COMPORT = new QSerialPort();
    COMPORT->setPortName(ui->cmbPorts->currentText());
    COMPORT->setBaudRate(QSerialPort::BaudRate::Baud9600);
    COMPORT->setParity(QSerialPort::Parity::NoParity);
    COMPORT->setDataBits(QSerialPort::DataBits::Data8);
    COMPORT->setStopBits(QSerialPort::StopBits::OneStop);
    COMPORT->setFlowControl(QSerialPort::FlowControl::NoFlowControl);
    COMPORT->open(QIODevice::ReadWrite);

    if (COMPORT->isOpen()) {
        QMessageBox msgBox;
        msgBox.setText("Port opened successfully");
        msgBox.setStyleSheet("QLabel{color: green;}"); // Change text color to green
        msgBox.setWindowTitle("Result");
        msgBox.exec();
    } else {
        QMessageBox msgBox;
        msgBox.setText("Unable to open specified port");
        msgBox.setStyleSheet("QLabel{color: red;}"); // Change text color to red
        msgBox.setWindowTitle("Port error");
        msgBox.exec();
    }

    connect(COMPORT, SIGNAL(readyRead()), this, SLOT(readData()));
}

void Electronic_load_app::on_Settings_clicked()
{
    qApp->installTranslator(&translator);
}
