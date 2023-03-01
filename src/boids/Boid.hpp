#include <stdlib.h>
#include "glm/fwd.hpp"
#include "p6/p6.h"
#define DOCTEST_CONFIG_IMPLEMENT
#include <random>
#include <vector>
#include "doctest/doctest.h"

class Boid {
private:
    glm::vec2         m_position;
    glm::vec2         m_direction{0.01, 0.01};
    float             m_size          = 0.2f;
    float             m_speed         = 1;
    float             detectionRadius = 0.2f;
    std::vector<Boid> neighbors;

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

    void setDirection(glm::vec2 direction)
    {
        this->m_direction = direction;
    }

    void draw(auto& ctx)
    {
        ctx.use_stroke = false;
        ctx.fill       = {0.f, 0.f, 0.f, 0.2f};
        ctx.circle(this->m_position, p6::Radius{this->detectionRadius});

        if (!this->neighbors.empty())
        {
            ctx.fill = {1.f, 0.f, 0.f, 1.f};
        }
        else
        {
            ctx.fill = {0.5f, 0.5f, 0.5f, 0.7f};
        }
        ctx.equilateral_triangle(this->m_position, this->m_size, p6::Angle(this->m_direction));
    }

    void update(auto& ctx, std::vector<Boid> boids)
    {
        this->m_position += this->m_direction;
        checkBorders(ctx);
        checkNeighbors(boids);
        this->draw(ctx);
        this->neighbors.clear();
    }

    void checkBorders(auto& ctx)
    {
        if (this->m_position.x > ctx.aspect_ratio() + this->m_size)
        {
            this->m_position.x = -ctx.aspect_ratio() - this->m_size;
        }
        if (this->m_position.y > 1 + this->m_size)
        {
            this->m_position.y = -1 - this->m_size;
        }
        if (this->m_position.x < -ctx.aspect_ratio() - this->m_size)
        {
            this->m_position.x = ctx.aspect_ratio() + this->m_size;
        }
        if (this->m_position.y < -1 - this->m_size)
        {
            this->m_position.y = 1 + this->m_size;
        }
    }

    void checkNeighbors(std::vector<Boid> boids)
    {
        for (auto& boid : boids)
        {
            if ((boid.m_position.x < this->m_position.x + this->detectionRadius || boid.m_position.x > this->m_position.x - this->detectionRadius) && (boid.m_position.y > this->m_position.y - this->detectionRadius || boid.m_position.y < this->m_position.y + this->detectionRadius))
            {
                this->neighbors.push_back(boid);
            }
        }
    }
};