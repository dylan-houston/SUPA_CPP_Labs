#include <iostream>
#include <cmath>

int main() {
    double x = 2.3;
    double y = 4.5;
    
    double mag = std::sqrt(x*x + y*y);
    std::cout << mag << std::endl;
    return 0;
}
