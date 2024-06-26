#include "electronic_load_app.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <qstylefactory.h>

int main(int argc, char *argv[]){
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
    w.show();
    return a.exec();
}
