#include <iostream>
#include <string>
#include <format>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <numeric>
#include <vector>
#include <set>
#include <map>
#include <chrono>

std::vector<std::string> readFile(const std::string& filename) {
	std::vector<std::string> lines;
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Error opening file!\n";
		return lines;
	}
	std::string line;
	while (std::getline(file, line)) {
		lines.push_back(line);
	}
	return lines;
}

std::set<int> getChar(const std::string& str, char c) {
	std::set<int> indices;
	for (size_t i = 0; i < str.size(); i++) {
		if (str[i] == c) {
			indices.insert((int)i);
		}
	}
	return indices;
}

int main()
{
	int num_intersect = 0;
	std::vector<std::string> input = readFile("input.txt");
	std::set<int> current = { *getChar(input[0], 'S').begin()};
	uint64_t lr[256]{};
	lr[*current.begin()] = { 1 };

	auto t1 = std::chrono::high_resolution_clock::now();
	for (int i = 1; i < input.size(); i++)
	{
		std::set<int> next = getChar(input[i], '^');
		std::set<int> intersect;
		std::set_intersection(current.begin(), current.end(), next.begin(), next.end(),
			std::inserter(intersect, intersect.begin()));
		for (int v : intersect)
		{
			num_intersect++;
			current.erase(v);
			current.insert(v - 1); lr[v - 1] += lr[v];
			current.insert(v + 1); lr[v + 1] += lr[v];
			lr[v] = 0;
		}
	}
	auto t2 = std::chrono::high_resolution_clock::now();
	
	std::cout << num_intersect << "\n";
	std::cout << std::accumulate(lr, lr + 256, 0ull) << "\n";
	std::cout << "algo time: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() / 1000.0 << "ms\n";
	std::cout << "bonus stars\n * *  ** **  * * **   ** ***  *  **  * * *** *    *      ** ** * **  * * ***  ** ***  ** **    * **  *** **  * *  * ***   *      * *  ** ** **   **    * **   ** ** * **  *      ** ** * **  * *  *       * ** *  *      ***  ** ** *  * **  *** ** ***  **  * * **  *    *      **  *   **    * ** ***  ** *  * **  * * ** **  " << std::endl;
}
