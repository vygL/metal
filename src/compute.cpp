#include "compute.hpp"

Compute::Compute(MTL::Device* device) {
    pool_ = NS::AutoreleasePool::alloc()->init();
    _library = defaultLibrary();
    if (!defaultLibrary()) {
        std::cerr("Failed to load library");
        std::exit(-1);
    }
}

Compute::~Compute() {
    pool_->release();
}