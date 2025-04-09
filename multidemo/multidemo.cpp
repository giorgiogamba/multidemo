// multidemo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "Renderer.h"

int main()
{
    std::cout << "This application writes on window using multi-threading\n";

    try {
        multidemo::Renderer renderer(600, 400);
        renderer.run();
    }
    catch (std::exception e)
    {
        std::cout << "Error while executing application: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
