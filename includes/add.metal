#include <metal_std_lib>

using namespace metal;

kernel void add(device const float* inA, device const float* inB, 
                device float* result, 
                uint index [[thread_position_in_grid]]) {
    *result = *inA + *inB;
}