#ifndef SHADERFUNCTIONS_H
#define SHADERFUNCTIONS_H

/**
 * Header File for shader functions. Functions are formatted in the form of a raw string, because I can't get .metal to work when linking to a library.
 * Default library may work with xcode because it will build automatically, but with Cmake I have had no success so far.
 * */

const char* add_func = R"(
    #include <metal_stdlib>

    using namespace metal;

    kernel void add(device const float* inA, device const float* inB, device float* result, uint index [[thread_position_in_grid]]) {
        float two = 2.0;
        *result = *inA + *inB;
}
)";
#endif