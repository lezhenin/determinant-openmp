#include <benchmark/benchmark.h>
#include "determinant.h"
#include "omp.h"


static void BM_determinant(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        int size = state.range(0);
        int threads = state.range(1);
        auto matrix = Matrix<double>::random(size, size);
        omp_set_num_threads(threads);
        bool enable_parallel = threads > 1;
        state.ResumeTiming();
        determinant(matrix, enable_parallel);
    }
}

BENCHMARK(BM_determinant)
    ->RangeMultiplier(2)
    ->Ranges({{128, 2048}, {1, 8}})
    ->Repetitions(10)
    ->DisplayAggregatesOnly()
    ->UseRealTime()->MeasureProcessCPUTime()
    ->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();

// --benchmark_out_format=json --benchmark_out=../output/benchmark3.json