#pragma once
#include <SFML/Graphics.hpp>  
#include <cmath>

enum NumMethod {
    ForwardEuler,
    Verlet,
    RungeKutta
};

class DoublePendulum {
public:
    static constexpr float g = 100.f;
    sf::Vector2f anchor;
    float r1, r2;
    float l1, l2;
    float m1, m2;
    float theta1, theta2;
    float omega1 = 0.f, omega2 = 0.f;
    std::array<sf::Vertex, 2> line1;
    std::array<sf::Vertex, 2> line2;
    sf::CircleShape bob1;
    sf::CircleShape bob2;

    DoublePendulum(
        sf::Vector2f anchor,
        float r1 = 10.f,
        float r2 = 10.f,
        float l1 = 100.f,
        float l2 = 100.f,
        float m1 = 1.f,
        float m2 = 1.f,
        float theta1 = 0.f,
        float theta2 = 0.f
    );

    float computeAlpha1();
    float computeAlpha2();
    sf::Vector2f getBob1Position();
    sf::Vector2f getBob2Position();
    void update(float dt);
    void draw(sf::RenderWindow& window);
};