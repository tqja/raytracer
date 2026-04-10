#pragma once

#include <memory>
#include <vector>

#include "Hittable.h"
#include "Interval.h"
#include "Ray.h"
#include "Utility.h"
#include "simd/simd.h"

using std::make_shared;
using std::shared_ptr;

class HittableList : public Hittable {
public:
    HittableList() {}
    HittableList(std::shared_ptr<Hittable> object) { add(object); }

    void clear() { m_objects.clear(); }
    void add(shared_ptr<Hittable> object) { m_objects.push_back(object); }

    void hit(const RayGroup& rays, const IntervalGroup& ray_t, HitRecordGroup& records) const override {
        simd::DispatchBase* dp{ simd::GetDispatch() };
        HitRecordGroup temp_records{ rays.Size() };
        std::vector<float> hit_anything(rays.Size());
        std::vector<float> closest_so_far{ ray_t.Max() };

        for (const auto& object : m_objects) {
            IntervalGroup new_intervals{ ray_t.Min(), closest_so_far };
            object->hit(rays, new_intervals, temp_records);

            // update hit_anything from hits in temp_records
            dp->Or(hit_anything.data(), temp_records.GetHitMask().data(), globals::samples);

            // update closest_so_far for rays that hit in this iteration
            dp->BlendVectors(temp_records.GetTValues().data(), closest_so_far.data(), temp_records.GetHitMask().data(),
                             globals::samples);
            records = temp_records;
        }

        records.SetHitMask(hit_anything);
    }

private:
    std::vector<std::shared_ptr<Hittable>> m_objects;
};