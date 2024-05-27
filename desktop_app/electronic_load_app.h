#ifndef ELECTRONIC_LOAD_APP_H
#define ELECTRONIC_LOAD_APP_H

#include <QMainWindow>
#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <QtSerialPort/QSerialPort>
#include <QElapsedTimer>


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

    void processReceivedData();

private slots:
    void readData();

    void on_setCurrent_editingFinished();
    void on_cutoffVoltage_editingFinished();
    void on_load_on_offfButton_clicked();
    void on_resetMeas_clicked();
    void on_SaveButton_clicked();

    void on_NominalCapacity_editingFinished();

    void on_capacity_mAh_editingFinished();

private:
    Ui::Electronic_load_app *ui;
    QSerialPort* COMPORT;
    QString Data_From_Serial_Port;
    bool Is_data_received = false;
    QString prevCurrent;
    QString prevCutoff;
    QElapsedTimer timer;

    Measurements measurements;
};
#endif // ELECTRONIC_LOAD_APP_H
