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
    Compute* compute = new Compute(newApp->device(), add_func);

    compute->chooseFunc("add");
    float* float1 = new float();
    float* float2 = new float();
    
    for (int i = 0; i < 2; ++i) {
        float1[i] = i + 1;
        float2[i] = i + 3;
    }

    MTL::Device* d = newApp->device();

    MTL::Buffer* buffer1 = d->newBuffer(sizeof(float_t), MTL::ResourceStorageModeShared);
    MTL::Buffer* buffer2 = d->newBuffer(sizeof(float_t), MTL::ResourceStorageModeShared);
    MTL::Buffer* result = d->newBuffer(sizeof(float_t), MTL::ResourceStorageModeShared);

    memcpy(buffer1->contents(), float1, sizeof(float_t));
    memcpy(buffer2->contents(), float2, sizeof(float_t));
    compute->process();
    compute->send();
    return 0;

}
