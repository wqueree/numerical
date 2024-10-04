#include <array>
#include <string>
#include <memory>


template <typename T, std::size_t M, std::size_t N>
class Matrix {
private:
    std::array<T, M * N> elements_;

public:
    Matrix(std::array<T, M * N>& elements);
    Matrix(std::array<T, M * N>&& elements);

    std::string to_string();
};


template <typename T, std::size_t M, std::size_t N>
Matrix<T, M, N>::Matrix(std::array<T, M * N>& elements)
    : elements_{elements} {}


template <typename T, std::size_t M, std::size_t N>
Matrix<T, M, N>::Matrix(std::array<T, M * N>&& elements)
    : elements_{elements} {}


template <typename T, std::size_t M, std::size_t N>
std::string Matrix<T, M, N>::to_string() {
    std::string result {};
    const size_t format_buffer_size = 8;
    const std::string format_string {"%lf"};
    for (size_t i = 0; i < M; ++i) {
        for (size_t j = 0; j < N; ++j) {
            std::unique_ptr<char[]> element_format_buffer = std::make_unique<char[]>(format_buffer_size);
            std::snprintf(element_format_buffer.get(), format_buffer_size, format_string.c_str(), elements_[i * N + j]);
            result.append(std::string(element_format_buffer.get()));
            if (j < N - 1)
                result.append(" ");
        }
        if (i < M - 1)
            result.append("\n");
    }
    return result;
}
