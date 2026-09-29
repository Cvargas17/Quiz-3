#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <fstream>
#include <algorithm>


namespace {

int busquedaBinaria(const std::vector<int>& arr, int objetivo) {
    int izquierda = 0;
    int derecha = static_cast<int>(arr.size()) - 1;

    while (izquierda <= derecha) {
        int medio = izquierda + (derecha - izquierda) / 2;

        if (arr[medio] == objetivo)
            return medio;
        if (arr[medio] < objetivo)
            izquierda = medio + 1;
        else
            derecha = medio - 1;
    }
    return -1;
}

void merge(std::vector<int>& arr, int left, int mid, int right, std::vector<int>& temp) {
    int i = left;
    int j = mid + 1;
    int k = left;

    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }

    while (i <= mid) {
        temp[k++] = arr[i++];
    }

    while (j <= right) {
        temp[k++] = arr[j++];
    }

    for (i = left; i <= right; i++) {
        arr[i] = temp[i];
    }
}

void mergeSortAux(std::vector<int>& arr, int left, int right, std::vector<int>& temp) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortAux(arr, left, mid, temp);
        mergeSortAux(arr, mid + 1, right, temp);
        merge(arr, left, mid, right, temp);
    }
}

void mergeSort(std::vector<int>& arr) {
    if (arr.empty()) return;
    std::vector<int> temp(arr.size());
    mergeSortAux(arr, 0, static_cast<int>(arr.size()) - 1, temp);
}

}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::vector<int> size = { 1000, 5000, 10000, 50000, 100000, 500000, 1000000 };

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(1, 10000000);

    std::ofstream archivoBinaria("benchmark_busqueda_binaria.csv");
    archivoBinaria << "Tamano_n,Tiempo_Promedio_ns\n";

    std::ofstream archivoMerge("benchmark_mergesort.csv");
    archivoMerge << "Tamano_n,Tiempo_Promedio_ms\n";


    for (int n : size) {
        // --- Benchmark Búsqueda Binaria ---
        std::vector<int> arrOrdenado(n);
        for (int i = 0; i < n; i++) arrOrdenado[i] = i * 2;
        int objetivo = arrOrdenado[n / 2];

        int repeticionesBin = 1000;
        auto inicioBin = std::chrono::high_resolution_clock::now();
        for (int r = 0; r < repeticionesBin; r++) {
            volatile int resultado = busquedaBinaria(arrOrdenado, objetivo);
            (void)resultado;
        }
        auto finBin = std::chrono::high_resolution_clock::now();
        double tiempoPromBin = std::chrono::duration<double, std::nano>(finBin - inicioBin).count() / repeticionesBin;

        archivoBinaria << n << "," << tiempoPromBin << "\n";
        std::cout << "[Busqueda Binaria] n = " << n << " -> " << tiempoPromBin << " ns\n";

        // --- Benchmark Merge Sort ---
        int repeticionesMerge = 5;
        double tiempoTotalMerge = 0.0;

        for (int r = 0; r < repeticionesMerge; r++) {
            std::vector<int> arrDesordenado(n);
            for (int i = 0; i < n; i++) {
                arrDesordenado[i] = dis(gen);
            }

            auto inicioMerge = std::chrono::high_resolution_clock::now();
            mergeSort(arrDesordenado);
            auto finMerge = std::chrono::high_resolution_clock::now();

            tiempoTotalMerge += std::chrono::duration<double, std::milli>(finMerge - inicioMerge).count();
        }

        double tiempoPromMerge = tiempoTotalMerge / repeticionesMerge;
        archivoMerge << n << "," << tiempoPromMerge << "\n";
        std::cout << "[Merge Sort] n = " << n << " -> " << tiempoPromMerge << " ms\n";
    }

    archivoBinaria.close();
    archivoMerge.close();

    return 0;
}