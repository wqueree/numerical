#include <iostream>
#include "Matrix.h"

int main () {
    std::array<double, 4> nums {1, 2, 3, 4};
    Matrix<double, 2, 2> m {std::move(nums)};
    std::cout << m.to_string() << std::endl;
    return 0;
}