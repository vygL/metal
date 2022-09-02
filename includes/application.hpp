#ifndef APPLICATION_H
#define APPLICATION_H

#include "Metal/Metal.hpp"
#include "MetalKit/MetalKit.hpp"

class Application {
    public: 
        Application();
        ~Application();

    private:
        NS::AutoreleasePool* _pool;
        MTL::Device* _device;


};
#endif