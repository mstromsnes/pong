[[nodiscard]] constexpr auto Drawer::findInitialConditions(const Triangle<int>& tri) -> TriangleDrawParams
{
    auto vertices{tri.getVertices()};
    float xstart = vertices[0].x;
    int ystart = vertices[0].y;
    if (vertices[0].y == vertices[1].y)
    {
        float dxleft =
            static_cast<float>(vertices[2].x - vertices[0].x) / static_cast<float>(vertices[2].y - vertices[0].y);
        float dxright =
            static_cast<float>(vertices[2].x - vertices[1].x) / static_cast<float>(vertices[2].y - vertices[1].y);
        float xend = vertices[1].x;
        int yend = vertices[2].y;

        return TriangleDrawParams(xstart, xend, ystart, yend, dxleft, dxright);
    }
    else
    {
        float dxleft =
            static_cast<float>(vertices[1].x - vertices[0].x) / static_cast<float>(vertices[1].y - vertices[0].y);
        float dxright =
            static_cast<float>(vertices[2].x - vertices[0].x) / static_cast<float>(vertices[2].y - vertices[0].y);
        float xend = vertices[0].x;
        int yend = vertices[1].y;
        return TriangleDrawParams(xstart, xend, ystart, yend, dxleft, dxright);
    }
}
template <typename T>
constexpr auto Drawer::findEndPoints(Line<T> line) -> std::tuple<int, int, int, int>
{
    if constexpr (std::is_integral_v<T>)
    {
        auto x0 = line.start().x;
        auto x1 = line.end().x;
        auto y0 = line.start().y;
        auto y1 = line.end().y;
        return std::tuple{x0, x1, y0, y1};
    }
    else
    {
        auto x0 = std::lround(line.start().x);
        auto x1 = std::lround(line.end().x);
        auto y0 = std::lround(line.start().y);
        auto y1 = std::lround(line.end().y);
        return std::tuple{x0, x1, y0, y1};
    }
}

constexpr void Drawer::drawHorizontalLine(int xs, int xe, int y, Color color)
{
    for (int x = xs; x < xe; x++)
    {
        paintPixel(x, y, color);
    }
}

constexpr void Drawer::drawTriangle(Triangle<int> triangle, Color color)
{
    auto params = findInitialConditions(triangle);
    if (params.ystart >= 0 || params.yend < m_pixmap.getHeight())
    {
        params.ystart = std::clamp(params.ystart, 0, m_pixmap.getHeight());
        params.yend = std::clamp(params.yend, 0, m_pixmap.getHeight());
        for (int y = params.ystart; y < params.yend; y++)
        {
            int xstart_r = std::lround(params.xstart);
            int xend_r = std::lround(params.xend);
            if (xstart_r >= 0 || xend_r < m_pixmap.getWidth())
            {
                drawHorizontalLine(xstart_r, xend_r, y, color);
                params.xstart += params.dxleft;
                params.xend += params.dxright;
            }
        }
    }
}

// Bresenham line drawing algorithm that reverts to naive for axis
// aligned lines.
template <typename T>
constexpr void Drawer::drawLine(Line<T> line, Color color)
{
    auto [x0, x1, y0, y1] = findEndPoints(line);
    if (x1 < x0)
    {
        // We want to draw from left to right
        std::swap(x1, x0);
        std::swap(y1, y0);
    }
    if (y1 == y0)
    {
        drawHorizontalLine(x0, x1, y0, color);
        return;
    }
    if (x0 == x1)
    {
        // Here we iterate y, so we want to draw from top to bottom
        if (y1 < y0)
            std::swap(y1, y0);
        drawVerticalLine(x0, y0, y1, color);
        return;
    }
    auto c = static_cast<float>(y1 - y0) / static_cast<float>(x1 - x0);
    for (int x = x0; x < x1; x++)
    {
        auto y = std::lround(c * (x - x0) + y0);
        paintPixel(x, y, color);
    }
}

constexpr void Drawer::drawVerticalLine(int x, int y0, int y1, Color color)
{
    for (int y = y0; y < y1; y++)
    {
        paintPixel(x, y, color);
    }
}

constexpr void Drawer::paintPixel(int x, int y, Color color)
{
    if (x < 0 || x >= m_pixmap.getWidth() || y < 0 || y >= m_pixmap.getHeight())
        return;
    for (int j = 0; j < m_pixmap.getChannelCount(); j++)
    {
        auto idx = y * m_pixmap.getRowSize() + x * m_pixmap.getChannelCount() + j;
        m_pixmap[idx] = color[j];
    }
}

// Draws a line normal to the line given, starting at the middle of the line
// given.
template <typename T>
constexpr void Drawer::drawLineNormal(Line<T> line, Color color)
{
    auto normalVec = line.normal();
    auto length = std::min<double>(line.length(), 10.0f);
    auto normalLine = Line<T>(line.start() + line.direction() * line.length() / 2.0f, normalVec, length);
    drawLine(normalLine, color);
}
