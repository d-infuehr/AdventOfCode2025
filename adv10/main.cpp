#define _CRT_SECURE_NO_WARNINGS
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
#include <bitset>
#include <immintrin.h>

#define IndexOf(X) _tzcnt_u16(X)
#define Bitloop(X) for(;X; X = _blsr_u32(X))

static void bit_twiddle_permute(uint16_t& v)
{
	uint16_t t = v | (v - 1);
	v = (t + 1) | (((~t & -~t) - 1) >> (std::countr_zero(v) + 1)); 
}

constexpr uint32_t ncr(int n, int r)
{
	uint32_t ans = 1;
	r = std::min(r, n - r);
	for (int j = 1; j <= r; j++, n--)
	{
		if (n % j == 0)
		{
			ans *= n / j;
		}
		else if (ans % j == 0)
		{
			ans = (ans / j) * n;
		}
		else
		{
			ans = (ans * n) / j;
		}
	}
	return ans;
}

struct Matrix
{
	int* A{};
	int rows{};
	int cols{}; 
	std::vector<int> pivotCols;

	Matrix() : A(nullptr), rows(0), cols(0), pivotCols()
	{
	}

	Matrix(int r, int c) : rows(r), cols(c), pivotCols()
	{
		A = new int[rows * cols]{};
	}

	int& operator[](int i)
	{
		return A[i];
	}
	const int& operator[](int i) const
	{
		return A[i];
	}

	int& at(int r, int c)
	{
		return A[r * cols + c];
	}
	const int& at(int r, int c) const
	{
		return A[r * cols + c];
	}

	int& at(int i)
	{
		return A[i];
	}
	const int& at(int i) const
	{
		return A[i];
	}

	void swapRows(int r1, int r2)
	{
		for (int c = 0; c < cols; c++)
		{
			std::swap(A[r1 * cols + c], A[r2 * cols + c]);
		}
	}

	void swapCols(int c1, int c2)
	{
		for (int r = 0; r < rows; r++)
		{
			std::swap(A[r * cols + c1], A[r * cols + c2]);
		}
	}


	void print() const
	{
		for (int i = 0; i < rows; i++)
		{
			for (int c = 0; c < cols; c++)
			{
				std::cout << at(i, c) << ' ';
			}
			std::cout << "|\n";
		}
	}

	static const bool printSteps = false;

	void makeTriangular()
	{
		if (printSteps)
		{
			print();
		}

		pivotCols.clear();  // Reset pivot column tracking

		int lead = 0;
		for (int col = 0; col < cols - 1 && lead < rows; ++col)
		{
			// Find row with non-zero entry in current column, starting from lead
			int pivot_row = lead;
			while (pivot_row < rows && at(pivot_row, col) == 0)
				++pivot_row;

			// No pivot in this column → skip (free variable possible here)
			if (pivot_row == rows)
				continue;

			// Swap rows if needed
			if (pivot_row != lead)
			{
				swapRows(lead, pivot_row);
				if (printSteps)
				{
					std::cout << "Swapped rows " << lead << " and " << pivot_row << "\n";
					print();
				}
			}

			// Make pivot positive
			if (at(lead, col) < 0)
			{
				for (int c = col; c < cols; ++c)
					at(lead, c) = -at(lead, c);

				if (printSteps)
				{
					std::cout << "Negated row " << lead << "\n";
					print();
				}
			}

			// Eliminate below
			int pivot = at(lead, col);
			for (int r = lead + 1; r < rows; ++r)
			{
				int factor = at(r, col);
				if (factor == 0)
					continue;

				int g = std::gcd(std::abs(factor), pivot);
				int a = pivot / g;
				int b = factor / g;

				for (int c = col; c < cols; ++c)
				{
					at(r, c) = a * at(r, c) - b * at(lead, c);
				}

				if (printSteps)
				{
					std::cout << "Eliminated row " << r << " using row " << lead << "\n";
					print();
				}
			}

			// Record that this column has a pivot in row 'lead'
			pivotCols.push_back(col);

			++lead;
		}

		if (printSteps)
		{
			std::cout << "Triangular form:\n";
			print();
		}
	}

	int getNumFreeVariables() const
	{
		if (rows == 0 || cols < 2) return cols - 1;
		return (cols - 1) - static_cast<int>(pivotCols.size());
	}

	std::vector<int> getFreeVariableUpperBounds() const
	{
		int numVars = cols - 1;
		std::vector<int> bounds(numVars, 1000); 

		bool isPivotCol[16] = {};
		for (int pc : pivotCols)
			if (pc < numVars) isPivotCol[pc] = true;

		std::vector<int> freeVars;
		for (int col = 0; col < numVars; ++col)
			if (!isPivotCol[col])
				freeVars.push_back(col);

		if (freeVars.empty())
			return { 1 };  

		for (int r = 0; r < rows; ++r)
		{
			int rhs = at(r, cols - 1);
			if (rhs < 0) continue; 

			for (size_t i = 0; i < freeVars.size(); ++i)
			{
				int col = freeVars[i];
				int coeff = at(r, col);

				if (coeff > 0)
				{
					int max_possible = rhs / coeff;
					if (max_possible < bounds[col])
						bounds[col] = max_possible;
				}
			}
		}

		std::vector<int> result;
		for (int fv : freeVars)
		{
			int b = bounds[fv];
			result.push_back(std::max(1, std::min(500, b + 35)));  // +35 for safety margin. REALLY NEEDED
		}

		return result;
	}

	bool trysolve(int x[16], int m = 0, int n = 0, int k = 0)
	{
		int numVars = cols - 1;

		// Clear solution
		for (int i = 0; i < 16; i++) x[i] = 0;

		// Identify free variables (columns without pivot)
		std::vector<int> freeVars;
		bool isPivotCol[16] = {};
		for (int pc : pivotCols)
		{
			if (pc < numVars)
				isPivotCol[pc] = true;
		}
		for (int col = 0; col < numVars; ++col)
		{
			if (!isPivotCol[col])
				freeVars.push_back(col);
		}

		// Assign parameters to free variables
		int freeCount = freeVars.size();
		if (freeCount >= 1) x[freeVars[0]] = m;
		if (freeCount >= 2) x[freeVars[1]] = n;
		if (freeCount >= 3) x[freeVars[2]] = k;

		// Back substitution using known pivot positions
		// We need to map pivot columns to their rows
		std::vector<int> pivotRowForCol(numVars, -1);
		for (int i = 0; i < pivotCols.size(); ++i)
		{
			int col = pivotCols[i];
			// Find which row has the pivot in this column (should be row i)
			// But to be safe, we'll assume pivot is at (i, pivotCols[i])
			pivotRowForCol[col] = i;
		}

		// Now back-substitute from bottom to top, only for pivot rows
		for (int i = pivotCols.size() - 1; i >= 0; --i)
		{
			int pivotCol = pivotCols[i];
			int r = i;  // pivot is in row i

			int rhs = at(r, cols - 1);
			for (int c = pivotCol + 1; c < numVars; ++c)
			{
				rhs -= at(r, c) * x[c];
			}

			int pivotVal = at(r, pivotCol);
			if (pivotVal == 0) throw; // Should not happen

			if (rhs % pivotVal != 0)
				return false;

			x[pivotCol] = rhs / pivotVal;
			if (x[pivotCol] < 0)
				return false;
		}

		// After solving all pivot variables, check all rows for consistency
		// (important: catch any redundant/inconsistent rows)
		for (int r = 0; r < rows; ++r)
		{
			int computed = 0;
			for (int c = 0; c < numVars; ++c)
			{
				computed += at(r, c) * x[c];
			}
			if (computed != at(r, cols - 1))
				return false;
		}

		return true;
	}
};

struct Entry
{
	std::string data;
	uint16_t goal{};
	uint16_t source[14]{};
	int num_source{};

	int target_vector[16]{};
	int num_target{};
	int rows{};
	int cols{};

	Matrix A;

	void createMatrix(std::vector<int> coefficients)
	{
		cols++;
		A = Matrix(rows, cols);

		int col = 0;
		int row = 0;
		for (int coeff : coefficients)
		{
			if (coeff == -1)
			{
				col++;
				row = 0;
			}
			else
			{
				int idx = coeff * cols + col;
				if (idx >= rows * cols) throw;
				A[coeff * cols + col] = 1;
			}
		}
		if (num_target > rows) throw;

		for(int r = 0; r < num_target; r++)
		{
			A[r * cols + (cols - 1)] = target_vector[r];
		}
	}

	Entry() { }
	Entry(std::string value)
	{
		data = value;
		int length = value.length();
		int i = 1;
		int shift = 0;
		for (; i < length; i++, shift++)
		{
			if (value[i] == ']') break;
			rows++;
			if (value[i] == '.') continue;
			goal |= (1 << shift);
		}
		i += 2;
		//std::cout << value << ":\n" << goal << " - ";

		num_source = 0;
		std::vector<int> coeffvec;
		while (value[i++] == '(')
		{
			uint16_t src_val = 0;
			while (value[i] != ' ')
			{
				int coeff = (value[i++] - '0');
				if (coeff >= rows) throw;
				coeffvec.push_back(coeff);
				src_val |= (1 << coeff);
				i++;
			}
			coeffvec.push_back(-1); //delimit
			source[num_source] = src_val;
			//std::cout << src_val << " ";
			num_source++;
			cols++;
			i++;
		}
		//std::cout << "-" << num_source << "\n";
		if (i >= length) throw;

		//Part 2 - these are binary vectors in the form of (00100), (01010) etc.

		char* token;
		const char* str = value.c_str() + i;
		token = strtok((char*)str, ",");
		//std::cout << "|"
		while (token != NULL)
		{
			target_vector[num_target++] = atoi(token);
			token = strtok(NULL, ",");
			//std::cout << target_vector[num_target - 1] << "\n";
		}
		if (num_target >= 16) throw;
		if (num_target != rows) throw; 
		if (num_source != cols) throw;

		createMatrix(coeffvec);
	}

	uint16_t generate(uint16_t val, uint16_t bitmask) const
	{
		Bitloop(bitmask)
		{
			val ^= source[IndexOf(bitmask)];
		}
		return val;
	}

	int solve() const
	{
		//We observe the problem is XOR, so A^A == 0, any permutation of 1s and 0s is valid.
		//We use ncr to find number of permutations. Number of bits set = n
		if (num_source >= 14) throw;
		for(int n = 1; n <= num_source; n++)
		{
			uint16_t count = ncr(num_source, n);
			uint16_t pattern = (1ull << n) - 1;
			//std::cout << n << " bit\n";
			for (uint16_t i = 0; i < count; i++)
			{
				if (generate(0, pattern) == goal) return n;
				//std::cout << std::bitset<12>(value) << "\n";
				bit_twiddle_permute(pattern);
			}
		}
		//No solution - cannot be
		throw;
	}

	void push(int* current, int idx) const
	{
		uint16_t src = source[idx];
		Bitloop(src)
		{
			int index = IndexOf(src);
			current[index]++;
		}
		
		int score = 0;
		for (int i = 0; i < num_target; i++)
		{
			std::cout << (target_vector[i] - current[i]) << ' ';
		}
		std::cout <<" - " << score << "\n";
	}

	void pop(int* current, int idx) const
	{
		uint16_t src = source[idx];
		Bitloop(src)
		{
			int index = IndexOf(src);
			current[index]--;
		}
	}

	int sumOf(int x[16])
	{
		int sum = 0;
		for (int i = 0; i < 16; i++) sum += x[i];
		return sum;
	}

	int solve2()
	{
		A.makeTriangular();
		A.print();

		int x[16]{};
		int freeVars = A.getNumFreeVariables();
		std::cout << "matrix has " << freeVars << " free variables. solving...";

		int bestSum = INT_MAX;
		if (freeVars < 0 || freeVars > 3) throw;
		
		int bestSolution[16];
		std::vector<int> bounds = A.getFreeVariableUpperBounds(); 
		int mMax = (freeVars >= 1) ? bounds[0] : 1;
		int nMax = (freeVars >= 2) ? bounds[1] : 1;
		int kMax = (freeVars >= 3) ? bounds[2] : 1;

		for (int m = 0; m < mMax; m++) {
		for (int n = 0; n < nMax; n++) {
		for (int k = 0; k < kMax; k++)
		{
			if (A.trysolve(x, m, n, k))
			{
				int sum = sumOf(x);
				if (sum < bestSum)
				{
					bestSum = sum;
				}
			}
		}}}

		std::cout <<" best sum = " << bestSum << "\n";
		return bestSum;
	}

};

void readFile(const std::string& filename, std::vector<Entry>& data) {
	std::ifstream file(filename);
	std::string line;
	while (std::getline(file, line)) {
		data.push_back(line);
	}
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
	std::vector<Entry> input;
	readFile("input.txt", input);

	int sum = 0;
	int sum2 = 0;
	for (auto& x : input)
	{
		sum += x.solve(); 
		sum2 += x.solve2();
	}
	std::cout << "Part 1: " << sum << "\n";
	std::cout << "Part 2: " << sum2 << "\n";
}