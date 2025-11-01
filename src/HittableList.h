#pragma once

#include "Hittable.h"
#include "Interval.h"
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

    bool hit(const Ray& ray, const Interval& ray_t, HitRecord& record) const override {
        HitRecord temp_record{};
        bool hit_anything{ false };
        double closest_so_far{ ray_t.Max() };

        for (const auto& object : m_objects) {
            Interval new_interval{ ray_t.Min(), closest_so_far };

            if (object->hit(ray, new_interval, temp_record)) {
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