#include "qtpong.h"

#include "pong/input.h"

QtPong::QtPong() : m_window{m_pong.getStage(), this}, m_loopTimer{this}
{
    m_window.resize(768, 768);
    m_window.show();
    m_loopTimer.setInterval(1000.0f / 100.0f);
    m_loopTimer.callOnTimeout([&]() {
        m_pong.eventLoop();
        render();
    });
    m_loopTimer.start();
}

void QtPong::render() { m_window.renderPong(); }

void QtPong::keyPressEvent(QKeyEvent* event)
{
    auto key = event->key();
    switch (key)
    {
    case Qt::Key_W:
        m_pong.keyPress(KeyPress::P1Up);
        break;
    case Qt::Key_S:
        m_pong.keyPress(KeyPress::P1Down);
        break;
    case Qt::Key_Up:
        m_pong.keyPress(KeyPress::P2Up);
        break;
    case Qt::Key_Down:
        m_pong.keyPress(KeyPress::P2Down);
        break;
    case Qt::Key_Escape:
        m_pong.keyPress(KeyPress::Escape);
        break;
    case Qt::Key_Return:
        m_pong.keyPress(KeyPress::Enter);
        break;
    default:
        break;
    }
}

void QtPong::keyReleaseEvent(QKeyEvent* event)
{
    auto key = event->key();
    switch (key)
    {
    case Qt::Key_W:
        m_pong.keyRelease(KeyPress::P1Up);
        break;
    case Qt::Key_S:
        m_pong.keyRelease(KeyPress::P1Down);
        break;
    case Qt::Key_Up:
        m_pong.keyRelease(KeyPress::P2Up);
        break;
    case Qt::Key_Down:
        m_pong.keyRelease(KeyPress::P2Down);
        break;
    case Qt::Key_Escape:
        m_pong.keyRelease(KeyPress::Escape);
        break;
    case Qt::Key_Return:
        m_pong.keyRelease(KeyPress::Enter);
        break;
    default:
        break;
    }
}
