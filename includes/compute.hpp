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

        void setup(MTL::Device* device);
        bool attachDevice(MTL::Device* device) const;
        bool addFunction(std::string func_name) const;
        std::vector<MTL::Buffer*> setArgs(std::vector<auto> args)
bool    bool process

bool    bool send
        int add(int a, int b);
    private:
       NS::AutoreleasePool* pool_;
       MTL::Library* _library;
};
#endif