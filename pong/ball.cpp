#include "ball.h"

void Ball::move()
{
    m_prevPlacement = m_placement;
    m_placement.pos += m_speed;
}

void Ball::render(Stage& stage)
{
    clear(stage);
    stage.fill(m_placement, Color(0x00, 0x00, 0xff));
}

void Ball::clear(Stage& stage) { stage.fill(m_prevPlacement, 0x0); }
