# raytracer
A C++ path tracer implementation based on Peter Shirley's [_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html). Renders scenes with spheres of varying size and materials.

<img width="1200" height="675" alt="rtiow" src="https://github.com/user-attachments/assets/11de5d58-754b-4370-8128-b1d59726bc35" />

*~480 spheres, 500 samples per pixel, ray bounce depth = 20*

<img width="1920" height="1080" alt="rtiow12" src="https://github.com/user-attachments/assets/4aab0dfc-d5ce-48bd-8532-6a3f0e670843" />

*Hollow glass sphere, diffuse sphere, fuzzed metallic sphere, 500spp, bounce depth = 20*

## Features
- Adjustable camera (FoV, position, depth of field)
- Lambertian, Metallic, Dielectric material types
- Antialiasing
- Fuzzy reflection

## SIMD implementation
After finishing my base implementation, I attempted to optimise the path tracer using SIMD (single input, multiple data) intrinsics through Google's [**Highway**](https://github.com/google/highway) library. For antialiasing, multiple samples are computed for each pixel; because the calculations on these samples are identical, SIMD processing can be leveraged to process these samples in parallel.

Instead of iterating through each sample individually, all samples for a given pixel are generated simultaneously and stored in a grouped data structure. This data structure is fed into the SIMD functions, which will perform the operations in batches of N, where N is the number of lanes:

```C++
HWY_ATTR float ReduceSum(float* HWY_RESTRICT in, const size_t count) const override {
    hn::ScalableTag<float> d{};
    const size_t N{ hn::Lanes(d) };

    auto sum{ hn::Zero(d) };

    // process data in batches of N and accumulate result in sum
    size_t i{ 0 };
    if (count >= N) {
        for (; i <= count - N; i += N) {
            hn::Vec<decltype(d)> vec{ hn::LoadU(d, in + i) };
            sum = hn::Add(sum, vec);
        }
    }

    if (HWY_UNLIKELY(i == count)) return hn::ReduceSum(d, sum);

    // process the tail values if any remain
    const size_t remaining{ count - i };
    HWY_DASSERT(0 != remaining && remaining < N);
    const hn::Vec<decltype(d)> vec = hn::LoadN(d, in + i, remaining);
    sum = hn::Add(sum, vec);

    return hn::ReduceSum(d, sum);
}
```

The lane count **N** is the number of values that can fit in the vector register for the given data type d (in this case, float). For the SIMD implementation I chose to trade the precision of double for the smaller size of float, as it allows twice the number of values to fit into the SIMD register at once.

The implementation can be accessed via the "simd" branch, but it is currently a work in progress. While it is able to render an image, it is not yet as fast as I'd hoped and has rendering issues related to the material type. I plan to revisit this in the future to remedy these issues.
