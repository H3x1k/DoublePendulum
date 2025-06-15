#include <SFML/Graphics.hpp>
#include <array>
#include <vector>

#include "DoublePendulum.hpp"
#include "SFMLPlot.hpp"
//#include <iostream>

constexpr float PI = 3.141592653589793238462643383279f;

const float g = 10.0f;



struct CircleGenerator {
    static float constexpr pi{ 3.141592653 };
    float const radius = 0.0f;
    uint32_t const quality = 0;
    float const da = 0.0f;

    CircleGenerator(float radius_, uint32_t quality_)
        : radius{ radius_ }
        , quality{ quality_ }
        , da{ (2.0f * pi) / static_cast<float>(quality) }
    {
    }

    sf::Vector2f getPoint(uint32_t i) const
    {
        float const angle{ da * static_cast<float>(i) };
        return { radius * sf::Vector2f{cos(angle), sin(angle)} };
    }
};

void generateCircle(sf::VertexArray& vertex_array, sf::Vector2f position, float radius, uint32_t quality)
{
    CircleGenerator const generator{ radius, quality };
    for (uint32_t i{ 0 }; i < quality; ++i) {
        vertex_array[i].position = position + generator.getPoint(i);
    }
}

struct RoundedRectangleGenerator {
    sf::Vector2f const size;
    sf::Vector2f const centers[4];
    uint32_t const arc_quality;
    CircleGenerator const generator;

    RoundedRectangleGenerator(sf::Vector2f size_, float radius, uint32_t quality)
        : size{size_}
        , centers{
            {size.x - radius, size.y - radius},
            {radius, size.y - radius},
            {radius, radius},
            {size.x - radius, radius}
        }
        , arc_quality{quality / 4}
        , generator{radius, quality - 4}
    { }

    sf::Vector2f getPoint(uint32_t i) const {
        uint32_t const corner_idx{ i / arc_quality };
        return centers[corner_idx] + generator.getPoint(i - corner_idx);
    }
};

void generateRoundedRectangle(sf::VertexArray& vertex_array, sf::Vector2f position, sf::Vector2f size, float radius, uint32_t quality) {
    RoundedRectangleGenerator const generator{ size, radius, quality };
    for (uint32_t i{ 0 }; i < quality; ++i) {
        vertex_array[i].position = position + generator.getPoint(i);
    }
}

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

    int N = 100;
    std::vector<DoublePendulum> pendulums;

    uint32_t quality = 4 * 20;
    float borderWidth = 10.f;

    sf::VertexArray rrect;
    rrect.setPrimitiveType(sf::PrimitiveType::TriangleFan);
    rrect.resize(quality);
    generateRoundedRectangle(rrect, sf::Vector2f(500.f, 0.f), sf::Vector2f(500.f, 500.f), 50, quality);
    for (std::size_t i = 0; i < rrect.getVertexCount(); ++i) {
        rrect[i].color = sf::Color::Blue;
    }

    sf::VertexArray rrect2;
    rrect2.setPrimitiveType(sf::PrimitiveType::TriangleFan);
    rrect2.resize(quality);
    generateRoundedRectangle(rrect2, sf::Vector2f(500.f + borderWidth, borderWidth), sf::Vector2f(500.f - 2.0f * borderWidth, 500.f - 2.0f * borderWidth), 50 - borderWidth, quality);
    for (std::size_t i = 0; i < rrect2.getVertexCount(); ++i) {
        rrect2[i].color = sf::Color::White;
    }


    float thetarange = 0.001f;
    //for (int i = 0; i < N; i++) {
    //    pendulums.push_back(DoublePendulum{ anchor, r1, r2, l1, l2, m1, m2, theta1 + i * thetarange / N, theta2 });
    //}
    pendulums.push_back(DoublePendulum{ anchor, r1, r2, l1, l2, m1, m2, theta1, theta2 });

    sf::Font font("../fonts/arial.ttf");
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

        //window.draw(rrect);
        //window.draw(rrect2);

        window.display();
    }

    return 0;
}

