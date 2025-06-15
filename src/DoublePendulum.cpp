#include "DoublePendulum.hpp"


DoublePendulum::DoublePendulum(
    sf::Vector2f anchor,
    float r1,
    float r2,
    float l1,
    float l2,
    float m1,
    float m2,
    float theta1,
    float theta2)
    : anchor(anchor), r1(r1), r2(r2), l1(l1), l2(l2),
    m1(m1), m2(m2), theta1(theta1), theta2(theta2)
{
    bob1.setRadius(r1);
    bob1.setOrigin({ r1, r1 });
    bob1.setFillColor(sf::Color::Blue);

    bob2.setRadius(r2);
    bob2.setOrigin({ r2, r2 });
    bob2.setFillColor(sf::Color::Red);

    sf::Vector2f pos1 = {
        anchor.x + l1 * sinf(theta1),
        anchor.y + l1 * cosf(theta1)
    };

    sf::Vector2f pos2 = {
        pos1.x + l2 * sinf(theta2),
        pos1.y + l2 * cosf(theta2)
    };

    bob1.setPosition(pos1);
    bob2.setPosition(pos2);

    line1 = { sf::Vertex{anchor}, sf::Vertex{pos1} };
    line2 = { sf::Vertex{pos1}, sf::Vertex{pos2} };
}

float DoublePendulum::computeAlpha1() {
    return (-g * (2 * m1 + m2) * sinf(theta1) - m2 * g * sinf(theta1 - 2 * theta2) -
        2 * sinf(theta1 - theta2) * m2 * (omega2 * omega2 * l2 + omega1 * omega1 * l1 * cosf(theta1 - theta2)))
        / (l1 * (2 * m1 + m2 - m2 * cosf(2 * theta1 - 2 * theta2)));
}

float DoublePendulum::computeAlpha2() {
    return (2 * sinf(theta1 - theta2) *
        (omega1 * omega1 * l1 * (m1 + m2) + g * (m1 + m2) * cosf(theta1) +
            omega2 * omega2 * l2 * m2 * cosf(theta1 - theta2)))
        / (l2 * (2 * m1 + m2 - m2 * cosf(2 * theta1 - 2 * theta2)));
}

sf::Vector2f DoublePendulum::getBob1Position() {
    sf::Vector2f pos = {
        anchor.x + l1 * sinf(theta1),
        anchor.y + l1 * cosf(theta1)
    };
    return pos;
}

sf::Vector2f DoublePendulum::getBob2Position() {
    sf::Vector2f pos1 = {
        anchor.x + l1 * sinf(theta1),
        anchor.y + l1 * cosf(theta1)
    };

    sf::Vector2f pos2 = {
        pos1.x + l2 * sinf(theta2),
        pos1.y + l2 * cosf(theta2)
    };

    return pos2;
}

void DoublePendulum::update(float dt) {
    float alpha1_old = computeAlpha1();
    float alpha2_old = computeAlpha2();

    theta1 += omega1 * dt + 0.5f * alpha1_old * dt * dt;
    theta2 += omega2 * dt + 0.5f * alpha2_old * dt * dt;

    float alpha1_new = computeAlpha1();
    float alpha2_new = computeAlpha2();

    omega1 += 0.5f * (alpha1_old + alpha1_new) * dt;
    omega2 += 0.5f * (alpha2_old + alpha2_new) * dt;

    sf::Vector2f pos1 = {
        anchor.x + l1 * sinf(theta1),
        anchor.y + l1 * cosf(theta1)
    };

    sf::Vector2f pos2 = {
        pos1.x + l2 * sinf(theta2),
        pos1.y + l2 * cosf(theta2)
    };

    line1 = { sf::Vertex{anchor}, sf::Vertex{pos1} };
    line2 = { sf::Vertex{pos1}, sf::Vertex{pos2} };

    bob1.setPosition(pos1);
    bob2.setPosition(pos2);
}

void DoublePendulum::draw(sf::RenderWindow& window) {
    window.draw(line1.data(), line1.size(), sf::PrimitiveType::Lines);
    window.draw(line2.data(), line2.size(), sf::PrimitiveType::Lines);
    window.draw(bob1);
    window.draw(bob2);
}