#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{    
    //io_font::input = QFont("consolas",12);
    io_font::input.setFamily("Courier");
    io_font::input.setStyleHint(QFont::Monospace);
    io_font::input.setFixedPitch(true);
    io_font::input.setPointSize(12);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
