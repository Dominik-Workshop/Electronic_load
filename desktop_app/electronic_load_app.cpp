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

Electronic_load_app::~Electronic_load_app()
{
    delete ui;
}

void Electronic_load_app::Read_Data(){

    ui->capacity_mAh->setText(QString::number(capacitymAh));
    ui->capacity_Wh->setText(QString::number(capacityWh));
    capacityWh = 0.001;
    capacitymAh = 0.002;
    if(COMPORT->isOpen()){
        while(COMPORT->bytesAvailable()){
            Data_From_Serial_Port += COMPORT->readAll();
            if(Data_From_Serial_Port.at(Data_From_Serial_Port.length()-1) == char(10))
                Is_data_received = true;
        }
        if(Is_data_received){
            //qDebug() << "data from serial: " << Data_From_Serial_Port;
            QStringList parts = Data_From_Serial_Port.split(";");
            if(parts[0] == 'm'){
                if (parts.size() != 7) {
                    qDebug() << "Invalid data format. Expected 7 parts separated by semicolons.";
                }else{
                    if(prevCurrent != parts[4]){
                        qDebug() << prevCurrent << parts[4];
                        ui->setCurrent->setText(parts[4]);
                        prevCurrent = QString(parts[4]);
                    }
                    if(prevCutoff != parts[5]){
                        qDebug() << prevCutoff << parts[5];
                        ui->cutoffVoltage->setText(parts[5]);
                        prevCutoff = QString(parts[5]);
                    }

                    ui->measuredVoltage->setText(parts[1]);
                    ui->measuredCurrent->setText(parts[3]);

                    if(parts[6]=="1\r\n"){
                        ui->load_on_offfButton->setText("Load ON");
                        ui->load_on_offfButton->setStyleSheet("* { background-color: rgb(0,255,0) }");
                    }
                    else {
                        ui->load_on_offfButton->setStyleSheet("* { background-color: rgb(255,0,0) }");
                        ui->load_on_offfButton->setText("Load OFF");
                    }
                }
            }
            if(parts[0] == 'd'){
                if (parts.size() != 2) {
                    qDebug() << "Invalid data format. Expected 2 parts separated by semicolons.";
                }else{
                    ui->setCurrent->setText(parts[1]);
                }
            }
            if(parts[0] == 'q'){
                if (parts.size() != 2) {
                    qDebug() << "Invalid data format. Expected 2 parts separated by semicolons.";
                }else{
                    ui->cutoffVoltage->setText(parts[1]);
                }
            }

            Data_From_Serial_Port = "";
            Is_data_received = false;
        }
    }
}


void Electronic_load_app::on_load_on_offfButton_clicked(){
    if(COMPORT->isOpen()){
        COMPORT->write("o");

    }
}


void Electronic_load_app::on_setCurrent_editingFinished(){
    qDebug() << "ell";
    if(COMPORT->isOpen()){
        COMPORT->write("a");
        COMPORT->write(ui->setCurrent->text().toLatin1()+ char(10));
    }
}


void Electronic_load_app::requestData(RequestedData data){


}


void Electronic_load_app::on_cutoffVoltage_editingFinished(){
    if(COMPORT->isOpen()){
        COMPORT->write("c");
        COMPORT->write(ui->cutoffVoltage->text().toLatin1()+ char(10));
    }
}



void Electronic_load_app::on_resetMeas_clicked()
{
    capacityWh = 0;
    capacitymAh = 0;
}
