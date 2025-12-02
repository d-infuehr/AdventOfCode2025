#include <iostream>
#include <string>
#include <format>
#include <fstream>
#include <iterator>
#include <vector>

class int100
{
public:
	int100() : value(0), overflow(0)
	{
	}

	int100(int v, int of) : value(v), overflow(of)
	{
	}

	int getValue() const
	{
		return value;
	}

	int getOverflow() const
	{
		return overflow;
	}

	int100& operator+=(int v)
	{
		overflow += std::abs(v) / 100;
		int remainder = v % 100;
		int newPoint = value + remainder;
		if (newPoint <= 0)
		{
			if (value != 0) overflow++;
			if (newPoint < 0) newPoint += 100;
		}
		else if (newPoint >= 100)
		{
			overflow++;
			newPoint -= 100;
		}
		value = newPoint;
		return *this;
	}

private:
	int value;
	int overflow;
};

int fromNum(const char* p)
{
	return p[0] == 'L' ? -std::stoi(p + 1) : std::stoi(p + 1);
}

std::vector<std::string> readFile(const std::string& filename)
{
	std::vector<std::string> lines;
	std::ifstream file(filename);
	if (!file.is_open())
	{
		std::cerr << "Error opening file!\n";
		return lines;
	}
	std::string line;
	while (std::getline(file, line))
	{
		if (!line.empty())
		{
			lines.push_back(line);
		}
	}
	return lines;
}

int main()
{
	int100 a(50, 0);
	std::vector<std::string> input = readFile("input.txt");
	std::cout << std::format("The dial starts by pointing at {}.\n", a.getValue());
	int exact0 = 0;
	for (auto& dir : input)
	{
		a += fromNum(dir.c_str());
		if (a.getValue() == 0)
		{
			exact0++;
		}
		std::cout << std::format("The dial is rotated {} to point at {}.\n", dir, a.getValue());
	}

	std::cout << std::format("The password is Part 1: {}\n", exact0);
	std::cout << std::format("The password is Part 2: {}\n", a.getOverflow());
}
