#pragma once

#include <thread>
#include <random>
#include <limits>
#include <vector>
#include <future>
#include <algorithm>

template <typename Type>
class NormalDistGenerator
{
public:
    struct RequestsStats
    {
        Type minVal = std::numeric_limits<Type>::max() - 1;
        Type maxVal = std::numeric_limits<Type>::max() + 1;
        double coverRatio = 0;
    };

    NormalDistGenerator()
        : mMin(0)
        , mMax(std::numeric_limits<Type>::max() - 1)
        , mMean(mMax / 2)
        , mThreadsNum(std::thread::hardware_concurrency() - 1)
    {}

    void setMean(double mean) { mMean = mean; }

    void configureStd(Type const &min, Type const &max);


    //! fills array with normally distributed values
    //! @arg percent is coverage ratio. Must be in [0..1] interval
    RequestsStats fill(std::vector<Type> &array, double percent);


private:
    Type mMin;
    Type mMax;
    double mMean;
    size_t const mThreadsNum;

    struct NormalGenerator
    {
        std::normal_distribution<double> distributer;
        std::mt19937 generator;

        NormalGenerator(double mean, double stdDev, unsigned long seedId)
            : distributer(mean, stdDev)
        {
            auto const seed = static_cast<unsigned long>(std::chrono::system_clock::now().time_since_epoch().count());
            generator.seed(seed - seedId);
        }

        Type rand()
        {
            return static_cast<Type> (distributer(generator));
        }
    };
    std::vector<NormalGenerator> mGenerators;

    double percent2Deviation(double percent, Type n) const { return percent * double(n) / 6; }
    void initGenerators(double deviation);

};




template <typename Type>
void NormalDistGenerator<Type>::initGenerators(double deviation)
{
    mGenerators.clear();
    mGenerators.reserve(mThreadsNum);
    for (auto i = 0u; i < mThreadsNum + 1; i++)
        mGenerators.push_back(NormalGenerator(mMean, deviation, i));
}

template <typename Type>
auto NormalDistGenerator<Type>::fill(std::vector<Type> &array, double percent) -> RequestsStats
{
    if (percent <= 0 || percent > 1)
        throw std::invalid_argument("check coverage ratio");

    auto worker = [this] (Type *arr, size_t locSize, NormalGenerator &generator) -> RequestsStats
    {
        RequestsStats r;
        size_t lcount = 0;
        while (lcount < locSize)
        {
            Type value = generator.rand();

            if (value < mMin || value > mMax)
                continue;

            arr[lcount] = value;
            r.minVal = (value < r.minVal)? value : r.minVal;
            r.maxVal = (value > r.maxVal)? value : r.maxVal;
            ++lcount;
        }
        return r;
    };

    size_t const length = array.size();
    initGenerators(percent2Deviation(percent, mMax));

    size_t const chunkSize = length / mThreadsNum;
    std::vector<std::future<RequestsStats>> statistics(mThreadsNum);
    size_t position = 0;
    RequestsStats totalStat;
    for (auto i = 0u; i < mThreadsNum; i++)
    {
        statistics[i] = std::async(std::launch::async, worker, &array[position], chunkSize, std::ref(mGenerators[i]));
        position += chunkSize;
    }
    if (position < length)
        totalStat = worker(&array[position], length - mThreadsNum * chunkSize, std::ref(mGenerators[mThreadsNum]));

    return std::accumulate(statistics.begin(), statistics.end(), totalStat
                           , [this] (RequestsStats accStats, std::future<RequestsStats> &future)
    {
        auto iStats = future.get();
        accStats.minVal = iStats.minVal < accStats.minVal? iStats.minVal : accStats.minVal;
        accStats.maxVal = iStats.maxVal > accStats.maxVal? iStats.maxVal : accStats.maxVal;
        accStats.coverRatio = double(accStats.maxVal - accStats.minVal) / (mMax - mMin);
        return accStats;
    });
}

template <typename Type>
void NormalDistGenerator<Type>::configureStd(const Type &min, const Type &max)
{
    mMin = min;
    mMax = max;
    mMean = double(max + min) / 2.0;
}
