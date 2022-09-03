#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#include "shaderfunctions.hpp"
#include "application.cpp"
#include "compute.cpp"
#include <Metal/Metal.hpp>
#include <string.h>
#include <iostream>

int main() {
    Application* newApp = new Application();
    Compute* compute = new Compute(newApp->device(), add_func);
    return 0;

}
