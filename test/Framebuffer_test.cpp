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

    Colour colour{ 0.0, 128.0, 255.999 };
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

    const auto pixels = framebuffer.GetPixels();
    EXPECT_EQ(pixels[middle.y * framebuffer.GetWidth() + middle.x], colour);
}

TEST_F(FramebufferTest, SetPixelColourShouldNotAffectOtherPixels) {
    auto before = framebuffer.GetPixels();

    framebuffer.SetPixelColour(colour, middle);
    auto after = framebuffer.GetPixels();


    for (int y = 0; y < height; ++y) {
        const int row_offset{ y * width };

        for (int x = 0; x < width; ++x) {
            if (x == middle.x && y == middle.y) {
                EXPECT_EQ(after[row_offset + x], colour);
            }
            else {
                EXPECT_EQ(before[row_offset + x],
                          after[row_offset + x]);
            }
        }
    }
}

TEST_F(FramebufferTest, SetPixelColourShouldWorkAtCorners) {
    framebuffer.SetPixelColour(colour, top_left);
    framebuffer.SetPixelColour(colour, bottom_right);

    const auto pixels = framebuffer.GetPixels();
    EXPECT_EQ(pixels[0], colour);
    EXPECT_EQ(pixels.back(), colour);
}
