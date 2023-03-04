#include <stdlib.h>
#include <vcruntime.h>
#include "glm/fwd.hpp"
#include "p6/p6.h"
#define DOCTEST_CONFIG_IMPLEMENT
#include <random>
#include <vector>
#include "boids/Boid.hpp"
#include "doctest/doctest.h"

int main(int argc, char* argv[])
{
    { // Run the tests
        if (doctest::Context{}.run() != 0)
            return EXIT_FAILURE;
        // The CI does not have a GPU so it cannot run the rest of the code.
        const bool no_gpu_available = argc >= 2 && strcmp(argv[1], "-nogpu") == 0; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        if (no_gpu_available)
            return EXIT_SUCCESS;
    }

    // Actual app
    auto ctx = p6::Context{{.title = "progObjet"}};
    ctx.maximize_window();

    // Declare your infinite update loop.
    std::vector<Boid> boids;

    for (size_t i = 0; i < 12; i++)
    {
        glm::vec2 positions{p6::random::number(-ctx.aspect_ratio(), ctx.aspect_ratio()), p6::random::number(-1, 1)};
        Boid      b(positions, 0.02f);
        b.setDirection(glm::vec2(p6::random::number(-1.f, 1.f), p6::random::number(-1.f, 1.f)));
        boids.push_back(b);
    }

    int   boidsIndependance = 10;
    float boidsSpeed        = 0.6f;
    float deviationStrength = 1.f;
    float detectionRadius   = 0.05f;
    float avoidance         = 0.0250f;

    ctx.update = [&]() {
        ctx.background({0.18f, 0.18f, 0.18f});

        ImGui::Begin("Test");
        ImGui::SliderFloat("Speed", &boidsSpeed, 0.f, 2.f);
        ImGui::SliderInt("Independance", &boidsIndependance, 5, 100);
        ImGui::SliderFloat("Detection radius", &detectionRadius, 0.01f, 0.5f);
        ImGui::SliderFloat("Deviation", &deviationStrength, 0.1f, 5.f);
        ImGui::SliderFloat("Avoidance", &avoidance, 0.005f, 0.5f);

        ImGui::End();
        // Show the official ImGui demo window
        // It is very useful to discover all the widgets available in ImGui
        ImGui::ShowDemoWindow();

        if (ctx.mouse_button_is_pressed(p6::Button::Left))
        {
            Boid b(ctx.mouse(), 0.02f);
            b.setDirection(glm::vec2(p6::random::number(-1.f, 1.f), p6::random::number(-1.f, 1.f)));
            boids.push_back(b);
        }

        for (auto& boid : boids)
        {
            boid.setDeviationStrength(deviationStrength);
            boid.setSpeed(boidsSpeed);
            boid.setDetectionRadius(detectionRadius);
            boid.setIndependance(boidsIndependance);
            boid.setCollisionTolerance(avoidance);

            boid.update(ctx, boids);
        }
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
};

void squares(){

};