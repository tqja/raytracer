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


void WriteFramebufferToPng(const char* filename, const int width, const int height, const Framebuffer framebuffer) {
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

shared_ptr<Material> RandomMaterial() {
    auto choose_mat{ RandomDouble() };

    if (choose_mat < 0.8) {
        auto albedo{ Colour::Random() * Colour::Random() };
        return make_shared<Lambertian>(albedo);
    }

    if (choose_mat < 0.95) {
        auto albedo{ Colour::Random(0.5, 1) };
        auto fuzz{ RandomDouble(0, 0.5) };
        return make_shared<Metal>(albedo, fuzz);
    }

    return make_shared<Dielectric>(1.5);
}

HittableList InitWorld() {
    HittableList world{};

    auto material_ground{ make_shared<Lambertian>(Colour(0.5, 0.5, 0.5)) };
    Point3 p_ground{ 0, -1000, 0 };
    world.add(make_shared<Sphere>(p_ground, 1000, material_ground));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            Point3 random_center{ a + 0.9 * RandomDouble(), 0.2, b + 0.9 * RandomDouble() };
            if ((random_center - Point3(4, 0.2, 0)).Length() > 0.9) {
                world.add(make_shared<Sphere>(random_center, 0.2, RandomMaterial()));
            }
        }
    }

    Point3 p_diffuse{ -4, 1, 0 };
    Point3 p_glass{ 0, 1, 0 };
    Point3 p_metal{ 4, 1, 0 };

    auto material_diffuse{ make_shared<Lambertian>(Colour(0.1, 0.2, 0.5)) };
    auto material_glass{ make_shared<Dielectric>(1.50) };
    auto material_metal{ make_shared<Metal>(Colour(0.7, 0.6, 0.5), 0.0) };

    world.add(make_shared<Sphere>(p_diffuse, 1.0, material_diffuse));
    world.add(make_shared<Sphere>(p_glass, 1.0, material_glass));
    world.add(make_shared<Sphere>(p_metal, 1.0, material_metal));

    return world;
}

int main() {
    constexpr float aspect_ratio{ 16.0 / 9.0 };
    constexpr int image_width{ 1200 };
    constexpr int image_height{ static_cast<int>(image_width / aspect_ratio) };
    constexpr double fov{ 34 };

    HittableList world{ InitWorld() };
    
    Camera camera{ image_width, image_height, fov };
    camera.SetSamplesPerPixel(500);
    camera.SetMaxBounceDepth(50);
    camera.SetCameraCenter(Point3(13, 2, 3));
    camera.SetCameraTarget(Point3(0, 0, 0));
    camera.SetDefocusAngle(0.6);
    camera.SetFocusDistance(10.0);

    Framebuffer framebuffer{ camera.Render(world) };

    WriteFramebufferToPng("image.png", image_width, image_height, framebuffer);

    return 0;
}