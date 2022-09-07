#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#include "shaderfunctions.hpp"
#include "application.cpp"
#include "compute.cpp"
#include <Metal/Metal.hpp>
#include <string.h>
#include <iostream>
#include <vector>
#include <variant>

int main() {
    Application* newApp = new Application();
    Compute* compute = new Compute(newApp->device(), add_func, sizeof(float_t));

    compute->chooseFunc("add");
    float* float1 = new float();
    float* float2 = new float();
    
    for (int i = 0; i < 3; ++i) {
        float1[i] = i + 1;
        float2[i] = i + 3;
    }

    MTL::Device* d = newApp->device();

    MTL::Buffer* buffer1 = d->newBuffer(sizeof(d->maxBufferLength()), MTL::ResourceStorageModeShared);
    MTL::Buffer* buffer2 = d->newBuffer(sizeof(d->maxBufferLength()), MTL::ResourceStorageModeShared);
    MTL::Buffer* result = d->newBuffer(sizeof(d->maxBufferLength()), MTL::ResourceStorageModeShared);

    memcpy(buffer1->contents(), float1, sizeof(d->maxBufferLength()));
    memcpy(buffer2->contents(), float2, sizeof(d->maxBufferLength()));
    MTL::ComputeCommandEncoder* enc = compute->encoder();

    enc->setBuffer(buffer1, 0, 0);
    enc->setBuffer(buffer2, 0, 1);
    enc->setBuffer(result, 0, 2);

    compute->process();
    compute->send();

    float* result_float = (float *)result->contents();

    for (int i = 0; i < 3; ++i) {
        std::cout << result_float[i] << std::endl;
    }
    return 0;

}
