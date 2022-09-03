#include "compute.hpp"

Compute::Compute() {

}

Compute::Compute(MTL::Device* device, const char* source) {
    pool_ = NS::AutoreleasePool::alloc()->init();
    _device = device;
    setup();

}

void Compute::setup() {
    if (!_device) {
        std::cerr << "Unable to perform setup without first linking device";
        std::exit(-1);
    }

    NS::Error* error = nullptr;
    _library = device->newLibrary(NS::String::string(source, NS::StringEncoding::ASCIIStringEncoding), nullptr, &error);

    _queue = device->newCommandQueue();
    _buffer = _queue->commandBuffer();
    _encoder = _buffer->computeCommandEncoder();
    _desc = ComputePipelineDescriptor::alloc()->init();
    _desc->setLabel("Compute_Pass");
}

bool Compute::attachDevice(MTL::Device* device) {
    _device = device;
    setup();
}

// TODO: To reuse pipelines by setting to different functions, or make one for each?

bool Compute::chooseFunc(std::string label) {
    MTL::Function* addFunction = _library->newFunction(NS::String::string(func_name));
    if (!addFunction) {
        std::cerr << "Failed to link function";
        std::exit(-1);
    }
    _desc->setComputeFunction(addFunction);
}

bool Compute::setArgs(std::vector<auto> args, size_t returnSize) {
    size_t i = 0;
    for (; i < args.size(); ++i) {
        MTL::Buffer* buff = device->newBuffer(sizeof(args[i]), MTL::ResourceStorageModeShared);
        memcpy(buff->contents(), args, sizeof(args[i]));
        // buff->didModifyRange(NS::Range::Make(0, buff->length()));
        _encoder->setBuffer(buff, 0, i);
    }
    _encoder->setBuffer(returnSize, 0, i);
    

}
bool Compute::process() {
    NS::Error* error = nullptr;
    _state = _device->newComputePipelineStateWithDescriptor(_desc, nullptr, nullptr, &error);
    if (!state) {
        std::cerr << "State error";
    }

    MTL::Size gridSize = MTL::Size::Make(1, 1, 1);
    NS::UInteger threadGroupSize = computePipeline->maxTotalThreadsPerThreadgroup();
    MTL::Size threadgroupSize = MTL::Size::Make(threadGroupSize, 1, 1);

    _encoder->setComputePipelineState(_state);
    _encoder->dispatchThreads(gridSize, threadgroupSize);
    _encoder->endEncoding();
}

bool Compute::send() {
    _buffer->commit();

    _buffer->waitUntilCompleted();
}

Compute::~Compute() {
    //TODO: Release all objects
    pool_->release();
}