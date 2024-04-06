#include "electronic_load_app.h"
#include "ui_electronic_load_app.h"

Electronic_load_app::Electronic_load_app(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Electronic_load_app)
{
    ui->setupUi(this);
    connect(Electronic_load_app::findChild<QPushButton *>("Button_off"), SIGNAL(released()), this, SLOT(send_off_cmd()));

    COMPORT = new QSerialPort();
    COMPORT->setPortName("COM5");
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
}

Electronic_load_app::~Electronic_load_app()
{
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
            qDebug() << "data from serial: " << Data_From_Serial_Port;
            QStringList parts = Data_From_Serial_Port.split(";");
            if (parts.size() != 3) {
                qDebug() << "Invalid data format. Expected 3 parts separated by semicolons.";
            }else{
                ui->measuredVoltage->setText(parts[0]);
                ui->measuredCurrent->setText(parts[2]);
            }
            Data_From_Serial_Port = "";
            Is_data_received = false;
        }
    }
}


void Electronic_load_app::on_load_on_offfButton_clicked(){
    if(COMPORT->isOpen()){
        COMPORT->write("o");
        if(IsOutputOn){
            ui->load_on_offfButton->setStyleSheet("* { background-color: rgb(255,0,0) }");
            ui->load_on_offfButton->setText("Load OFF");
        }
        else{
            ui->load_on_offfButton->setText("Load ON");
            ui->load_on_offfButton->setStyleSheet("* { background-color: rgb(0,255,0) }");
        }
        /*
        if(ui->load_on_offfButton->text() == "Load ON"){
            ui->load_on_offfButton->setStyleSheet("* { background-color: rgb(255,0,0) }");
            ui->load_on_offfButton->setText("Load OFF");
        }
        else{
            ui->load_on_offfButton->setText("Load ON");
            ui->load_on_offfButton->setStyleSheet("* { background-color: rgb(0,255,0) }");
        }*/
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

void Electronic_load_app::requestData(RequestedData data){
    if(COMPORT->isOpen()){
        switch(data){
        case DischargingCurrent:
            COMPORT->write("d");
            break;
        case CutoffVoltage:
            COMPORT->write("c");
            break;
        case OutputState:
            COMPORT->write("o");
        default:
            break;
        }
    }
}
