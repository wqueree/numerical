#pragma once

#include <algorithm>
#include <array>
#include <string>
#include <memory>
#include <cmath>
#include <limits>
#include <numeric>
#include <iomanip>
#include <type_traits>
#include <iosfwd>
#include <tuple>

template <typename T, std::size_t M, std::size_t N, typename E = typename std::enable_if<std::is_floating_point<T>::value>::type>
class Matrix;

template <typename T, std::size_t M>
using Vector = Matrix<T, M, 1>;

template <typename T, std::size_t M, std::size_t N, typename E>
class Matrix {
private:
    std::array<std::array<T, N>, M> elements_;

    mutable bool determinant_cache_valid_ = false;
    mutable T determinant_cache_ = 0;
    mutable bool lup_decomposition_cache_valid_ = false;
    mutable std::shared_ptr<Matrix<T, M, N>> lower_;
    mutable std::shared_ptr<Matrix<T, M, N>> upper_;
    mutable std::array<std::size_t, M> permutation_;


    template <std::size_t _M, std::size_t _N>
    struct is_square : std::integral_constant<bool, _M == _N> {};


    template <std::size_t _M, std::size_t _N, typename _T>
    using enable_if_square = std::enable_if<is_square<_M, _N>::value, _T>;


    template <std::size_t _M, std::size_t _N, typename _T>
    using enable_if_square_v = typename enable_if_square<_M, _N, _T>::value;


    template <std::size_t _M, std::size_t _N, typename _T>
    using enable_if_square_t = typename enable_if_square<_M, _N, _T>::type;


    void invalidate_caches() {
        determinant_cache_valid_ = false;
        lup_decomposition_cache_valid_ = false;
    }


    template <typename _T = T, std::size_t _M = M, std::size_t _N = N>
    constexpr typename enable_if_square<_M, _N, void>::type lup_decompose() const {
        Matrix<T, M, N> A {*this};
        for (std::size_t k = 0; k < M; ++k)
        {
            T p = 0;
            std::size_t l = 0;
            for (std::size_t i = k; i < M; ++i)
            {
                T q = std::abs(A(i, k));
                if (q > p)
                {
                    p = q;
                    l = i;
                }
            }
            std::swap(permutation_[k], permutation_[l]);
            for (std::size_t i = 0; i < M; ++i) std::swap(A(k, i), A(l, i));
            for (std::size_t i = k + 1; i < M; ++i)
            {
                A(i, k) /= A(k, k);
                for (std::size_t j = k + 1; j < M; ++j)
                    A(i, j) -= A(i, k) * A(k, j);
            }

            lower_ = std::make_shared<Matrix<T, M, N>>(std::move(Matrix<T, M, N>::identity()));
            for (std::size_t i = 1; i < M; ++i)
                for (std::size_t j = 0; j < i; ++j)
                    (*lower_)(i, j) = A(i, j);

            upper_ = std::make_shared<Matrix<T, M, N>>();
            for (std::size_t i = 0; i < M; ++i)
                for (std::size_t j = i; j < N; ++j)
                    (*upper_)(i, j) = A(i, j);
        }
        lup_decomposition_cache_valid_ = true;
    }


public:
    Matrix()
        : elements_{{}}
        , lower_{nullptr}
        , upper_{nullptr}
    {
        std::iota(std::begin(permutation_), std::end(permutation_), 0);
    }


    Matrix(const Matrix<T, M, N>& other)
        : elements_{other.elements_}
        , lower_{other.lower_}
        , upper_{other.upper_}
        , permutation_{other.permutation_}
    {}

    Matrix(const Matrix<T, M, N>&& other)
        : elements_{std::move(other.elements_)}
        , lower_{std::move(other.lower_)}
        , upper_{std::move(other.upper_)}
        , permutation_{std::move(other.permutation_)}
    {}


    Matrix(const std::array<std::array<T, N>, M>& elements)
        : elements_{elements}
        , lower_{nullptr}
        , upper_{nullptr}
    {
        std::iota(std::begin(permutation_), std::end(permutation_), 0);
    }


    Matrix(std::array<std::array<T, N>, M>&& elements)
        : elements_{std::move(elements)}
        , lower_{nullptr}
        , upper_{nullptr}
        , permutation_{}
    {
        std::iota(std::begin(permutation_), std::end(permutation_), 0);
    }


    template <typename _T = T, std::size_t _M = M, std::size_t _N = N>
    static typename enable_if_square<_M, _N, Matrix<_T, _M, _N>>::type identity() {
        Matrix<_T, _M, _N> _identity;
        for (size_t i = 0; i < _M; ++i)
            _identity(i, i) = _T(1);
        return _identity;
    }


    constexpr T& operator()(std::size_t row, std::size_t col) {
        if (row >= M || col >= N) {
            throw std::out_of_range("Matrix indices out of bounds");
        }
        invalidate_caches();
        return elements_[row][col];
    }

    constexpr const T operator()(std::size_t row, std::size_t col) const {
        if (row >= M || col >= N) {
            throw std::out_of_range("Matrix indices out of bounds");
        }
        return elements_[row][col];
    }


    bool operator==(const Matrix<T, M, N>& other) const {
        for (size_t i = 0; i < M; ++i) {
            for (size_t j = 0; j < N; ++j) {
                if ((*this)(i, j) != other(i, j)) {
                    return false;
                }
            }
        }
        return true;
    }


    Matrix<T, M, N> operator+(const Matrix<T, M, N>& other) {
        Matrix<T, M, N> sum;
        for (size_t i = 0; i < M; ++i) {
            for (size_t j = 0; j < N; ++j) {
                sum(i, j) = (*this)(i, j) + other(i, j);
            }
        }
        return sum;
    }

    Matrix<T, M, N> operator-(const Matrix<T, M, N>& other) {
        Matrix<T, M, N> sum;
        for (size_t i = 0; i < M; ++i) {
            for (size_t j = 0; j < N; ++j) {
                sum(i, j) = (*this)(i, j) - other(i, j);
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
    constexpr typename std::enable_if<is_square<_M, _N>::value && _M == 1, _T>::type determinant() {
        return (*this)(0, 0);
    }


    template <typename _T = T, std::size_t _M = M, std::size_t _N = N>
    constexpr typename std::enable_if<is_square<_M, _N>::value && _M == 2, _T>::type determinant() {
        return (*this)(0, 0) * (*this)(1, 1) - (*this)(0, 1) * (*this)(1, 0);
    }


    template <typename _T = T, std::size_t _M = M, std::size_t _N = N>
    constexpr typename std::enable_if<is_square<_M, _N>::value && (_M > 2), _T>::type determinant() {
        if (determinant_cache_valid_) return determinant_cache_;

        _T _determinant = 0;
        for (std::size_t i = 0; i < _M; ++i) {
            std::array<std::array<_T, _N - 1>, _M - 1> sub_elements{};
            for (std::size_t j = 1; j < _M; ++j) {
                std::size_t sub_column = 0;
                for (std::size_t k = 0; k < _M; ++k) {
                    if (k != i) {
                        sub_elements[j - 1][sub_column] = (*this)(j, k);
                        ++sub_column;
                    }
                }
            }
            Matrix<_T, _M - 1, _N - 1> sub_matrix(sub_elements);
            _determinant += (i % 2 == 0 ? 1 : -1) * (*this)(0, i) * sub_matrix.determinant();
        }

        determinant_cache_ = _determinant;
        determinant_cache_valid_ = true;
        return _determinant;
    }


    constexpr Matrix<T, N, M> transpose() const {
        Matrix<T, N, M> _transpose;
        for (size_t i = 0; i < M; ++i)
            for (size_t j = 0; j < N; ++j)
                _transpose(j, i) = (*this)(i, j);
        return _transpose;
    }


    template <typename _T = T, std::size_t _M = M, std::size_t _N = N>
    constexpr typename enable_if_square<_M, _N, std::shared_ptr<Matrix<T, M, N>>>::type lower() const {
        if (!lup_decomposition_cache_valid_) lup_decompose();
        return lower_;
    }


    template <typename _T = T, std::size_t _M = M, std::size_t _N = N>
    constexpr typename enable_if_square<_M, _N, std::shared_ptr<Matrix<T, M, N>>>::type upper() const {
        if (!lup_decomposition_cache_valid_) lup_decompose();
        return upper_;
    }


    template <typename _T = T, std::size_t _M = M, std::size_t _N = N>
    constexpr typename enable_if_square<_M, _N, std::array<std::size_t, M>>::type permutation() const {
        if (!lup_decomposition_cache_valid_) lup_decompose();
        return permutation_;
    }


    template <typename _T = T, std::size_t _M = M, std::size_t _N = N>
    constexpr typename enable_if_square<_M, _N, std::tuple<std::shared_ptr<Matrix<T, M, N>>, std::shared_ptr<Matrix<T, M, N>>, std::array<std::size_t, M>>>::type lup_decomposition() const {
        if (!lup_decomposition_cache_valid_) lup_decompose();
        return {lower_, upper_, permutation_};
    }


    template <typename _T = T, std::size_t _M = M, std::size_t _N = N>
    constexpr typename enable_if_square<_M, _N, Vector<_T, _M>>::type lup_solve(Vector<_T, _M>& b) const {
        if (!lup_decomposition_cache_valid_) lup_decompose();
        Matrix<_T, _M, 1> x;
        Matrix<_T, _M, 1> y;
        for (std::size_t i = 0; i < _M; ++i)
        {
            y(i, 0) = b(permutation_[i], 0);
            for (std::size_t j = 0; j < i; ++j)
            {
                y(i, 0) -= (*lower_)(i, j) * y(j, 0);
            }
        }

        for (std::size_t i = _M - 1; i != std::numeric_limits<size_t>::max(); --i)
        {
            x(i, 0) = y(i, 0);
            for (std::size_t j = i + 1; j < _N; ++j)
                x(i, 0) -= (*upper_)(i, j) * x(j, 0);
            x(i, 0) /= (*upper_)(i, i);
        }

        return x;
    }

    
    constexpr void permute(std::array<std::size_t, M>& permutation) {
        std::size_t swaps = 0;
        std::size_t next = 0;
        while (swaps < M - 1) {
            std::swap(elements_[next], elements_[permutation[next]]);
            next = permutation[next];
            ++swaps;
        }
        invalidate_caches();
    }


    constexpr bool square() {
        return M == N;
    }


    constexpr std::string to_string() {
        std::ostringstream width_calc;
        width_calc << std::scientific << std::setprecision(3);
        size_t max_width = 0;
        
        for (size_t i = 0; i < M; ++i) {
            for (size_t j = 0; j < N; ++j) {
                width_calc.str("");
                width_calc.clear();
                width_calc << (*this)(i, j);
                max_width = std::max(max_width, width_calc.str().length());
            }
        }

        std::ostringstream result;
        result << std::scientific << std::setprecision(3);
        for (size_t i = 0; i < M; ++i) {
            for (size_t j = 0; j < N; ++j) {
                result << std::setw(max_width) << std::right << (*this)(i, j);
                if (j < N - 1) result << " ";
            }
            if (i < M - 1) result << "\n";
        }
        return result.str();
    }
};
