#include <algorithm>
#include <iostream>
#include <vector>

#include "Camera.h"
#include "Colour.h"
#include "Framebuffer.h"
#include "Geometry.h"
#include "Hittable.h"
#include "HittableList.h"
#include "Material.h"
#include "Ray.h"
#include "Utility.h"
#include "Vec3.h"
#include "stb_image_write.h"


void WriteFramebufferToPng(const char* filename, const int width, const int height, Framebuffer framebuffer) {
    constexpr int channels{ 3 };
    std::vector<uint8_t> image(width * height * channels);
    std::vector<Colour> pixels{ framebuffer.GetFramebuffer() };

    for (int i = 0; i < pixels.size(); i++) {
        for (int colour = 0; colour < channels; colour++) {
            image[i * channels + colour] =
                static_cast<uint8_t>(pixels[i][colour]);
        }
    }

    stbi_write_png(filename, width, height, channels, image.data(), width * channels);
}

HittableList InitWorld() {
    HittableList world{};

    Point3 p_ground{ 0, -100.5, -1 };
    Point3 p_center{ 0, 0, -1.2 };
    Point3 p_left{ -1.0, 0.0, -1.0 };
    Point3 p_right{ 1.0, 0.0, -1.0 };

    auto material_ground{ make_shared<Lambertian>(Colour(0.4, 0.4, 0.3)) };
    auto material_center{ make_shared<Lambertian>(Colour(0.1, 0.2, 0.5)) };
    auto material_left{ make_shared<Dielectric>(1.50) };
    auto material_bubble{ make_shared<Dielectric>(1.00 / 1.50) };
    auto material_right{ make_shared<Metal>(Colour(0.8, 0.6, 0.2), 0.2) };

    world.add(make_shared<Sphere>(p_ground, 100, material_ground));
    world.add(make_shared<Sphere>(p_center, 0.5, material_center));
    world.add(make_shared<Sphere>(p_left, 0.5, material_left));
    world.add(make_shared<Sphere>(p_left, 0.4, material_bubble));
    world.add(make_shared<Sphere>(p_right, 0.5, material_right));

    return world;
}

int main() {
    constexpr float aspect_ratio{ 16.0f / 9.0f };
    constexpr int image_width{ 1200 };
    constexpr int image_height{ static_cast<int>(image_width / aspect_ratio) };

    HittableList world{ InitWorld() };
    
    Camera camera{ image_width, image_height };
    camera.SetSamplesPerPixel(100);
    camera.SetMaxBounceDepth(50);

    Framebuffer framebuffer{ camera.Render(world) };

    WriteFramebufferToPng("image.png", image_width, image_height, framebuffer);

    return 0;
}