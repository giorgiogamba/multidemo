// multidemo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "Renderer.h"

int main()
{
    std::cout << "This application writes on window using multi-threading\n";

    multidemo::Renderer renderer(600, 400);
    renderer.run();

    return EXIT_SUCCESS;
}
