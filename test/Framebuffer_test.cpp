#include "pch.h"
#include "Framebuffer.h"

//class FramebufferTest : public testing::Test {
//protected:
//    FramebufferTest() {
//    }
//
//    Framebuffer f0{ 1080, 1920 };
//};


TEST(FramebufferTest, ConstructorShouldThrowWithZeroHeight) {
    EXPECT_THROW(Framebuffer f(0, 1920), std::invalid_argument);
}

TEST(FramebufferTest, ConstructorShouldThrowWithZeroWidth) {
    EXPECT_THROW(Framebuffer f(1080, 0), std::invalid_argument);
}

TEST(FramebufferTest, ConstructorShouldSucceedWithMinimumDimensions) {
    EXPECT_NO_THROW(Framebuffer f(1, 1));
}

TEST(FramebufferTest, ConstructorShouldSucceedWithMaximumDimensions) {
    EXPECT_NO_THROW(Framebuffer f(Framebuffer::max_height, Framebuffer::max_width));
}

TEST(FramebufferTest, SetPixelColourShouldModifyPixelsCorrectly) {
    Framebuffer f0{ 25, 25 };
    int f0_height = f0.GetHeight();
    int f0_width = f0.GetWidth();

    colour c1{ 1, 0, 0 };
    colour c2{ 0, 1, 0 };
    colour c3{ 0, 0, 1 };

    std::vector<colour> test_framebuffer(f0_height * f0_width);
    test_framebuffer[0] = c1;
    test_framebuffer[test_framebuffer.size() / 2] = c2;
    test_framebuffer[test_framebuffer.size() - 1] = c3;

    Point p1{ 0, 0 };
    Point p2{ f0_height / 2, f0_width / 2 };
    Point p3{ f0_height - 1, f0_width - 1 };

    f0.SetPixelColour(c1, p1);
    f0.SetPixelColour(c2, p2);
    f0.SetPixelColour(c3, p3);

    EXPECT_EQ(f0.GetFramebuffer(), test_framebuffer);
}
