#include <stdlib.h>
#include "p6/p6.h"
#define DOCTEST_CONFIG_IMPLEMENT
#include <random>
#include <vector>
#include "Square.hpp"
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

    p6::Angle rotation = 0.011_turn;
    ctx.mouse_scrolled = [&](p6::MouseScroll e) {
        rotation += e.dy * 0.025_turn;
    };

    Square s1(0.5, 0.5);

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ctx.background(p6::NamedColor::Blue);
        // ctx.circle(
        //     p6::Center{ctx.mouse()},
        //     p6::Radius{0.2f}
        // );
        // ctx.square(
        //     p6::TopLeftCorner{{p6::random::number(-1, 1), p6::random::number(-1, 1)}},
        //     p6::Radius{0.3f}
        // );
        s1.draw(ctx);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();

    std::vector<int> v{};
};

void squares(){

};