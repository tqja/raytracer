#include "Geometry.h"

#include <cstdint>
#include <vector>

#include "Hittable.h"
#include "Interval.h"
#include "Ray.h"
#include "Utility.h"
#include "Vec3.h"
#include "simd/simd.h"

void Sphere::hit(const RayGroup& rays, const IntervalGroup& ray_t, HitRecordGroup& records) const {
    Vec3Group oc{};
    Vec3Group dir = rays.GetDirections();
    oc.SubVecLike(m_center, rays.GetOrigins());
    simd::DispatchBase* dp{ simd::GetDispatch() };

    // calculate the discriminant components
    std::vector<float> a(rays.Size());
    dir.LengthSquared(a.data());

    // h == b/-2 allowing a simplification of the quadratic
    std::vector<float> h(rays.Size());
    dir.Dot(oc, h);

    std::vector<float> c(rays.Size());
    oc.LengthSquared(c.data());
    dp->Sub(c.data(), m_radius * m_radius, rays.Size());

    std::vector<float> ac(rays.Size());
    dp->Mul(a.data(), c.data(), ac.data(), rays.Size());
    std::vector<float> discriminant(rays.Size());
    dp->MulSub(h.data(), h.data(), ac.data(), discriminant.data(), rays.Size());

    // use discriminant to determine intersections
    std::vector<float> hit_mask(rays.Size());
    dp->GreaterEqual(discriminant.data(), 0.0f, hit_mask.data(), rays.Size());
    records.SetHitMask(hit_mask);

    // calculate the roots of the quadratic, h ± sqrt(discriminant) / a
    std::vector<float> sqrt_d{ discriminant };
    dp->Sqrt(discriminant.data(), sqrt_d.data(), rays.Size());

    std::vector<float> roots1(rays.Size());
    dp->Sub(h.data(), sqrt_d.data(), roots1.data(), rays.Size());
    dp->Div(roots1.data(), a.data(), rays.Size());
    std::vector<float> roots2(rays.Size());
    dp->Add(h.data(), sqrt_d.data(), roots2.data(), rays.Size());
    dp->Div(roots2.data(), a.data(), rays.Size());

    std::vector<float> roots1_mask(rays.Size());
    ray_t.Surrounds(roots1.data(), roots1_mask.data());

    // Select from roots1 (closer root) if mask is true, otherwise roots2
    std::vector<float> final_roots(rays.Size());
    dp->BlendVectors(roots1_mask.data(), roots1.data(), roots2.data(), final_roots.data(), rays.Size());

    // update t values for rays that hit
    std::vector<float> t_values(records.GetSize());
    dp->BlendVectors(hit_mask.data(), final_roots.data(), records.GetTValues().data(), t_values.data(), rays.Size());
    records.SetTValues(t_values);

    // update hit_points for rays that hit
    Vec3Group hit_points{ records.GetHitPoints() };
    std::vector<float> no_hit_mask(rays.Size());
    dp->Not(hit_mask.data(), no_hit_mask.data(), rays.Size());
    // if no hit, don't replace hit_points value
    hit_points.BlendVectors(rays.at(final_roots), no_hit_mask);
    records.SetHitPoints(hit_points);

    // calculate normals
    Vec3Group normal_directions{};
    records.GetHitPoints().Sub(m_center, normal_directions);

    Vec3 radius_reciprocal{};
    radius_reciprocal.Broadcast(1.0f / m_radius);

    Vec3Group outward_normals{};
    records.GetHitPoints().Sub(m_center, outward_normals);
    outward_normals.Mul(radius_reciprocal);

    records.SetFaceNormal(rays, outward_normals);
    records.SetMaterial(m_material);
}