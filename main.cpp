#include "mainwindow.h"
#include "qtpong.h"

int main(int argc, char* argv[])
{
    QApplication app{argc, argv};
    QtPong pong{};
    return app.exec();
}
