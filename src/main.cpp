#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#include <Metal/Metal.hpp>
#include <string.h>
#include <iostream>

int main() {

    // Currently, add 2 numbers together
    MTL::Device* device = MTLCreateSystemDefaultDevice();

    std::cout << "hi" << std::endl;
    NS::Error* error_lib = nullptr;
    const char* add_func = R"(
    #include <metal_stdlib>

    using namespace metal;

    kernel void add(device const float* inA, device const float* inB, device float* result, uint index [[thread_position_in_grid]]) {
    *result = *inA;
}
)";
    //NS::String* filePath = NS::String::alloc()->string("includes/add.metal", NS::StringEncoding::ASCIIStringEncoding);
    MTL::Library* funcLibrary = device->newLibrary(NS::String::string(add_func, NS::StringEncoding::UTF8StringEncoding), nullptr, &error_lib);
    if (!funcLibrary) {
        std::cout << "error lib: " << error_lib->localizedDescription()->cString(NS::StringEncoding::ASCIIStringEncoding) << std::endl;
    }
    NS::String* func_str = NS::String::alloc()->string("add", NS::StringEncoding::UTF8StringEncoding);
    NS::String* funcName = NS::String::string(func_str);
    MTL::Function* addFunction = funcLibrary->newFunction(funcName);
    if (addFunction == nullptr) {
        throw std::runtime_error("Failed to find the function.");
    }

    NS::Error* error = nullptr;
    MTL::ComputePipelineState* computePipeline = device->newComputePipelineState(addFunction, &error);
    //std::cout << error->code() << std::endl;
    MTL::CommandQueue* cmdqueue = device->newCommandQueue();

    NS::UInteger bufferSize = device->maxBufferLength();
    MTL::Buffer* buf1 = device->newBuffer(bufferSize, MTL::ResourceStorageModeShared);
    MTL::Buffer* buf2 = device->newBuffer(bufferSize, MTL::ResourceStorageModeShared);
    MTL::Buffer* resultBuff = device->newBuffer(bufferSize, MTL::ResourceStorageModeShared);
    //std::make_shared<float>(one);
    //std::make_shared<float>(two);

    *(float*)(buf1->contents()) = 1.0;
    *(float*)(buf2->contents()) = 2.0;
    //memcpy(buf1->contents(), one, sizeof(NS::Integer));
    //memcpy(buf2->contents(), two, sizeof(NS::Integer));

    MTL::CommandBuffer* cmdbuf = cmdqueue->commandBuffer();

    MTL::ComputeCommandEncoder* computeEncoder = cmdbuf->computeCommandEncoder();

    computeEncoder->setComputePipelineState(computePipeline);
    computeEncoder->setBuffer(buf1, 0, 0);
    computeEncoder->setBuffer(buf2, 0, 1);
    computeEncoder->setBuffer(resultBuff, 0, 2);

    MTL::Size gridSize = MTL::Size::Make(1, 1, 1);

    NS::UInteger threadGroupSize = computePipeline->maxTotalThreadsPerThreadgroup();

    MTL::Size threadgroupSize = MTL::Size::Make(threadGroupSize, 1, 1);

    computeEncoder->dispatchThreads(gridSize, threadgroupSize);
    computeEncoder->endEncoding();

    cmdbuf->commit();

    float* resultContents = new float();
    memcpy(resultContents, resultBuff->contents(), sizeof(float));
    std::cout << *resultContents << std::endl;

    delete resultContents;
    resultContents = nullptr;

    buf1->release();
    buf2->release();
    resultBuff->release();
    computeEncoder->release();
    cmdbuf->release();
    cmdqueue->release();
    computePipeline->release();
    addFunction->release();
    funcLibrary->release();
    device->release();
    return 0;
}
