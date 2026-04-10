#pragma once

#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

#include "Interval.h"
#include "Ray.h"
#include "Utility.h"
#include "vec3.h"

class Material;

class HitRecord {
public:
    Point3 hit_point{};
    Vec3 normal{};
    shared_ptr<Material> material;
    float t{};
    bool front_face{};

    void SetFaceNormal(const Ray& ray, const Vec3& outward_normal) {
        front_face = Dot(ray.GetDirection(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class HitRecordGroup {
public:
    HitRecordGroup(size_t size) {
        m_t.resize(size);
        m_hit_mask.resize(size);
        m_front_face.resize((size + 7) / 8);
        m_size = size;
    };

    const std::vector<float>& GetTValues() const { return m_t; }

    const Vec3Group& GetHitPoints() const { return m_hit_points; }

    const std::shared_ptr<Material>& GetMaterial() const { return m_material; }

    const Vec3Group& GetNormals() const { return m_normals; }

    const std::vector<float>& GetFrontFace() const { return m_front_face; }

    const std::vector<float>& GetHitMask() const { return m_hit_mask; }

    const size_t GetSize() const { return m_size; }

    void SetTValues(std::vector<float> t_values) { m_t = std::move(t_values); }

    void SetHitPoints(Vec3Group points) { m_hit_points = std::move(points); }

    void SetMaterial(const std::shared_ptr<Material>& mat) { m_material = mat; }

    void SetHitMask(std::vector<float> mask) { m_hit_mask = std::move(mask); }

    void SetFaceNormal(const RayGroup& rays, const Vec3Group& outward_normal) {
        Vec3Group directions{ rays.GetDirections() };
        std::vector<float> dot_products(rays.Size());
        directions.Dot(outward_normal, dot_products);

        simd::DispatchBase* dp{ simd::GetDispatch() };
        m_front_face = dot_products;
        dp->LessThan(m_front_face.data(), 0.0f, m_size);

        // ifthenelse negation on outward_normal
        //

        for (size_t i = 0; i < rays.Size(); i++) {
            if (!m_hit_mask[i]) continue;

            m_front_face[i] = dot_products[i] < 0;
            if (m_front_face[i]) {
                m_normals.SetElement(i, outward_normal[i]);
            } else {
                m_normals.SetElement(i, -outward_normal[i]);
            }
        }
    }

private:
    std::shared_ptr<Material> m_material{};
    Vec3Group m_hit_points{};
    Vec3Group m_normals{};
    std::vector<float> m_t{};
    std::vector<float> m_hit_mask{};
    std::vector<float> m_front_face{};
    size_t m_size{};
};

class Hittable {
public:
    virtual ~Hittable() = default;

    virtual void hit(const RayGroup& rays, const IntervalGroup& ray_t, HitRecordGroup& records) const = 0;
};