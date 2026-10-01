#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <iomanip>

// --- Búsqueda Binaria O(log n) ---
bool binarySearch(const std::vector<int>& arr, int target) {
    int left = 0;
    int right = static_cast<int>(arr.size()) - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == target) return true;
        if (arr[mid] < target) left = mid + 1;
        else right = mid - 1;
    }
    return false;
}

// --- Mergesort O(n log n) ---
void merge(std::vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    std::vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void mergeSort(std::vector<int>& arr, int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib(1, 100000000);

    // Tamaños a evaluar
    std::vector<int> sizes = { 10000, 50000, 100000, 500000, 1000000, 5000000 };

    std::cout << "======================================================\n";
    std::cout << "                BENCHMARK COMPARATIVO     \n";
    std::cout << "======================================================\n";
    std::cout << std::setw(12) << "N"
        << std::setw(22) << "Busq. Binaria (us)"
        << std::setw(20) << "Mergesort (ms)" << "\n";
    std::cout << "------------------------------------------------------\n";

    for (int n : sizes) {
        // 1. Generamos el arreglo base con valores aleatorios para este tamaño N
        std::vector<int> base_arr(n);
        for (int i = 0; i < n; ++i) {
            base_arr[i] = distrib(gen);
        }

        // --- PRUEBA MERGESORT ---
        // Usamos una copia exacta del arreglo aleatorio desordenado
        std::vector<int> arr_ms = base_arr;

        auto start_ms = std::chrono::high_resolution_clock::now();
        mergeSort(arr_ms, 0, n - 1);
        auto end_ms = std::chrono::high_resolution_clock::now();

        auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_ms - start_ms).count();


        // --- PRUEBA BÚSQUEDA BINARIA ---
        // Usamos otra copia del mismo arreglo aleatorio, pero ordenado por exigencia del algoritmo
        std::vector<int> arr_bs = base_arr;
        std::sort(arr_bs.begin(), arr_bs.end());

        int iterations = 10000;
        auto start_bs = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < iterations; ++i) {
            // Buscamos un objetivo que exista en el conjunto original aleatorio
            int target = base_arr[distrib(gen) % n];
            binarySearch(arr_bs, target);
        }
        auto end_bs = std::chrono::high_resolution_clock::now();

        auto duration_us = std::chrono::duration_cast<std::chrono::microseconds>(end_bs - start_bs).count();
        double avg_time_us = static_cast<double>(duration_us) / iterations;

        // Imprimimos la fila comparativa
        std::cout << std::setw(12) << n
            << std::setw(22) << std::fixed << std::setprecision(3) << avg_time_us
            << std::setw(20) << duration_ms << "\n";
    }

    std::cout << "------------------------------------------------------\n";

    return 0;
}