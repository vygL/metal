#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#include "shaderfunctions.hpp"
#include "application.cpp"
#include "compute.cpp"
#include <Metal/Metal.hpp>
#include <string.h>
#include <iostream>
#include <vector>
#include <variant>

int main() {
    Application* newApp = new Application();
    Compute* compute = new Compute(newApp->device(), add_func);

    compute->chooseFunc("add");
    std::vector<float*>* args = new std::vector<float*>{ new float(1.0),
                                                        new float(2.0)};
    compute->setArgs(*args, sizeof(float_t));
    compute->process();
    compute->send();
    return 0;

}
