#include <algorithm>
#include <iostream>
#include <vector>

#include "Camera.h"
#include "Colour.h"
#include "Framebuffer.h"
#include "Geometry.h"
#include "Hittable.h"
#include "HittableList.h"
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

Colour LerpColours(const Colour& c1, const Colour& c2, double blend) {
    return (1.0 - blend) * c1 + blend * c2;
    
}

Colour RayColour(const Ray& ray, const Sphere& sphere) {
    double t{ sphere.hit(ray) };
    bool ray_hit{ t > 0.0 };

    if (ray_hit) {
        Vec3 N = unit_vector(ray.at(t) - sphere.GetCenter());
        return 0.5 * Colour(N.x() + 1.0, N.y() + 1.0, N.z() + 1.0);
    }

    Vec3 unit_direction = unit_vector(ray.GetDirection());
    double blend = (unit_direction.y() + 1.0) / 2;
    return LerpColours(Colour(1.0, 1.0, 1.0), Colour(0.5, 0.7, 1.0), blend);

}

HittableList InitWorld() {
    HittableList world{};

    Point3 p1{ 0, 0, -1 };
    Point3 p2{ 0, -100.5, -1 };

    world.add(make_shared<Sphere>(p1, 0.5));
    world.add(make_shared<Sphere>(p2, 100));

    return world;
}

Framebuffer Render(int image_width, int image_height, const Camera& camera, HittableList world) {
    Framebuffer framebuffer{ image_width, image_height };
    Sphere sphere{ Vec3(0, 0, -1), 0.5 };

    for (int y = 0; y < image_height; y++) {
        std::clog << "\rScanlines remaining: " << (image_height - y) << ' ' << std::flush;

        Vec3 row_start{ camera.GetOriginPixel() + (y * camera.GetPixelDeltaV()) };

        for (int x = 0; x < image_width; x++) {
            Vec3 pixel_center{ row_start + camera.GetPixelDeltaU() * x };
            Vec3 ray_direction{ pixel_center - camera.GetCameraCenter() };

            Ray ray{ camera.GetCameraCenter(), ray_direction };
            Colour ray_colour = RayColour(ray, world);

            Point current_pixel{ x, y };
            framebuffer.SetPixelColour(ray_colour, current_pixel);
        }
    }

    return framebuffer;
}

int main() {
    constexpr float aspect_ratio{ 16.0f / 9.0f };
    constexpr int image_width{ 1920 };
    constexpr int image_height{ static_cast<int>(image_width / aspect_ratio) };
    constexpr double viewport_height{ 2.0 };

    HittableList world{ InitWorld() };
    Camera camera{ image_width, image_height, viewport_height };
    Framebuffer framebuffer{ Render(image_width, image_height, camera, world) };

    WriteFramebufferToPng("image.png", image_width, image_height, framebuffer);

    return 0;
}