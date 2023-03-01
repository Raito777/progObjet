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

    for (size_t i = 0; i < 20; i++)
    {
        glm::vec2 positions{p6::random::number(-ctx.aspect_ratio(), ctx.aspect_ratio()), p6::random::number(-1, 1)};
        Boid      b(positions, 0.1f);
        b.setDirection(glm::vec2(p6::random::number(-0.01f, 0.01f), p6::random::number(-0.01f, 0.01f)));
        boids.push_back(b);
    }

    ctx.update = [&]() {
        ctx.background(p6::NamedColor::Blue);
        for (auto& boid : boids)
        {
            boid.update(ctx, boids);
        }
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
};

void squares(){

};