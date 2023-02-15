#include <stdlib.h>
#include "glm/fwd.hpp"
#include "p6/p6.h"
#define DOCTEST_CONFIG_IMPLEMENT
#include <random>
#include <vector>
#include "doctest/doctest.h"

class Boid {
private:
    glm::vec2 m_position;
    glm::vec2 m_direction{0.01, 0.01};
    float     m_size  = 0.2f;
    float     m_speed = 1;

public:
    explicit Boid(const glm::vec2& position, const float& size)
        : m_position(position), m_size(size){};

    glm::vec2 getPosition()
    {
        return this->m_position;
    }

    float getSpeed() const
    {
        return this->m_speed;
    }
    void setPosition(const glm::vec2& position)
    {
        this->m_position = position;
    }

    void setSpeed(float speed)
    {
        this->m_speed = speed;
    }
    void draw(auto& ctx)
    {
        // ctx.square(
        //     p6::TopLeftCorner{{this->m_position.x, this->m_position.y}},
        //     p6::Radius{0.3f}
        // );
        p6::Point2D p1(this->m_position.x, this->m_position.y + this->m_size);
        p6::Point2D p2(this->m_position.x - this->m_size, this->m_position.y - this->m_size);
        p6::Point2D p3(this->m_position.x + this->m_size, this->m_position.y - this->m_size);

        ctx.triangle(p1, p2, p3);
    }
    void update()
    {
        this->m_position += this->m_direction;
        if (this->m_position.x)
    }
};