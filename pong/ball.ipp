

template <typename T>
constexpr void Ball<T>::collide(Vector2D<double> minimumTranslationVector) noexcept
{
    m_placement.translate(minimumTranslationVector);
    m_speed.reflect(minimumTranslationVector.normalized());
    m_speed.increase(0.2);
    m_collision = true;
}

template <typename T>
constexpr void Ball<T>::reset()
{
    m_speed.reset();
    m_placement = m_origPlacement;
};
template <typename T>
constexpr void Ball<T>::setPlacement(Rectangle<T> placement)
{
    m_placement = placement;
    m_origPlacement = placement;
};
template <typename T>
constexpr void Ball<T>::setSpeed(Speed<double> speed)
{
    m_speed = speed;
    m_origSpeed = speed;
}
template <typename T>
constexpr void Ball<T>::render(Stage& stage, bool debug)
{
    auto color = m_collision && debug ? getDebugColor() : getColor();
    stage.fillRectangle(m_placement, color);
    m_collision = false;
}
