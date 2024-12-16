#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <vector>

typedef int KeyType;
typedef int DataType;
typedef int BOOL;

#define TRUE 1
#define FALSE 0

typedef struct entry {
    KeyType key;
    DataType data;
} Entry;

typedef struct list {
    int n;
    Entry* D;
} List;

void generateRandomNumbers(std::vector<int>& numbers, int size) {
    numbers.clear();
    for (int i = 0; i < size; ++i) {
        numbers.push_back(std::rand());
    }
}

void fillListWithNumbers(List* list, const std::vector<int>& numbers) {
    list->n = numbers.size();
    list->D = new Entry[list->n];
    for (int i = 0; i < list->n; ++i) {
        list->D[i].key = numbers[i];
        list->D[i].data = 0; // Assuming data is not needed for sorting
    }
}

void swap(Entry* D, int i, int j) {
    Entry temp;
    if (i == j) return;
    temp = *(D + i);
    *(D + i) = *(D + j);
    *(D + j) = temp;
}

void selectionSort(List* list) {
    int minIndex, startIndex = 0;
    while (startIndex < list->n - 1) {
        minIndex = startIndex;
        for (int i = startIndex + 1; i < list->n; i++) {
            if (list->D[i].key < list->D[minIndex].key) {
                minIndex = i;
            }
        }
        swap(list->D, startIndex, minIndex);
        startIndex++;
    }
}

void insertSort(List* list) {
    int i, j;
    Entry insertItem;
    for (i = 1; i < list->n; i++) {
        insertItem = list->D[i];
        for (j = i - 1; j >= 0; j--) {
            if (insertItem.key < list->D[j].key) list->D[j + 1] = list->D[j];
            else break;
        }
        list->D[j + 1] = insertItem;
    }
}

void bubbleSort(List* list) {
    int i, j;
    for (i = list->n - 1; i > 0; i--) {
        BOOL isSwap = FALSE;
        for (j = 0; j < i; j++) {
            if (list->D[j].key > list->D[j + 1].key) {
                swap(list->D, j, j + 1);
                isSwap = TRUE;
            }
        }
        if (!isSwap) break;
    }
}

int partition(List* list, int low, int high) {
    int i = low, j = high + 1;
    Entry pivot = list->D[low];
    do {
        do i++; while (i <= high && list->D[i].key < pivot.key);
        do j--; while (list->D[j].key > pivot.key);
        if (i < j) swap(list->D, i, j);
    } while (i < j);
    swap(list->D, low, j);
    return j;
}

void quickSort(List* list, int low, int high) {
    int k;
    if (low < high) {
        k = partition(list, low, high);
        quickSort(list, low, k - 1);
        quickSort(list, k + 1, high);
    }
}

void quickSort(List* list) {
    quickSort(list, 0, list->n - 1);
}

void merge(List* list, Entry* temp, int low, int n1, int n2) {
    int i = low, j = low + n1;
    while (i <= low + n1 - 1 && j <= low + n1 + n2 - 1) {
        if (list->D[i].key < list->D[j].key) *temp++ = list->D[i++];
        else *temp++ = list->D[j++];
    }
    while (i <= low + n1 - 1) *temp++ = list->D[i++];
    while (j <= low + n1 + n2 - 1) *temp++ = list->D[j++];
}

void mergeSort(List* list) {
    Entry* temp = new Entry[list->n];
    int low, n1, n2, i, size = 1;
    while (size < list->n) {
        low = 0;
        while (low + size < list->n) {
            n1 = size;
            if (low + size * 2 < list->n) n2 = size;
            else n2 = list->n - low - size;
            merge(list, temp, low, n1, n2);
            for (i = 0; i < n1 + n2; i++) list->D[low + i] = temp[i];
            low += n1 + n2;
        }
        size *= 2;
    }
    delete[] temp;
}

typedef struct maxHeap {
    int n, maxSize;
    Entry* D;
} MaxHeap;

void adjustDown(Entry* temp, int i, int n) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left <= n && temp[left].key > temp[largest].key) {
        largest = left;
    }

    if (right <= n && temp[right].key > temp[largest].key) {
        largest = right;
    }

    if (largest != i) {
        swap(temp, i, largest);
        adjustDown(temp, largest, n);
    }
}

void heapSort(MaxHeap* hp) {
    int i;

    for (i = (hp->n / 2) - 1; i >= 0; i--) {
        adjustDown(hp->D, i, hp->n - 1);
    }

    for (i = hp->n - 1; i > 0; i--) {
        swap(hp->D, 0, i);
        adjustDown(hp->D, 0, i - 1);
    }
}

void measureSortTime(void (*sortFunction)(List*), List* list, const char* sortName, std::ofstream& csvFile, int size) {
    auto start = std::chrono::high_resolution_clock::now();

    sortFunction(list);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << sortName << " on size " << size << " took " << elapsed.count() << " seconds." << std::endl;
    csvFile << sortName << "," << size << "," << elapsed.count() << std::endl;
}

void measureHeapSortTime(MaxHeap* heap, const char* sortName, std::ofstream& csvFile, int size) {
    auto start = std::chrono::high_resolution_clock::now();
    heapSort(heap);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << sortName << " on size " << size << " took " << elapsed.count() << " seconds." << std::endl;
    csvFile << sortName << "," << size << "," << elapsed.count() << std::endl;
}

int main() {
    std::srand(std::time(0));

    std::ofstream csvFile("sorting_times.csv");
    csvFile << "Sort Algorithm,Size,Time (seconds)" << std::endl;

    int sizes[] = {500, 10000, 50000, 100000};
    for (int size : sizes) {
        std::vector<int> numbers;
        generateRandomNumbers(numbers, size);

        List list;
        fillListWithNumbers(&list, numbers);

        List listCopy;
        listCopy.D = new Entry[list.n];
        listCopy.n = list.n;

        std::copy(list.D, list.D + list.n, listCopy.D);
        measureSortTime(selectionSort, &listCopy, "Selection Sort", csvFile, size);

        std::copy(list.D, list.D + list.n, listCopy.D);
        measureSortTime(insertSort, &listCopy, "Insertion Sort", csvFile, size);

        std::copy(list.D, list.D + list.n, listCopy.D);
        measureSortTime(bubbleSort, &listCopy, "Bubble Sort", csvFile, size);

        std::copy(list.D, list.D + list.n, listCopy.D);
        measureSortTime(quickSort, &listCopy, "Quick Sort", csvFile, size);

        std::copy(list.D, list.D + list.n, listCopy.D);
        measureSortTime(mergeSort, &listCopy, "Merge Sort", csvFile, size);

        MaxHeap heap;
        heap.n = list.n;
        heap.maxSize = size;
        heap.D = new Entry[heap.maxSize];
        std::copy(list.D, list.D + list.n, heap.D);
        measureHeapSortTime(&heap, "Heap Sort", csvFile, size);

        delete[] list.D;
        delete[] listCopy.D;
        delete[] heap.D;
    }

    csvFile.close();
    return 0;
}
