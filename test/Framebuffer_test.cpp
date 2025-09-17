#include "pch.h"
#include "Framebuffer.h"
#include "Colour.h"
#include "Point.h"

class FramebufferTest : public testing::Test {
protected:
    FramebufferTest() {
    }

    const int width{ 12 };
    const int height{ 9 };
    Framebuffer framebuffer{ width, height };

    Point top_left{ 0, 0 };
    Point middle{ width / 2, height / 2 };
    Point bottom_right{ width - 1, height - 1 };

    Colour colour{ 1.0f, 1.0f, 1.0f };
};


TEST_F(FramebufferTest, ConstructorShouldThrowWithZeroHeight) {
    EXPECT_THROW(Framebuffer f(0, 1), std::invalid_argument);
}

TEST_F(FramebufferTest, ConstructorShouldThrowWithZeroWidth) {
    EXPECT_THROW(Framebuffer f(1, 0), std::invalid_argument);
}

TEST_F(FramebufferTest, ConstructorShouldSucceedWithMinimumDimensions) {
    EXPECT_NO_THROW(Framebuffer f(1, 1));
}

TEST_F(FramebufferTest, ConstructorShouldThrowAboveMaximumDimensions) {
    EXPECT_THROW(
        Framebuffer fb(Framebuffer::max_height + 1, Framebuffer::max_width + 1),
        std::invalid_argument
    );
}

TEST_F(FramebufferTest, SetPixelColourShouldUpdateSpecifiedPixel) {

    framebuffer.SetPixelColour(colour, middle);

    const auto pixels = framebuffer.GetFramebuffer();
    EXPECT_EQ(pixels[middle.y * framebuffer.GetWidth() + middle.x], colour);
}

    Point p1{ 0, 0 };
    Point p2{ f0_height / 2, f0_width / 2 };
    Point p3{ f0_height - 1, f0_width - 1 };

    f0.SetPixelColour(c1, p1);
    f0.SetPixelColour(c2, p2);
    f0.SetPixelColour(c3, p3);

    EXPECT_EQ(f0.GetFramebuffer(), test_framebuffer);
}
