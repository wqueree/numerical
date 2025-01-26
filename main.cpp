#include <iostream>
#include "Matrix.hpp"
#include "LinearSystem.hpp"

int main()
{
    std::array<std::array<double, 3>, 3> am3 {{
        {1, 5, 4},
        {2, 0, 3},
        {5, 8, 2}
    }};

    Matrix<double, 3, 3> m3 {std::move(am3)};

    std::array<std::array<double, 4>, 4> am4 {{
        {2, 0, 2, 0.6},
        {3, 3, 4, -2},
        {5, 5, 4, 2},
        {-1, -2, 3.4, -1}
    }};

    Matrix<double, 4, 4> m4 {std::move(am4)};

    // auto m4s = m4.lup_solve();

    // std::cout << m4s.to_string() << "\n";
    Matrix<double, 3, 1> b {{12, 9, 5}};
    auto x = m3.lup_solve(b);

    std::cout << x.to_string() << "\n";

    return 0;
}
