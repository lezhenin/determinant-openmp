#ifndef PARALLEL_PROG_DETERMINANT_H
#define PARALLEL_PROG_DETERMINANT_H

#include <vector>
#include <cassert>
#include <omp.h>
#include <iostream>

#include "matrix.h"


template <typename T>
std::tuple<bool, typename Matrix<T>::Index> swapRows(Matrix<T> &matrix) {

    using Index = typename Matrix<T>::Index;

    const Index n = matrix.rowNumber();
    Index permutations = 0;

    std::vector<Index> indices(n);
    indices.resize(n);

    for (int i = 0; i < n; ++i) {

        indices[i] = i;

        for (int j = i + 1; j < n; ++j) {
            if (std::abs(matrix(j, i)) > std::abs(matrix(indices[i], i))) {
                indices[i] = j;
            }
        }

        if (std::abs(matrix(indices[i], i)) < 1e-12) {
            return {true, permutations};
        }

        if (i != indices[i]) {
            matrix.swapRows(i, indices[i]);
            permutations += 1;
        }

    }

    return {false, permutations};
}


template <typename T>
T determinant(Matrix<T> matrix, bool enable_parallel=true) {

    static_assert(std::is_floating_point<T>::value);
    assert(matrix.columnNumber() == matrix.rowNumber());

    using Index = typename Matrix<T>::Index;

    const auto[singular, permutations] = swapRows(matrix);
    if (singular) {
        return 0.0;
    }

    const Index n = matrix.columnNumber();
    std::vector<T> ratio(n); ratio.resize(n);
    T determinant = permutations % 2 == 0 ? 1 : -1;

    #pragma omp parallel default(none) shared(matrix, ratio) reduction(*:determinant) if (enable_parallel)
    {
        for (Index i = 0; i < n; ++i) {
            #pragma omp for schedule(static)
            for (Index j = i + 1; j < n; ++j) {
                ratio[j] = matrix(j, i) / matrix(i, i);
            }
            #pragma omp for collapse(2) schedule(static)
            for (Index j = i + 1; j < n; ++j) {
                for (Index k = 0; k < n; ++k) {
                    matrix(j, k) = matrix(j, k) - ratio[j] * matrix(i, k);
                }
            }
        }

        #pragma omp for schedule(static)
        for (Index l = 0; l < n; ++l) {
            determinant *= matrix(l, l);
        }
    }

    return determinant;

}


#endif //PARALLEL_PROG_DETERMINANT_H
