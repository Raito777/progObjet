#include "boids/Boid.hpp"
#include <stdlib.h>
#include <random>
#include <vector>
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "p6/p6.h"

void Boid::update(p6::Context& ctx, std::vector<Boid>& boids)
{
    moove(ctx);
    checkNeighbors(boids);
    calculateDirection();
    checkBorders(ctx);
    checkCollisions();
    draw(ctx);

    // il faut bien check les collisions à la fin !

    this->neighbors.clear();
}

void Boid::draw(p6::Context& ctx)
{
    // ctx.fill = {0.f, 0.f, 0.f, 0.1f};
    // // dessine le cercle de detection
    // ctx.use_stroke = false;
    // ctx.circle(this->m_position, p6::Radius{this->m_detectionRadius});

    // // dessine le cercle de collision
    // ctx.use_stroke    = true;
    // ctx.stroke_weight = 0.003f;
    // ctx.circle(this->m_position, p6::Radius{this->m_collisionTolerance});

    ctx.stroke_weight = 0.005f;

    ctx.use_stroke = true;
    if (this->neighbors.size() > 1)
    {
        ctx.stroke = {0.f, 0.7f, 0.0f};
        ctx.fill   = {0.f, 0.7f, 0.0f, 0.5f};
    }
    else
    {
        ctx.stroke = {0.7f, 0.0f, 0.0f};
        ctx.fill   = {0.7f, 0.f, 0.f, 0.5f};
    }
    p6::Point2D p1(-this->m_size, this->m_size);
    p6::Point2D p2(-this->m_size, -this->m_size);
    p6::Point2D p3(this->m_size * 1.5, 0);

    ctx.triangle(p1, p2, p3, this->m_position, p6::Angle(this->m_direction));

    // ctx.equilateral_triangle(this->m_position, this->m_size, p6::Angle(this->m_direction));
}

void Boid::moove(p6::Context& ctx)
{
    // Calculate the current angle based on the current position
    // float angle = glm::radians(100 * sin(100 * this->m_position.y));
    float angle = glm::radians(20.0f) * sin(ctx.delta_time());

    glm::mat2 rotationMatrix(glm::cos(angle), -glm::sin(angle), glm::sin(angle), glm::cos(angle));

    // Calculate the new direction based on the angle
    // this->m_direction = glm::normalize(glm::vec2(glm::cos(angle), glm::sin(angle)));

    this->m_position += rotationMatrix * this->m_direction * ctx.delta_time() * this->m_speed;
}

void Boid::checkBorders(p6::Context& ctx)
{
    if (this->m_position.x + this->m_size > ctx.aspect_ratio())
    {
        this->m_direction.x = -this->m_direction.x;
    }
    if (this->m_position.y + this->m_size > 1)
    {
        this->m_direction.y = -this->m_direction.y;
    }
    if (this->m_position.x - this->m_size < -ctx.aspect_ratio())
    {
        this->m_direction.x = -this->m_direction.x;
    }
    if (this->m_position.y - this->m_size < -1)
    {
        this->m_direction.y = -this->m_direction.y;
    }
    // if (this->m_position.x > ctx.aspect_ratio() + this->m_size)
    // {
    //     this->m_position.x = -ctx.aspect_ratio() - this->m_size;
    // }
    // if (this->m_position.y > 1 + this->m_size)
    // {
    //     this->m_position.y = -1 - this->m_size;
    // }
    // if (this->m_position.x < -ctx.aspect_ratio() - this->m_size)
    // {
    //     this->m_position.x = ctx.aspect_ratio() + this->m_size;
    // }
    // if (this->m_position.y < -1 - this->m_size)
    // {
    //     this->m_position.y = 1 + this->m_size;
    // }
}

void Boid::checkNeighbors(std::vector<Boid>& boids)
{
    for (auto& boid : boids)
    {
        if (glm::distance(this->m_position, boid.m_position) < this->m_detectionRadius + boid.m_detectionRadius)
        {
            this->neighbors.push_back(boid);
        }
    }
}

void Boid::calculateDirection()
{
    glm::vec2 newDirection(0, 0);
    for (auto& boid : this->neighbors)
    {
        newDirection += boid.m_direction;
    }
    for (size_t i = 0; i < this->m_independance; i++)
    {
        newDirection += this->m_direction;
    }
    newDirection /= static_cast<float>(this->neighbors.size());

    float     deviation = p6::random::number(0.f, this->m_deviationStrength); // adjust this value to control the amount of randomness
    glm::vec2 randomDeviation(p6::random::number(-deviation, deviation), p6::random::number(-deviation, deviation));
    newDirection += randomDeviation;

    // normalize the new direction vector
    newDirection = glm::normalize(newDirection);

    this->m_direction = newDirection;
}

void Boid::checkCollisions()
{
    std::vector<Boid> collisions;
    glm::vec2         directionSum(0.0f, 0.0f);
    for (auto& boid : this->neighbors)
    {
        float distance = glm::distance(this->m_position, boid.m_position);
        if ((distance < (this->m_size + this->m_collisionTolerance) && glm::distance(this->m_position, boid.m_position) != 0))
        {
            // le prob c'est que quand deux boids se chevauchent completement, ils ne se sépareront jamais
            // on envoit le boid à l'opposé
            glm::vec2 diff = this->m_position - boid.m_position;
            diff           = glm::normalize(diff);
            diff /= distance; // Weight by distance

            directionSum += diff;

            collisions.push_back(boid);
        }
    }

    if (!collisions.empty())
    {
        directionSum /= static_cast<float>(collisions.size());
        // Choisir une nouvelle direction aléatoire
        this->m_direction = glm::normalize(glm::mix(this->m_direction, directionSum, 0.5f));
    }
}