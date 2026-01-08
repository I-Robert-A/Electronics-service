#include <QApplication>
#include "mainwindow.h"
#include "../headers/iAngajat.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
    
}
