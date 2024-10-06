#include <iostream>
#include "Matrix.h"

int main () {
    std::array<std::array<double, 3>, 3> nums {{
        {1, 2, 3},
        {4, 50000, 6},
        {0.0007, 8, 9}
    }};
    Matrix<double, 3, 3> m {std::move(nums)};
    std::cout << m.to_string() << std::endl;
    std::cout << m(2, 2) << std::endl;

    auto m2 = m.transpose();
    std::cout << m2.to_string() << std::endl;
    return 0;
}
