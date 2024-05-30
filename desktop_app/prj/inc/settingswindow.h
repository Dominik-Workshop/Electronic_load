#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QDialog>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QMessageBox>
#include <QTimer>

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QDialog
{
    Q_OBJECT

signals:
    void dataReadyToRead();

public:
    explicit SettingsWindow(QWidget *parent = nullptr, QSerialPort *port = nullptr);
    ~SettingsWindow();

private slots:
    void on_portOpenButton_clicked();
    void updateAvailablePorts();

private:
    Ui::SettingsWindow *ui;
    QSerialPort *serialPort;
    QStringList currentPorts;
    QTimer *portCheckTimer;
};

#endif // SETTINGSWINDOW_H
