#include <SFML/Graphics.hpp>
#include <array>
#include <vector>

#include "DoublePendulum.hpp"
#include "SFMLPlot.hpp"
//#include <iostream>

constexpr float PI = 3.141592653589793238462643383279f;

const float g = 10.0f;

class CustomRectangle {
public:
    sf::Vector2f size;
    sf::Vector2f position;
    sf::Vector2f origin;
    float borderWidth;
    float cornerRadius;
    unsigned int quality;

    CustomRectangle() {

    }
};


static float wrapToPi(float angle) {
    angle = fmodf(angle + PI, 2 * PI);
    if (angle < 0) angle += 2 * PI;
    return angle - PI;
}

int main() {
    sf::VideoMode fullscreenMode = sf::VideoMode::getDesktopMode();

    sf::RenderWindow window(fullscreenMode, "Simple SFML Program", sf::State::Fullscreen);
    window.setTitle("Double Pendulum Simulation");
    window.setFramerateLimit(120);

    sf::Vector2u size = window.getSize();
    auto [width, height] = size;

    sf::Vector2f anchor = sf::Vector2f{ (float)width * 0.5f, (float)height * 0.5f };
    const float r1 = 10.0f, r2 = 10.0f, l1 = 100.0f, l2 = 100.0f, m1 = 1.0f, m2 = 1.0f;
    float theta1 = 2.0f;
    float theta2 = 0.f;

    sf::RectangleShape rect(sf::Vector2f(100.f, 100.f));
    rect.getSize();

    int N = 100;
    std::vector<DoublePendulum> pendulums;

    float thetarange = 0.001f;
    //for (int i = 0; i < N; i++) {
    //    pendulums.push_back(DoublePendulum{ anchor, r1, r2, l1, l2, m1, m2, theta1 + i * thetarange / N, theta2 });
    //}
    pendulums.push_back(DoublePendulum{ anchor, r1, r2, l1, l2, m1, m2, theta1, theta2 });

    sf::Font font("../arial.ttf");
    sf::Text theta1Text(font);
    sf::Text theta2Text(font);
    theta1Text.setCharacterSize(24);
    theta2Text.setCharacterSize(24);
    theta2Text.setPosition(sf::Vector2f(0.f, 50.f));

    SFMLPlot plot(sf::Vector2f{ 100.f, 0.f }, sf::Vector2u(400, 400), {-PI, PI}, {-PI, PI});
    plot.addLine();
    

    const float dt = 1.f / 120.f;
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
            if (event->is<sf::Event::KeyPressed>() && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
                window.close();
        }

        for (auto& p : pendulums) {
            p.update(dt);
            plot.addPointToLine(0, sf::Vector2f(wrapToPi(p.theta1), wrapToPi(p.theta2)));
            theta1Text.setString(std::to_string(p.theta1));
            theta2Text.setString(std::to_string(p.theta2));
        }
        

        window.clear(sf::Color::Black);

        for (auto& p : pendulums) {
            p.draw(window);
        }
        plot.draw(window);
        window.draw(theta1Text);
        window.draw(theta2Text);

        window.display();
    }

    return 0;
}

