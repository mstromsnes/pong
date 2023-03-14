#include "mainwindow.h"

#include "qtpong.h"

MainWindow::MainWindow(const Stage& stage, QtPong* pong) : QMainWindow(nullptr), p_pong{pong}
{
    const auto& pixmap = stage.getPixmap();
    auto scene = stage.bounds();
    const auto data = pixmap.data();
    m_pixmapScene = QImage(data, scene.width, scene.height, QImage::Format_RGB32);
    m_centralWidget.setMinimumSize(scene.width, scene.height);
    setCentralWidget(&m_centralWidget);
}

void MainWindow::renderPong()
{
    m_centralWidget.setPixmap(QPixmap::fromImage(m_pixmapScene).scaled(width(), height(), Qt::KeepAspectRatio));
    update();
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    p_pong->keyPressEvent(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent* event)
{
    p_pong->keyReleaseEvent(event);
}
