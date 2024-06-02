#include "electronic_load_app.h"
#include "ui_electronic_load_app.h"

#include <QFileDialog>
#include <QMessageBox>

Electronic_load_app::Electronic_load_app(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Electronic_load_app)
{
    ui->setupUi(this);

    this->setWindowIcon(QIcon(":/icon.ico"));
    translator_PL.load(":/lang/polish.qm");
    translator_DE.load(":/lang/german.qm");

    // Max two digits before decimal place, max three digits after decimal place, positive
    QRegExp rx("^\\d{0,2}(\\.\\d{1,3})?$");
    QRegExpValidator *rxv = new QRegExpValidator(rx, this);
    ui->setCurrent->setValidator(rxv);
    ui->cutoffVoltage->setValidator(rxv);

    updateAvailablePorts();

    // Set up timer to periodically check for available ports
    portCheckTimer = new QTimer(this);
    connect(portCheckTimer, &QTimer::timeout, this, &Electronic_load_app::updateAvailablePorts);
    portCheckTimer->start(1000); // Check every second

    ui->cmbSaveAs->addItem("jpg");
    ui->cmbSaveAs->addItem("csv");

    updateCapacityUI();

    ui->VoltageAndCurrentPlot->xAxis->setLabel(QObject::tr("Time [s]"));
    ui->VoltageAndCurrentPlot->yAxis->setLabel(QObject::tr("Voltage [V]"));
    ui->VoltageAndCurrentPlot->yAxis2->setLabel(QObject::tr("Current [A]"));
    ui->VoltageAndCurrentPlot->yAxis2->setVisible(true);

    elapsedTimer.start();
}

Electronic_load_app::~Electronic_load_app(){
    delete ui;
    if(serialPort != nullptr){
        serialPort->close();
        delete serialPort;
    }
}

/**
 * @brief Checks available ports and fills the combo box with them
 */
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

/**
 * @brief Read data from serial port
 */
void Electronic_load_app::readData(){
    if(serialPort->isOpen()){
        readAllDataFromSerialPort();
        if(isDataFromSerialPortReceived){
            processData();
            updateCapacityUI();
            clearDataFromSerialPort();
            checkAndPlotVoltageAndCurrent();
        }
    }
}

void Electronic_load_app::readAllDataFromSerialPort() {
    while(serialPort->bytesAvailable()){
        dataFromSerialPort += serialPort->readAll();
        if(dataFromSerialPort.at(dataFromSerialPort.length()-1) == char(10))
            isDataFromSerialPortReceived = true;
    }
}

void Electronic_load_app::processData() {
    //qDebug() << "data from serial: " << dataFromSerialPort;
    processReceivedData();
    measurements.calculateCapacity();
}

void Electronic_load_app::updateCapacityUI() {
    // Update UI with calculated capacity
    ui->capacity_mAh->setText(QString::number(measurements.capacity_mAh, 'f', 3));
    ui->capacity_Wh->setText(QString::number(measurements.capacity_Wh, 'f', 3));
}

void Electronic_load_app::clearDataFromSerialPort() {
    // Clear data buffer and reset flag
    dataFromSerialPort = "";
    isDataFromSerialPortReceived = false;
}

void Electronic_load_app::checkAndPlotVoltageAndCurrent() {
    // Check if plotting is required
    if (!ui->actionStop_when_load_off->isChecked() || isLoadOn) {
        plotVoltageAndCurrent();
    }
}

/**
 * @brief calculateCRC
 * @param data
 * @param length
 * @return
 */
uint32_t calculateCRC32(const uint8_t* data, size_t length) {
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
        quint16 receivedVoltage = parts[MeasuredVoltage].toUShort(&ok);
        quint16 receivedCurrent = parts[MeasuredCurrent].toUShort(&ok);
        quint16 receivedTemperature = parts[MeasuredTemperature].toUShort(&ok);
        quint16 receivedCutoffVoltage = parts[SetCutofffVoltage].toUShort(&ok);
        quint16 receivedDischargeCurrent = parts[SetCurret].toUShort(&ok);
        quint16 receivedIsLoadOn = parts[IsLoadOn].toUShort(&ok);
        qint32  receivedTime = parts[Time].toLong(&ok);
        uint32_t receivedCRC = parts[CRC].toUInt(&ok);

        if (!ok) {
            qDebug() << "Error converting received data";
            return;
        }

        // Prepare the data for CRC calculation
        uint8_t message[6 * sizeof(quint16) + sizeof(qint32)];
        size_t index = 0;
        memcpy(&message[index], &receivedVoltage, sizeof(quint16));
        index += sizeof(quint16);
        memcpy(&message[index], &receivedCurrent, sizeof(quint16));
        index += sizeof(quint16);
        memcpy(&message[index], &receivedTemperature, sizeof(quint16));
        index += sizeof(quint16);
        memcpy(&message[index], &receivedCutoffVoltage, sizeof(quint16));
        index += sizeof(quint16);
        memcpy(&message[index], &receivedDischargeCurrent, sizeof(quint16));
        index += sizeof(quint16);
        memcpy(&message[index], &receivedIsLoadOn, sizeof(quint16));
        index += sizeof(quint16);
        memcpy(&message[index], &receivedTime, sizeof(qint32));

        uint32_t calculatedCRC = calculateCRC32(message, sizeof(message));

        //qDebug() << "Received CRC (Decimal):" << receivedCRC;
        //qDebug() << "Calculated CRC (Decimal):" << calculatedCRC;

        if (receivedCRC == calculatedCRC) {
            // qDebug() << "CRC Matched!";
        } else {
            qDebug() << "!!!!!!!CRC Mismatch!!!!!!!";
            return;
        }

        // Update the set current and cutoff voltage if changed
        setCurrent_A = parts[SetCurret].toFloat() / 1000;
        setCutoffVoltage_V = parts[SetCutofffVoltage].toFloat() / 1000;
        //qDebug() << setCurrent << " flag " << setCurrentEdited_flag;
        if(!setCurrentEdited_flag){
            setCurrentEdited_flag = false;
            ui->setCurrent->setText(QString::number(setCurrent_A, 'f', 3));
        }

        if (!setCutoffVoltageEdited_flag) {
            ui->cutoffVoltage->setText(QString::number(setCutoffVoltage_V, 'f', 3));
        }

        // Add reading to measurements and update UI
        float measuredVoltage = parts[MeasuredVoltage].toFloat() / 1000;
        float measuredCurrent = parts[MeasuredCurrent].toFloat() / 1000;
        int measuredTemperature = parts[MeasuredTemperature].toInt();
        float elapsedTime = elapsedTimer.elapsed() / 1000.0;

        measurements.addReading(measuredVoltage, measuredCurrent, elapsedTime, measuredTemperature);

        ui->measuredVoltage->setText(QString::number(measuredVoltage, 'f', 3));
        ui->measuredCurrent->setText(QString::number(measuredCurrent, 'f', 3));

        isLoadOn = (parts[IsLoadOn] == "1");
        if(isLoadOn){
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
        ui->batteryPercentageLabel->setText(QString::number(capacityPercentage) + "%");
        ui->BatCapacityBar->setValue(capacityPercentage);
        ui->BatCapacityBar->update();
    }
}

void Electronic_load_app::on_load_on_offfButton_clicked(){
    if(serialPort != nullptr && serialPort->isOpen()){
        serialPort->write("o");
    }
}

/**
 * @brief Clears measurements
 */
void Electronic_load_app::on_resetMeas_clicked(){
    measurements.resetMeasurements();
    if(serialPort != nullptr && serialPort->isOpen()){
        serialPort->write("r");
        serialPort->write(ui->setCurrent->text().toLatin1()+ char(10));
    }
    if (ui->actionStop_when_load_off->isChecked() && !isLoadOn){
        ui->VoltageAndCurrentPlot->clearGraphs();
        ui->VoltageAndCurrentPlot->replot();
        ui->VoltageAndCurrentPlot->update();
    }
    elapsedTimer.restart();
}

/**
 * @brief Saves the voltage and current plot as jpg or csv
 */
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

void Electronic_load_app::on_setCurrent_editingFinished(){
    if(serialPort != nullptr && serialPort->isOpen()){
        serialPort->write("a");
        serialPort->write(ui->setCurrent->text().toLatin1()+ char(10));
    }
    setCurrentEdited_flag = false;
}

void Electronic_load_app::on_cutoffVoltage_editingFinished(){
    if(serialPort != nullptr && serialPort->isOpen()){
        serialPort->write("c");
        serialPort->write(ui->cutoffVoltage->text().toLatin1()+ char(10));
    }
    setCutoffVoltageEdited_flag = false;
}

void Electronic_load_app::on_setCurrent_textEdited(const QString &arg1){
    Q_UNUSED(arg1);
    setCurrentEdited_flag = true;
}

void Electronic_load_app::on_cutoffVoltage_textEdited(const QString &arg1){
    Q_UNUSED(arg1);
    setCutoffVoltageEdited_flag = true;
}

void Electronic_load_app::on_NominalCapacity_editingFinished(){
    measurements.nominalCapacity_mAh = ui->NominalCapacity->text().toInt();
}

/**
 * @brief Plots voltage and current graphs from measurements class instance
 */
void Electronic_load_app::plotVoltageAndCurrent(){
    unsigned int lenght = measurements.readings.size();
    QVector<double> x(lenght), y1(lenght), y2(lenght);
    for (unsigned long i = 0; i < lenght; ++i) {
        x[i] = measurements.readings[i].time_s;
        y1[i] = measurements.readings[i].voltage_V;
        y2[i] = measurements.readings[i].current_A;
    }

    // Clear existing graph and update with new data
    ui->VoltageAndCurrentPlot->clearGraphs();
    ui->VoltageAndCurrentPlot->legend->setVisible(true);
    ui->VoltageAndCurrentPlot->addGraph(ui->VoltageAndCurrentPlot->xAxis, ui->VoltageAndCurrentPlot->yAxis);
    ui->VoltageAndCurrentPlot->graph(0)->setPen(QPen(QColor(255, 100, 0)));
    ui->VoltageAndCurrentPlot->graph(0)->setData(x, y1);
    ui->VoltageAndCurrentPlot->graph(0)->setName(QObject::tr("Voltage"));
    ui->VoltageAndCurrentPlot->addGraph(ui->VoltageAndCurrentPlot->xAxis2, ui->VoltageAndCurrentPlot->yAxis2);
    ui->VoltageAndCurrentPlot->graph(1)->setPen(QPen(QColor(0, 100, 255)));
    ui->VoltageAndCurrentPlot->graph(1)->setData(x, y2);
    ui->VoltageAndCurrentPlot->graph(1)->setName(QObject::tr("Current"));

    ui->VoltageAndCurrentPlot->rescaleAxes();

    if(ui->actionPlot_from_zero->isChecked()){    // Set axis ranges to start from 0
        ui->VoltageAndCurrentPlot->yAxis->setRange(0, *std::max_element(y1.constBegin(), y1.constEnd()) + 0.1);
        ui->VoltageAndCurrentPlot->yAxis2->setRange(0, *std::max_element(y2.constBegin(), y2.constEnd())+ 0.01);
    }

    ui->VoltageAndCurrentPlot->replot();
    ui->VoltageAndCurrentPlot->update();
}

/**
 * @brief Shows message box containing information about this app
 */
void Electronic_load_app::on_actionAbout_triggered(){
    QMessageBox msgBox;
    msgBox.setText("This app, designed for the "
                   "<a href='https://github.com/Dominik-Workshop/Electronic_load'>Electronic Load (github)</a> "
                   "project, communicates with the electronic load via serial port, reading time, current, "
                   "and voltage during battery discharge. It plots discharge curves, calculates "
                   "battery capacity (mAh and mWh), and allows saving graphs in jpg and csv formats. "
                   "You can also control discharge parameters within the app.<br><br>"
                   "Created by: Dominik Workshop");
    msgBox.setStyleSheet("QLabel{color: black;}");
    msgBox.setWindowTitle(QObject::tr("About"));
    msgBox.exec();
}

void Electronic_load_app::changeEvent(QEvent *event){
    if(event->type() == QEvent::LanguageChange){
        ui->retranslateUi(this);
    }
    QWidget::changeEvent(event);
}

/**
 * @brief Changes app's language to polish
 */
void Electronic_load_app::on_actionPL_triggered(){
    qApp->installTranslator(&translator_PL);
    ui->VoltageAndCurrentPlot->xAxis->setLabel(QObject::tr("Time [s]"));
    ui->VoltageAndCurrentPlot->yAxis->setLabel(QObject::tr("Voltage [V]"));
    ui->VoltageAndCurrentPlot->yAxis2->setLabel(QObject::tr("Current [A]"));
    ui->VoltageAndCurrentPlot->replot();
    ui->VoltageAndCurrentPlot->update();
}

/**
 * @brief Changes app's language to english
 */
void Electronic_load_app::on_actionEN_triggered(){
    qApp->removeTranslator(&translator_PL);
    qApp->removeTranslator(&translator_DE);
    ui->VoltageAndCurrentPlot->xAxis->setLabel(QObject::tr("Time [s]"));
    ui->VoltageAndCurrentPlot->yAxis->setLabel(QObject::tr("Voltage [V]"));
    ui->VoltageAndCurrentPlot->yAxis2->setLabel(QObject::tr("Current [A]"));
    ui->VoltageAndCurrentPlot->replot();
    ui->VoltageAndCurrentPlot->update();
}

/**
 * @brief Changes app's language to german
 */
void Electronic_load_app::on_actionDE_triggered(){
    qApp->installTranslator(&translator_DE);
    ui->VoltageAndCurrentPlot->xAxis->setLabel(QObject::tr("Time [s]"));
    ui->VoltageAndCurrentPlot->yAxis->setLabel(QObject::tr("Voltage [V]"));
    ui->VoltageAndCurrentPlot->yAxis2->setLabel(QObject::tr("Current [A]"));
    ui->VoltageAndCurrentPlot->replot();
    ui->VoltageAndCurrentPlot->update();
}
