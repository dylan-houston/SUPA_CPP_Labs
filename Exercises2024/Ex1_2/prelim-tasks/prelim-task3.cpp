#include <iostream>
#include <cmath>
#include <string.h>

int main() {
    double x;
    double y;

    std::string inp;

    std::cout << "Enter x: ";
    std::cin >> inp;
    x = std::stof(inp);

    std::cout << "Enter y: ";
    std::cin >> inp;
    y = std::stof(inp);

    double mag = std::sqrt(x*x + y*y);
    std::cout << mag << std::endl;
    return 0;
}
