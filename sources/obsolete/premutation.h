#ifndef PARALLEL_PROG_PREMUTATION_H
#define PARALLEL_PROG_PREMUTATION_H

#include <vector>
#include <cassert>
#include <algorithm>
#include <numeric>
#include <forward_list>


class PermutationCode {

    int n;
    std::vector<int> code;

public:

    enum Parity {
        Odd, Even
    };

    explicit PermutationCode(int n) : n(n), code{n} {

        assert(n > 0);

        code.resize(n);
        std::fill(code.begin(), code.end(), 0);
    }

    bool increment() {

        assert(code[n - 1] <= 0);

        code[0] += 1;
        for (int i = 0; i < n - 1; ++i) {
            if (code[i] == n - i) {
                code[i] = 0;
                code[i + 1] += 1;
            } else {
                return true;
            }
        }
        return false;
    }

    const std::vector<int> &getCode() const {
        return code;
    }

    Parity permute(std::vector<int> &sequence) {

        assert(code[n - 1] <= 0);

        std::forward_list<int> list(sequence.begin(), sequence.end());
        for (int i = 0; i < n; ++i) {
            auto list_iterator = std::next(list.before_begin(), code[i]);
            sequence[i] = *std::next(list_iterator);
            list.erase_after(list_iterator);
        }

        int sum = std::accumulate(code.begin(), code.end(), 0);
        return (sum % 2 == 0) ? Even : Odd;

    }

};



#endif //PARALLEL_PROG_PREMUTATION_H
