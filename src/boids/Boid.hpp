#pragma once

#include <stdlib.h>
#include <random>
#include <vector>
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "p6/p6.h"

class Boid {
private:
    glm::vec2         m_position;
    glm::vec2         m_direction{0.01, 0.01};
    float             m_size               = 0.2f;
    float             m_speed              = 0.6f;
    float             m_detectionRadius    = this->m_size + 0.05f;
    float             m_borderStrength     = 0.01f;
    float             m_collisionTolerance = this->m_size + 0.015f;
    int               m_independance       = 10;
    float             m_deviationStrength  = p6::random::number(0.1f, 2.f);
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

    void setIndependance(int independance)
    {
        this->m_independance = independance;
    }

    void setDetectionRadius(float radius)
    {
        this->m_detectionRadius = this->m_size + radius;
    }

    void setDeviationStrength(float strength)
    {
        this->m_deviationStrength = p6::random::number(0.1f, strength);
    }

    void setCollisionTolerance(float tolerance)
    {
        this->m_collisionTolerance = tolerance;
    }

    void setDirection(glm::vec2 direction)
    {
        this->m_direction = direction;
    }

    void draw(p6::Context& ctx);

    void moove(p6::Context& ctx);

    void update(p6::Context& ctx, std::vector<Boid>& boids);

    void checkBorders(p6::Context& ctx);

    void checkNeighbors(std::vector<Boid>& boids);

    void calculateDirection();

    void checkCollisions();
};