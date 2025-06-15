#include "SFMLPlot.hpp"

SFMLPlot::SFMLPlot(sf::Vector2f pos, sf::Vector2u sz,
    std::array<float, 2> dom,
    std::array<float, 2> rng)
    : position(pos), size(sz), domain(dom), range(rng) {
}

void SFMLPlot::addLine() {
    lines.emplace_back();
}

void SFMLPlot::addPointToLine(size_t lineIndex, sf::Vector2f point) {
    if (lineIndex < lines.size()) {
        point = transformPoint(point);
        sf::Vertex vertex{point};
        lines[lineIndex].emplace_back(vertex);
    } else {
        throw std::runtime_error("Indexing non-existing line");
    }
}

sf::Vector2f SFMLPlot::transformPoint(sf::Vector2f point) {
    float nx = (point.x - domain[0]) / (domain[1] - domain[0]);
    float ny = (point.y - range[0]) / (range[1] - range[0]);

    return {
        position.x + nx * size.x,
        position.y + (1.f - ny) * size.y // flip Y so higher y-values are up
    };
}

void SFMLPlot::draw(sf::RenderWindow& window) {
    for (const auto& line : lines) {
        if (line.size() >= 2)
            window.draw(line.data(), line.size(), sf::PrimitiveType::LineStrip);
    }
}
