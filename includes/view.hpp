#ifndef VIEW_H
#define VIEW_H

#include "Metal/Metal.hpp"
#include "MetalKit/MetalKit.hpp"
class View {
    public:
        View();
        ~View();
    private:
        NS::AutoReleasePool* _pool;
        MTK::View* _view;
};
#endif