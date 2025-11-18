#include <algorithm>
#include <iostream>
#include <vector>
#include <memory>

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

#pragma warning(push)
#pragma warning(disable: 4365)

#include "stb_image_write.h"

#pragma warning(pop)

static void WriteFramebufferToPng(const char* filename, const int width, const int height, const std::vector<Colour>& pixels) {
    constexpr size_t channels{ 3 };
    std::vector<float> image(static_cast<size_t>(width * height * channels));

    for (size_t i = 0; i < pixels.size(); i++) {
        for (int colour = 0; colour < channels; colour++) {
            image[i * channels + colour] = pixels[i][colour];
        }
    }

    stbi_write_png(filename, width, height, channels, image.data(), width * static_cast<int>(channels));
}

static HittableList InitWorld() {
    HittableList world{};

    auto material_ground{ make_shared<Lambertian>(Colour(0.5f, 0.5f, 0.5f)) };
    Point3 p_ground{ 0, -1000, 0 };
    world.add(make_shared<Sphere>(p_ground, 1000, material_ground));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            Point3 random_center{ a + 0.9f * RandomFloat(), 0.2f, b + 0.9f * RandomFloat() };
            if ((random_center - Point3(4, 0.2f, 0)).Length() > 0.9f) {
                world.add(make_shared<Sphere>(random_center, 0.2f, RandomMaterial()));
            }
        }
    }

    Point3 p_diffuse{ -4, 1, 0 };
    Point3 p_glass{ 0, 1, 0 };
    Point3 p_metal{ 4, 1, 0 };

    auto material_diffuse{ make_shared<Lambertian>(Colour(0.1f, 0.2f, 0.5f)) };
    auto material_glass{ make_shared<Dielectric>(1.50f) };
    auto material_metal{ make_shared<Metal>(Colour(0.7f, 0.6f, 0.5f), 0.0) };

    world.add(make_shared<Sphere>(p_diffuse, 1.0f, material_diffuse));
    world.add(make_shared<Sphere>(p_glass, 1.0f, material_glass));
    world.add(make_shared<Sphere>(p_metal, 1.0f, material_metal));

    return world;
}

int main() {
    constexpr float aspect_ratio{ 16.0f / 9.0f };
    constexpr int image_width{ 1920 };
    constexpr int image_height{ static_cast<int>(image_width / aspect_ratio) };
    constexpr float fov{ 34.0f };

    HittableList world{ InitWorld() };
    
    Camera camera{ image_width, image_height, fov };
    camera.SetSamplesPerPixel(10);
    camera.SetMaxBounceDepth(20);
    camera.SetCameraCenter(Point3(13, 2, 3));
    camera.SetCameraTarget(Point3(0, 0, 0));
    camera.SetDefocusAngle(0.6f);
    camera.SetFocusDistance(10.0f);

    Framebuffer framebuffer{ camera.Render(world) };
    WriteFramebufferToPng("image.png", image_width, image_height, framebuffer.GetPixels());

    return 0;
}