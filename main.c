#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/// 
/// writeFile.c
/// 

char writeFile(FILE* outputFile, int* arr, unsigned int arrSize)
{
	for (unsigned int i = 0; i < arrSize; i++)
	{
		if (fprintf(outputFile, "%d ", arr[i]) < 0)
			return 1;
	}
	if (fprintf(outputFile, "%c", (char)10) < 0)
		return 1;

	return 0;
}

/// 
/// writeFile.c
/// 

/// 
/// readFile.c
/// 
/// 

unsigned int arrLengthFinding(FILE* inputFile)
{
	unsigned int arrSize;

	if (fscanf(inputFile, "%u", &arrSize) < 1)
		return 0;

	return arrSize;
}

char readFile(FILE* inputFile, int* arr, unsigned int arrSize)
{
	for (unsigned int i = 0; i < arrSize; i++)
	{
		if (fscanf(inputFile, "%d", &arr[i]) < 1)
			return 1;
	}
	return 0;
}

/// 
/// readFile.c
/// 

/// 
/// quickSort.c
/// 

#define cutOffPoint 7000

void quickSort(int* arr, int left, int right)
{
	const int currLeft = left, currRight = right;
	const int pivot = arr[(left + right) / 2];

	while (left <= right)
	{
		while (arr[left] < pivot)
			left++;
		while (arr[right] > pivot)
			right--;
		if (left <= right)
		{
			int buff = arr[left];
			arr[left] = arr[right];
			arr[right] = buff;
			left++;
			right--;
		}
	}

	if (left < currRight)
		quickSort(arr, left, currRight);

	if (currLeft < right)
		quickSort(arr, currLeft, right);
}

void quickSortSection(int* arr, int left, int right)
{
	const int currLeft = left, currRight = right;
	const int pivot = arr[(left + right) / 2];

	while (left <= right)
	{
		while (arr[left] < pivot)
			left++;
		while (arr[right] > pivot)
			right--;

		if (left <= right)
		{
			int buff = arr[left];
			arr[left] = arr[right];
			arr[right] = buff;
			left++;
			right--;
		}
	}

	unsigned short int rightValue = 0;
	unsigned short int leftValue = 0;

	if (currLeft < right)
	{
		rightValue++;
		if (right - currLeft > cutOffPoint)
			rightValue++;
	}

	if (left < currRight)
	{
		leftValue++;
		if (currRight - left > cutOffPoint)
			leftValue++;
	}

	if (rightValue == 2 && leftValue == 2)
	{
#pragma omp parallel
		{
#pragma omp sections nowait
			{
#pragma omp section
				{
					quickSortSection(arr, currLeft, right);
				}

#pragma omp section
				{
					quickSortSection(arr, left, currRight);
				}
			}
		}
	}
	else if (rightValue == 2 && leftValue == 1)
	{
#pragma omp parallel
		{
#pragma omp sections nowait
			{
#pragma omp section
				{
					quickSortSection(arr, currLeft, right);
				}

#pragma omp section
				{
					quickSort(arr, left, currRight);
				}
			}
		}
	}
	else if (rightValue == 1 && leftValue == 2)
	{
#pragma omp parallel
		{
#pragma omp sections nowait
			{
#pragma omp section
				{
					quickSortSection(arr, left, currRight);
				}

#pragma omp section
				{
					quickSort(arr, currLeft, right);
				}
			}
		}
	}
	else if (rightValue == 1 && leftValue == 1)
	{
		quickSort(arr, currLeft, right);
		quickSort(arr, left, currRight);
	}
	else if (rightValue == 0 && leftValue == 2)
	{
		quickSortSection(arr, left, currRight);
	}
	else if (rightValue == 2 && leftValue == 0)
	{
		quickSortSection(arr, currLeft, right);
	}
	else if (rightValue == 0 && leftValue == 1)
	{
		quickSort(arr, left, currRight);
	}
	else if (rightValue == 1 && leftValue == 0)
	{
		quickSort(arr, currLeft, right);
	}
}

void quickSortTask(int* arr, int left, int right)
{
	int currLeft = left, currRight = right;
	int pivot = arr[(left + right) / 2];

	while (left <= right)
	{
		while (arr[left] < pivot)
			left++;
		while (arr[right] > pivot)
			right--;

		if (left <= right)
		{
			int buff = arr[left];
			arr[left] = arr[right];
			arr[right] = buff;
			left++;
			right--;
		}
	}

	unsigned short int rightValue = 0;
	unsigned short int leftValue = 0;

	if (currLeft < right)
	{
		rightValue++;
		if (right - currLeft > cutOffPoint)
			rightValue++;
	}

	if (left < currRight)
	{
		leftValue++;
		if (currRight - left > cutOffPoint)
			leftValue++;
	}

	if (rightValue == 2 && leftValue == 2)
	{
#pragma omp task
		quickSortTask(arr, currLeft, right);

		quickSortTask(arr, left, currRight);
	}
	else if (rightValue == 1 && leftValue == 2)
	{
#pragma omp task
		quickSortTask(arr, left, currRight);

		quickSort(arr, currLeft, right);
	}
	else if (rightValue == 2 && leftValue == 1)
	{
#pragma omp task
		quickSortTask(arr, currLeft, right);

		quickSort(arr, left, currRight);
	}
	else if (rightValue == 1 && leftValue == 1)
	{
#pragma omp task
		quickSort(arr, currLeft, right);

		quickSort(arr, left, currRight);
	}
	else if (rightValue == 2 && leftValue == 0)
	{
		quickSortTask(arr, currLeft, right);
	}
	else if (rightValue == 0 && leftValue == 2)
	{
		quickSortTask(arr, left, currRight);
	}
	else if (rightValue == 1 && leftValue == 0)
	{
		quickSort(arr, currLeft, right);
	}
	else if (rightValue == 0 && leftValue == 1)
	{
		quickSort(arr, left, currRight);
	}
}

/// 
/// quickSort.c
///

/// 
/// main.c
/// 

int main(int argc, char* argv[])
{
	if (argc == 5)
	{
		short int numOfThreads = atoi(argv[3]);
		unsigned short int implementationType = atoi(argv[4]);

		if (numOfThreads < -1)
		{
			fprintf(stderr, "Incorrect number of threads!\n");
			return 1;
		}

		if (0 > implementationType || implementationType > 2)
		{
			fprintf(stderr, "Incorrect implementation type!\n");
			return 1;
		}

		FILE* inputFile = fopen(argv[1], "r");
		if (inputFile == NULL)
		{
			fprintf(stderr, "Input file open error!\n");
			return 1;
		}

		FILE* outputFile = fopen(argv[2], "w");
		if (outputFile == NULL)
		{
			fprintf(stderr, "Output file open error!\n");
			fclose(inputFile);
			return 1;
		}

		const unsigned int arrSize = arrLengthFinding(inputFile);
		if (arrSize < 0)
		{
			fprintf(stderr, "Invalid array size!\n");
			fclose(inputFile);
			fclose(outputFile);
			return 1;
		}

		int* arr = (int*)malloc(sizeof(int) * arrSize);
		if (arr == NULL)
		{
			fprintf(stderr, "Insufficient memory available!\n");
			free(arr);
			fclose(inputFile);
			fclose(outputFile);
			return 1;
		}

		if (readFile(inputFile, arr, arrSize))
		{
			fprintf(stderr, "Invalid file format!\n");
			free(arr);
			fclose(inputFile);
			fclose(outputFile);
			return 1;
		}

		unsigned short int maxThreads = omp_get_max_threads();
		unsigned short int actualNumberOfThreads = 1;

		double startRuntime, endRuntime;

		if (numOfThreads == 0)
			numOfThreads = maxThreads;

		if (numOfThreads == -1)
			implementationType = 0;

		if (implementationType == 0)
		{
			startRuntime = omp_get_wtime();
			quickSort(arr, 0, arrSize - 1);
			endRuntime = omp_get_wtime();
		}
		else if (implementationType == 1)
		{
			omp_set_max_active_levels(numOfThreads);
			//omp_set_max_active_levels((int)log2f(numOfThreads));
			actualNumberOfThreads = numOfThreads;
			
			if (numOfThreads == 1)
				omp_set_num_threads(1);
			else
				omp_set_num_threads(2);

			startRuntime = omp_get_wtime();
			quickSortSection(arr, 0, arrSize - 1);
			endRuntime = omp_get_wtime();
		}
		else if (implementationType == 2)
		{
			startRuntime = omp_get_wtime();
#pragma omp parallel num_threads(numOfThreads) shared(arr, arrSize)
			{
#pragma omp master
				{
					actualNumberOfThreads = omp_get_num_threads();
					quickSortTask(arr, 0, arrSize - 1);
				}
			}
			endRuntime = omp_get_wtime();
		}

		printf("Time (%i thread(s)): %g ms\n", actualNumberOfThreads, (endRuntime - startRuntime) * 1000);

		if (writeFile(outputFile, arr, arrSize))
		{
			fprintf(stderr, "File write error!\n");
			free(arr);
			fclose(inputFile);
			fclose(outputFile);
			return 1;
		}

		free(arr);
		fclose(inputFile);
		fclose(outputFile);
	}
	else
	{
		fprintf(stderr, "Wrong number of arguments!\n");
		return 1;
	}
}
