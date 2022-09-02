#ifndef COMPUTE_H
#define COMPUTE_H
/**
 * Setup for compute methods
 * */


class Compute {
    public:
        Compute(MTL::Device* device);
        ~Compute();

        int add(int a, int b);
    private:
       NS::AutoreleasePool* pool_;
       MTL::Library* _library;
};
#endif