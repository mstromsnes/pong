#include "paddle.h"

void Paddle::move(PaddleDirection dir, int speed, const Stage& stage)
{
    switch (dir)
    {
    case PaddleDirection::Down:
        if (placement.bottom() < stage.bounds().bottom())
        {
            placement.pos.y += speed;
            collision = false;
        }
        else
            collision = true;
        break;
    case PaddleDirection::Up:
        if (placement.top() > stage.bounds().top())
        {
            placement.pos.y -= speed;
            collision = false;
        }
        else
            collision = true;
        break;
    default:
        break;
    }
}

void Paddle::render(Stage& stage)
{
    clear(stage);
    prev_placement = placement;
    stage.fill(placement, Color(0x00, 0x00, 0xff));
}

void Paddle::clear(Stage& stage) { stage.fill(prev_placement, 0x0); }
