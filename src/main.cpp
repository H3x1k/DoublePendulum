#include <SFML/Graphics.hpp>
#include <array>
#include <vector>

#include "DoublePendulum.hpp"
//#include "SFMLPlot.hpp"
//#include <iostream>

#define PI 3.141592653589793238462643383279f

const float g = 10.0f;

int main() {
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Simple SFML Program");
    window.setTitle("Double Pendulum Simulation");
    window.setFramerateLimit(120);

    sf::Vector2u size = window.getSize();
    auto [width, height] = size;

    sf::Vector2f anchor = sf::Vector2f{ (float)width * 0.5f, (float)height * 0.5f };

    const float r1 = 10.0f;
    const float r2 = 10.0f;

    const float l1 = 100.0f;
    const float l2 = 100.0f;
    const float m1 = 1.0f;
    const float m2 = 1.0f;

    float theta1 = PI;
    float theta2 = 3.0f;

    int N = 100;

    std::vector<DoublePendulum> pendulums;

    float thetarange = 0.001f;

    for (int i = 0; i < N; i++) {
        pendulums.push_back(DoublePendulum{ anchor, r1, r2, l1, l2, m1, m2, theta1 + i * thetarange / N, theta2 });
    }

    sf::Clock clock;
    const float dt = 1.f / 120.f;

    sf::Image densityMap(size, sf::Color(0, 0, 0, 0));


    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        //float dt = clock.restart().asSeconds();

        for (auto& p : pendulums) {
            p.update(dt);
            sf::Vector2f pos = p.bob2.getPosition();
            int x = static_cast<int>(pos.x);
            int y = static_cast<int>(pos.y);
            if (x >= 0 && x < width && y >= 0 && y < height) {
                sf::Color c = densityMap.getPixel({ (unsigned int)x, (unsigned int)y });
                if (c.r < 255) c.r += 1;
                c.a = 255;
                densityMap.setPixel({ (unsigned int)x, (unsigned int)y }, c);
            }
        }

        window.clear(sf::Color::Black);
        sf::Texture texture;
        texture.loadFromImage(densityMap);
        sf::Sprite sprite(texture);
        window.draw(sprite);
        for (auto& p : pendulums) {
            p.draw(window);
        }
        window.display();
    }

    return 0;
}

