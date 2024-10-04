#include <iostream>
#include "Matrix.h"

int main () {
    std::array<double, 9> nums {1, 2, 3, 4, 5, 6, 7, 8, 9};
    Matrix<double, 3, 3> m {std::move(nums)};
    std::cout << m.to_string() << std::endl;
    std::cout << m(2, 2) << std::endl;
    return 0;
}