#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <cmath>
#include <iomanip>

using namespace std;
using namespace std::chrono;

// Implementación de búsqueda binaria
int busquedaBinaria(const vector<int>& arr, int target) {
    int left = 0;
    int right = arr.size() - 1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == target) return mid;
        if (arr[mid] < target) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}

int main() {
    // Tamaños de arreglo a evaluar (multiplicando por 10 cada iteración)
    vector<int> sizes = {1000, 10000, 100000, 1000000, 10000000, 100000000};
    int num_queries = 5000000; // 5 millones de búsquedas para hacer el tiempo medible
    
    cout << left << setw(12) << "N" 
         << setw(15) << "Tiempo (ms)" 
         << setw(15) << "log2(N)" 
         << "Diferencia de Tiempo" << endl;
    cout << string(60, '-') << endl;
    
    double tiempo_anterior = 0;

    for (int n : sizes) {
        // 1. Generar arreglo ordenado
        vector<int> arr(n);
        for (int i = 0; i < n; i++) {
            arr[i] = i * 2; 
        }

        // 2. Generar consultas aleatorias
        mt19937 gen(42); 
        uniform_int_distribution<> dist(0, n * 2);
        vector<int> queries(num_queries);
        for (int i = 0; i < num_queries; i++) {
            queries[i] = dist(gen);
        }

        // 3. Medir el tiempo de ejecución
        long long checksum = 0; // Evita que el compilador elimine el ciclo por optimización (Dead Code Elimination)
        
        auto start = high_resolution_clock::now();
        
        for (int q : queries) {
            checksum += busquedaBinaria(arr, q);
        }
        
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start).count();

        // 4. Mostrar resultados
        cout << left << setw(12) << n 
             << setw(15) << duration 
             << setw(15) << log2(n);
             
        if (tiempo_anterior > 0) {
            cout << "+" << (duration - tiempo_anterior) << " ms";
        } else {
            cout << "-";
        }
        cout << endl;
        
        tiempo_anterior = duration;
    }
    
    return 0;
}