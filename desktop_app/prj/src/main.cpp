#include "electronic_load_app.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QStyleFactory>
#include <QLineEdit>
#include <QDebug>

constexpr qreal scaleFactor = 0.6;

void scaleWidget(QWidget *widget, qreal scaleFactor) {
    // Scale widget size
    widget->resize(widget->size() * scaleFactor);

    // Scale font size
    QFont font = widget->font();
    font.setPointSizeF(font.pointSizeF() * scaleFactor);
    widget->setFont(font);

    // Recursively scale child widgets
    foreach (QObject *child, widget->children()) {
        if (QWidget *childWidget = qobject_cast<QWidget *>(child)) {
            scaleWidget(childWidget, scaleFactor);
        }
    }
}

int main(int argc, char *argv[]) {
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "electronic_load_control_app_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    Electronic_load_app w;
    scaleWidget(&w, scaleFactor);
    w.show();

    // Find the central widget first
    QWidget *centralWidget = w.findChild<QWidget *>("centralwidget");
    if (centralWidget) {
        QLineEdit *setCurret = centralWidget->findChild<QLineEdit *>("setCurret", Qt::FindChildrenRecursively);
        QLineEdit *cutoffVoltage = centralWidget->findChild<QLineEdit *>("cutoffVoltage", Qt::FindChildrenRecursively);
        QLineEdit *measuredCurrent = centralWidget->findChild<QLineEdit *>("measuredCurrent", Qt::FindChildrenRecursively);

        qDebug() << "setCurret:" << setCurret;
        qDebug() << "cutoffVoltage:" << cutoffVoltage;
        qDebug() << "measuredCurrent:" << measuredCurrent;

        if (cutoffVoltage && measuredCurrent) {
            //setCurret->setFixedWidth(measuredCurrent->width());
            cutoffVoltage->setFixedWidth(measuredCurrent->width());
            qDebug() << "scaled";
        } else {
            qDebug() << "One or more QLineEdit widgets not found.";
        }
    } else {
        qDebug() << "centralwidget not found.";
    }

    return a.exec();
}
