#ifndef PARALLEL_PROG_MATRIX_H
#define PARALLEL_PROG_MATRIX_H

#include <random>

template<typename T = double>
class Matrix {

    static_assert(std::is_floating_point<T>::value);

public:

    using Index = unsigned long;

private:

    Index column_number;
    Index row_number;

    std::vector<T> data;

    [[nodiscard]] inline Index index(Index row, Index column) const {
        return row * column_number + column;
    }

public:

    Matrix(Index row_number, Index column_number)
        : column_number(column_number), row_number(row_number),
        data(std::vector<T>(column_number * row_number)) {
        data.resize(column_number * row_number);
    }

    [[nodiscard]] Index columnNumber() const{
        return column_number;
    }
    [[nodiscard]] Index rowNumber() const {
        return row_number;
    }

    void swapRows(Index a, Index b) {
        auto a_row_begin = std::next(data.begin(), column_number * a);
        auto a_row_end = std::next(a_row_begin, column_number);
        auto b_row_begin = std::next(data.begin(), column_number * b);
        std::swap_ranges(a_row_begin, a_row_end, b_row_begin);
    }

    T &operator()(Index row, Index column) {
        return data[index(row, column)];
    }

    const T &operator()(Index row, Index column) const {
        return data[index(row, column)];
    }

    static Matrix random(Index column_number, Index row_number) {
        static std::random_device rnd_device;
        return random(row_number, column_number, rnd_device());
    }

    static Matrix random(Index column_number, Index row_number, Index seed) {
        static std::normal_distribution<T> default_distribution{0, 1};
        return random(row_number, column_number, seed, default_distribution);
    }

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-msc32-c"
    template <typename D>
    static Matrix random(Index row_number, Index column_number, Index seed, D distribution) {
        static std::default_random_engine engine;
        engine.seed(seed);
        Matrix m(row_number, column_number);
        std::generate(m.data.begin(), m.data.end(), [&distribution](){ return distribution(engine); });
        return m;
    }
#pragma clang diagnostic pop

};


#endif //PARALLEL_PROG_MATRIX_H
