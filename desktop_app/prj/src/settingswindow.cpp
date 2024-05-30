#include "settingswindow.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(QWidget *parent, QSerialPort *port) :
    QDialog(parent),
    ui(new Ui::SettingsWindow),
    serialPort(port)  // Initialize the serial port member
{
    ui->setupUi(this);

    // Fill combo box with available ports at startup
    updateAvailablePorts();

    // Set up timer to periodically check for available ports
    portCheckTimer = new QTimer(this);
    connect(portCheckTimer, &QTimer::timeout, this, &SettingsWindow::updateAvailablePorts);
    portCheckTimer->start(1000); // Check every second

    ui->cmbLanguage->addItem("ENG");
    ui->cmbLanguage->addItem("PL");
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::updateAvailablePorts(){
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

void SettingsWindow::on_portOpenButton_clicked(){
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
        msgBox.setStyleSheet("QLabel{color: green;}");
        msgBox.setWindowTitle("Result");
        msgBox.exec();
        emit dataReadyToRead();
    } else {
        QMessageBox msgBox;
        msgBox.setText(QObject::tr("Unable to open specified port"));
        msgBox.setStyleSheet("QLabel{color: red;}");
        msgBox.setWindowTitle(QObject::tr("Port error"));
        msgBox.exec();
    }

    //connect(serialPort, SIGNAL(readyRead()), this, SLOT(&Electronic_load_app::readData()));
}
