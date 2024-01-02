#include <iostream>
#include <cmath>
#include <thread>
#include <vector>
#include <chrono>

const double a = 0.0;  // Dolna granica przedziału całkowania
const double b = 1.0;  // Górna granica przedziału całkowania

// Funkcja, której całkę obliczamy (przykładowa funkcja)
double funkcja(double x) {
    return 4.0 / (1.0 + x * x);
}

// Funkcja obliczająca przybliżoną wartość PI dla danego przedziału
double oblicz_pi_dla_przedzialu(double poczatek, double koniec, int liczba_podzialow) {
    double krok = (koniec - poczatek) / liczba_podzialow;
    double suma = 0.0;

    for (int i = 0; i < liczba_podzialow; ++i) {
        double x = poczatek + (i + 0.5) * krok;
        suma += funkcja(x);
    }

    return suma * krok;
}

int main() {
    int liczba_podzialow, liczba_watkow;

    // Ustawienia użytkownika
    std::cout << "Podaj ilość liczb z przedziału całki oznaczonej: ";
    std::cin >> liczba_podzialow;

    std::cout << "Podaj ilość wątków do użycia: ";
    std::cin >> liczba_watkow;

    // Wymiary każdego przedziału dla wątków
    int przedzialy_na_watek = liczba_podzialow / liczba_watkow;

    // Przygotowanie wektorów do przechowywania wyników i wątków
    std::vector<std::thread> watki;
    std::vector<double> wyniki(liczba_watkow, 0.0);

    auto start_time = std::chrono::high_resolution_clock::now();

    // Uruchomienie wątków
    for (int i = 0; i < liczba_watkow; ++i) {
        double poczatek_przedzialu = a + i * przedzialy_na_watek * (b - a) / liczba_podzialow;
        double koniec_przedzialu = a + (i + 1) * przedzialy_na_watek * (b - a) / liczba_podzialow;

        watki.emplace_back([i, poczatek_przedzialu, koniec_przedzialu, przedzialy_na_watek, &wyniki]() {
            wyniki[i] = oblicz_pi_dla_przedzialu(poczatek_przedzialu, koniec_przedzialu, przedzialy_na_watek);
            });
    }

    // Poczekaj na zakończenie pracy wątków
    for (auto& watek : watki) {
        watek.join();
    }

    // Sumowanie wyników z wątków
    double wynik = 0.0;
    for (const auto& wynik_watku : wyniki) {
        wynik += wynik_watku;
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    // Wyświetlanie wyników
    std::cout << "Przybliżona wartość liczby PI: " << wynik << std::endl;
    std::cout << "Czas obliczeń: " << duration.count() << " ms" << std::endl;

    return 0;
}
