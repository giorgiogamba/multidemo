// multidemo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "Renderer.h"

int main() 
{
    std::cout << "Hello World!\n";

    multidemo::Renderer renderer(1080, 920);
    renderer.run();

    return 0;
}
