#include "mainwindow.h"
#include <QApplication>
#include "test.h"
#include <QFile>
#include <QDebug>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    MainWindow w;
    w.show();
    test_add_remove_update();
    test_undo_redo();
    test_filtering();
    return a.exec();
}