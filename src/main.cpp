#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#include <Metal/Metal.hpp>

int main() {
    MTL::Device* device = MTLCreateSystemDefaultDevice();
    device->release();
    return 0;
}