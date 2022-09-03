#include "compute.hpp"

Compute::Compute() {

}

Compute::Compute(MTL::Device* device, const char* libSource) {
    pool_ = NS::AutoreleasePool::alloc()->init();
    _device = device;
    setup(libSource);

}

void Compute::setup(const char* libSource) {
    if (!_device) {
        std::cerr << "Unable to perform setup without first linking device";
        std::exit(-1);
    }

    NS::Error* error = nullptr;
    _library = _device->newLibrary(NS::String::string(libSource, NS::StringEncoding::ASCIIStringEncoding), nullptr, &error);

    _queue = _device->newCommandQueue();
    _buffer = _queue->commandBuffer();
    _encoder = _buffer->computeCommandEncoder();
    _desc = MTL::ComputePipelineDescriptor::alloc()->init();
    _desc->setLabel(NS::String::string("Compute_Pass", NS::StringEncoding::ASCIIStringEncoding));
}

bool Compute::attachDevice(MTL::Device* device, const char* libSource) {
    _device = device;
    setup(libSource);

    return 1;
}

// TODO: To reuse pipelines by setting to different functions, or make one for each?

bool Compute::chooseFunc(const char* functionName) {
    MTL::Function* addFunction = _library->newFunction(NS::String::string(functionName, NS::StringEncoding::ASCIIStringEncoding));
    if (!addFunction) {
        std::cerr << "Failed to link function";
        std::exit(-1);
    }
    _desc->setComputeFunction(addFunction);

    return 1;
}

template <typename T>
MTL::Buffer* Compute::setArgs(std::vector<T>& args, size_t returnSize) {
    size_t i = 0;
    for (; i < args.size(); ++i) {
        MTL::Buffer* buff = _device->newBuffer(sizeof(args[i]), MTL::ResourceStorageModeShared);
        memcpy(buff->contents(), args[i], sizeof(T));
        // buff->didModifyRange(NS::Range::Make(0, buff->length()));
        _encoder->setBuffer(buff, 0, i);
    }
    // Change to return buffer
    MTL::Buffer* result = _device->newBuffer(sizeof(returnSize), MTL::ResourceStorageModeShared);
    _encoder->setBuffer(result, 0, i);
    
    return result;
}

bool Compute::process() {
    NS::Error* error = nullptr;
    _state = _device->newComputePipelineState(_desc, MTL::PipelineOptionNone, nullptr, &error);
    if (!_state) {
        std::cerr << "State error";
    }

    MTL::Size gridSize = MTL::Size::Make(1, 1, 1);
    NS::UInteger threadGroupSize = _state->maxTotalThreadsPerThreadgroup();
    MTL::Size threadgroupSize = MTL::Size::Make(threadGroupSize, 1, 1);

    _encoder->setComputePipelineState(_state);
    _encoder->dispatchThreads(gridSize, threadgroupSize);
    _encoder->endEncoding();

    return 1;
}

bool Compute::send() {
    _buffer->commit();

    _buffer->waitUntilCompleted();

    return 1;
}

Compute::~Compute() {
    //TODO: Release all objects
    pool_->release();
}