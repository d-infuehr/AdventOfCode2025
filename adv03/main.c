#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <string.h>
#define int64 long long int

int64 pow10(int n)
{
	int64 result = 1;
	for (int i = 0; i < n; i++)
		result *= 10;
	return result;
}

int64 maxOf(char* input, int length, int digits)
{
	int64 num = 0;
	int start = 0;
	while (digits >= 0)
	{
		for (int x = 9; x >= 0; x--)
		{
			for (int i = start; i <= length - digits; i++)
			{
				if (input[i] == '0' + x)
				{
					num += pow10(digits) * (int64)(input[i] - '0');
					start = i + 1;
					i = length;
					x = 0; 
					digits--;
					break;
				}
			}
		}
	}
	return num;
}

int main()
{
	FILE* file = fopen("input.txt", "r");
	if (file == NULL) return 1;
	
	char line[256];
	int64 sum1 = 0;
	int64 sum2 = 0;
	while (fgets(line, sizeof(line), file))
	{
		int end = strlen(line);
		sum1 += maxOf(line, strlen(line) - (line[end - 1] == '\n' ? 2 : 1), 1);
		sum2 += maxOf(line, strlen(line) - (line[end - 1] == '\n' ? 2 : 1), 11);
	}
	printf("TOTAL 1: %lld\n", sum1);
	printf("TOTAL 2: %lld\n", sum2);

	return 0;
}