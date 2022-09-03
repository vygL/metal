#include "application.hpp"

Application::Application() {
    _pool = NS::AutoreleasePool::alloc()->init();
    _device = MTLCreateSystemDefaultDevice();
}

Application::~Application() {
    _pool->release();
}

MTL::Device* Application::device() {
    return _device;
}