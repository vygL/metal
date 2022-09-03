#ifndef COMPUTE_H
#define COMPUTE_H
/**
 * Setup for compute methods
 * */

#include <iostream>

// TODO: Setup indirect command buffer. So commands can be reused.
class Compute {
    public:
        Compute();
        Compute(MTL::Device* device, const char* source);
        ~Compute();

        void setup(const char* libSource);
        bool attachDevice(MTL::Device* device, const char* libSource);

        bool chooseFunc(const char* functionName);


        template<typename T>
        MTL::Buffer* setArgs(std::vector<T>& args, size_t returnSize);
        bool process();

        bool send();
        int add(int a, int b);
    private:
        MTL::Device* _device;

        NS::AutoreleasePool* pool_;
        MTL::Library* _library;
        MTL::CommandQueue* _queue;
        MTL::CommandBuffer* _buffer;
        MTL::ComputeCommandEncoder* _encoder;

        MTL::ComputePipelineState* _state;
        MTL::ComputePipelineDescriptor* _desc;
};
#endif