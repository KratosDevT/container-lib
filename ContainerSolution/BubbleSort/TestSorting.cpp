#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <functional>
#include <chrono>
#include "sorting.h"

using namespace STDev;

// ============================================================================
// UTILITY FUNCTIONS
// ============================================================================

void printArray(const std::vector<int>& vec, const char* label)
{
	std::cout << label << ": ";
	for (const auto& elem : vec)
		std::cout << elem << " ";
	std::cout << std::endl;
}

void printArray(const int* arr, int size, const char* label)
{
	std::cout << label << ": ";
	for (int i = 0; i < size; ++i)
		std::cout << arr[i] << " ";
	std::cout << std::endl;
}

bool isSorted(const std::vector<int>& vec)
{
	return std::is_sorted(vec.begin(), vec.end());
}

// ============================================================================
// TEST SUITE
// ============================================================================

class TestSuite
{
private:
	int passedTests = 0;
	int totalTests = 0;

	void printTestResult(const char* testName, bool passed)
	{
		totalTests++;
		if (passed)
		{
			passedTests++;
			std::cout << testName << " - PASSED" << std::endl;
		}
		else
		{
			std::cout << testName << " - FAILED" << std::endl;
		}
	}

public:
	// Test con array normale
	template<typename SortFunc>
	void testNormalArray(const char* sortName, SortFunc sortFunc)
	{
		std::vector<int> v = { 7, 5, 9, 1, 4, 2, 6 };
		sortFunc(v.begin(), v.end());
		bool passed = isSorted(v);
		printTestResult((std::string(sortName) + " - Normal Array").c_str(), passed);
	}

	template<typename SortFunc>
	void testSortedArray(const char* sortName, SortFunc sortFunc)
	{
		std::vector<int> v = { 1, 2, 3, 4, 5, 6, 7 };
		sortFunc(v.begin(), v.end());
		bool passed = isSorted(v);
		printTestResult((std::string(sortName) + " - Already Sorted").c_str(), passed);
	}

	template<typename SortFunc>
	void testReversedArray(const char* sortName, SortFunc sortFunc)
	{
		std::vector<int> v = { 9, 8, 7, 6, 5, 4, 3, 2, 1 };
		sortFunc(v.begin(), v.end());
		bool passed = isSorted(v);
		printTestResult((std::string(sortName) + " - Reversed Array").c_str(), passed);
	}

	template<typename SortFunc>
	void testDuplicates(const char* sortName, SortFunc sortFunc)
	{
		std::vector<int> v = { 5, 2, 8, 2, 9, 1, 5, 5 };
		sortFunc(v.begin(), v.end());
		bool passed = isSorted(v);
		printTestResult((std::string(sortName) + " - Array with Duplicates").c_str(), passed);
	}

	template<typename SortFunc>
	void testSingleElement(const char* sortName, SortFunc sortFunc)
	{
		std::vector<int> v = { 42 };
		sortFunc(v.begin(), v.end());
		bool passed = isSorted(v) && v[0] == 42;
		printTestResult((std::string(sortName) + " - Single Element").c_str(), passed);
	}

	template<typename SortFunc>
	void testEmptyArray(const char* sortName, SortFunc sortFunc)
	{
		std::vector<int> v = {};
		sortFunc(v.begin(), v.end());
		bool passed = v.empty();
		printTestResult((std::string(sortName) + " - Empty Array").c_str(), passed);
	}

	template<typename SortFunc>
	void testTwoElements(const char* sortName, SortFunc sortFunc)
	{
		std::vector<int> v = { 5, 2 };
		sortFunc(v.begin(), v.end());
		bool passed = isSorted(v) && v[0] == 2 && v[1] == 5;
		printTestResult((std::string(sortName) + " - Two Elements").c_str(), passed);
	}

	template<typename SortFunc>
	void testAllSame(const char* sortName, SortFunc sortFunc)
	{
		std::vector<int> v = { 5, 5, 5, 5, 5 };
		sortFunc(v.begin(), v.end());
		bool passed = isSorted(v) && v.size() == 5;
		printTestResult((std::string(sortName) + " - All Same Elements").c_str(), passed);
	}

	template<typename SortFunc>
	void testNegativeNumbers(const char* sortName, SortFunc sortFunc)
	{
		std::vector<int> v = { -5, 3, -1, 0, -10, 7 };
		sortFunc(v.begin(), v.end());
		bool passed = isSorted(v);
		printTestResult((std::string(sortName) + " - Negative Numbers").c_str(), passed);
	}

	template<typename SortFunc>
	void testCustomComparator(const char* sortName, SortFunc sortFunc)
	{
		std::vector<int> v = { 3, 1, 4, 1, 5, 9, 2, 6 };
		sortFunc(v.begin(), v.end(), std::greater<int>());
		bool passed = std::is_sorted(v.begin(), v.end(), std::greater<int>());
		printTestResult((std::string(sortName) + " - Custom Comparator (Descending)").c_str(), passed);
	}

	template<typename SortFunc>
	void runAllTests(const char* sortName, SortFunc sortFunc)
	{
		std::cout << "\n========================================" << std::endl;
		std::cout << "Testing: " << sortName << std::endl;
		std::cout << "========================================" << std::endl;

		testNormalArray(sortName, sortFunc);
		testSortedArray(sortName, sortFunc);
		testReversedArray(sortName, sortFunc);
		testDuplicates(sortName, sortFunc);
		testSingleElement(sortName, sortFunc);
		testEmptyArray(sortName, sortFunc);
		testTwoElements(sortName, sortFunc);
		testAllSame(sortName, sortFunc);
		testNegativeNumbers(sortName, sortFunc);
		testCustomComparator(sortName, sortFunc);
	}

	void printSummary()
	{
		std::cout << "\n========================================" << std::endl;
		std::cout << "TEST SUMMARY" << std::endl;
		std::cout << "========================================" << std::endl;
		std::cout << "Total Tests: " << totalTests << std::endl;
		std::cout << "Passed: " << passedTests << std::endl;
		std::cout << "Failed: " << (totalTests - passedTests) << std::endl;
		std::cout << "Success Rate: " << (passedTests * 100.0 / totalTests) << "%" << std::endl;
		std::cout << "========================================\n" << std::endl;
	}
};

// ============================================================================
// MAIN
// ============================================================================

int main()
{
	TestSuite suite;

	// Test tutti gli algoritmi
	suite.runAllTests("Bubble Sort",
		[](auto first, auto last, auto... args)
		{
			if constexpr (sizeof...(args) == 0)
				bubble_sort(first, last);
			else
				bubble_sort(first, last, args...);
		});

	suite.runAllTests("Insertion Sort",
		[](auto first, auto last, auto... args)
		{
			if constexpr (sizeof...(args) == 0)
				insertion_sort(first, last);
			else
				insertion_sort(first, last, args...);
		});

	suite.runAllTests("Selection Sort",
		[](auto first, auto last, auto... args)
		{
			if constexpr (sizeof...(args) == 0)
				selection_sort(first, last);
			else
				selection_sort(first, last, args...);
		});

	suite.runAllTests("Quick Sort",
		[](auto first, auto last, auto... args)
		{
			if constexpr (sizeof...(args) == 0)
				quick_sort(first, last);
			else
				quick_sort(first, last, args...);
		});

	suite.runAllTests("Merge Sort",
		[](auto first, auto last, auto... args)
		{
			if constexpr (sizeof...(args) == 0)
				merge_sort(first, last);
			else
				merge_sort(first, last, args...);
		});

	suite.runAllTests("Heap Sort",
		[](auto first, auto last, auto... args)
		{
			if constexpr (sizeof...(args) == 0)
				heap_sort(first, last);
			else
				heap_sort(first, last, args...);
		});

	suite.runAllTests("Intro Sort",
		[](auto first, auto last, auto... args)
		{
			if constexpr (sizeof...(args) == 0)
				intro_sort(first, last);
			else
				intro_sort(first, last, args...);
		});

	suite.printSummary();

	// ========================================================================
	// TEST CON STRATEGY PATTERN (tutti gli algoritmi)
	// ========================================================================

	std::cout << "\n========================================" << std::endl;
	std::cout << "Testing Strategy Pattern" << std::endl;
	std::cout << "========================================" << std::endl;

	int test_arr[] = { 7, 5, 9, 1, 4, 2, 6 };
	int size = 7;

	std::vector<std::unique_ptr<SortStrategy<int>>> strategies;
	strategies.push_back(std::make_unique<BubbleSort<int>>());
	strategies.push_back(std::make_unique<InsertionSort<int>>());
	strategies.push_back(std::make_unique<SelectionSort<int>>());
	strategies.push_back(std::make_unique<QuickSort<int>>());
	strategies.push_back(std::make_unique<MergeSort<int>>());
	strategies.push_back(std::make_unique<HeapSort<int>>());
	strategies.push_back(std::make_unique<IntroSort<int>>());

	for (auto& strategy : strategies)
	{
		int arr[] = { 7, 5, 9, 1, 4, 2, 6 };
		Algorithm<int> algo;
		algo.setStrategy(std::move(strategy));

		std::cout << "\nStrategy: " << algo.getCurrentStrategyName() << std::endl;
		printArray(arr, size, "Before");
		algo.sort(arr, arr + size);
		printArray(arr, size, "After");

		bool passed = std::is_sorted(arr, arr + size);
		std::cout << (passed ? "PASSED" : "FAILED") << std::endl;
	}

	return 0;
}