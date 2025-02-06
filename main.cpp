#include <iostream>
#include <libnumerical/Matrix.hpp>

int main()
{
    std::array<std::array<double, 3UL>, 3UL> am3 {{
        {1, 5, 4},
        {2, 0, 3},
        {5, 8, 2}
    }};

    Matrix<double, 3, 3> m3 {std::move(am3)};

    Vector<double, 3UL> b {{12, 9, 5}};

    Vector<double, 3UL> x = m3.lup_solve(b);

    std::cout << x.to_string() << "\n";

    return 0;
}
