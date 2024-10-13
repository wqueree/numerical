#include <iostream>
#include "Matrix.hpp"

int main () {
    std::array<std::array<double, 3>, 5> am0 {{
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9},
        {1, 2, 3},
        {4, 5, 0}
    }};

    Matrix<double, 5, 3> m0 {std::move(am0)};

    std::array<std::array<double, 3>, 5> am1 {{
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9},
        {1, 2, 3},
        {4, 5, 6}
    }};

    Matrix<double, 5, 3> m1 {std::move(am1)};

    Matrix<double, 2, 2> m;

    std::array<std::array<double, 4>, 3> am2 {{
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 1, 2, 3}
    }};

    Matrix<double, 3, 4> m2 {std::move(am2)};

    // auto m3 = m1 * m2;
    // std::cout << m3.to_string() << std::endl;

    std::array<std::array<double, 2>, 2> am4 {{
        {1, 2},
        {5, 6}

    }};

    Matrix<double, 2, 2> m4 {std::move(am4)};

    // std::cout << m4.determinant() << std::endl;

    auto m5 { m4.transpose() };

    return 0;
}
