#pragma once

#include "Hittable.h"
#include "Ray.h"

#include <memory>
#include <vector>

using std::make_shared;
using std::shared_ptr;

class HittableList : public Hittable {
public:

    HittableList() {}
    HittableList(std::shared_ptr<Hittable> object) { add(object); }

    void clear() { m_objects.clear(); }
    void add(shared_ptr<Hittable> object) { m_objects.push_back(object); }

    bool hit(const Ray& ray, double ray_tmin, double ray_tmax, HitRecord& record) const override {
        HitRecord temp_record{};
        bool hit_anything{ false };
        double closest_so_far{ ray_tmax };

        for (const auto& object : m_objects) {
            if (object->hit(ray, ray_tmin, closest_so_far, temp_record)) {
                hit_anything = true;
                closest_so_far = temp_record.t;
                record = temp_record;
            }
        }

        return hit_anything;
    }

private:
    std::vector<std::shared_ptr<Hittable>> m_objects;
};