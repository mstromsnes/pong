template <typename T>
constexpr void Paddle<T>::render(Stage& stage, bool debug)
{
    auto color = m_collision && debug ? getDebugColor() : getColor();
    stage.fillRectangle(m_placement, color);
    m_collision = false;
}

template <typename T>
constexpr void Paddle<T>::move(PaddleDirection dir, T speed)
{
    switch (dir)
    {
    case PaddleDirection::Down:
        m_placement.translate(Vector2D<double>(0, speed));
        break;
    case PaddleDirection::Up:
        m_placement.translate(Vector2D<double>(0, -speed));
        break;
    case PaddleDirection::Left:
        m_placement.translate(Vector2D<double>(-speed, 0));
        break;
    case PaddleDirection::Right:
        m_placement.translate(Vector2D<double>(speed, 0));
        break;
    }
}
