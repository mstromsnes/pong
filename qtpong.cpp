#include "qtpong.h"

#include "pong/input.h"

QtPong::QtPong() : m_window{m_pong.getStage(), this}, m_gameLoopTimer{this}
{
    m_window.resize(768, 768);
    m_window.show();
    m_gameLoopTimer.setInterval(1000.0f / 75.0f);
    m_gameLoopTimer.callOnTimeout([&]() { m_pong.eventLoop(); });
    m_gameLoopTimer.start();
    m_renderLoopTimer.setInterval(1000.0f / 120.0f);
    m_renderLoopTimer.callOnTimeout([&]() {
        m_pong.renderLoop();
        render();
    });
    m_renderLoopTimer.start();
}

void QtPong::render()
{
    m_window.renderPong();
}

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
    case Qt::Key_A:
        m_pong.keyPress(KeyPress::P1Left);
        break;
    case Qt::Key_D:
        m_pong.keyPress(KeyPress::P1Right);
        break;
    case Qt::Key_Up:
        m_pong.keyPress(KeyPress::P2Up);
        break;
    case Qt::Key_Down:
        m_pong.keyPress(KeyPress::P2Down);
        break;
    case Qt::Key_Left:
        m_pong.keyPress(KeyPress::P2Left);
        break;
    case Qt::Key_Right:
        m_pong.keyPress(KeyPress::P2Right);
        break;
    case Qt::Key_Escape:
        m_pong.keyPress(KeyPress::Escape);
        break;
    case Qt::Key_Return:
        m_pong.keyPress(KeyPress::Enter);
        break;
    case Qt::Key_I:
        m_pong.keyPress(KeyPress::i);
        break;
    case Qt::Key_J:
        m_pong.keyPress(KeyPress::j);
        break;
    case Qt::Key_K:
        m_pong.keyPress(KeyPress::k);
        break;
    case Qt::Key_L:
        m_pong.keyPress(KeyPress::l);
        break;
    case Qt::Key_AE:
        m_pong.keyPress(KeyPress::DebugMode);
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
    case Qt::Key_A:
        m_pong.keyRelease(KeyPress::P1Left);
        break;
    case Qt::Key_D:
        m_pong.keyRelease(KeyPress::P1Right);
        break;
    case Qt::Key_Up:
        m_pong.keyRelease(KeyPress::P2Up);
        break;
    case Qt::Key_Down:
        m_pong.keyRelease(KeyPress::P2Down);
        break;
    case Qt::Key_Left:
        m_pong.keyRelease(KeyPress::P2Left);
        break;
    case Qt::Key_Right:
        m_pong.keyRelease(KeyPress::P2Right);
        break;
    case Qt::Key_Escape:
        m_pong.keyRelease(KeyPress::Escape);
        break;
    case Qt::Key_Return:
        m_pong.keyRelease(KeyPress::Enter);
        break;
    case Qt::Key_I:
        m_pong.keyRelease(KeyPress::i);
        break;
    case Qt::Key_J:
        m_pong.keyRelease(KeyPress::j);
        break;
    case Qt::Key_K:
        m_pong.keyRelease(KeyPress::k);
        break;
    case Qt::Key_L:
        m_pong.keyRelease(KeyPress::l);
        break;
    case Qt::Key_AE:
        m_pong.keyRelease(KeyPress::DebugMode);
        break;
    default:
        break;
    }
}
