#pragma once

#include "matrix.hpp"


template<typename T, std::size_t V, typename E = typename std::enable_if<std::is_floating_point<T>::value>::type>
class LinearSystem {
private:
    matrix<T, V, V> A_;
    matrix<T, V, 1> b_;


public:
    constexpr LinearSystem()
        : A_{}
        , b_{}
    {}


    constexpr LinearSystem(const matrix<T, V, V>& A, const matrix<T, V, 1>& b)
        : A_{A}
        , b_{b}
    {}


    constexpr LinearSystem(matrix<T, V, V>&& A, matrix<T, V, 1>&& b)
        : A_{std::move(A)}
        , b_{std::move(b)}
    {}


    constexpr std::string to_string() {
        std::ostringstream result;
        result << std::fixed << std::setprecision(3);
        for (size_t i = 0; i < V; ++i) {
            for (size_t j = 0; j < V; ++j) {
                auto val = A_(i, j);
                result << std::scientific << val << " ";
            }
            result << "| " << b_(i, 0); 
            if (i < V - 1)
                result << "\n";
        }
        return result.str();
    }
};
