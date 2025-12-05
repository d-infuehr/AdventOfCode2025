#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <numeric>

#define cuCheck(ans) { cuAssert((ans), __FILE__, __LINE__); }
inline void cuAssert(cudaError_t code, const char* file, int line, bool abort = true)
{
    if (code != cudaSuccess)
    {
        fprintf(stderr, "GPUassert: %s %s %d\n", cudaGetErrorString(code), file, line);
        if (abort) exit(code);
    }
}

__global__ void clearSpoiled(std::pair<uint64_t, uint64_t>* ranges, int ranges_length, uint64_t* data, int output_length)
{
	int idx = threadIdx.x + blockIdx.x * blockDim.x;
	if (idx >= output_length) return;

    for( int i = 0; i < ranges_length; i++)
    {
        uint64_t begin = ranges[i].first;
        uint64_t end = ranges[i].second;
        if (data[idx] >= begin && data[idx] <= end)
            return;
	}
    data[idx] = 0;
}

__global__ void sumRanges(std::pair<uint64_t, uint64_t>* ranges, int ranges_length)
{
	int idx = threadIdx.x + blockIdx.x * blockDim.x;
	if (idx >= ranges_length) return;

	uint64_t sum = ranges[idx].second - ranges[idx].first + 1;
    ranges[idx].first = sum;
}

void read();
void transform();
void print();

void transform2();
void print2();

void free();

std::vector<std::pair<uint64_t, uint64_t>> ranges;
std::vector<uint64_t> values;

std::pair<uint64_t, uint64_t>* d_ranges = nullptr;
uint64_t* d_data = nullptr;

int main()
{
    //Prepare
    read();

    //Solve Part 1
    transform();
    print();

	//Solve Part 2
    transform2();
    print2();

	//Cleanup
    free();
    return 0;
}

void read()
{
    std::ifstream file("input.txt");
    if (!file.is_open()) throw;

    std::string line;
    while (std::getline(file, line))
    {
        if (line == "") continue;
        if (line.find('-') != std::string::npos)
        {
            size_t dashPos = line.find('-');
            uint64_t start = std::stoull(line.substr(0, dashPos));
            uint64_t end = std::stoull(line.substr(dashPos + 1));
            ranges.emplace_back(start, end);
        }
        else
        {
            uint64_t value = std::stoull(line);
            values.push_back(value);
        }
    }

    //Normalize input ranges
    std::sort(ranges.begin(), ranges.end(),
        [](const auto& a, const auto& b)
        {
            return a.first < b.first;
		});
    
    for(int i=0;i<ranges.size()-1;i++)
    {
        if (ranges[i].first >= ranges[i + 1].first)
        {
            if (ranges[i].second <= ranges[i + 1].second)
                ranges.erase(ranges.begin() + i);
            else
                ranges.erase(ranges.begin() + i + 1);
            i--;
            continue;
                
        }
        if (ranges[i].second >= ranges[i + 1].first)
        {
            ranges[i + 1].second = std::max(ranges[i].second, ranges[i + 1].second);
            ranges[i].second = ranges[i + 1].first - 1;
        }
	}
    size_t data_size = values.size() * sizeof(uint64_t);
    size_t ranges_size = ranges.size() * sizeof(std::pair<uint64_t, uint64_t>);
    cuCheck(cudaMalloc((void**)&d_data, data_size));
    cuCheck(cudaMalloc((void**)&d_ranges, ranges_size));
    cuCheck(cudaMemcpy(d_data, values.data(), data_size, cudaMemcpyHostToDevice));
    cuCheck(cudaMemcpy(d_ranges, ranges.data(), ranges_size, cudaMemcpyHostToDevice));
}

void transform()
{
    size_t data_size = values.size() * sizeof(uint64_t);
    int threadsPerBlock = 256;
    int blocksPerGrid = (values.size() + threadsPerBlock - 1) / threadsPerBlock;
    clearSpoiled<<<blocksPerGrid, threadsPerBlock>>>(d_ranges, ranges.size(), d_data, values.size());
    cuCheck(cudaPeekAtLastError());
    cuCheck(cudaDeviceSynchronize());
    cuCheck(cudaMemcpy(values.data(), d_data, data_size, cudaMemcpyDeviceToHost));
}

void transform2()
{
    size_t ranges_size = ranges.size() * sizeof(std::pair<uint64_t, uint64_t>);
    int threadsPerBlock = 256;
    int blocksPerGrid = (ranges.size() + threadsPerBlock - 1) / threadsPerBlock;
    sumRanges<<<blocksPerGrid, threadsPerBlock>>>(d_ranges, ranges.size());
    cuCheck(cudaPeekAtLastError());
    cuCheck(cudaDeviceSynchronize());
	cuCheck(cudaMemcpy(ranges.data(), d_ranges, ranges_size, cudaMemcpyDeviceToHost));
}

void print()
{
    auto count = std::count_if(values.begin(), values.end(),
        [](uint64_t v)
        {
            return v != 0;
        });
    std::cout << "Count: " << count << std::endl;
}


void print2()
{
    uint64_t total = std::accumulate(
        ranges.begin(),
        ranges.end(),
        uint64_t{ 0 },
        [](uint64_t sum, const auto& r)
        {
            return sum + r.first;
        });

    std::cout << "Total: " << total << std::endl;
}

void free()
{
    if (d_data)
    {
        cuCheck(cudaFree(d_data));
        d_data = nullptr;
    }
    if (d_ranges)
    {
        cuCheck(cudaFree(d_ranges));
        d_ranges = nullptr;
    }
}