#include "benchmark/benchmark.h"

#include "fixed_sort.h"

#include <array>
#include <random>

#include <cpp-sort/fixed_sorters.h>
#include <cpp-sort/sorters.h>

static constexpr uint32_t num_iters = 1000000;

std::vector<std::array<int, 8>> test_data_bit_8;

static void bench_sort_bit_8(benchmark::State& state)
{
    int i = 0;

    for (auto _ : state)
    {
        sort_8<true>(test_data_bit_8[i].data());

        benchmark::DoNotOptimize(test_data_bit_8[i++]);
    }
}
BENCHMARK(bench_sort_bit_8)->Iterations(num_iters)->MinWarmUpTime(1);

std::vector<std::array<int, 8>> test_data_stl_8;

static void bench_sort_stl_8(benchmark::State& state)
{
    int i = 0;

    for (auto _ : state)
    {
        std::sort(test_data_stl_8[i].data(), test_data_stl_8[i].data() + 8);

        benchmark::DoNotOptimize(test_data_stl_8[i++]);
    }
}
BENCHMARK(bench_sort_stl_8)->Iterations(num_iters)->MinWarmUpTime(1);

std::vector<std::array<int, 8>> test_data_pdq_8;

static void bench_sort_pdq_8(benchmark::State& state)
{
    int i = 0;

    for (auto _ : state)
    {
        cppsort::pdq_sort(test_data_pdq_8[i]);

        benchmark::DoNotOptimize(test_data_pdq_8[i++]);
    }
}
BENCHMARK(bench_sort_pdq_8)->Iterations(num_iters)->MinWarmUpTime(1);

std::vector<std::array<int, 8>> test_data_ska_8;

static void bench_sort_ska_8(benchmark::State& state)
{
    int i = 0;

    for (auto _ : state)
    {
        cppsort::ska_sort(test_data_ska_8[i]);

        benchmark::DoNotOptimize(test_data_ska_8[i++]);
    }
}
BENCHMARK(bench_sort_ska_8)->Iterations(num_iters)->MinWarmUpTime(1);

cppsort::sorting_network_sorter<8> sn_8;

std::vector<std::array<int, 8>> test_data_sn_8;

static void bench_sort_sn_8(benchmark::State& state)
{
    int i = 0;

    for (auto _ : state)
    {
        sn_8(test_data_sn_8[i]);

        benchmark::DoNotOptimize(test_data_sn_8[i++]);
    }
}
BENCHMARK(bench_sort_sn_8)->Iterations(num_iters)->MinWarmUpTime(1);

std::vector<std::array<int, 16>> test_data_bit_16;

static void bench_sort_bit_16(benchmark::State& state)
{
    int i = 0;

    for (auto _ : state)
    {
        sort_16<true>(test_data_bit_16[i].data());

        benchmark::DoNotOptimize(test_data_bit_16[i++]);
    }
}
BENCHMARK(bench_sort_bit_16)->Iterations(num_iters)->MinWarmUpTime(1);

std::vector<std::array<int, 16>> test_data_stl_16;

static void bench_sort_stl_16(benchmark::State& state)
{
    int i = 0;

    for (auto _ : state)
    {
        std::sort(test_data_stl_16[i].data(), test_data_stl_16[i].data() + 16);

        benchmark::DoNotOptimize(test_data_stl_16[i++]);
    }
}
BENCHMARK(bench_sort_stl_16)->Iterations(num_iters)->MinWarmUpTime(1);

std::vector<std::array<int, 16>> test_data_pdq_16;

static void bench_sort_pdq_16(benchmark::State& state)
{
    int i = 0;

    for (auto _ : state)
    {
        cppsort::pdq_sort(test_data_pdq_16[i]);

        benchmark::DoNotOptimize(test_data_pdq_16[i++]);
    }
}
BENCHMARK(bench_sort_pdq_16)->Iterations(num_iters)->MinWarmUpTime(1);

std::vector<std::array<int, 16>> test_data_ska_16;

static void bench_sort_ska_16(benchmark::State& state)
{
    int i = 0;

    for (auto _ : state)
    {
        cppsort::ska_sort(test_data_ska_16[i]);

        benchmark::DoNotOptimize(test_data_ska_16[i++]);
    }
}
BENCHMARK(bench_sort_ska_16)->Iterations(num_iters)->MinWarmUpTime(1);

cppsort::sorting_network_sorter<16> sn_16;

std::vector<std::array<int, 16>> test_data_sn_16;

static void bench_sort_sn_16(benchmark::State& state)
{
    int i = 0;

    for (auto _ : state)
    {
        sn_16(test_data_sn_16[i]);

        benchmark::DoNotOptimize(test_data_sn_16[i++]);
    }
}
BENCHMARK(bench_sort_sn_16)->Iterations(num_iters)->MinWarmUpTime(1);

std::vector<std::array<int, 32>> test_data_bit_32;

static void bench_sort_bit_32(benchmark::State& state)
{
    int i = 0;

    for (auto _ : state)
    {
        sort_32<true>(test_data_bit_32[i].data());

        benchmark::DoNotOptimize(test_data_bit_32[i++]);
    }
}
BENCHMARK(bench_sort_bit_32)->Iterations(num_iters)->MinWarmUpTime(1);

std::vector<std::array<int, 32>> test_data_stl_32;

static void bench_sort_stl_32(benchmark::State& state)
{
    int i = 0;

    for (auto _ : state)
    {
        std::sort(test_data_stl_32[i].data(), test_data_stl_32[i].data() + 32);

        benchmark::DoNotOptimize(test_data_stl_32[i++]);
    }
}
BENCHMARK(bench_sort_stl_32)->Iterations(num_iters)->MinWarmUpTime(1);

std::vector<std::array<int, 32>> test_data_pdq_32;

static void bench_sort_pdq_32(benchmark::State& state)
{
    int i = 0;

    for (auto _ : state)
    {
        cppsort::pdq_sort(test_data_pdq_32[i]);

        benchmark::DoNotOptimize(test_data_pdq_32[i++]);
    }
}
BENCHMARK(bench_sort_pdq_32)->Iterations(num_iters)->MinWarmUpTime(1);

std::vector<std::array<int, 32>> test_data_ska_32;

static void bench_sort_ska_32(benchmark::State& state)
{
    int i = 0;

    for (auto _ : state)
    {
        cppsort::ska_sort(test_data_ska_32[i]);

        benchmark::DoNotOptimize(test_data_ska_32[i++]);
    }
}
BENCHMARK(bench_sort_ska_32)->Iterations(num_iters)->MinWarmUpTime(1);

cppsort::sorting_network_sorter<32> sn_32;

std::vector<std::array<int, 32>> test_data_sn_32;

static void bench_sort_sn_32(benchmark::State& state)
{
    int i = 0;

    for (auto _ : state)
    {
        sn_32(test_data_sn_32[i]);

        benchmark::DoNotOptimize(test_data_sn_32[i++]);
    }
}
BENCHMARK(bench_sort_sn_32)->Iterations(num_iters)->MinWarmUpTime(1);

std::vector<std::array<int, 64>> test_data_bit_64;

static void bench_sort_bit_64(benchmark::State& state)
{
    int i = 0;

    for (auto _ : state)
    {
        sort_64<true>(test_data_bit_64[i].data());

        benchmark::DoNotOptimize(test_data_bit_64[i++]);
    }
}
BENCHMARK(bench_sort_bit_64)->Iterations(num_iters)->MinWarmUpTime(1);

std::vector<std::array<int, 64>> test_data_stl_64;

static void bench_sort_stl_64(benchmark::State& state)
{
    int i = 0;

    for (auto _ : state)
    {
        std::sort(test_data_stl_64[i].data(), test_data_stl_64[i].data() + 64);

        benchmark::DoNotOptimize(test_data_stl_64[i++]);
    }
}
BENCHMARK(bench_sort_stl_64)->Iterations(num_iters)->MinWarmUpTime(1);

std::vector<std::array<int, 64>> test_data_pdq_64;

static void bench_sort_pdq_64(benchmark::State& state)
{
    int i = 0;

    for (auto _ : state)
    {
        cppsort::pdq_sort(test_data_pdq_64[i]);

        benchmark::DoNotOptimize(test_data_pdq_64[i++]);
    }
}
BENCHMARK(bench_sort_pdq_64)->Iterations(num_iters)->MinWarmUpTime(1);

std::vector<std::array<int, 64>> test_data_ska_64;

static void bench_sort_ska_64(benchmark::State& state)
{
    int i = 0;

    for (auto _ : state)
    {
        cppsort::ska_sort(test_data_ska_64[i]);

        benchmark::DoNotOptimize(test_data_ska_64[i++]);
    }
}
BENCHMARK(bench_sort_ska_64)->Iterations(num_iters)->MinWarmUpTime(1);

cppsort::sorting_network_sorter<64> sn_64;

std::vector<std::array<int, 64>> test_data_sn_64;

static void bench_sort_sn_64(benchmark::State& state)
{
    int i = 0;

    for (auto _ : state)
    {
        sn_64(test_data_sn_64[i]);

        benchmark::DoNotOptimize(test_data_sn_64[i++]);
    }
}
BENCHMARK(bench_sort_sn_64)->Iterations(num_iters)->MinWarmUpTime(1);

std::vector<std::array<int, 128>> test_data_bit_128;

static void bench_sort_bit_128(benchmark::State& state)
{
    int i = 0;

    for (auto _ : state)
    {
        sort_128<true>(test_data_bit_128[i].data());

        benchmark::DoNotOptimize(test_data_bit_128[i++]);
    }
}
BENCHMARK(bench_sort_bit_128)->Iterations(num_iters)->MinWarmUpTime(1);

std::vector<std::array<int, 128>> test_data_stl_128;

static void bench_sort_stl_128(benchmark::State& state)
{
    int i = 0;

    for (auto _ : state)
    {
        std::sort(test_data_stl_128[i].data(), test_data_stl_128[i].data() + 128);

        benchmark::DoNotOptimize(test_data_stl_128[i++]);
    }
}
BENCHMARK(bench_sort_stl_128)->Iterations(num_iters)->MinWarmUpTime(1);

std::vector<std::array<int, 128>> test_data_pdq_128;

static void bench_sort_pdq_128(benchmark::State& state)
{
    int i = 0;

    for (auto _ : state)
    {
        cppsort::pdq_sort(test_data_pdq_128[i]);

        benchmark::DoNotOptimize(test_data_pdq_128[i++]);
    }
}
BENCHMARK(bench_sort_pdq_128)->Iterations(num_iters)->MinWarmUpTime(1);

std::vector<std::array<int, 128>> test_data_ska_128;

static void bench_sort_ska_128(benchmark::State& state)
{
    int i = 0;

    for (auto _ : state)
    {
        cppsort::ska_sort(test_data_ska_128[i]);

        benchmark::DoNotOptimize(test_data_ska_128[i++]);
    }
}
BENCHMARK(bench_sort_ska_128)->Iterations(num_iters)->MinWarmUpTime(1);

template
    <uint32_t size>
inline __attribute__((always_inline)) std::vector<std::array<int, size>> generate_data()
{
    std::random_device random_device;
    std::mt19937       generator;

    std::vector<std::array<int, size>> data;

    data.resize(num_iters);

    generator.seed(random_device());

    std::uniform_int_distribution<uint32_t> uniform_distribution(0, 1 << 24);

    for (int i = 0; i < num_iters; ++i)
    {
        for (int j = 0; j < size; ++j)
            data[i][j] = uniform_distribution(generator);
    }

    return data;
};

void init()
{
    test_data_bit_8   = generate_data<8>();
    test_data_stl_8   = test_data_bit_8;
    test_data_pdq_8   = test_data_bit_8;
    test_data_ska_8   = test_data_bit_8;
    test_data_sn_8    = test_data_bit_8;

    test_data_bit_16  = generate_data<16>();
    test_data_stl_16  = test_data_bit_16;
    test_data_pdq_16  = test_data_bit_16;
    test_data_ska_16  = test_data_bit_16;
    test_data_sn_16   = test_data_bit_16;

    test_data_bit_32  = generate_data<32>();
    test_data_stl_32  = test_data_bit_32;
    test_data_pdq_32  = test_data_bit_32;
    test_data_ska_32  = test_data_bit_32;
    test_data_sn_32   = test_data_bit_32;

    test_data_bit_64  = generate_data<64>();
    test_data_stl_64  = test_data_bit_64;
    test_data_pdq_64  = test_data_bit_64;
    test_data_ska_64  = test_data_bit_64;
    test_data_sn_64   = test_data_bit_64;

    test_data_bit_128 = generate_data<128>();
    test_data_stl_128 = test_data_bit_128;
    test_data_pdq_128 = test_data_bit_128;
    test_data_ska_128 = test_data_bit_128;
}

int main(int argc, char** argv)
{
   init();

   ::benchmark::Initialize(&argc, argv);
   ::benchmark::RunSpecifiedBenchmarks();
}