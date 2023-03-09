#ifndef QTPONG_H
#define QTPONG_H

#include "mainwindow.h"
#include "pong/collider.h"
#include "pong/paddle.h"
#include "pong/pong.h"

#include <QApplication>
#include <QKeyEvent>
#include <QTimer>
#include <vector>

class QtPong : public QWidget
{
    Q_OBJECT
  public:
    QtPong();

    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

  private:
    void render();

    Pong m_pong;
    QTimer m_gameLoopTimer;
    QTimer m_renderLoopTimer;
    MainWindow m_window;
};

#endif // QTPONG_H
