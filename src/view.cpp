#include "view.hpp"

View::View() {
    NS::AutoReleasePool* _pool = NS::AutoReleasePool::alloc()->init();
    _view = MTL::View::alloc()->init(frame, device);
}
View::~View() {
    _pool->release();
}