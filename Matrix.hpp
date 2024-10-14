#pragma once

#include <algorithm>
#include <array>
#include <string>
#include <memory>
#include <cmath>
#include <limits>
#include <iomanip>
#include <type_traits>
#include <unordered_map>


template <typename T, std::size_t M, std::size_t N, typename E = typename std::enable_if<std::is_floating_point<T>::value>::type>
class Matrix {
private:
    std::array<std::array<T, N>, M> elements_;


public:
    constexpr Matrix()
        : elements_{} 
    {}


    constexpr Matrix(const std::array<std::array<T, N>, M>& elements)
        : elements_{elements}
    {}


    constexpr Matrix(const std::array<std::array<T, N>, M>&& elements)
        : elements_{std::move(elements)}
    {}


    T& operator()(std::size_t row, std::size_t col) {
        return elements_[row][col];
    }


    const T& operator()(std::size_t row, std::size_t col) const {
        return elements_[row][col];
    }


    constexpr bool operator==(const Matrix<T, M, N>& other) {
        for (size_t i = 0; i < M; ++i) {
            for (size_t j = 0; j < N; ++j) {
                if (fabs(elements_[i][j] - other.elements_[i][j]) > std::numeric_limits<T>::epsilon())
                    return false;
            }
        }
        return true;
    }


    constexpr Matrix<T, M, N> operator+(const Matrix<T, M, N>& other) {
        Matrix<T, M, N> sum;
        for (size_t i = 0; i < M; ++i) {
            for (size_t j = 0; j < N; ++j) {
                sum.elements_[i][j] = elements_[i][j] + other.elements_[i][j];
            }
        }
        return sum;
    }


    constexpr Matrix<T, M, N> operator-(const Matrix<T, M, N>& other) {
        Matrix<T, M, N> sum;
        std::array<std::array<T, N>, M> sum_array {};
        for (size_t i = 0; i < M; ++i) {
            for (size_t j = 0; j < N; ++j) {
                sum.elements_[i][j] = elements_[i][j] - other.elements_[i][j];
            }
        }
        return sum;
    }


    template <std::size_t P>
    constexpr Matrix<T, M, P> operator*(const Matrix<T, N, P>& other) {
        Matrix<T, M, P> product;
        for (size_t i = 0; i < M; ++i) {
            for (size_t j = 0; j < P; ++j) {
                T dot_product = 0;
                for (size_t k = 0; k < N; ++k) {
                    dot_product += (*this)(i, k) * other(k, j);
                }
                product(i, j) = dot_product;
            }
        }
        return product;
    }


    template <typename _T = T, std::size_t _M = M, std::size_t _N = N>
    constexpr typename std::enable_if<(_M == _N), _T>::type determinant() {
        return 0.0;
    }


    constexpr Matrix<T, N, M> transpose() {
        Matrix<T, N, M> _transpose;
        for (size_t i = 0; i < M; ++i) {
            for (size_t j = 0; j < N; ++j) {
                _transpose.elements_[j][i] = elements_[i][j];
            }
        }
        return _transpose;
    }


    constexpr void permute(std::array<T, M> permutation) {
        std::size_t swaps = 0;
        std::size_t next = 0;
        while (swaps < M - 1) {
            std::swap(elements_[next], elements_[permutation[next]]);
            next = permutation[next];
            ++swaps;
        }
    }


    constexpr bool square() {
        return M == N;
    }


    constexpr std::string to_string() {
        std::ostringstream result;
        result << std::fixed << std::setprecision(3);
        for (size_t i = 0; i < M; ++i) {
            for (size_t j = 0; j < N; ++j) {
                result << std::scientific << elements_[i][j];
                if (j < N - 1)
                    result << " ";
            }
            if (i < M - 1)
                result << "\n";
        }
        return result.str();
    }


};
