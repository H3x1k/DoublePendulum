#include "SFMLPlot.hpp"

SFMLPlot::SFMLPlot(sf::Vector2f pos, sf::Vector2u sz,
    std::array<float, 2> dom,
    std::array<float, 2> rng)
    : position(pos), size(sz), domain(dom), range(rng) {
}

void SFMLPlot::addLine() {
    lines.emplace_back();
}

void SFMLPlot::addPointToLine(size_t lineIndex, sf::Vector2f point, sf::Color color) {
    if (lineIndex < lines.size()) {
        lines[lineIndex].emplace_back(sf::Vertex{ point });
    }
}

void SFMLPlot::draw(sf::RenderWindow& window) {
    for (const auto& line : lines) {
        if (line.size() >= 2)
            window.draw(line.data(), line.size(), sf::PrimitiveType::LineStrip);
    }
}
