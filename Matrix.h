#include <array>
#include <string>
#include <memory>
#include <cmath>
#include <limits>
#include <iomanip>


template <typename T, std::size_t M, std::size_t N>
class Matrix {
private:
    std::array<std::array<T, N>, M> elements_;

public:
    constexpr Matrix();
    constexpr Matrix(const std::array<std::array<T, N>, M>& elements);
    constexpr Matrix(const std::array<std::array<T, N>, M>&& elements);

    constexpr T& operator()(const std::size_t row, const std::size_t col);
    constexpr bool operator==(const Matrix<T, M, N>& matrix);
    constexpr Matrix<T, M, N> operator+(const Matrix<T, M, N>& matrix);
    constexpr Matrix<T, M, N> operator-(const Matrix<T, M, N>& matrix);

    template <typename _T, std::size_t _M, std::size_t _N, std::size_t _P>
    constexpr friend Matrix<_T, _M, _P> operator*(const Matrix<_T, _M, _N>& a, const Matrix<_T, _N, _P>& b);

    template <typename _T = T, std::size_t _M = M, std::size_t _N = N>
    constexpr typename std::enable_if<(_M == _N), _T>::type determinant();


    constexpr Matrix<T, N, M> transpose();
    constexpr bool square();
    constexpr std::string to_string();
};


template <typename T, std::size_t M, std::size_t N>
constexpr Matrix<T, M, N>::Matrix()
    : elements_{} {}


template <typename T, std::size_t M, std::size_t N>
constexpr Matrix<T, M, N>::Matrix(const std::array<std::array<T, N>, M>& elements)
    : elements_{std::move(elements)} {}


template <typename T, std::size_t M, std::size_t N>
constexpr Matrix<T, M, N>::Matrix(const std::array<std::array<T, N>, M>&& elements)
    : elements_{std::move(elements)} {}


template <typename T, std::size_t M, std::size_t N>
constexpr T& Matrix<T, M, N>::operator()(std::size_t row, std::size_t col) {
    return elements_[row - 1][col - 1];
}


template <typename T, std::size_t M, std::size_t N>
constexpr bool Matrix<T, M, N>::operator==(const Matrix<T, M, N>& matrix) {
    for (size_t i = 0; i < M; ++i) {
        for (size_t j = 0; j < N; ++j) {
            if (fabs(elements_[i][j] - matrix.elements_[i][j]) > std::numeric_limits<T>::epsilon())
                return false;
        }
    }
    return true;
}

template <typename T, std::size_t M, std::size_t N>
constexpr Matrix<T, M, N> Matrix<T, M, N>::operator+(const Matrix<T, M, N>& matrix) {
    Matrix<T, M, N> sum;
    for (size_t i = 0; i < M; ++i) {
        for (size_t j = 0; j < N; ++j) {
            sum.elements_[i][j] = elements_[i][j] + matrix.elements_[i][j];
        }
    }
    return sum;
}


template <typename T, std::size_t M, std::size_t N>
constexpr Matrix<T, M, N> Matrix<T, M, N>::operator-(const Matrix<T, M, N>& matrix) {
    Matrix<T, M, N> sum;
    std::array<std::array<T, N>, M> sum_array {};
    for (size_t i = 0; i < M; ++i) {
        for (size_t j = 0; j < N; ++j) {
            sum.elements_[i][j] = elements_[i][j] - matrix.elements_[i][j];
        }
    }
    return sum;
}


template <typename _T, std::size_t _M, std::size_t _N, std::size_t _P>
constexpr Matrix<_T, _M, _P> operator*(const Matrix<_T, _M, _N>& a, const Matrix<_T, _N, _P>& b) {
    Matrix<_T, _M, _P> product;
    for (size_t i = 0; i < _M; ++i) {
        for (size_t j = 0; j < _P; ++j) {
            _T dot_product = 0;
            for (size_t k = 0; k < _N; ++k) {
                dot_product += a.elements_[i][k] * b.elements_[k][j];
            }
            product.elements_[i][j] = dot_product;
        }
    }
    return product;
}


template <typename T, std::size_t M, std::size_t N>
template <typename _T, std::size_t _M, std::size_t _N>
constexpr typename std::enable_if<(_M == _N), _T>::type Matrix<T, M, N>::determinant() {
    return 0.0;
}


template <typename T, std::size_t M, std::size_t N>
constexpr Matrix<T, N, M> Matrix<T, M, N>::transpose() {
    Matrix<T, N, M> _transpose;
    for (size_t i = 0; i < M; ++i) {
        for (size_t j = 0; j < N; ++j) {
            _transpose.elements_[j][i] = elements_[i][j];
        }
    }
    return _transpose;
}


template <typename T, std::size_t M, std::size_t N>
constexpr bool Matrix<T, M, N>::square() {
    return M == N;
}


template <typename T, std::size_t M, std::size_t N>
constexpr std::string Matrix<T, M, N>::to_string() {
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
