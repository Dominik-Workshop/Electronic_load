#include "electronic_load_app.h"
#include "ui_electronic_load_app.h"
#include <QDoubleValidator>

Electronic_load_app::Electronic_load_app(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Electronic_load_app)
{
    ui->setupUi(this);
    connect(Electronic_load_app::findChild<QPushButton *>("Button_off"), SIGNAL(released()), this, SLOT(send_off_cmd()));

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
    }

    connect(COMPORT, SIGNAL(readyRead()), this, SLOT(Read_Data()));

    ui->capacity_mAh->setText(QString::number(capacitymAh));
    ui->capacity_Wh->setText(QString::number(capacityWh));

}

Electronic_load_app::~Electronic_load_app(){
    delete ui;
}

void Electronic_load_app::Read_Data(){
    if(COMPORT->isOpen()){
        while(COMPORT->bytesAvailable()){
            Data_From_Serial_Port += COMPORT->readAll();
            if(Data_From_Serial_Port.at(Data_From_Serial_Port.length()-1) == char(10))
                Is_data_received = true;
        }
        if(Is_data_received){

            //qDebug() << "data from serial: " << Data_From_Serial_Port;
            processReacivedData();
            Data_From_Serial_Port = "";
            Is_data_received = false;
        }
    }
}


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

void Electronic_load_app::processReacivedData(){
    QStringList parts = Data_From_Serial_Port.split(";");

        // Extract the received CRC checksum from the last part of the received data
        uint32_t receivedCRC = parts.back().toUInt();

        // Remove the CRC part from the received data
        parts.removeLast();

        // Remove the 'm' character from the beginning of the data
        QByteArray data = parts.join(";").toUtf8();
        data.remove(0, 1);

        // Ensure there are no leading semicolons after removing 'm'
        if (data.startsWith(';')) {
            data.remove(0, 1);
        }

        // Print the received data and extracted CRC checksum for debugging
        qDebug() << "Received data: " << data;
        qDebug() << "Received CRC: " << receivedCRC;

        // Calculate CRC checksum over the received data
        qDebug() << (data.constData());
        uint32_t calculatedCRC = calculateCRC(reinterpret_cast<const uint8_t*>(data.constData()), data.size());

        // Print the calculated CRC for debugging
        qDebug() << "Calculated CRC: " << calculatedCRC;

        // Compare received CRC with calculated CRC
        if (receivedCRC == calculatedCRC) {
            qDebug() << "CRC checksum matches. Data integrity verified.";
            // Now you can proceed with processing the received data
        } else {
            qDebug() << "CRC checksum does not match. Data may be corrupted.";
        }




    if(parts[Command] == 'm'){
        if (parts.size() != 9) {
            qDebug() << "Invalid data format. Expected 7 parts separated by semicolons.";
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
        measurements.addReadings(parts[MeasuredVoltage].toFloat(), parts[MeasuredCurrent].toFloat());
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

}

void Electronic_load_app::on_load_on_offfButton_clicked(){
    if(COMPORT->isOpen()){
        COMPORT->write("o");
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

void Electronic_load_app::on_resetMeas_clicked(){
    measurements.resetMeasurements();
    if(COMPORT->isOpen()){
        COMPORT->write("r");
        COMPORT->write(ui->setCurrent->text().toLatin1()+ char(10));
    }
}

void Electronic_load_app::on_SaveButton_clicked(){
    for(int i =0; i < measurements.numberOfReadings; ++i){
           qDebug()<< measurements.voltageReadings[i] << ", " << measurements.currentReadings[i];
    }
}