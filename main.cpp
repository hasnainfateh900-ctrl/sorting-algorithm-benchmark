#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <random>
#include <iomanip>
#include <string>

using namespace std;

// ============================================================================
// SORTING ALGORITHM IMPLEMENTATIONS
// ============================================================================

/**
 * Bubble Sort - O(n^2) average and worst case
 * Simple comparison-based sorting algorithm.
 * Compares adjacent elements and swaps them if out of order.
 * Optimized with early termination when no swaps occur in a pass.
 */
void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    bool swapped;
    for (int i = 0; i < n - 1; i++) {
        swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        // Early termination: if no swaps in this pass, array is sorted
        if (!swapped)
            break;
    }
}

/**
 * Selection Sort - O(n^2) all cases
 * Finds the minimum element in the unsorted portion and places it at the front.
 * Always performs O(n^2) comparisons but at most n-1 swaps.
 */
void selectionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            swap(arr[i], arr[minIndex]);
        }
    }
}

/**
 * Insertion Sort - O(n^2) average/worst, O(n) best case
 * Builds a sorted array one element at a time.
 * Highly efficient for small or nearly sorted datasets.
 */
void insertionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

/**
 * Merge Sort - Helper: merge two sorted subarrays
 */
void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> leftArr(n1);
    vector<int> rightArr(n2);

    for (int i = 0; i < n1; i++)
        leftArr[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        rightArr[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (leftArr[i] <= rightArr[j]) {
            arr[k] = leftArr[i++];
        } else {
            arr[k] = rightArr[j++];
        }
        k++;
    }
    while (i < n1) arr[k++] = leftArr[i++];
    while (j < n2) arr[k++] = rightArr[j++];
}

/**
 * Merge Sort - O(n log n) all cases
 * Divide-and-conquer algorithm. Recursively splits and merges.
 * Requires O(n) additional memory.
 */
void mergeSortHelper(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortHelper(arr, left, mid);
        mergeSortHelper(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

void mergeSort(vector<int>& arr) {
    if (!arr.empty()) {
        mergeSortHelper(arr, 0, arr.size() - 1);
    }
}

/**
 * Quicksort - O(n log n) average, O(n^2) worst case
 * Uses median-of-three pivot selection to reduce worst-case likelihood.
 * Excellent cache performance; fastest in practice on random data.
 */
int partition(vector<int>& arr, int low, int high) {
    int mid = low + (high - low) / 2;

    // Median-of-three pivot selection
    if (arr[mid] < arr[low]) swap(arr[low], arr[mid]);
    if (arr[high] < arr[low]) swap(arr[low], arr[high]);
    if (arr[high] < arr[mid]) swap(arr[mid], arr[high]);

    swap(arr[mid], arr[high]);
    int pivot = arr[high];

    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            swap(arr[++i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSortHelper(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pivotIndex = partition(arr, low, high);
        quickSortHelper(arr, low, pivotIndex - 1);
        quickSortHelper(arr, pivotIndex + 1, high);
    }
}

void quickSort(vector<int>& arr) {
    if (!arr.empty()) {
        quickSortHelper(arr, 0, arr.size() - 1);
    }
}

/**
 * Shell Sort - O(n log n) to O(n^(3/2)) depending on gap sequence
 * Generalization of Insertion Sort using diminishing gap intervals.
 * Uses Knuth's gap sequence: h = 3h + 1 (1, 4, 13, 40, 121, 364, ...)
 */
void shellSort(vector<int>& arr) {
    int n = arr.size();
    int gap = 1;
    while (gap < n / 3) {
        gap = 3 * gap + 1;
    }
    while (gap >= 1) {
        for (int i = gap; i < n; i++) {
            int key = arr[i];
            int j = i;
            while (j >= gap && arr[j - gap] > key) {
                arr[j] = arr[j - gap];
                j -= gap;
            }
            arr[j] = key;
        }
        gap /= 3;
    }
}

// ============================================================================
// DATA GENERATION AND FILE I/O
// ============================================================================

/**
 * Generate a random dataset of 'size' distinct integers drawn from
 * the range [1, maxRange] and write to file.
 * Uses Fisher-Yates shuffle for unbiased random selection.
 */
void generateDataset(const string& filename, int size, int maxRange = 4000000) {
    cout << "Generating dataset of " << size << " items (from range 1-"
         << maxRange << ")..." << endl;

    // Build a pool and shuffle it for guaranteed distinct values
    vector<int> pool(maxRange);
    for (int i = 0; i < maxRange; i++) pool[i] = i + 1;

    random_device rd;
    mt19937 gen(rd());
    shuffle(pool.begin(), pool.end(), gen);

    ofstream outFile(filename);
    if (!outFile.is_open()) {
        cerr << "Error: Could not open file " << filename << " for writing." << endl;
        return;
    }
    for (int i = 0; i < size; i++) {
        outFile << pool[i] << "\n";
    }
    outFile.close();
    cout << "  -> Saved to " << filename << endl;
}

/**
 * Generate a dataset of 'size' distinct integers in ascending order.
 * Values are evenly spaced across the range [1, maxRange].
 */
void generateSortedAscendingDataset(const string& filename, int size) {
    int maxRange = 4000000;
    int step = maxRange / size;

    ofstream outFile(filename);
    if (!outFile.is_open()) {
        cerr << "Error: Could not open file " << filename << " for writing." << endl;
        return;
    }
    for (int i = 0; i < size; i++) {
        outFile << (i + 1) * step << "\n";
    }
    outFile.close();
    cout << "Sorted ascending dataset of " << size << " items saved to " << filename << endl;
}

/**
 * Generate a dataset of 'size' distinct integers in descending order.
 * Values are evenly spaced across the range [1, maxRange].
 */
void generateSortedDescendingDataset(const string& filename, int size) {
    int maxRange = 4000000;
    int step = maxRange / size;

    ofstream outFile(filename);
    if (!outFile.is_open()) {
        cerr << "Error: Could not open file " << filename << " for writing." << endl;
        return;
    }
    for (int i = size; i > 0; i--) {
        outFile << i * step << "\n";
    }
    outFile.close();
    cout << "Sorted descending dataset of " << size << " items saved to " << filename << endl;
}

/**
 * Load dataset from file into a vector<int>.
 */
vector<int> loadDataset(const string& filename) {
    vector<int> data;
    ifstream inFile(filename);
    if (!inFile.is_open()) {
        cerr << "Error: Could not open file " << filename << " for reading." << endl;
        return data;
    }
    int value;
    while (inFile >> value) {
        data.push_back(value);
    }
    inFile.close();
    cout << "Loaded " << data.size() << " items from " << filename << endl;
    return data;
}

// ============================================================================
// BENCHMARKING
// ============================================================================

/**
 * Benchmark a sorting algorithm on a single run.
 * 'data' is passed by value so the original remains unsorted for repeated runs.
 * Returns elapsed time in milliseconds.
 */
double benchmarkSort(void (*sortFunc)(vector<int>&), vector<int> data) {
    auto start = chrono::high_resolution_clock::now();
    sortFunc(data);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> elapsed = end - start;
    return elapsed.count();
}

/**
 * Run a sorting algorithm 'iterations' times and return the average time (ms).
 * Prints individual run times and the average.
 */
double benchmarkSortMultipleTimes(void (*sortFunc)(vector<int>&),
                                   const vector<int>& data, int iterations) {
    double totalTime = 0.0;
    for (int i = 0; i < iterations; i++) {
        double t = benchmarkSort(sortFunc, data);  // data copied each call
        totalTime += t;
        cout << "    Run " << (i + 1) << ": " << fixed << setprecision(2) << t << " ms" << endl;
    }
    double avg = totalTime / iterations;
    cout << "    Average: " << fixed << setprecision(2) << avg << " ms" << endl;
    return avg;
}

// ============================================================================
// USER INTERFACE
// ============================================================================

void displayMenu() {
    cout << "\n============================================" << endl;
    cout << "  Sorting Algorithms Benchmark Tool" << endl;
    cout << "  CSCI 6622 - Project Part 2" << endl;
    cout << "============================================" << endl;
    cout << "1. Generate Random Datasets" << endl;
    cout << "2. Run Single Algorithm Benchmark" << endl;
    cout << "3. Run All Algorithms Benchmark (Full Report)" << endl;
    cout << "4. Generate Sorted Ascending Datasets (Extra Credit)" << endl;
    cout << "5. Generate Sorted Descending Datasets (Extra Credit)" << endl;
    cout << "6. Exit" << endl;
    cout << "--------------------------------------------" << endl;
    cout << "Enter your choice: ";
}

void generateDatasetsMenu(const vector<int>& sizes) {
    cout << "\n--- Random Dataset Generation ---" << endl;
    for (int i = 0; i < (int)sizes.size(); i++) {
        cout << (i + 1) << ". Generate " << sizes[i] << " items dataset" << endl;
    }
    cout << (sizes.size() + 1) << ". Generate ALL datasets" << endl;
    cout << (sizes.size() + 2) << ". Back to Main Menu" << endl;
    cout << "Enter your choice: ";
    int choice;
    cin >> choice;

    if (choice >= 1 && choice <= (int)sizes.size()) {
        string filename = "dataset_" + to_string(sizes[choice - 1]) + ".txt";
        generateDataset(filename, sizes[choice - 1]);
    } else if (choice == (int)sizes.size() + 1) {
        for (int i = 0; i < (int)sizes.size(); i++) {
            string filename = "dataset_" + to_string(sizes[i]) + ".txt";
            generateDataset(filename, sizes[i]);
        }
        cout << "\nAll random datasets generated successfully!" << endl;
    }
}

void algorithmMenu() {
    cout << "\n--- Select Algorithm ---" << endl;
    cout << "1. Bubble Sort" << endl;
    cout << "2. Selection Sort" << endl;
    cout << "3. Insertion Sort" << endl;
    cout << "4. Merge Sort" << endl;
    cout << "5. Quicksort" << endl;
    cout << "6. Shell Sort" << endl;
    cout << "Enter your choice: ";
}

// ============================================================================
// MAIN PROGRAM
// ============================================================================

int main() {
    // Project-required dataset sizes
    vector<int> dataSizes = {25000, 75000, 120000, 350000, 500000};

    // Sorting function pointers
    void (*sortFunctions[])(vector<int>&) = {
        bubbleSort, selectionSort, insertionSort,
        mergeSort, quickSort, shellSort
    };
    string sortNames[] = {
        "Bubble Sort", "Selection Sort", "Insertion Sort",
        "Merge Sort", "Quicksort", "Shell Sort"
    };
    const int NUM_ALGOS = 6;
    const int NUM_RUNS  = 5;

    int choice;
    while (true) {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                generateDatasetsMenu(dataSizes);
                break;

            case 2: {
                algorithmMenu();
                int algoChoice;
                cin >> algoChoice;
                if (algoChoice < 1 || algoChoice > NUM_ALGOS) {
                    cout << "Invalid algorithm choice." << endl;
                    break;
                }

                cout << "\n--- Select Dataset Size ---" << endl;
                for (int i = 0; i < (int)dataSizes.size(); i++) {
                    cout << (i + 1) << ". " << dataSizes[i] << " items" << endl;
                }
                cout << "Enter your choice: ";
                int sizeChoice;
                cin >> sizeChoice;
                if (sizeChoice < 1 || sizeChoice > (int)dataSizes.size()) {
                    cout << "Invalid size choice." << endl;
                    break;
                }

                string filename = "dataset_" + to_string(dataSizes[sizeChoice - 1]) + ".txt";
                vector<int> data = loadDataset(filename);
                if (data.empty()) {
                    cout << "Error: Dataset is empty or file not found." << endl;
                    cout << "Please generate datasets first (Option 1)." << endl;
                    break;
                }

                cout << "\nRunning " << sortNames[algoChoice - 1]
                     << " on " << data.size() << " items (" << NUM_RUNS << " runs):" << endl;
                cout << "--------------------------------------------" << endl;
                double avg = benchmarkSortMultipleTimes(sortFunctions[algoChoice - 1], data, NUM_RUNS);
                cout << "--------------------------------------------" << endl;
                cout << sortNames[algoChoice - 1] << " | Size: " << data.size()
                     << " | Average: " << fixed << setprecision(2) << avg << " ms" << endl;
                break;
            }

            case 3: {
                cout << "\n============================================" << endl;
                cout << "  Full Benchmark: All Algorithms x All Sizes" << endl;
                cout << "============================================" << endl;

                // results[algorithm][datasetIndex] = average ms
                vector<vector<double>> results(NUM_ALGOS, vector<double>(dataSizes.size(), 0.0));

                for (int s = 0; s < (int)dataSizes.size(); s++) {
                    string filename = "dataset_" + to_string(dataSizes[s]) + ".txt";
                    vector<int> data = loadDataset(filename);
                    if (data.empty()) {
                        cout << "Error: Could not load " << filename << endl;
                        cout << "Please generate datasets first (Option 1)." << endl;
                        break;
                    }

                    cout << "\n--- Dataset Size: " << dataSizes[s] << " ---" << endl;
                    for (int a = 0; a < NUM_ALGOS; a++) {
                        cout << "  " << sortNames[a] << ":" << endl;
                        results[a][s] = benchmarkSortMultipleTimes(sortFunctions[a], data, NUM_RUNS);
                    }
                }

                // Print summary table
                cout << "\n============================================" << endl;
                cout << "         BENCHMARK RESULTS SUMMARY" << endl;
                cout << "============================================" << endl;
                cout << left << setw(18) << "Algorithm";
                for (int s = 0; s < (int)dataSizes.size(); s++) {
                    cout << right << setw(12) << dataSizes[s];
                }
                cout << endl;
                cout << string(18 + 12 * (int)dataSizes.size(), '-') << endl;

                for (int a = 0; a < NUM_ALGOS; a++) {
                    cout << left << setw(18) << sortNames[a];
                    for (int s = 0; s < (int)dataSizes.size(); s++) {
                        cout << right << setw(10) << fixed << setprecision(2)
                             << results[a][s] << "ms";
                    }
                    cout << endl;
                }
                cout << string(18 + 12 * (int)dataSizes.size(), '-') << endl;
                break;
            }

            case 4: {
                cout << "\nGenerating sorted ascending datasets..." << endl;
                for (int i = 0; i < (int)dataSizes.size(); i++) {
                    string filename = "dataset_asc_" + to_string(dataSizes[i]) + ".txt";
                    generateSortedAscendingDataset(filename, dataSizes[i]);
                }
                cout << "All ascending datasets generated!" << endl;
                break;
            }

            case 5: {
                cout << "\nGenerating sorted descending datasets..." << endl;
                for (int i = 0; i < (int)dataSizes.size(); i++) {
                    string filename = "dataset_desc_" + to_string(dataSizes[i]) + ".txt";
                    generateSortedDescendingDataset(filename, dataSizes[i]);
                }
                cout << "All descending datasets generated!" << endl;
                break;
            }

            case 6:
                cout << "\nExiting program. Goodbye!" << endl;
                return 0;

            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }
    }
    return 0;
}
