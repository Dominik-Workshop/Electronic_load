#ifndef ELECTRONIC_LOAD_APP_H
#define ELECTRONIC_LOAD_APP_H

#include <QMainWindow>
#include <QDebug>
#include <QString>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QElapsedTimer>
#include <QTimer>
#include <QTranslator>

#include "measurements.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Electronic_load_app;
}
QT_END_NAMESPACE

/**
 * @brief The RXdata enum used to idex specific data from received data array
 */
enum RXdata{
    Command,
    MeasuredVoltage,
    MeasuredCurrent,
    MeasuredTemperature,
    SetCutofffVoltage,
    SetCurret,
    IsLoadOn,
    Time,
    CRC
};

class Electronic_load_app : public QMainWindow{
    Q_OBJECT

public:
    Electronic_load_app(QWidget *parent = nullptr);
    ~Electronic_load_app();
    QTranslator translator_PL;
    QTranslator translator_DE;
    void processReceivedData();
    void plotVoltageAndCurrent();
    void readAllDataFromSerialPort();
    void processData();
    void updateCapacityUI();
    void clearDataFromSerialPort();
    void checkAndPlotVoltageAndCurrent();


private slots:
    void readData();
    void updateAvailablePorts();

    void on_setCurrent_editingFinished();
    void on_cutoffVoltage_editingFinished();
    void on_NominalCapacity_editingFinished();

    void on_load_on_offfButton_clicked();
    void on_resetMeas_clicked();
    void on_SaveButton_clicked();
    void on_portOpenButton_clicked();

    void on_actionPL_triggered();
    void on_actionEN_triggered();
    void on_actionDE_triggered();

    void on_actionAbout_triggered();

    void on_setCurrent_textEdited(const QString &arg1);
    void on_cutoffVoltage_textEdited(const QString &arg1);

private:
    Ui::Electronic_load_app *ui;
    QSerialPort* serialPort = nullptr;
    QString dataFromSerialPort;
    bool isDataFromSerialPortReceived = false;

    QElapsedTimer elapsedTimer;
    QTimer *portCheckTimer;
    QStringList currentPorts;

    float setCurrent;
    float setCutoffVoltage;
    bool setCurrentEdited_flag = false;
    bool setCutoffVoltageEdited_flag = false;

    Measurements measurements;
    bool isLoadOn;

    // QWidget interface
protected:
    void changeEvent(QEvent *);
};
#endif // ELECTRONIC_LOAD_APP_H
