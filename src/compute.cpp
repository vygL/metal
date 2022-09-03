#include "compute.hpp"

Compute::Compute() {

}

Compute::Compute(MTL::Device* device, const char* source) {
    pool_ = NS::AutoreleasePool::alloc()->init();

    if (!device) {
        std::cerr << "Device not set up yet";
    } else {
        setup(device);
    }

}

void Compute::setup(MTL::Device* device) {
    NS::Error* error = nullptr;
    _library = device->newLibrary(NS::String::string(source, NS::StringEncoding::ASCIIStringEncoding), nullptr, &error);

    MTL::CommandQueue* q = device->newCommandQueue();
}

bool Compute::attachDevice(MTL::Device* device) {

}

// TODO: To reuse pipelines by setting to different functions, or make one for each?
bool Compute::addFunction(std::string func_name) const {
    MTL::Function* addFunction = _library->newFunction(NS::String::string(func_name));
    if (!addFunction) {
        std::cerr << "Failed to link function";
        std::exit(-1);
    }

    MTL::ComputePipelineDescriptor* desc = ComputePipelineDescriptor::alloc()->init();
    desc->setLabel(func_name);
    desc->setComputeFunction(addFunction);
}

bool setArgs(std::vector<auto> args)
bool process

bool send
Compute::~Compute() {
    pool_->release();
}