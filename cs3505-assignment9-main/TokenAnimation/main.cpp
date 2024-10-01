#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    //Q_INIT_RESOURCE(resources);
    MainWindow window;
    window.show();
    return app.exec();
}
