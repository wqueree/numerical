#pragma once

#include "Matrix.hpp"


template<typename T, std::size_t V, typename E = typename std::enable_if<std::is_floating_point<T>::value>::type>
class LinearSystem {
private:
    Matrix<T, V, V> A_;
    Matrix<T, V, 1> b_;


public:
    constexpr LinearSystem()
        : A_{}
        , b_{}
    {}


    constexpr LinearSystem(const Matrix<T, V, V>& A, const Matrix<T, V, 1>& b)
        : A_{A}
        , b_{b}
    {}


    constexpr LinearSystem(const Matrix<T, V, V>&& A, const Matrix<T, V, 1>&& b)
        : A_{std::move(A)}
        , b_{std::move(b)}
    {}


};
