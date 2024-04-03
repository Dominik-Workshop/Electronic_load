#ifndef ELECTRONIC_LOAD_APP_H
#define ELECTRONIC_LOAD_APP_H

#include <QMainWindow>
#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <QtSerialPort/QSerialPort>

QT_BEGIN_NAMESPACE
namespace Ui {
class Electronic_load_app;
}
QT_END_NAMESPACE

class Electronic_load_app : public QMainWindow
{
    Q_OBJECT

public:
    Electronic_load_app(QWidget *parent = nullptr);
    ~Electronic_load_app();

private slots:
    void Read_Data();

    void on_load_on_offfButton_clicked();

    void on_setCurrent_editingFinished();

private:
    Ui::Electronic_load_app *ui;
    QSerialPort* COMPORT;
    QString Data_From_Serial_Port;
    bool Is_data_received = false;
};
#endif // ELECTRONIC_LOAD_APP_H
