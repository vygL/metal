#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#include <Metal/Metal.hpp>
#include <string.h>
#include <iostream>

int main() {

    // Currently, add 2 numbers together
    MTL::Device* device = MTLCreateSystemDefaultDevice();


    // Creating the function library, and loading functions into it.
    // Xcode loads .metal shaders to a defaultLibrary, but without it, file loading seems to not work.
    // Loading from raw string is the solution I found to it.
    NS::Error* error_lib = nullptr;
    const char* add_func = R"(
    #include <metal_stdlib>

    using namespace metal;

    kernel void add(device const float* inA, device const float* inB, device float* result, uint index [[thread_position_in_grid]]) {
        float two = 2.0;
        *result = *inA + *inB;
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

    // Creating the pipeline. Using different pipelines can load multiple functions to use when necessary.
    // TODO: A Descriptor is another option that can be used in the parameter, can specify and link buffers early.
    NS::Error* error = nullptr;
    MTL::ComputePipelineState* computePipeline = device->newComputePipelineState(addFunction, &error);
    std::cout << "error code: " << error->code() << std::endl;
    MTL::CommandQueue* cmdqueue = device->newCommandQueue();

    // Creating the buffers.
    const size_t floatSize = sizeof(float);
    NS::UInteger bufferSize = device->maxBufferLength();

    float* one = new float(1.0);
    float* two = new float(3.0);
    const size_t buf1Size = sizeof(one);
    const size_t buf2Size = sizeof(two);
    MTL::Buffer* buf1 = device->newBuffer(buf1Size, MTL::ResourceStorageModeShared);
    MTL::Buffer* buf2 = device->newBuffer(buf2Size, MTL::ResourceStorageModeShared);
    MTL::Buffer* resultBuff = device->newBuffer(bufferSize, MTL::ResourceStorageModeShared);
    //std::make_shared<float>(one);
    //std::make_shared<float>(two);

    memcpy(buf1->contents(), one, buf1Size);
    memcpy(buf2->contents(), two, buf2Size);


    MTL::CommandBuffer* cmdbuf = cmdqueue->commandBuffer();

    MTL::ComputeCommandEncoder* computeEncoder = cmdbuf->computeCommandEncoder();

    // Setting the function to execute, along with associating argument indexes to buffers.
    computeEncoder->setComputePipelineState(computePipeline);
    computeEncoder->setBuffer(buf1, 0, 0);
    computeEncoder->setBuffer(buf2, 0, 1);
    computeEncoder->setBuffer(resultBuff, 0, 2);

    //buf1->didModifyRange(NS::Range::Make(0, buf1->length()));
    //uf2->didModifyRange(NS::Range::Make(0, buf2->length()));

    MTL::Size gridSize = MTL::Size::Make(1, 1, 1);

    NS::UInteger threadGroupSize = computePipeline->maxTotalThreadsPerThreadgroup();

    MTL::Size threadgroupSize = MTL::Size::Make(threadGroupSize, 1, 1);

    // Formatting threads for execution
    computeEncoder->dispatchThreads(gridSize, threadgroupSize);
    computeEncoder->endEncoding();

    // Sends the command down to the GPU to execute
    cmdbuf->commit();

    // Important
    // Wait for the GPU to finish comuptation, then continue. Otherwise reading the value will only return 0.
    cmdbuf->waitUntilCompleted();

    float* resultContents = new float();
    memcpy(resultContents, resultBuff->contents(), floatSize);
    std::cout << "result: " << resultContents[0] << std::endl;

    delete resultContents;
    resultContents = nullptr;

    // Releasing the objects using built-in functions
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
