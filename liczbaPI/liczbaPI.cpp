#include <iostream>
#include <cmath>
#include <thread>
#include <vector>
#include <chrono>

// Funkcja reprezentująca funkcję podcałkową
double function(double x) {
    return 4.0 / (1.0 + x * x);
}

// Funkcja wykonująca obliczenia numeryczne w danym przedziale
double calculatePi(double start, double end, int numSteps) {
    double step = (end - start) / numSteps;
    double sum = 0.0;

    for (int i = 0; i < numSteps; ++i) {
        double x = start + (i + 0.5) * step;
        sum += function(x);
    }

    return sum * step;
}

int main() {
    // Ustawienia użytkownika
    int numThreads, numSteps;
    std::cout << "Podaj ilosc watkow: ";
    std::cin >> numThreads;
    std::cout << "Podaj ilosc krokow calkowania: ";
    std::cin >> numSteps;

    // Początek i koniec przedziału całkowania
    double start = 0.0;
    double end = 1.0;

    // Przygotowanie wątków
    std::vector<std::thread> threads;
    std::vector<double> results(numThreads, 0.0);

    // Mierzenie czasu rozpoczęcia obliczeń
    auto startTime = std::chrono::high_resolution_clock::now();

    // Rozpoczęcie obliczeń wątków
    for (int i = 0; i < numThreads; ++i) {
        double threadStart = start + i * (end - start) / numThreads;
        double threadEnd = threadStart + (end - start) / numThreads;

        threads.emplace_back([i, threadStart, threadEnd, numSteps, &results]() {
            results[i] = calculatePi(threadStart, threadEnd, numSteps);
            });
    }

    // Oczekiwanie na zakończenie wszystkich wątków
    for (auto& thread : threads) {
        thread.join();
    }

    // Sumowanie wyników
    double totalResult = 0.0;
    for (double result : results) {
        totalResult += result;
    }

    // Mierzenie czasu zakończenia obliczeń
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

    // Wyświetlanie wyników
    std::cout << "Wynik obliczen: " << totalResult << std::endl;
    std::cout << "Czas trwania obliczen: " << duration.count() << " ms" << std::endl;

    return 0;
}
