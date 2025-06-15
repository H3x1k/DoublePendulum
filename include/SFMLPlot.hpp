#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <array>
#include <cmath>

class SFMLPlot
{
public:
    sf::Vector2f position;
    sf::Vector2u size;
    std::array<float, 2> domain;
    std::array<float, 2> range;
    std::vector<std::vector<sf::Vertex>> lines;

    SFMLPlot(sf::Vector2f pos, sf::Vector2u sz,
        std::array<float, 2> dom,
        std::array<float, 2> rng);

    void addLine();
    void addPointToLine(size_t lineIndex, sf::Vector2f point, sf::Color color = sf::Color::White);
    void draw(sf::RenderWindow& window);
};

