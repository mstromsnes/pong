#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "pong/stage.h"

#include <QKeyEvent>
#include <QLabel>
#include <QMainWindow>
#include <vector>
class QtPong;

class MainWindow : public QMainWindow
{
    Q_OBJECT
  public:
    MainWindow(const Stage& stage, QtPong* pong);
    void renderPong();

  protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

  private:
    QtPong* p_pong;
    QLabel m_centralWidget;
    QImage m_pixmapScene;
};

#endif // MAINWINDOW_H
