#include <array>
#include <string>
#include <memory>


template <typename T, std::size_t M, std::size_t N>
class Matrix {
private:
    std::array<std::array<T, N>, M> elements_;

public:
    Matrix();
    Matrix(const std::array<std::array<T, N>, M>& elements);
    Matrix(const std::array<std::array<T, N>, M>&& elements);

    T& operator()(const std::size_t row, const std::size_t col);
    Matrix<T, M, N> operator+(const Matrix<T, M, N>& matrix);
    Matrix<T, M, N> operator-(const Matrix<T, M, N>& matrix);

    template <typename _T, std::size_t _M, std::size_t _N, std::size_t _P>
    friend Matrix<_T, _M, _P> operator*(const Matrix<_T, _M, _N>& a, const Matrix<_T, _N, _P>& b);

    Matrix<T, N, M> transpose();
    std::string to_string();
};


template <typename T, std::size_t M, std::size_t N>
Matrix<T, M, N>::Matrix()
    : elements_{} {}


template <typename T, std::size_t M, std::size_t N>
Matrix<T, M, N>::Matrix(const std::array<std::array<T, N>, M>& elements)
    : elements_{elements} {}


template <typename T, std::size_t M, std::size_t N>
Matrix<T, M, N>::Matrix(const std::array<std::array<T, N>, M>&& elements)
    : elements_{elements} {}


template <typename T, std::size_t M, std::size_t N>
T& Matrix<T, M, N>::operator()(std::size_t row, std::size_t col) {
    return elements_[row - 1][col - 1];
}


template <typename T, std::size_t M, std::size_t N>
Matrix<T, M, N> Matrix<T, M, N>::operator+(const Matrix<T, M, N>& matrix) {
    std::array<std::array<T, N>, M> sum_array {};
    for (size_t i = 0; i < M; ++i) {
        for (size_t j = 0; j < N; ++j) {
            sum_array[i][j] = elements_[i][j] + matrix.elements_[i][j];
        }
    }
    Matrix<T, M, N> sum {std::move(sum_array)};
    return sum;
}


template <typename T, std::size_t M, std::size_t N>
Matrix<T, M, N> Matrix<T, M, N>::operator-(const Matrix<T, M, N>& matrix) {
    std::array<std::array<T, N>, M> sum_array {};
    for (size_t i = 0; i < M; ++i) {
        for (size_t j = 0; j < N; ++j) {
            sum_array[i][j] = elements_[i][j] - matrix.elements_[i][j];
        }
    }
    Matrix<T, M, N> sum {std::move(sum_array)};
    return sum;
}


template <typename _T, std::size_t _M, std::size_t _N, std::size_t _P>
Matrix<_T, _M, _P> operator*(const Matrix<_T, _M, _N>& a, const Matrix<_T, _N, _P>& b) {
    std::array<std::array<_T, _P>, _M> product_array {};
    for (size_t i = 0; i < _M; ++i) {
        for (size_t j = 0; j < _P; ++j) {
            _T dot_product = 0;
            for (size_t k = 0; k < _N; ++k) {
                dot_product += a.elements_[i][k] * b.elements_[k][j];
            }
            product_array[i][j] = dot_product;
        }
    }
    Matrix<_T, _M, _P> product {std::move(product_array)};
    return product;
}


template <typename T, std::size_t M, std::size_t N>
Matrix<T, N, M> Matrix<T, M, N>::transpose() {
    std::array<std::array<T, M>, N> transpose_array {};
    for (size_t i = 0; i < M; ++i) {
        for (size_t j = 0; j < N; ++j) {
            transpose_array[j][i] = elements_[i][j];
        }
    }
    Matrix<T, N, M> _transpose {std::move(transpose_array)};
    return _transpose;
}


template <typename T, std::size_t M, std::size_t N>
std::string Matrix<T, M, N>::to_string() {
    std::string result {};
    const size_t format_buffer_size = 16;
    const std::string format_string {"%.3e"};
    for (size_t i = 0; i < M; ++i) {
        for (size_t j = 0; j < N; ++j) {
            std::unique_ptr<char[]> element_format_buffer = std::make_unique<char[]>(format_buffer_size);
            std::snprintf(element_format_buffer.get(), format_buffer_size, format_string.c_str(), elements_[i][j]);
            result.append(std::string(element_format_buffer.get()));
            if (j < N - 1)
                result.append(" ");
        }
        if (i < M - 1)
            result.append("\n");
    }
    return result;
}
